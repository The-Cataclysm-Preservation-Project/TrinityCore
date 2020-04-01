/*
 * Copyright (C) 2017-2019 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2008-2014 Forgotten Lands <http://www.forgottenlands.eu/>
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
#include "SpellMgr.h"
#include "Player.h"
#include "PhasingHandler.h"
#include "PassiveAI.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "TemporarySummon.h"
#include "Map.h"
#include "Unit.h"
#include "Creature.h"
#include "bastion_of_twilight.h"
#include "SpellAuras.h"
#include "GameObject.h"
#include "GameObjectAI.h"


#define YELL_AGGRO  "We were fools to entrust an imbecile like Cho'gall with such a sacred duty. I will deal with you intruders myself!"
#define YELL_KILL_0 "My brood will feed on your bones!"
#define YELL_KILL_1 "Powerless..."
#define YELL_SUMMON "Feed, children! Take your fill from their meaty husks!"

enum spells
{
    SPELL_WRACK                                   = 89421,
    SPELL_WRACK_JUMP                              = 89435,
    SPELL_FLAME_BREATH                            = 90125,
    SPELL_TWILIGHT_SLICER                         = 92851,
    SPELL_TWILIGHT_PULSE                          = 92957,
    SPELL_DRAINED                                 = 89350,
    SPELL_PURPLE_BEAM                             = 88518,
    SPELL_SIN_TWILIGHT_BLAST                      = 89280,
    SPELL_TWILIGHT_SPIT                           = 89299,
    SPELL_TWILIGHT_ESSENCE                        = 89284,
    SPELL_TWILIGHT_POWER                          = 87220,
    SPELL_FIERY_RESOLVE                           = 87221,
    SPELL_MANA_BARRIER                            = 87299,
    SPELL_PYRRHIC_FOCUS                           = 87323,
    SPELL_TWILIGHT_CARAPACE                       = 87654,
    SPELL_ESSENCE_OF_THE_RED                      = 87946,
    SPELL_ABSORB_ESSENCE                          = 90107,
    SPELL_TWILIGHT_BREATH                         = 110212,
    SPELL_UNLEASH_ESSENCE                         = 90028,
};

enum events
{
    EVENT_WRACK = 1,
    EVENT_FLAME_BREATH,
    EVENT_TWILIGHT_SLICER,
    EVENT_RESET_ORBS,
    EVENT_ORB_START_CHANNEL,
    EVENT_CHECK_MELEE,
    EVENT_WHELP,
    EVENT_START_MAGIC_FIGHT,
    EVENT_TWILIGHT_DRAKE,
    EVENT_SPITECALLER,
    EVENT_FLAMES_TRIGGER,
    EVENT_EXPOSE_EGG,
    EVENT_BLOCK_EGG,
};

enum sharedDatas
{
    DATA_PRIVATE_ORB0 = 0,
};

enum Phases
{
    PHASE_NONE = 0,
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3
};

enum sinestraActions
{
    ACTION_START_PHASE_3 = 1,
};

Position const spawnPos[9] =
{
    {-985.92f, -819.84f, 438.59f, 0.87f},
    {-975.81f, -815.15f, 438.59f, 1.02f},
    {-962.71f, -805.22f, 438.59f, 1.77f},
    {-948.02f, -799.14f, 438.59f, 2.09f},
    {-943.09f, -787.05f, 438.59f, 2.59f},
    {-994.34f, -739.91f, 438.59f, 5.39f},
    {-1005.45f, -746.29f, 438.59f, 5.51f},
    {-1018.57f, -760.42f, 438.59f, 5.71f},
    {-1029.42f, -774.76f, 438.59f, 0.22f},
};

Position const flamesPos[7] =
{
    {-895.89f, -765.88f, 442.16f, 0.22f},
    {-912.87f, -770.63f, 440.43f, 0.22f},
    {-994.33f, -665.81f, 440.45f, 0.22f},
    {-999.33f, -693.72f, 440.87f, 0.22f},
    {-932.12f, -774.44f, 439.78f, 0.22f},
    {-998.55f, -711.15f, 439.33f, 0.22f},
    {-996.73f, -731.15f, 438.30f, 0.22f},
};

class boss_sinestra : public CreatureScript
{
    public:
        boss_sinestra() : CreatureScript("boss_sinestra") {}

        struct boss_sinestraAI : public BossAI
        {
            boss_sinestraAI(Creature * creature) : BossAI(creature, DATA_SINESTRA), summons(me)
            {
                Initialize();
            }

            InstanceScript* instance;
            EventMap events;
            Creature* orbs[2];
            Creature* eggs[2];
            SummonList summons;
            uint8 killedEggs;

            void Initialize()
            {
                orbs[0] = NULL;
                orbs[1] = NULL;
                eggs[0] = NULL;
                eggs[1] = NULL;
                killedEggs = 0;
                events.SetPhase(PHASE_NONE);
            }

            void Reset() override
            {
                _Reset();

                summons.DespawnAll();

                std::list<Creature*> unitList;
                me->GetCreatureListWithEntryInGrid(unitList, NPC_TWILIGHT_WHELP, 250.0f);
                for (std::list<Creature*>::const_iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                {// despawn twilight whelps
                    (*itr)->DespawnOrUnsummon();
                }
                me->GetCreatureListWithEntryInGrid(unitList, NPC_ESSENCE, 250.0f);
                for (std::list<Creature*>::const_iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                {// despawn twilight essence
                    (*itr)->DespawnOrUnsummon();
                }

                Initialize();
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                _JustEngagedWith();

                // Sinestra begin fight with 60 % hp
                events.SetPhase(PHASE_ONE);
                me->SetHealth(me->CountPctFromMaxHealth(60));
                // Sinestra damage is reduced by 40% in first phase
                me->AddAura(SPELL_DRAINED, me);

                me->Yell(YELL_AGGRO, LANG_UNIVERSAL, 0);

                // Summon first egg
                if (Creature* egg = me->SummonCreature(NPC_PULSING_TWILIGHT_EGG, -993.72f, -669.54f, 440.20f, 4.57f, TEMPSUMMON_CORPSE_DESPAWN))
                {
                    eggs[0] = egg;
                    egg->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                    egg->SetReactState(REACT_PASSIVE);
                    egg->AttackStop();
                    egg->StopMoving();
                    me->AddAura(SPELL_TWILIGHT_CARAPACE, egg);
                }

                // Summon second egg
                if (Creature* egg = me->SummonCreature(NPC_PULSING_TWILIGHT_EGG, -901.25f, -768.70f, 441.35f, 3.33f, TEMPSUMMON_CORPSE_DESPAWN))
                {
                    eggs[1] = egg;
                    egg->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                    egg->SetReactState(REACT_PASSIVE);
                    egg->AttackStop();
                    egg->StopMoving();
                    me->AddAura(SPELL_TWILIGHT_CARAPACE, egg);
                }

                events.ScheduleEvent(EVENT_WRACK, 15s, PHASE_ONE);
                events.ScheduleEvent(EVENT_FLAME_BREATH, 20s, PHASE_ONE);
                events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 28s, PHASE_ONE);
                events.ScheduleEvent(EVENT_CHECK_MELEE, 2s, PHASE_ONE);
                events.ScheduleEvent(EVENT_WHELP, 45s, PHASE_ONE);
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                DoZoneInCombat(summon);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                // Summon the loot chest
                 me->SummonGameObject(GO_SINESTRA_CHEST, Position(-962.91f, -749.71f, 438.59f, 0.f), QuaternionData(), DAY);
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                me->Yell(urand(0, 1) == 1 ? YELL_KILL_0 : YELL_KILL_1, LANG_UNIVERSAL, 0);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
            {
                if (me->GetHealthPct() <= 30.0f && events.IsInPhase(PHASE_ONE))
                {
                    events.SetPhase(PHASE_TWO);
                    me->RemoveAura(SPELL_DRAINED);
                    DoCast(SPELL_MANA_BARRIER);
                    events.ScheduleEvent(EVENT_START_MAGIC_FIGHT, 2s, PHASE_TWO);
                    events.ScheduleEvent(EVENT_FLAMES_TRIGGER, 5s, PHASE_TWO);
                    events.ScheduleEvent(EVENT_EXPOSE_EGG, 10s, PHASE_TWO);
                    events.ScheduleEvent(EVENT_EXPOSE_EGG, 32s, PHASE_TWO);
                    events.ScheduleEvent(EVENT_TWILIGHT_DRAKE, urand(18000,30000), PHASE_TWO);
                    events.ScheduleEvent(EVENT_SPITECALLER, urand(18000,35000), PHASE_TWO);
                }
            }

            ObjectGuid GetGuidData(uint32 type)
            {
                switch (type)
                {
                    case DATA_PRIVATE_ORB0:
                        if (orbs[0])
                            return orbs[0]->GetGUID();
                        break;
                }

                return ObjectGuid::Empty;
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                if (summon->GetEntry() == NPC_PULSING_TWILIGHT_EGG)
                {
                    killedEggs++;

                    if (killedEggs >= 2 && events.IsInPhase(PHASE_TWO))
                    {
                        events.SetPhase(PHASE_THREE);
                        me->CastStop();
                        me->RemoveAura(SPELL_MANA_BARRIER);

                        if (Creature* calen = me->FindNearestCreature(NPC_CALEN, 100.0f, true))
                        {
                            calen->CastStop();
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                calen->CastSpell(pTarget, SPELL_ESSENCE_OF_THE_RED, true);
                            calen->DisappearAndDie();
                        }

                        events.ScheduleEvent(EVENT_WRACK, 15s, PHASE_THREE);
                        events.ScheduleEvent(EVENT_FLAME_BREATH, 20s, PHASE_THREE);
                        events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 28s, PHASE_THREE);
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 2s, PHASE_THREE);
                        events.ScheduleEvent(EVENT_WHELP, 45s, PHASE_THREE);
                    }
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) && !events.IsInPhase(PHASE_TWO))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WRACK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0.0f, 100.0f, true, 0))
                                DoCast(target, SPELL_WRACK, true);

                            events.ScheduleEvent(EVENT_WRACK, 75s);
                            break;
                        case EVENT_FLAME_BREATH:
                            DoCastAOE(SPELL_FLAME_BREATH);
                            events.ScheduleEvent(EVENT_FLAME_BREATH, 20s);
                            break;
                        case EVENT_TWILIGHT_SLICER:
                            for (uint8 i = 0; i < 2; i++)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0.0f, 100.0f, true, -SPELL_PURPLE_BEAM))
                                {
                                    Position pos = target->GetPosition();
                                    float width = frand(5, 20);
                                    float degree = frand(0, 6.28f);
                                    pos.Relocate(pos.GetPositionX() + cos(degree)*width, pos.GetPositionY() + sin(degree)*width);
                                    if (Creature* orb = me->SummonCreature(NPC_SHADOW_ORB, pos, TEMPSUMMON_TIMED_DESPAWN, 15s, 0))
                                    {
                                        if (!orbs[0])
                                        {
                                            orbs[0] = orb;
                                            if (instance)
                                                instance->SetGuidData(DATA_ORB_0, orb->GetGUID());
                                        }
                                        else
                                        {
                                            orbs[1] = orb;

                                            if (instance)
                                                instance->SetGuidData(DATA_ORB_1, orb->GetGUID());
                                        }

                                        orb->SetFlag(UNIT_FIELD_FLAGS, UnitFlags(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE));
                                        orb->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                                        orb->AddThreat(target, 1000000.0f);
                                        orb->Attack(target, true);

                                        // Twilight pulse!
                                        orb->CastSpell(orb, SPELL_TWILIGHT_PULSE, true);
                                        if (Aura* aur = orb->AddAura(SPELL_PURPLE_BEAM, target))
                                            aur->SetDuration(60000);
                                    }
                                }
                            }
                            events.ScheduleEvent(EVENT_RESET_ORBS, 18s);
                            events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 28s);
                            events.ScheduleEvent(EVENT_ORB_START_CHANNEL, 35s);
                            break;
                        case EVENT_ORB_START_CHANNEL:
                            if (orbs[0] && orbs[1])
                            {
                                orbs[1]->CastSpell(orbs[0], SPELL_TWILIGHT_SLICER, true);
                                orbs[1]->ClearUnitState(UNIT_STATE_CASTING);

                                if (orbs[1]->GetVictim())
                                    orbs[1]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                                    orbs[1]->GetMotionMaster()->MoveChase(orbs[1]->GetVictim());

                                if (orbs[0]->GetVictim())
                                    orbs[0]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                                    orbs[0]->GetMotionMaster()->MoveChase(orbs[0]->GetVictim());
                            }
                            break;
                        case EVENT_RESET_ORBS:
                            orbs[0] = NULL;
                            orbs[1] = NULL;
                            break;
                        case EVENT_CHECK_MELEE:
                            if(Unit* victim = me->GetVictim())
                            {
                                if (me->GetDistance2d(victim) >= 5.0f) {
                                    if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 0.0f, 100.0f, true, 0))
                                        DoCast(target, SPELL_SIN_TWILIGHT_BLAST, false);
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_MELEE, 2s);
                            break;
                        case EVENT_WHELP:
                            for (uint8 i = 0; i < 5; i++)
                            {
                                uint8 posId = urand(0, 8);
                                me->SummonCreature(NPC_TWILIGHT_WHELP, spawnPos[posId]);
                            }

                            me->Yell(YELL_SUMMON, LANG_UNIVERSAL, 0);
                            events.ScheduleEvent(EVENT_WHELP, 45s);
                            break;
                        case EVENT_FLAMES_TRIGGER:
                            for (uint8 i = 0; i < 7; i++)
                            {
                                uint8 posId = i;
                                me->SummonCreature(NPC_FLAME_TRIGGER, flamesPos[posId]);
                            }
                            break;
                        case EVENT_TWILIGHT_DRAKE:
                            me->SummonCreature(NPC_TWILIGHT_DRAKE, spawnPos[urand(0, 8)]);
                            events.ScheduleEvent(EVENT_TWILIGHT_DRAKE, urand(18000,30000), PHASE_TWO);
                            break;
                        case EVENT_SPITECALLER:
                            me->SummonCreature(NPC_SPITCALLER, spawnPos[urand(0, 8)]);
                            events.ScheduleEvent(EVENT_SPITECALLER, urand(18000,30000), PHASE_TWO);
                            break;
                        case EVENT_START_MAGIC_FIGHT:
                            if (Creature* calen = me->SummonCreature(NPC_CALEN, -1009.35f, -801.97f, 438.59f, 0.81f))
                            {
                                calen->Yell("Sintharia! Your master owes me a great debt... one that I intend to extract from his consort's hide!", LANG_UNIVERSAL, 0);
                                // calen->CastSpell(calen, SPELL_PYRRHIC_FOCUS, true);
                                calen->AddAura(SPELL_PYRRHIC_FOCUS, calen);

                                if (Creature* target = me->FindNearestCreature(NPC_LASER_TRIGGER, 100.0f, true))
                                {
                                    // target->GetMotionMaster()->MoveTakeoff(0, target->GetHomePosition());
                                    calen->setRegeneratingHealth(false);
                                    me->CastSpell(target, SPELL_TWILIGHT_POWER, false);
                                    calen->CastSpell(target, SPELL_FIERY_RESOLVE, false);
                                }
                            }
                            me->Yell("This will be your tomb as well as theirs!", LANG_UNIVERSAL, 0);
                            break;
                        case EVENT_EXPOSE_EGG:
                            // Expose eggs!
                            if(eggs[0])
                                eggs[0]->RemoveAura(SPELL_TWILIGHT_CARAPACE);
                            if(eggs[1])
                                eggs[1]->RemoveAura(SPELL_TWILIGHT_CARAPACE);
                            events.ScheduleEvent(EVENT_EXPOSE_EGG, 60s, PHASE_TWO);
                            break;
                        case EVENT_BLOCK_EGG:
                            //Reblock eggs
                            if(eggs[0])
                                eggs[0]->AddAura(SPELL_TWILIGHT_CARAPACE, eggs[0]);
                            if(eggs[1])
                                eggs[1]->AddAura(SPELL_TWILIGHT_CARAPACE, eggs[1]);
                            events.ScheduleEvent(EVENT_BLOCK_EGG, 60s, PHASE_TWO);
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBastionOfTwilightAI<boss_sinestraAI>(creature);
        }
};

enum whelpEvents
{
    EVENT_SPIT = 1,
};

enum whelpActions
{
    ACTION_SET_AS_RESPWANED = 1,
};

class npc_sinestra_twilight_whelp : public CreatureScript
{
    public:
        npc_sinestra_twilight_whelp() : CreatureScript("npc_sinestra_twilight_whelp")
        { }

        struct npc_sinestra_twilight_whelpAI : public ScriptedAI
        {
            npc_sinestra_twilight_whelpAI(Creature * creature) : ScriptedAI(creature)
            {
                Initialize();
            }

            InstanceScript* pInstance;
            EventMap events;
            bool respawned;

            void Initialize()
            {
                events.ScheduleEvent(EVENT_SPIT, urand(10000, 35000));
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                respawned = false;
            }

            void DoAction(int32 actionId) override
            {
                switch (actionId)
                {
                    case ACTION_SET_AS_RESPWANED:
                        respawned = true;
                        break;
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (respawned)
                    return;

                Position pos = me->GetPosition();
                if (Creature* essence = me->SummonCreature(NPC_ESSENCE, pos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0))
                {
                    DoZoneInCombat(essence);
                    essence->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                    essence->SetReactState(REACT_PASSIVE);
                    essence->AttackStop();
                    essence->StopMoving();
                }
            }

            void UpdateAI(uint32 uiDiff) override
            {
                events.Update(uiDiff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SPIT:
                            DoCastVictim(SPELL_TWILIGHT_SPIT);
                            events.ScheduleEvent(EVENT_SPIT, urand(35000, 50000));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBastionOfTwilightAI<npc_sinestra_twilight_whelpAI>(creature);
        }
};


enum sinestraAddEvents
{
    EVENT_TWILIGHT_BREATH = 1,
    EVENT_UNLEASH_ESSENCE,
};

class npc_sinestra_add : public CreatureScript
{
    public:
        npc_sinestra_add() : CreatureScript("npc_sinestra_add")
        { }

        struct npc_sinestra_addAI : public ScriptedAI
        {
            npc_sinestra_addAI(Creature * creature) : ScriptedAI(creature) {}

            InstanceScript* instance;
            EventMap events;
            bool respawned;

            void Reset() override
            {
                events.Reset();
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                if (me->GetEntry() == NPC_TWILIGHT_DRAKE)
                    events.ScheduleEvent(EVENT_TWILIGHT_BREATH, urand(7000, 10000));
                else
                    events.ScheduleEvent(EVENT_UNLEASH_ESSENCE, urand(22000, 30000));
            }

            void UpdateAI(uint32 uiDiff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(uiDiff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TWILIGHT_BREATH:
                            DoCastVictim(SPELL_TWILIGHT_BREATH);
                            events.ScheduleEvent(EVENT_TWILIGHT_BREATH, urand(15000, 20000));
                            break;
                        case EVENT_UNLEASH_ESSENCE:
                            DoCastAOE(SPELL_UNLEASH_ESSENCE);
                            events.ScheduleEvent(EVENT_UNLEASH_ESSENCE, urand(22000, 30000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBastionOfTwilightAI<npc_sinestra_addAI>(creature);
        }
};

class spell_sinestra_wreck : public SpellScriptLoader
{
    public:
        spell_sinestra_wreck() : SpellScriptLoader("spell_sinestra_wreck") { }

        class spell_sinestra_wreck_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sinestra_wreck_AuraScript);

            void Apply (AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTargetApplication())
                    return;

                if (!GetTargetApplication()->GetBase())
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                {
                    if (Unit* target = GetTarget())
                    {
                        CustomSpellValues values;
                        values.AddSpellMod(SPELLVALUE_MAX_TARGETS, 2);
                        values.AddSpellMod(SPELLVALUE_BASE_POINT1, GetTargetApplication()->GetBase()->GetDuration());
                        target->CastCustomSpell(SPELL_WRACK_JUMP, values, nullptr, TRIGGERED_FULL_MASK, nullptr, nullptr, GetCasterGUID());
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_sinestra_wreck_AuraScript::Apply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_sinestra_wreck_AuraScript();
        }
};


class spell_sinestra_wrack_jump : public SpellScriptLoader
{
    public:
        spell_sinestra_wrack_jump() :  SpellScriptLoader("spell_sinestra_wrack_jump") { }

        class spell_sinestra_wrack_jump_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sinestra_wrack_jump_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));

                if (targets.size() < 3)
                    return;

                targets.resize(2);
            }

            void HandleBeforeHit()
            {
                if (!GetHitUnit())
                    return;

                if (!GetCaster())
                    return;

                if (!GetOriginalCaster())
                    return;

                if (Aura* debuff = GetOriginalCaster()->AddAura(89421, GetHitUnit()))
                    if (Aura* aura = GetExplTargetUnit()->GetAura(89421, GetCaster()->GetGUID()))
                        debuff->SetDuration(aura->GetDuration());
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_wrack_jump_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_wrack_jump_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
                BeforeHit += SpellHitFn(spell_sinestra_wrack_jump_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_sinestra_wrack_jump_SpellScript();
        }
};

class OrientationCheck : public std::unary_function<Unit*, bool>
{
    public:
        explicit OrientationCheck(Unit* _orb1, Unit* _orb2) : orb1(_orb1), orb2(_orb2) { }
        bool operator()(WorldObject* object)
        {
            return !object->IsInBetween(orb1, orb2, 2.0f);
        }

    private:
        Unit* orb1;
        Unit* orb2;
};

class spell_sinestra_twilight_slicer : public SpellScriptLoader
{
    public:
        spell_sinestra_twilight_slicer() :  SpellScriptLoader("spell_sinestra_twilight_slicer") { }

        class spell_sinestra_twilight_slicer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sinestra_twilight_slicer_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                // Select Other orb, and filter targets between twos
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                {
                    if (Unit* orb0 = instance->GetCreature(DATA_ORB_0))
                    {
                        if (Unit* orb1 = instance->GetCreature(DATA_ORB_1))
                        {
                            targets.remove_if(OrientationCheck(orb1, orb0));
                        }
                    }
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_twilight_slicer_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_sinestra_twilight_slicer_SpellScript();
        }
};

class ExactDistanceCheck
{
    public:
        ExactDistanceCheck(Unit* source, float dist) : _source(source), _dist(dist) {}

        bool operator()(WorldObject* unit)
        {
            return _source->GetExactDist2d(unit) > _dist;
        }

    private:
        Unit* _source;
        float _dist;
};

class spell_sinestra_twilight_essence : public SpellScriptLoader
{
    public:
        spell_sinestra_twilight_essence() : SpellScriptLoader("spell_sinestra_twilight_essence") { }

        class spell_sinestra_twilight_essence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sinestra_twilight_essence_SpellScript);

            void CorrectRange1(std::list<WorldObject*>& targets)
            {
                targets.remove_if(ExactDistanceCheck(GetCaster(), 5.0f * GetCaster()->GetObjectScale()));

                // Remove also the nearest dragon
                if (GetCaster()->ToTempSummon() && GetCaster()->ToTempSummon()->GetSummoner())
                    targets.remove(GetCaster()->ToTempSummon()->GetSummoner());
            }

            void CorrectRange2(std::list<WorldObject*>& targets)
            {
                targets.remove_if(ExactDistanceCheck(GetCaster(), 5.0f * GetCaster()->GetObjectScale()));
            }

            void HandleHit(SpellEffIndex /*effIndex*/)
            {
                if (!GetHitUnit())
                    return;

                if (!GetHitUnit()->ToCreature())
                    return;

                if (GetHitUnit()->GetEntry() == NPC_TWILIGHT_WHELP)
                {
                    if (GetHitUnit()->IsAlive())
                        return;

                    if (InstanceScript* instance = GetHitUnit()->GetInstanceScript())
                    {
                        if (Creature* sinestra = instance->GetCreature(DATA_SINESTRA))
                        {
                            GetHitUnit()->ToCreature()->Respawn();
                            sinestra->AI()->DoZoneInCombat(GetHitUnit()->ToCreature());
                            GetHitUnit()->ToCreature()->AI()->DoAction(ACTION_SET_AS_RESPWANED);
                        }
                    }
                    return;
                }

                if (GetHitUnit()->GetEntry() == NPC_TWILIGHT_DRAKE)
                {
                    // Remove two stack of aura
                    if (GetCaster())
                    {
                        if (Aura* essence = GetCaster()->GetAura(89288, GetCaster()->GetGUID()))
                        {
                            uint8 stacks = essence->GetStackAmount();

                            if (stacks <= 3)
                            {
                                GetCaster()->ToCreature()->DisappearAndDie();
                                return;
                            }

                            essence->SetStackAmount(stacks - 3);
                        }
                        GetHitUnit()->AddAura(SPELL_ABSORB_ESSENCE, GetHitUnit());
                    }
                    return;
                }

            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_twilight_essence_SpellScript::CorrectRange1, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sinestra_twilight_essence_SpellScript::CorrectRange2, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_sinestra_twilight_essence_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_sinestra_twilight_essence_SpellScript();
        }
};

class spell_sinestra_phyrric_focus : public SpellScriptLoader
{
    public:
        spell_sinestra_phyrric_focus() : SpellScriptLoader("spell_sinestra_phyrric_focus") { }

        class spell_sinestra_phyrric_focus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sinestra_phyrric_focus_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetHealthPct() <= 1)
                    {
                        if (InstanceScript* instance = caster->GetInstanceScript())
                        {
                            if (Creature* sinestra = instance->GetCreature(DATA_SINESTRA))
                            {
                                sinestra->CastStop();
                                sinestra->AI()->DoAction(ACTION_START_PHASE_3);
                            }
                        }
                        caster->Yell("All is lost.... Forgive me, my Queen....", LANG_UNIVERSAL, 0);
                        caster->DealDamage(caster, caster->GetMaxHealth() + 100);
                        return;
                    }
                    caster->DealDamage(caster, caster->GetMaxHealth() * 0.01f);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sinestra_phyrric_focus_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_sinestra_phyrric_focus_AuraScript();
        }
};

void AddSC_boss_sinestra()
{
    new boss_sinestra();
    new npc_sinestra_twilight_whelp();
    new npc_sinestra_add();
    new spell_sinestra_wreck();
    new spell_sinestra_wrack_jump();
    new spell_sinestra_twilight_slicer();
    new spell_sinestra_twilight_essence();
    new spell_sinestra_phyrric_focus();
}
