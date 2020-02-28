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
#include "CommonPredicates.h"
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "Map.h"
#include "blackwing_descent.h"

enum Spells
{
    // Chimaeron
    SPELL_CAUSTIC_SLIME_TARGETING   = 82871,
    SPELL_BREAK                     = 82881,
    SPELL_MASSACRE                  = 82848,
    SPELL_DOUBLE_ATTACK             = 88826,
    SPELL_DOUBLE_ATTACK_TRIGGERED   = 82882,
    //SPELL_GUARDIAN_TAUNT            = 85667, Todo: research me
    SPELL_MORTALITY_1               = 82890,
    SPELL_MORTALITY_2               = 82934,
    SPELL_FEUD                      = 88872,

    // Bile-O-Tron 800
    SPELL_FINKLES_MIXTURE           = 82705,
    SPELL_FINKLES_MIXTURE_VISUAL    = 91106,
    SPELL_SYSTEM_FAILURE            = 88853,
    SPELL_REROUTE_POWER             = 88861,
    SPELL_SHUT_DOWN                 = 90405,

    // Lord Victor Nefarius
    SPELL_MOCKING_SHADOWS           = 91307,
    SPELL_TELEPORT_VISUAL_ONLY      = 41232,
    SPELL_SHADOW_WHIP               = 91304
};

enum Texts
{
    // Chimaeron
    SAY_WAKE_UP_1               = 0,
    SAY_WAKE_UP_2               = 1,
    SAY_WAKE_UP_3               = 2,
    SAY_ANNOUNCE_MASSACRE       = 3,

    // Finkle Einhorn
    SAY_CALL_FOR_HELP           = 0,
    SAY_BILE_O_TRON_KNOCKED_OUT = 1,
    SAY_CHIMAERON_DEAD          = 2,

    // Bile-O-Tron 800
    SAY_ACTIVATED               = 0,
    SAY_KNOCKED_OUT             = 1,
    SAY_ONLINE                  = 2,
    SAY_SHUT_DOWN               = 3,

    // Lord Victor Nefarius
    SAY_INTRODUCTION            = 0,
    SAY_STOP_FEUD               = 1,
    SAY_PHASE_2                 = 2,
    SAY_CHIMAERON_LOST          = 3,
    SAY_ENTRANCE_INTRO_HEROIC   = 4
};

enum Phases
{
    PHASE_ASLEEP    = 1,
    PHASE_1         = 2,
    PHASE_2         = 3
};

enum Events
{
    // Chimaeron
    EVENT_CAUSTIC_SLIME = 1,
    EVENT_BREAK,
    EVENT_MASSACRE,
    EVENT_DOUBLE_ATTACK,
    EVENT_SNORT,
    EVENT_GRUNT,
    EVENT_WAKE_UP,

    // Finkle Einhorn
    EVENT_CALL_FOR_HELP,
    EVENT_TALK_SEARCH_FOR_KEY,

    // Bile-O-Tron 800
    EVENT_SHUT_DOWN,

    // Lord Victor Nefarius
    EVENT_INTRODUCE_CHIMAERON = 1,
    EVENT_MOCKING_SHADOWS,
    EVENT_TALK_CHIMAERON_DIED,
    EVENT_TELEPORT_AWAY,
    EVENT_TALK_HEROIC_INTRO
};

enum Actions
{
    // Chimaeron
    ACTION_WAKE_UP_CHIMAERON        = 1,
    ACTION_START_FEUD               = 2,
    ACTION_END_FEUD                 = 3,

    // Fikle Einhorn
    ACTION_CHIMAERON_DIED           = 1,

    // Bile-O-Tron 800
    ACTION_ACTIVATE_BILE_O_TRON     = 1,
    ACTION_KNOCK_OUT_BILE_O_TRON    = 2,
    ACTION_BILE_O_TRON_BACK_ONLINE  = 3,
    ACTION_SHUT_DOWN                = 4,
    ACTION_ENCOUNTER_STARTED        = 5,

    // Lord Victor Nefarius
    ACTION_STOP_FEUD                = 1,
    ACTION_ENTER_PHASE_2            = 2,
    ACTION_CHIMAERON_DEFEATED       = 3,
};

enum GossipMenuMisc
{
    GOSSIP_MENU_ID_RESTRICTIONS = 11837,
    NPC_TEXT_MISSING_KEY        = 16565
};

enum Data
{
    DATA_ACHIEVEMENT_ENLIGIBLE = 0
};

Position const LordVictorNefariusSummonPosition = { -113.8229f, 45.86111f, 80.36481f, 4.817109f };

struct boss_chimaeron : public BossAI
{
    boss_chimaeron(Creature* creature) : BossAI(creature, DATA_CHIMAERON),  _isInFeud(false)
    {
        Initialize();
    }

    void Initialize()
    {
        me->SetReactState(REACT_PASSIVE);
        _knockOutChance = 40;
    }

    void Reset() override
    {
        _Reset();
        events.SetPhase(PHASE_ASLEEP);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _JustEngagedWith();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        me->SetReactState(REACT_AGGRESSIVE);
        events.SetPhase(PHASE_1);
        events.ScheduleEvent(EVENT_CAUSTIC_SLIME, 5s, 0, PHASE_1);
        events.ScheduleEvent(EVENT_BREAK, 5s, 0, PHASE_1);
        events.ScheduleEvent(EVENT_DOUBLE_ATTACK, 5s, 0, PHASE_1);
        events.ScheduleEvent(EVENT_MASSACRE, 26s, 0, PHASE_1);

        if (IsHeroic())
            DoSummon(NPC_LORD_VICTOR_NEFARIUS_CHIMAERON, LordVictorNefariusSummonPosition, 0, TEMPSUMMON_MANUAL_DESPAWN);

        if (Creature * finkle = instance->GetCreature(DATA_FINKLE_EINHORN))
            if (finkle->IsAIEnabled)
                finkle->AI()->DoAction(ACTION_ENCOUNTER_STARTED);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        _EnterEvadeMode();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        me->RemoveAllAuras();

        if (Creature* bileOTron = instance->GetCreature(DATA_BILE_O_TRON_800))
            bileOTron->DespawnOrUnsummon(0ms, 30s);

        if (Creature* finkle = instance->GetCreature(DATA_FINKLE_EINHORN))
            finkle->DespawnOrUnsummon(0ms, 30s);

        if (Creature* nefarius = instance->GetCreature(DATA_LORD_VICTOR_NEFARIUS_CHIMAERON))
            nefarius->DespawnOrUnsummon();

        _DespawnAtEvade();
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        me->RemoveAllAuras();

        if (Creature* bileOTron = instance->GetCreature(DATA_BILE_O_TRON_800))
            if (bileOTron->IsAIEnabled)
                bileOTron->AI()->DoAction(ACTION_SHUT_DOWN);

        if (Creature* finkle = instance->GetCreature(DATA_FINKLE_EINHORN))
            if (finkle->IsAIEnabled)
                finkle->AI()->DoAction(ACTION_CHIMAERON_DIED);

        if (Creature * nefarius = instance->GetCreature(DATA_LORD_VICTOR_NEFARIUS_CHIMAERON))
            if (nefarius->IsAIEnabled)
                nefarius->AI()->DoAction(ACTION_CHIMAERON_DEFEATED);
                
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->GetTypeId() == TYPEID_PLAYER)
            _killedPlayerCount++;
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) override
    {
        if (events.IsInPhase(PHASE_2))
            return;

        if (me->HealthBelowPctDamaged(20, damage))
        {
            events.SetPhase(PHASE_2);
            events.ScheduleEvent(EVENT_DOUBLE_ATTACK, 1ms, 0, PHASE_2);
            DoCastAOE(SPELL_MORTALITY_1, true);
            DoCastSelf(SPELL_MORTALITY_2, true);

            if (Creature * nefarius = instance->GetCreature(DATA_LORD_VICTOR_NEFARIUS_CHIMAERON))
                if (nefarius->IsAIEnabled)
                    nefarius->AI()->DoAction(ACTION_ENTER_PHASE_2);
        }
    }

    void OnSpellCastFinished(SpellInfo const* spell, SpellFinishReason reason) override
    {
        if (reason != SPELL_FINISHED_SUCCESSFUL_CAST)
            return;

        switch (spell->Id)
        {
            case SPELL_MASSACRE:
            {
                // Hotfix (2011-01-05): "Chimaeron now resets his melee attack cycle after each Massacre and removes the Double Attack buff." 
                me->RemoveAurasDueToSpell(SPELL_DOUBLE_ATTACK);
                me->resetAttackTimer();

                Creature* bileOTron = instance->GetCreature(DATA_BILE_O_TRON_800);
                if (!bileOTron || !bileOTron->HasAura(SPELL_FINKLES_MIXTURE_VISUAL))
                    break;

                /*
                    Patch 4.1.0 (2011-04-26): The Bile-O-Tron 800 in the Chimaeron encounter is now more resistant to being
                    knocked offline many times in rapid succession.
                */

                if (roll_chance_i(_knockOutChance))
                {
                    if (bileOTron->IsAIEnabled)
                        bileOTron->AI()->DoAction(ACTION_KNOCK_OUT_BILE_O_TRON);

                    if (Creature* finkle = instance->GetCreature(DATA_FINKLE_EINHORN))
                        if (finkle->IsAIEnabled)
                            finkle->AI()->Talk(SAY_BILE_O_TRON_KNOCKED_OUT, me);

                    me->StopMoving();
                    DoCastSelf(SPELL_FEUD);
                    _knockOutChance = 40;
                }
                else
                    _knockOutChance += 20;
                break;
            }
            default:
                break;
        }
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_WAKE_UP_CHIMAERON:
                events.ScheduleEvent(EVENT_SNORT, 6s, 0, PHASE_ASLEEP);
                break;
            case ACTION_START_FEUD:
                _isInFeud = true;
                if (IsHeroic())
                    if (Creature * nefarius = instance->GetCreature(DATA_LORD_VICTOR_NEFARIUS_CHIMAERON))
                        if (nefarius->IsAIEnabled)
                            nefarius->AI()->DoAction(ACTION_STOP_FEUD);
                break;
            case ACTION_END_FEUD:
                _isInFeud = false;
                break;
            default:
                break;
        }
    }

    void JustSummoned(Creature* summon) override
    {
        switch (summon->GetEntry())
        {
            case NPC_LORD_VICTOR_NEFARIUS_CHIMAERON:
                break;
            default:
                summons.Summon(summon);
                break;
        }
    }

    uint32 GetData(uint32 type) const override
    {
        switch (type)
        {
            case DATA_ACHIEVEMENT_ENLIGIBLE:
                return uint8(_killedPlayerCount <= 2);
            default:
                return 0;
        }

        return 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() && !events.IsInPhase(PHASE_ASLEEP) && !_isInFeud)
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING) && !_isInFeud)
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CAUSTIC_SLIME:
                    if (SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                        DoCastAOE(SPELL_CAUSTIC_SLIME_TARGETING, true);
                    events.Repeat(5s);
                    break;
                case EVENT_BREAK:
                    DoCastVictim(SPELL_BREAK);
                    events.Repeat(15s);
                    break;
                case EVENT_MASSACRE:
                    Talk(SAY_ANNOUNCE_MASSACRE);
                    me->InterruptNonMeleeSpells(true);
                    DoCastAOE(SPELL_MASSACRE);
                    events.Repeat(30s);
                    events.RescheduleEvent(EVENT_CAUSTIC_SLIME, 19s, 0, PHASE_1);
                    events.RescheduleEvent(EVENT_BREAK, 11s, 0, PHASE_1);
                    events.RescheduleEvent(EVENT_DOUBLE_ATTACK, 11s, 0, PHASE_1);
                    break;
                case EVENT_DOUBLE_ATTACK:
                    DoCastSelf(SPELL_DOUBLE_ATTACK, true);
                    events.Repeat(15s);
                    break;
                case EVENT_SNORT:
                    Talk(SAY_WAKE_UP_1);
                    events.ScheduleEvent(EVENT_GRUNT, 6s, 0, PHASE_ASLEEP);
                    break;
                case EVENT_GRUNT:
                    Talk(SAY_WAKE_UP_2);
                    events.ScheduleEvent(EVENT_WAKE_UP, 11s, 0, PHASE_ASLEEP);
                    break;
                case EVENT_WAKE_UP:
                    Talk(SAY_WAKE_UP_3);
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (Player* player = me->SelectNearestPlayer(70.0f))
                        AttackStart(player);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    uint8 _knockOutChance;
    uint8 _killedPlayerCount;
    bool _isInFeud;
};

struct npc_chimaeron_finkle_einhorn : public ScriptedAI
{
    npc_chimaeron_finkle_einhorn(Creature* creature) : ScriptedAI(creature), _instance(me->GetInstanceScript()), _bileOTronActivated(false) { }

    bool GossipSelect(Player* /*player*/, uint32 menuId, uint32 /*gossipListId*/) override
    {
        if (menuId == GOSSIP_MENU_ID_RESTRICTIONS && !_bileOTronActivated)
        {
            _events.Reset();
            if (Creature* bileOTron = _instance->GetCreature(DATA_BILE_O_TRON_800))
                if (bileOTron->IsAIEnabled)
                    bileOTron->AI()->DoAction(ACTION_ACTIVATE_BILE_O_TRON);

            if (Creature* chimaeron = _instance->GetCreature(DATA_CHIMAERON))
                chimaeron->AI()->DoAction(ACTION_WAKE_UP_CHIMAERON);

            _bileOTronActivated = true;
        }

        return false;
    }

    void Reset() override
    {
        if (_instance->GetBossState(DATA_CHIMAERON) == DONE)
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        else
            _events.ScheduleEvent(EVENT_CALL_FOR_HELP, 14s, 22s);
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_CHIMAERON_DIED:
                _events.ScheduleEvent(EVENT_TALK_SEARCH_FOR_KEY, 6s);
                break;
            case ACTION_ENCOUNTER_STARTED:
                _events.Reset();
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CALL_FOR_HELP:
                    if (me->SelectNearestPlayer(60.0f))
                        Talk(SAY_CALL_FOR_HELP);
                    _events.Repeat(14s, 22s);
                    break;
                case EVENT_TALK_SEARCH_FOR_KEY:
                    Talk(SAY_CHIMAERON_DEAD);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    InstanceScript* _instance;
    bool _bileOTronActivated;
};

struct npc_chimaeron_bile_o_tron : public ScriptedAI
{
    npc_chimaeron_bile_o_tron(Creature* creature) : ScriptedAI(creature), _instance(me->GetInstanceScript()) { }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_ACTIVATE_BILE_O_TRON:
                Talk(SAY_ACTIVATED);
                DoCastSelf(SPELL_FINKLES_MIXTURE);
                me->AddAura(SPELL_FINKLES_MIXTURE, me); // tempfix. spell has player only attribute so we gotta add the aura manually for now
                DoCastSelf(SPELL_FINKLES_MIXTURE_VISUAL);
                me->GetMotionMaster()->MovePath(me->GetEntry() * 100, true);
                break;
            case ACTION_KNOCK_OUT_BILE_O_TRON:
                me->RemoveAllAuras();
                me->ClearUnitState(UNIT_STATE_CASTING);
                DoCastSelf(SPELL_SYSTEM_FAILURE);
                DoCastSelf(SPELL_REROUTE_POWER);
                Talk(SAY_KNOCKED_OUT);
                break;
            case ACTION_BILE_O_TRON_BACK_ONLINE:
                me->RemoveAurasDueToSpell(SPELL_SYSTEM_FAILURE);
                me->AddAura(SPELL_FINKLES_MIXTURE, me); // tempfix. spell has player only attribute so we gotta add the aura manually for now
                Talk(SAY_ONLINE);
                break;
            case ACTION_SHUT_DOWN:
                _events.ScheduleEvent(EVENT_SHUT_DOWN, 6s);
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SHUT_DOWN:
                    me->GetMotionMaster()->InitDefault();
                    me->StopMoving();
                    me->RemoveAllAuras();
                    me->ClearUnitState(UNIT_STATE_CASTING);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoCastSelf(SPELL_SHUT_DOWN);
                    Talk(SAY_SHUT_DOWN);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    InstanceScript* _instance;
};

struct npc_chimaeron_lord_victor_nefarius : public NullCreatureAI
{
    npc_chimaeron_lord_victor_nefarius(Creature* creature) : NullCreatureAI(creature), _instance(me->GetInstanceScript()) { }

    void JustAppeared() override
    {
        DoCastSelf(SPELL_TELEPORT_VISUAL_ONLY);
        if (me->FindNearestCreature(BOSS_CHIMAERON, 50.0f, true))
            _events.ScheduleEvent(EVENT_INTRODUCE_CHIMAERON, 4s);
        else
            _events.ScheduleEvent(EVENT_TALK_HEROIC_INTRO, 3s);
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
                case EVENT_INTRODUCE_CHIMAERON:
                    Talk(SAY_INTRODUCTION);
                    break;
                case EVENT_MOCKING_SHADOWS:
                    Talk(SAY_PHASE_2);
                    DoCastSelf(SPELL_MOCKING_SHADOWS);
                    me->AddAura(SPELL_MOCKING_SHADOWS, me); // Tempfix until player only attribute spells accept creatures as original caster
                    break;
                case EVENT_TALK_CHIMAERON_DIED:
                    Talk(SAY_CHIMAERON_LOST);
                    _events.ScheduleEvent(EVENT_TELEPORT_AWAY, 6s);
                    break;
                case EVENT_TELEPORT_AWAY:
                    DoCastSelf(SPELL_TELEPORT_VISUAL_ONLY);
                    me->DespawnOrUnsummon(2s);
                    break;
                case EVENT_TALK_HEROIC_INTRO:
                    Talk(SAY_ENTRANCE_INTRO_HEROIC);
                    _events.ScheduleEvent(EVENT_TELEPORT_AWAY, 6s);
                    break;
                default:
                    break;
            }
        }
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_ENTER_PHASE_2:
                _events.ScheduleEvent(EVENT_MOCKING_SHADOWS, 1ms);
                break;
            case ACTION_CHIMAERON_DEFEATED:
                me->RemoveAllAurasExceptType(SPELL_AURA_DUMMY);
                _events.Reset();
                _events.ScheduleEvent(EVENT_TALK_CHIMAERON_DIED, 2s + 500ms);
                break;
            case ACTION_STOP_FEUD:
                Talk(SAY_STOP_FEUD);
                DoCastSelf(SPELL_SHADOW_WHIP);
                break;
            default:
                break;
        }
    }

private:
    EventMap _events;
    InstanceScript* _instance;
};

class spell_chimaeron_caustic_slime_targeting : public SpellScript
{
    PrepareSpellScript(spell_chimaeron_caustic_slime_targeting);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

        targets.remove_if(Trinity::Predicates::IsVictimOf(GetCaster()));

        uint8 size = GetCaster()->GetMap()->Is25ManRaid() ? 4 : 2;
        if (targets.size() > size)
            Trinity::Containers::RandomResize(targets, size);
    }

    void HandleDummyEffect(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_chimaeron_caustic_slime_targeting::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_chimaeron_caustic_slime_targeting::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_chimaeron_caustic_slime: public SpellScript
{
    PrepareSpellScript(spell_chimaeron_caustic_slime);

    void CountTargets(std::list<WorldObject*>& targets)
    {
        _targetCount = targets.size();
    }

    void SplitDamage()
    {
        if (!_targetCount)
            return;

        SetHitDamage(GetHitDamage() / _targetCount);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_chimaeron_caustic_slime::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        BeforeHit += SpellHitFn(spell_chimaeron_caustic_slime::SplitDamage);
    }

private:
    uint32 _targetCount = 0;
};

class spell_chimaeron_double_attack : public AuraScript
{
    PrepareAuraScript(spell_chimaeron_double_attack);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DOUBLE_ATTACK_TRIGGERED });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (Unit* target = eventInfo.GetProcTarget())
            GetTarget()->CastSpell(target, SPELL_DOUBLE_ATTACK_TRIGGERED, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_chimaeron_double_attack::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_chimaeron_finkles_mixture : public AuraScript
{
    PrepareAuraScript(spell_chimaeron_finkles_mixture);

    bool Load() override
    {
        healthThreshold = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
        return true;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MASSACRE,
                SPELL_FINKLES_MIXTURE_VISUAL
            });
    }

    void HandleMixtureDuration(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
            if (!caster->HasAura(SPELL_FINKLES_MIXTURE_VISUAL))
                Remove();
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();

        if (dmgInfo.GetDamage() >= target->GetHealth() && target->GetHealth() > healthThreshold)
            absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + 1;
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_chimaeron_finkles_mixture::HandleMixtureDuration, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_chimaeron_finkles_mixture::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_chimaeron_finkles_mixture::Absorb, EFFECT_1);
    }

private:
    uint32 healthThreshold = 0;
};

class spell_chimaeron_reroute_power : public AuraScript
{
    PrepareAuraScript(spell_chimaeron_reroute_power);

    void HandleActivation(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* target = GetTarget()->ToCreature())
            if (target->IsAIEnabled)
                target->AI()->DoAction(ACTION_BILE_O_TRON_BACK_ONLINE);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_chimaeron_reroute_power::HandleActivation, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_chimaeron_feud : public AuraScript
{
    PrepareAuraScript(spell_chimaeron_feud);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* target = GetTarget()->ToCreature())
            if (target->IsAIEnabled)
                target->AI()->DoAction(ACTION_START_FEUD);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* target = GetTarget()->ToCreature())
            if (target->IsAIEnabled)
                target->AI()->DoAction(ACTION_END_FEUD);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_chimaeron_feud::HandleApply, EFFECT_0, SPELL_AURA_MOD_PACIFY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_chimaeron_feud::HandleRemove, EFFECT_0, SPELL_AURA_MOD_PACIFY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_chimaeron_shadow_whip : public SpellScript
{
    PrepareSpellScript(spell_chimaeron_shadow_whip);

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        GetHitUnit()->InterruptNonMeleeSpells(true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_chimaeron_shadow_whip::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class achievement_full_of_sound_and_fury : public AchievementCriteriaScript
{
    public:
        achievement_full_of_sound_and_fury() : AchievementCriteriaScript("achievement_full_of_sound_and_fury") { }

        bool OnCheck(Player* /*source*/, Unit* target) override
        {
            if (!target)
                return false;

            if (target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_ACHIEVEMENT_ENLIGIBLE);

            return false;
        }
};

void AddSC_boss_chimaeron()
{
    RegisterBlackwingDescentCreatureAI(boss_chimaeron);
    RegisterBlackwingDescentCreatureAI(npc_chimaeron_finkle_einhorn);
    RegisterBlackwingDescentCreatureAI(npc_chimaeron_bile_o_tron);
    RegisterBlackwingDescentCreatureAI(npc_chimaeron_lord_victor_nefarius);
    RegisterSpellScript(spell_chimaeron_caustic_slime_targeting);
    RegisterSpellScript(spell_chimaeron_caustic_slime);
    RegisterAuraScript(spell_chimaeron_double_attack);
    RegisterAuraScript(spell_chimaeron_finkles_mixture);
    RegisterAuraScript(spell_chimaeron_reroute_power);
    RegisterAuraScript(spell_chimaeron_feud);
    RegisterSpellScript(spell_chimaeron_shadow_whip);
    new achievement_full_of_sound_and_fury();
}
