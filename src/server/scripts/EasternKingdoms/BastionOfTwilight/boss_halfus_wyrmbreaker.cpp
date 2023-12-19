/*
* This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "PassiveAI.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Player.h"
#include "InstanceScript.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "MotionMaster.h"
#include "bastion_of_twilight.h"

namespace BastionOfTwilight::HalfusWyrmbreaker
{
enum Spells
{
    // Halfus Wyrmbreaker
    SPELL_FRENZIED_ASSAULT      = 83693,
    SPELL_MALEVOLENT_STRIKES    = 39171,
    SPELL_SHADOW_NOVA           = 83703,
    SPELL_BIND_WILL_TRIGGERED   = 83432,
    SPELL_PARALYSIS             = 84030,
    SPELL_FURIOUS_ROAR          = 83710,
    SPELL_SHADOW_WRAPPED        = 83952,

    // Proto Behemoth
    SPELL_DRAGONS_VENGEANCE     = 87683,
    SPELL_DANCING_FLAMES        = 84106,
    SPELL_SUPERHEATED_BREATH    = 83956,
    SPELL_FIREBALL              = 86058,
    SPELL_FIREBALL_SLOW         = 83862,
    SPELL_FIREBALL_BARRAGE      = 83706,
    SPELL_FIREBALL_BARRAGE_FAST = 83720,
    SPELL_FIREBALL_BARRAGE_SLOW = 83733,
    SPELL_SCORCHING_BREATH      = 83707,

    // Enslaved Dragons
    SPELL_UNRESPONSIVE_DRAGON   = 86003,
    SPELL_UNRESPONSIVE_WHELP    = 86022,
    SPELL_NETHER_BLINDNESS      = 83611,
    SPELL_STONE_TOUCH           = 83603,
    SPELL_CYCLONE_WINDS         = 83612,
    SPELL_TIME_DILATION         = 83601,
    SPELL_ATROPHIC_POISON       = 83609,
    SPELL_BIND_WILL             = 84246
};

enum Events
{
    // Halfus Wyrmbreaker
    EVENT_SHADOW_NOVA = 1,
    EVENT_FURIOUS_ROAR,
    EVENT_RESET_ACHIEVEMENT,
    EVENT_BERSERK,

    // Proto-Behemoth
    EVENT_FIREBALL,
    EVENT_FIREBALL_BARRAGE,
    EVENT_SCORCHING_BREATH,

    // Enslaved Dragons
    EVENT_LIFTOFF,
    EVENT_CAST_DEBUFF,
    EVENT_BIND_TO_HALFUS_WILL,
    EVENT_MOVE_OUT_OF_CAGE,
    EVENT_ENGAGE_WITH_PLAYERS
};

enum Actions
{
    ACTION_ORPHANED_EMERALD_WHELP_KILLED = 0,
};

enum Texts
{
    // Halfus Wyrmbreaker
    SAY_AGGRO                   = 0,
    SAY_SLAY                    = 1,
    SAY_ANNOUNCE_BIND_DRAGON    = 2,
    SAY_ANNOUNCE_FURIOUS_ROAR   = 3
};

enum SpawnGroups
{
    SPAWN_GROUP_ID_ENCOUNTER_SETUP = 462
};

struct EncounterDragonData
{
    EncounterDragonData(uint32 dataType, uint32 encounterCreatureId) : DataType(dataType), EncounterCreatureId(encounterCreatureId) { }

    uint32 DataType = 0;
    uint32 EncounterCreatureId = 0;
};

static std::array<EncounterDragonData const, 4> DragonData =
{
    EncounterDragonData(DATA_SLATE_DRAGON, NPC_SLATE_DRAGON_ENCOUNTER),
    EncounterDragonData(DATA_NETHER_SCION, NPC_NETHER_SCION_ENCOUNTER),
    EncounterDragonData(DATA_STORM_RIDER, NPC_STORM_RIDER_ENCOUNTER),
    EncounterDragonData(DATA_TIME_WARDEN, NPC_TIME_WARDEN_ENCOUNTER)
};

static constexpr uint8 OrphanedEmeraldWhelpPackSize = 8;

struct boss_halfus_wyrmbreaker final : public BossAI
{
    boss_halfus_wyrmbreaker(Creature* creature) : BossAI(creature, DATA_HALFUS_WYRMBREAKER), _announcedOrphanedEmeraldWhelpBinding(false), _furiousRoarEnabled(false),
        _theOnlyEscapeAchievementState(NOT_STARTED), _furiousRoarCount(0) { }

    void JustAppeared() override
    {
        uint8 activeDragonFlags = static_cast<uint8>(instance->GetData(DATA_ACTIVE_DRAGON_FLAGS));
        if ((activeDragonFlags & DRAGON_FLAG_SLATE_DRAGON) != 0)
            DoCastSelf(SPELL_MALEVOLENT_STRIKES);

        if ((activeDragonFlags & DRAGON_FLAG_NETHER_SCION) != 0)
            DoCastSelf(SPELL_FRENZIED_ASSAULT);

        if ((activeDragonFlags & DRAGON_FLAG_STORM_RIDER) != 0)
            DoCastSelf(SPELL_SHADOW_WRAPPED);

        instance->instance->SpawnGroupSpawn(SPAWN_GROUP_ID_ENCOUNTER_SETUP, true);
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        Talk(SAY_AGGRO);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
        instance->DoUpdateWorldState(WORLD_STATE_ID_THE_ONLY_ESCAPE, 0);
        events.ScheduleEvent(EVENT_BERSERK, 10min);

        if (Creature* protoBehemoth = instance->GetCreature(DATA_PROTO_BEHEMOTH))
            DoZoneInCombat(protoBehemoth);

        for (EncounterDragonData const& dragonData : DragonData)
            if (Creature* dragon = instance->GetCreature(dragonData.DataType))
                if (dragon->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                    dragon->UpdateEntry(dragonData.EncounterCreatureId);

        if ((static_cast<uint8>(instance->GetData(DATA_ACTIVE_DRAGON_FLAGS) & DRAGON_FLAG_EMERALD_WHELPS) != 0))
            if (GameObject* cage = instance->GetGameObject(DATA_WHELP_CAGE))
                cage->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);

        if (me->HasAura(SPELL_SHADOW_WRAPPED))
            events.ScheduleEvent(EVENT_SHADOW_NOVA, 7s);
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->IsPlayer())
            Talk(SAY_SLAY);
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        instance->instance->SpawnGroupDespawn(SPAWN_GROUP_ID_ENCOUNTER_SETUP);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        _EnterEvadeMode();
        summons.DespawnAll();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        instance->instance->SpawnGroupDespawn(SPAWN_GROUP_ID_ENCOUNTER_SETUP);
        _DespawnAtEvade();
    }

    void SpellHit(WorldObject* caster, SpellInfo const* spell) override
    {
        if (!caster)
            return;

        switch (spell->Id)
        {
            case SPELL_BIND_WILL:
                if (caster->GetEntry() == NPC_ORPHANED_EMERALD_WELP && !_announcedOrphanedEmeraldWhelpBinding)
                {
                    Talk(SAY_ANNOUNCE_BIND_DRAGON, caster);
                    _announcedOrphanedEmeraldWhelpBinding = true;
                }
                else if (caster->GetEntry() != NPC_ORPHANED_EMERALD_WELP)
                    Talk(SAY_ANNOUNCE_BIND_DRAGON, caster);
                break;
            case SPELL_DRAGONS_VENGEANCE:
                if (_theOnlyEscapeAchievementState == NOT_STARTED)
                {
                    _theOnlyEscapeAchievementState = IN_PROGRESS;
                    instance->DoUpdateWorldState(WORLD_STATE_ID_THE_ONLY_ESCAPE, 1);
                    events.ScheduleEvent(EVENT_RESET_ACHIEVEMENT, 10s);
                }
                else if (_theOnlyEscapeAchievementState == IN_PROGRESS)
                {
                    _theOnlyEscapeAchievementState = DONE;
                    instance->DoUpdateWorldState(WORLD_STATE_ID_THE_ONLY_ESCAPE, 2);
                    events.CancelEvent(EVENT_RESET_ACHIEVEMENT);
                }
                break;
            default:
                break;
        }
    }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
    {
        if (me->HealthBelowPct(50) && !_furiousRoarEnabled)
        {
            events.ScheduleEvent(EVENT_FURIOUS_ROAR, 1ms);
            _furiousRoarEnabled = true;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasUnitState(UNIT_STATE_STUNNED))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SHADOW_NOVA:
                    DoCastAOE(SPELL_SHADOW_NOVA);
                    events.Repeat(8s);
                    break;
                case EVENT_FURIOUS_ROAR:
                    if (_furiousRoarCount == 0)
                        Talk(SAY_ANNOUNCE_FURIOUS_ROAR);

                    if (_furiousRoarCount != 2)
                    {
                        DoCastAOE(SPELL_FURIOUS_ROAR);
                        events.Repeat(3s);
                        _furiousRoarCount++;
                    }
                    else
                    {
                        DoCastAOE(SPELL_FURIOUS_ROAR);
                        events.Repeat(25s);
                        _furiousRoarCount = 0;
                    }
                    break;
                case EVENT_RESET_ACHIEVEMENT:
                    _theOnlyEscapeAchievementState = NOT_STARTED;
                    break;
                case EVENT_BERSERK:
                    DoCastSelf(SPELL_BERSERK, true);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    bool _announcedOrphanedEmeraldWhelpBinding;
    bool _furiousRoarEnabled;
    uint8 _theOnlyEscapeAchievementState;
    uint8 _furiousRoarCount;
};

struct npc_halfus_proto_behemoth final : public PassiveAI
{
    npc_halfus_proto_behemoth(Creature* creature) : PassiveAI(creature), _instance(nullptr), _killedEmeraldWhelps(0) { }

    void InitializeAI() override
    {
        _instance = me->GetInstanceScript();
    }

    void JustAppeared() override
    {
        uint8 activeDragonFlags = static_cast<uint8>(_instance->GetData(DATA_ACTIVE_DRAGON_FLAGS));
        if ((activeDragonFlags & DRAGON_FLAG_EMERALD_WHELPS) != 0)
            DoCastSelf(SPELL_SUPERHEATED_BREATH);

        if ((activeDragonFlags & DRAGON_FLAG_TIME_WARDEN) != 0)
            DoCastSelf(SPELL_DANCING_FLAMES);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_FIREBALL, 1s);

        if (me->HasAura(SPELL_DANCING_FLAMES))
        {
            _events.ScheduleEvent(EVENT_FIREBALL_BARRAGE, 13s);
            if (me->HasAura(SPELL_SUPERHEATED_BREATH))
                _events.ScheduleEvent(EVENT_SCORCHING_BREATH, 24s);
        }
        else if (me->HasAura(SPELL_SUPERHEATED_BREATH))
            _events.ScheduleEvent(EVENT_SCORCHING_BREATH, 13s);
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_ORPHANED_EMERALD_WHELP_KILLED:
                ++_killedEmeraldWhelps;
                if (_killedEmeraldWhelps == OrphanedEmeraldWhelpPackSize)
                    DoCastAOE(SPELL_DRAGONS_VENGEANCE, true);
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FIREBALL:
                    if (!me->HasAura(SPELL_TIME_DILATION))
                    {
                        DoCastAOE(SPELL_FIREBALL, CastSpellExtraArgs().AddSpellMod(SPELLVALUE_MAX_TARGETS, 1));
                        _events.Repeat(2s + 500ms);
                    }
                    else
                    {
                        DoCastAOE(SPELL_FIREBALL_SLOW, CastSpellExtraArgs().AddSpellMod(SPELLVALUE_MAX_TARGETS, 1));
                        _events.Repeat(3s + 500ms);
                    }
                    break;
                case EVENT_FIREBALL_BARRAGE:
                    DoCastSelf(SPELL_FIREBALL_BARRAGE);
                    _events.Repeat(31s);
                    _events.RescheduleEvent(EVENT_FIREBALL, 11s);
                    break;
                case EVENT_SCORCHING_BREATH:
                    DoCastSelf(SPELL_SCORCHING_BREATH);
                    _events.Repeat(31s);
                    _events.RescheduleEvent(EVENT_FIREBALL, 11s);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    InstanceScript* _instance;
    uint8 _killedEmeraldWhelps;
};

struct npc_halfus_enslaved_dragon final : public ScriptedAI
{
    npc_halfus_enslaved_dragon(Creature* creature) : ScriptedAI(creature), _instance(nullptr) { }

    void InitializeAI() override
    {
        _instance = me->GetInstanceScript();
        me->SetReactState(REACT_PASSIVE);
        if (isUnresponsive())
            DoCastSelf(me->GetEntry() != NPC_ORPHANED_EMERALD_WELP ? SPELL_UNRESPONSIVE_DRAGON : SPELL_UNRESPONSIVE_WHELP);
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->SetDisableGravity(false);
        me->SendSetPlayHoverAnim(false);
        me->DespawnOrUnsummon(6s);

        if (Creature* protoBehemoth = _instance->GetCreature(DATA_PROTO_BEHEMOTH))
        {
            if (me->GetOriginalEntry() != NPC_ORPHANED_EMERALD_WELP)
                protoBehemoth->CastSpell(nullptr, SPELL_DRAGONS_VENGEANCE, true);
            else if (CreatureAI* ai = protoBehemoth->AI())
                ai->DoAction(ACTION_ORPHANED_EMERALD_WHELP_KILLED);
        }
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_DRAGON_FREED:
                if (me->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    me->RemoveAllAuras();
                    _events.ScheduleEvent(EVENT_LIFTOFF, 1ms);
                }
                else if (me->GetEntry() == NPC_ORPHANED_EMERALD_WELP && me->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _events.ScheduleEvent(EVENT_MOVE_OUT_OF_CAGE, 1ms);
                }
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        UpdateVictim();

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LIFTOFF:
                {
                    Position const pos = Position(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5.5f, me->GetOrientation());
                    me->GetMotionMaster()->MoveTakeoff(0, pos, 4.f);
                    me->SetDisableGravity(true);
                    me->SetHover(true);
                    _events.ScheduleEvent(EVENT_CAST_DEBUFF, 2s + 500ms);
                    break;
                }
                case EVENT_MOVE_OUT_OF_CAGE:
                {
                    float angle = me->GetOrientation();
                    float x = me->GetPositionX() + std::cos(angle) * 20.f;
                    float y = me->GetPositionY() + std::sin(angle) * 20.f;
                    float z = 888.1f;
                    me->GetMotionMaster()->MovePoint(0, x, y, z);
                    _events.ScheduleEvent(EVENT_CAST_DEBUFF, 2s + 500ms);
                    break;
                }
                case EVENT_CAST_DEBUFF:
                {
                    Creature* halfus = _instance->GetCreature(DATA_HALFUS_WYRMBREAKER);
                    Creature* protoBehemoth = _instance->GetCreature(DATA_PROTO_BEHEMOTH);

                    if (!halfus || !protoBehemoth)
                        break;

                    switch (me->GetEntry())
                    {
                        case NPC_NETHER_SCION_ENCOUNTER:
                            me->SetFacingToObject(halfus);
                            DoCastSelf(SPELL_NETHER_BLINDNESS);
                            break;
                        case NPC_SLATE_DRAGON_ENCOUNTER:
                            me->SetFacingToObject(halfus);
                            DoCastSelf(SPELL_STONE_TOUCH);
                            break;
                        case NPC_STORM_RIDER_ENCOUNTER:
                            me->SetFacingToObject(halfus);
                            DoCastSelf(SPELL_CYCLONE_WINDS);
                            break;
                        case NPC_TIME_WARDEN_ENCOUNTER:
                            me->SetFacingToObject(protoBehemoth);
                            DoCastSelf(SPELL_TIME_DILATION);
                            break;
                        case NPC_ORPHANED_EMERALD_WELP:
                            me->SetFacingToObject(protoBehemoth);
                            DoCastSelf(SPELL_ATROPHIC_POISON);
                            break;
                        default:
                            break;
                    }
                    _events.ScheduleEvent(EVENT_BIND_TO_HALFUS_WILL, 2s);
                    break;
                }
                case EVENT_BIND_TO_HALFUS_WILL:
                    DoCastSelf(SPELL_BIND_WILL);
                    _events.ScheduleEvent(EVENT_ENGAGE_WITH_PLAYERS, 1s);
                    break;
                case EVENT_ENGAGE_WITH_PLAYERS:
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (!me->IsEngaged())
                        if (Unit* target = me->SelectNearestTarget(100.f))
                            me->EngageWithTarget(target);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    EventMap _events;
    InstanceScript* _instance;

    bool isUnresponsive()
    {
        uint8 activeDragonFlags = static_cast<uint8>(_instance->GetData(DATA_ACTIVE_DRAGON_FLAGS));
        if (me->GetEntry() == NPC_SLATE_DRAGON && (activeDragonFlags & DRAGON_FLAG_SLATE_DRAGON) == 0)
            return true;

        if (me->GetEntry() == NPC_NETHER_SCION && (activeDragonFlags & DRAGON_FLAG_NETHER_SCION) == 0)
            return true;

        if (me->GetEntry() == NPC_STORM_RIDER && (activeDragonFlags & DRAGON_FLAG_STORM_RIDER) == 0)
            return true;

        if (me->GetEntry() == NPC_TIME_WARDEN && (activeDragonFlags & DRAGON_FLAG_TIME_WARDEN) == 0)
            return true;

        if (me->GetEntry() == NPC_ORPHANED_EMERALD_WELP && (activeDragonFlags & DRAGON_FLAG_EMERALD_WHELPS) == 0)
            return true;

        return false;
    }
};

struct go_halfus_whelp_cage final : public GameObjectAI
{
    go_halfus_whelp_cage(GameObject* go) : GameObjectAI(go), _instance(go->GetInstanceScript()) {}

    bool OnReportUse(Player* /*player*/) override
    {
        if (me->HasFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE))
            return true;

        me->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);

        // There is no event linked to releasing the whelps so we have to resort to grid searching.
        std::vector<Creature*> whelpVector;
        me->GetCreatureListWithEntryInGrid(whelpVector, NPC_ORPHANED_EMERALD_WELP, 20.f);
        for (Creature* whelp : whelpVector)
            if (CreatureAI* ai = whelp->AI())
                ai->DoAction(ACTION_DRAGON_FREED);

        return true;
    }

private:
    InstanceScript* _instance;
};

class spell_halfus_bind_will final : public SpellScript
{
    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                target->CastSpell(caster, GetSpellInfo()->Effects[effIndex].BasePoints, true);
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_halfus_bind_will::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class spell_halfus_fireball final : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_FIREBALL_SLOW,
            SPELL_FIREBALL_BARRAGE_FAST,
            SPELL_FIREBALL_BARRAGE_SLOW
        });
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Id == SPELL_FIREBALL_SLOW ? SPELL_FIREBALL_BARRAGE_SLOW : SPELL_FIREBALL_BARRAGE_FAST, true);
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_halfus_fireball::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_halfus_fireball_barrage final : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_FIREBALL_BARRAGE_FAST,
            SPELL_FIREBALL_BARRAGE_SLOW,
            SPELL_TIME_DILATION
        });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() > 1)
            Trinity::Containers::RandomResize(targets, 1);
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), GetCaster()->HasAura(SPELL_TIME_DILATION) ? SPELL_FIREBALL_BARRAGE_SLOW : SPELL_FIREBALL_BARRAGE_FAST, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_halfus_fireball_barrage::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget.Register(&spell_halfus_fireball_barrage::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_halfus_stone_touch final : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PARALYSIS });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetTarget())
            caster->CastSpell(caster, SPELL_PARALYSIS, true);
    }

    void Register() override
    {
        OnEffectPeriodic.Register(&spell_halfus_stone_touch::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_halfus_cyclone_winds final : public SpellScript
{
    void SetDestPosition(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            float angle = caster->GetOrientation();
            float x = caster->GetPositionX() + cos(angle) * 7.0f;
            float y = caster->GetPositionY() + sin(angle) * 7.0f;
            float z = caster->GetPositionZ();

            const_cast<WorldLocation*>(GetExplTargetDest())->Relocate(x, y, z);
            GetHitDest()->Relocate(x, y, z);
        }
    }

    void Register()
    {
        OnEffectLaunch.Register(&spell_halfus_cyclone_winds::SetDestPosition, EFFECT_0, SPELL_EFFECT_SUMMON);
    }
};

class spell_halfus_dancing_flames final : public SpellScript
{
    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() < 1)
            return;

        targets.remove_if([caster = GetCaster()](WorldObject const* target) { return (target->GetDistance2d(caster) <= 40.0f || target->GetDistance2d(caster) >= 100.0f); });

        if (targets.size() > 1)
            Trinity::Containers::RandomResize(targets, 1);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_halfus_dancing_flames::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_halfus_unresponsive final : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetHitUnit()->IsCreature())
            GetHitUnit()->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_halfus_unresponsive::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};
}

void AddSC_boss_halfus_wyrmbreaker()
{
    using namespace BastionOfTwilight;
    using namespace BastionOfTwilight::HalfusWyrmbreaker;
    RegisterBastionOfTwilightCreatureAI(boss_halfus_wyrmbreaker);
    RegisterBastionOfTwilightCreatureAI(npc_halfus_proto_behemoth);
    RegisterBastionOfTwilightCreatureAI(npc_halfus_enslaved_dragon);
    RegisterGameObjectAI(go_halfus_whelp_cage);
    RegisterSpellScript(spell_halfus_bind_will);
    RegisterSpellScript(spell_halfus_fireball);
    RegisterSpellScript(spell_halfus_fireball_barrage);
    RegisterSpellScript(spell_halfus_stone_touch);
    RegisterSpellScript(spell_halfus_cyclone_winds);
    RegisterSpellScript(spell_halfus_dancing_flames);
    RegisterSpellScript(spell_halfus_unresponsive);
}
