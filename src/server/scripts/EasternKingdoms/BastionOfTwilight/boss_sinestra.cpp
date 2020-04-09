/*
* Copyright (C) 2008-2019 TrinityCore <http://www.trinitycore.org/>
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
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "bastion_of_twilight.h"

enum Texts
{
    SAY_INTRO                   = 0,
    SAY_SUMMON_TWILIGHT_WHELPS  = 1
};

enum Spells
{
    // Sinestra
    SPELL_DRAINED                   = 89350,
    SPELL_REDUCE_PARRY_CHANCE_100   = 64651,
    SPELL_CALL_FLAMES               = 95855,
    SPELL_TWILIGHT_BLAST            = 89280,
    SPELL_FLAME_BREATH              = 90125,
    SPELL_WRACK                     = 89421,
    SPELL_SUMMON_SLICERS            = 92856, // Serverside Spell

    // Shadow Orb
    SPELL_AGGRO_CREATOR             = 92902,
    SPELL_TWILIGHT_PULSE_PERIODIC   = 92957,
    SPELL_TWILIGHT_SLICER           = 92851,

    // Invisible Stalker
    SPELL_LAVA_VISUAL_DND           = 95824,

    // Flames (DND)
    SPELL_FLAMES_DND                = 95823,

    // Players
    SPELL_WRACK_DISPELLED           = 89435,
    SPELL_SUMMON_SHADOW_ORB_PITCHER = 92875, // Serverside Spell
    SPELL_SUMMON_SHADOW_ORB_CATCHER = 92858  // Serverside Spell

};

enum Events
{
    // Sinestra
    EVENT_TALK_INTRO = 1,
    EVENT_MAKE_ATTACKABLE,
    EVENT_TWILIGHT_BLAST,
    EVENT_FLAME_BREATH,
    EVENT_WRACK,
    EVENT_TWILIGHT_SLICER,

    // Shadow Orb
    EVENT_AGGRO_CREATOR,
    EVENT_ACTIVATE_SLICER,
};

enum Phases
{
    // Sinestra
    PHASE_INTRO = 0,
    PHASE_ONE   = 1,
};

enum SummonGroups
{
    SUMMON_GROUP_ID_SPAWN_SETUP = 0
};

enum EncounterFrames
{
    ENCOUNTER_FRAME_INDEX_SINESTRA = 1
};

struct boss_sinestra : public BossAI
{
    boss_sinestra(Creature* creature) : BossAI(creature, DATA_SINESTRA), _respawned(false)
    {
        me->SetReactState(REACT_PASSIVE);
        me->setRegeneratingHealth(false);
        me->SetHealth(CalculatePct(me->GetMaxHealth(), 60));
    }

    void Reset() override
    {
        if (instance->GetBossState(DATA_SINESTRA) == FAIL)
            _respawned = true;
        BossAI::Reset();
    }

    void JustAppeared() override
    {
        events.SetPhase(PHASE_INTRO);
        events.ScheduleEvent(EVENT_MAKE_ATTACKABLE, 9s + 500ms, 0, PHASE_INTRO);

        if (!_respawned)
            events.ScheduleEvent(EVENT_TALK_INTRO, 3s + 200ms, 0, PHASE_INTRO);

        if (Creature* stalker = instance->GetCreature(DATA_INVISIBLE_STALKER_LAVA_VISUAL))
            stalker->CastSpell(stalker, SPELL_LAVA_VISUAL_DND);

        me->SummonCreatureGroup(SUMMON_GROUP_ID_SPAWN_SETUP);
        DoCastSelf(SPELL_DRAINED);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _JustEngagedWith();
        me->SetReactState(REACT_AGGRESSIVE);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, ENCOUNTER_FRAME_INDEX_SINESTRA);

        DoCastSelf(SPELL_REDUCE_PARRY_CHANCE_100);
        DoCastAOE(SPELL_CALL_FLAMES);

        events.SetPhase(PHASE_ONE);
        events.ScheduleEvent(EVENT_TWILIGHT_BLAST, 6s, 0, PHASE_ONE);
        events.ScheduleEvent(EVENT_FLAME_BREATH, 23s, 0, PHASE_ONE);
        events.ScheduleEvent(EVENT_WRACK, 16s, 0, PHASE_ONE);
        events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 27s, 0, PHASE_ONE);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        summons.DespawnAll();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        _EnterEvadeMode();
        _DespawnAtEvade();
    }

    void JustSummoned(Creature* summon) override
    {
        switch (summon->GetEntry())
        {
            case NPC_SHADOW_ORB_CATCHER:
            case NPC_SHADOW_ORB_PITCHER:
                summons.Summon(summon);
                break;
            default:
                BossAI::JustSummoned(summon);
                break;
        }
    }
    
    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() && !events.IsInPhase(PHASE_INTRO))
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TALK_INTRO:
                    Talk(SAY_INTRO);
                    break;
                case EVENT_MAKE_ATTACKABLE:
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    break;
                case EVENT_TWILIGHT_BLAST:
                    if (Unit* victim = me->GetVictim())
                        if (!me->IsWithinMeleeRange(victim))
                            DoCastVictim(SPELL_TWILIGHT_BLAST);
                    events.Repeat(6s);
                    break;
                case EVENT_FLAME_BREATH:
                    DoCastAOE(SPELL_FLAME_BREATH);
                    events.Repeat(23s);
                    break;
                case EVENT_WRACK:
                    DoCastAOE(SPELL_WRACK);
                    events.Repeat(61s);
                    break;
                case EVENT_TWILIGHT_SLICER:
                    DoCastAOE(SPELL_SUMMON_SLICERS);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    bool _respawned;
};

struct npc_sinestra_shadow_orb : public ScriptedAI
{
    npc_sinestra_shadow_orb(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        _events.ScheduleEvent(EVENT_AGGRO_CREATOR, 1s + 500ms);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_AGGRO_CREATOR:
                    DoCastSelf(SPELL_AGGRO_CREATOR);
                    /*
                    if (Unit* owner = me->ToTempSummon()->GetSummoner())
                    {
                        me->Attack(owner, true);
                        me->GetMotionMaster()->MoveChase(owner, ChaseRange(0.f, 0.f), ChaseAngle(0.f, 0.f));
                    }
                    */
                    DoZoneInCombat();
                    _events.ScheduleEvent(EVENT_ACTIVATE_SLICER, 2s + 400ms);
                    break;
                case EVENT_ACTIVATE_SLICER:
                    DoCastSelf(SPELL_TWILIGHT_PULSE_PERIODIC);
                    if (me->GetEntry() == NPC_SHADOW_ORB_PITCHER)
                        if (Creature* catcher = me->FindNearestCreature(NPC_SHADOW_ORB_CATCHER, 100.f))
                            DoCast(catcher, SPELL_TWILIGHT_SLICER);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
};

class spell_sinestra_call_flames : public SpellScript
{
    PrepareSpellScript(spell_sinestra_call_flames);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_FLAMES_DND });
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        GetHitUnit()->CastSpell(GetHitUnit(), SPELL_FLAMES_DND);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sinestra_call_flames::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_sinestra_wrack : public AuraScript
{
    PrepareAuraScript(spell_sinestra_wrack);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WRACK_DISPELLED });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        // Each tick adds exactly 50% of the previous value
        AuraEffect* effect = GetEffect(EFFECT_0);
        effect->SetAmount(CalculatePct(effect->GetAmount(), 150));
    }

    void HandleDispel(DispelInfo* /*dispelInfo*/)
    {
        Unit* target = GetUnitOwner();
        if (!target)
            return;

        int32 duration = GetDuration();

        target->CastCustomSpell(SPELL_WRACK_DISPELLED, SPELLVALUE_BASE_POINT1, duration, nullptr, TriggerCastFlags(TRIGGERED_FULL_MASK & (TRIGGERED_FULL_MASK & ~TRIGGERED_CAST_DIRECTLY)));
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sinestra_wrack::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        AfterDispel += AuraDispelFn(spell_sinestra_wrack::HandleDispel);
    }
};

class spell_sinestra_wrack_dispelled : public SpellScript
{
    PrepareSpellScript(spell_sinestra_wrack_dispelled);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_WRACK,
                SPELL_WRACK_DISPELLED,
            });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

        Unit* caster = GetCaster();

        targets.remove_if([caster](WorldObject* target)->bool
        {
            Unit* unit = target->ToUnit();
            if (!unit || unit == caster)
                return true;

            for (uint32 spellId : { SPELL_WRACK, SPELL_WRACK_DISPELLED })
            {
                uint32 difficultyId = sSpellMgr->GetSpellIdForDifficulty(spellId, caster);
                if (unit->HasAura(difficultyId))
                    return true;
            }

            return false;
        });

        if (targets.size() > 2)
            Trinity::Containers::RandomResize(targets, 2);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_wrack_dispelled::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_sinestra_wrack_dispelled_AuraScript : public AuraScript
{
    PrepareAuraScript(spell_sinestra_wrack_dispelled_AuraScript);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_WRACK,
                SPELL_WRACK_DISPELLED,
            });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        // Each tick adds exactly 50% of the previous value
        AuraEffect* effect = GetEffect(EFFECT_0);
        effect->SetAmount(CalculatePct(effect->GetAmount(), 150));
    }

    void HandleOriginalDamage(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Wrack returns to its original damage value after bouncing to another player
        uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_WRACK, GetUnitOwner());
        amount = sSpellMgr->AssertSpellInfo(spellId)->Effects[EFFECT_0].CalcValue();
    }

    void HandleDuration(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        GetAura()->SetDuration(aurEff->GetAmount());
    }

    void HandleDispel(DispelInfo* /*dispelInfo*/)
    {
        Unit* target = GetUnitOwner();
        if (!target)
            return;

        int32 duration = GetDuration();

        target->CastCustomSpell(SPELL_WRACK_DISPELLED, SPELLVALUE_BASE_POINT1, duration, nullptr, TriggerCastFlags(TRIGGERED_FULL_MASK & (TRIGGERED_FULL_MASK & ~TRIGGERED_CAST_DIRECTLY)));
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sinestra_wrack_dispelled_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sinestra_wrack_dispelled_AuraScript::HandleOriginalDamage, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        AfterEffectApply += AuraEffectApplyFn(spell_sinestra_wrack_dispelled_AuraScript::HandleDuration, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterDispel += AuraDispelFn(spell_sinestra_wrack_dispelled_AuraScript::HandleDispel);
    }
};

class spell_sinestra_summon_slicers : public SpellScript
{
    PrepareSpellScript(spell_sinestra_summon_slicers);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_SUMMON_SHADOW_ORB_CATCHER,
                SPELL_SUMMON_SHADOW_ORB_PITCHER
            });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

        /*
        targets.remove_if([this](WorldObject const* target)->bool
        {
            return target != GetCaster()->GetVictim();
        });

        if (targets.size() <= 1)
        {
            // Do not summon any orbs when we only have one valid targets
            targets.clear();
            return;
        }
        */

        if (targets.size() > 2)
            Trinity::Containers::RandomResize(targets, 2);
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (!_firstHit)
        {
            target->CastSpell(target, SPELL_SUMMON_SHADOW_ORB_CATCHER, true);
            //_firstHit = true;
        //}
        //else
            target->CastSpell(target, SPELL_SUMMON_SHADOW_ORB_PITCHER, true);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_summon_slicers::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_sinestra_summon_slicers::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
private:
    bool _firstHit = false;
};

class spell_sinestra_twilight_slicer : public SpellScript
{
    PrepareSpellScript(spell_sinestra_twilight_slicer);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_twilight_slicer::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class at_sinestra_intro : public AreaTriggerScript
{
public:
    at_sinestra_intro() : AreaTriggerScript("at_sinestra_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
            instance->SetData(DATA_AT_SINESTRA_INTRO, IN_PROGRESS);
        return true;
    }
};

void AddSC_boss_sinestra()
{
    RegisterBastionOfTwilightCreatureAI(boss_sinestra);
    RegisterBastionOfTwilightCreatureAI(npc_sinestra_shadow_orb);

    RegisterSpellScript(spell_sinestra_call_flames);
    RegisterAuraScript(spell_sinestra_wrack);
    RegisterSpellAndAuraScriptPair(spell_sinestra_wrack_dispelled, spell_sinestra_wrack_dispelled_AuraScript);
    RegisterSpellScript(spell_sinestra_summon_slicers);
    RegisterSpellScript(spell_sinestra_twilight_slicer);
    new at_sinestra_intro();
}
