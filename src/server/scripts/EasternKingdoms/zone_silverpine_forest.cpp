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

#include "CellImpl.h"
#include "CombatAI.h"
#include "CreatureAIImpl.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GridNotifiers.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SharedDefines.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Unit.h"
#include "Vehicle.h"

enum TalksFromCreatures
{
    TALK_0              = 0,
    TALK_1,
    TALK_2,
    TALK_3,
    TALK_4,
    TALK_5,
    TALK_6,
    TALK_7,
    TALK_8,
    TALK_9,
    TALK_10,
    TALK_11,
    TALK_12,
    TALK_13,
    TALK_14,
    TALK_15,
    TALK_16,
    TALK_17,
    TALK_18,
    TALK_19,
    TALK_20,
    TALK_21,
};

enum PhasesforQuestline
{
    PHASE_FORSAKEN_HIGH_COMMAND_INTRODUCTION            = 264, 
    PHASE_WAITING_TO_EXSANGUINATE                       = 265,
    PHASE_NO_ESCAPE                                     = 266, // NYI
    PHASE_FOR_LORDAERON                                 = 267,
    PHASE_THE_WATERS_RUN_RED                            = 273, // NYI
    PHASE_GILNEAS_ACT_I                                 = 274, // After accepting To Forsaken Forward Command
    PHASE_GILNEAS_ACT_II                                = 275, // Most likely after during Arthura and Belmont scort quest
    PHASE_GILNEAS_ACT_III                               = 276, // Most likely after the Gilneas part is completed or rewarded
    PHASE_THE_FORSAKEN_FRONT_I                          = 277, // Most likely pre Rise Godfrey quest
    PHASE_THE_FORSAKEN_FRONT_II                         = 278, // Most likely after Rise Godfrey quest
    PHASE_AMBER_MILL_I                                  = 279, // During Transdimensional Warfare: Chapter I and II quests
    PHASE_AMBER_MILL_II                                 = 280, // After Transdimensional Warfare: Chapter III is completed but not rewarded
    PHASE_AMBER_MILL_III                                = 281, // After Transdimensional Warfrase: Chapter III is rewarded
    PHASE_THE_FORSAKEN_FRONT_III                        = 284, // During Cities in Dust quest
    PHASE_THE_FORSAKEN_FRONT_IV                         = 289  // After Cities in Dust is completed or rewarded
};

enum SilverpineTransports
{
    NPC_HORDE_ENGINEER                      = 44734,
    NPC_SUBDUED_FOREST_ETTIN                = 44737,
    NPC_FORSAKEN_TROOPER_F                  = 44732,
    NPC_FORSAKEN_TROOPER_M                  = 44733,

    SPELL_CHAIN_RIGHT                       = 83467,
    SPELL_CHAIN_LEFT                        = 83464,
    SPELL_EJECT_PASSENGERS_3_8              = 83477,

    EVENT_TO_FORSAKEN_HIGH_COMMAND          = 1,
    EVENT_TO_THE_SEPULCHER                  = 2,
    EVENT_TO_THE_FORSAKEN_FRONT             = 3,
    EVENT_TO_DESPAWN                        = 4,
    EVENT_YELL_1                            = 5,
    EVENT_YELL_2                            = 6,
    EVENT_YELL_3                            = 7,

    DATA_WAYPOINT_TO_FORSAKEN_HIGH          = 447310,
    DATA_WAYPOINT_TO_SEPULCHER              = 447311,
    DATA_WAYPOINT_TO_FORSAKEN_FRONT         = 447312,
    DATA_WAYPOINT_TO_DESPAWN                = 447313,

    DATA_ANIMKIT_HORDE_ENGINEER             = 91298
};

// Horde Hauler - 44731
struct npc_silverpine_horde_hauler : public ScriptedAI
{
    npc_silverpine_horde_hauler(Creature* creature) : ScriptedAI(creature)
    {
        Initilize();
    }

    void Initilize()
    {
        if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
        {
            if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
            {
                engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT, true);
                engineer->CastSpell(ettin, SPELL_CHAIN_LEFT, true);
                engineer->AddAura(DATA_ANIMKIT_HORDE_ENGINEER, engineer);

                _events.ScheduleEvent(EVENT_TO_FORSAKEN_HIGH_COMMAND, 5s);
            }
        }
    }

    void Reset() override
    {
        _events.Reset();
    }

    void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
    {
        if (!apply)
            return;

        _playerGUID = passenger->GetGUID();

        if (seatId == 2)
        {
            if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    engineer->AI()->Talk(TALK_0, player);
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOINT_TO_FORSAKEN_HIGH)
        {
            if (waypointId == 17)
            {
                _events.ScheduleEvent(EVENT_TO_THE_SEPULCHER, 15s);
                _events.ScheduleEvent(EVENT_YELL_1, 1s);
            }
        }

        if (pathId == DATA_WAYPOINT_TO_SEPULCHER)
        {
            if (waypointId == 207)
            {
                _events.ScheduleEvent(EVENT_TO_THE_FORSAKEN_FRONT, 15s);
                _events.ScheduleEvent(EVENT_YELL_2, 1s);
            }
        }

        if (pathId == DATA_WAYPOINT_TO_FORSAKEN_FRONT)
        {
            if (waypointId == 165)
            {
                _events.ScheduleEvent(EVENT_TO_DESPAWN, 15s);
                _events.ScheduleEvent(EVENT_YELL_3, 1s);
            }
        }

        if (pathId == DATA_WAYPOINT_TO_DESPAWN)
        {
            if (waypointId == 32)
                me->DespawnOrUnsummon(1s);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TO_FORSAKEN_HIGH_COMMAND:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, SPELL_CHAIN_LEFT, true);
                            engineer->AddAura(DATA_ANIMKIT_HORDE_ENGINEER, engineer);

                            me->GetMotionMaster()->MovePath(DATA_WAYPOINT_TO_FORSAKEN_HIGH, false);
                        }
                    }

                    break;
                }

                case EVENT_YELL_1:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(TALK_1);
                    break;
                }

                case EVENT_TO_THE_SEPULCHER:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, SPELL_CHAIN_LEFT, true);
                            engineer->AddAura(DATA_ANIMKIT_HORDE_ENGINEER, engineer);

                            me->GetMotionMaster()->MovePath(DATA_WAYPOINT_TO_SEPULCHER, false);
                        }
                    }

                    break;
                }

                case EVENT_YELL_2:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(TALK_2);
                    break;
                }

                case EVENT_TO_THE_FORSAKEN_FRONT:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, SPELL_CHAIN_LEFT, true);
                            engineer->AddAura(DATA_ANIMKIT_HORDE_ENGINEER, engineer);

                            me->GetMotionMaster()->MovePath(DATA_WAYPOINT_TO_FORSAKEN_FRONT, false);
                        }
                    }

                    break;
                }

                case EVENT_YELL_3:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(TALK_3);

                    me->CastSpell(me, SPELL_EJECT_PASSENGERS_3_8, true);

                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    break;
                }

                case EVENT_TO_DESPAWN:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, SPELL_CHAIN_LEFT, true);
                            engineer->AddAura(DATA_ANIMKIT_HORDE_ENGINEER, engineer);

                            me->GetMotionMaster()->MovePath(DATA_WAYPOINT_TO_DESPAWN, false);
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

enum QuestTheGilneasLiberationFront
{
    NPC_FORSAKEN_TROOPER1           = 44791,
    NPC_FORSAKEN_TROOPER2           = 44792,
    NPC_WORGEN_RENEGATE             = 44793,

    SPELL_HEARTSTRIKE               = 84182,
    SPELL_DARKENED                  = 73307,
    SPELL_KILL_ME_PERIODIC          = 84181,
    SPELL_FLURRY_OF_CLAWS_CHANNEL   = 80365,
    SPELL_FLURRY_OF_CLAWS           = 80367,

    EVENT_FLURRY_OF_CLAWS           = 102,
    EVENT_SPELL_DAMAGE,

    MOVE_TO_HOMEPOSITION,
};

// Worgen Renegade - 44793
struct npc_silverpine_worgen_renegade : public ScriptedAI
{
    npc_silverpine_worgen_renegade(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();

        if (me->IsSummon())
            me->CastSpell(me, SPELL_KILL_ME_PERIODIC, true);

        me->RemoveAura(SPELL_DARKENED);

        me->SetReactState(REACT_AGGRESSIVE);
    }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (!caster)
            return;

        if (spell->Id == SPELL_HEARTSTRIKE)
            me->AddAura(SPELL_DARKENED, me);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_FLURRY_OF_CLAWS, 3s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FLURRY_OF_CLAWS:
                {
                    DoCastVictim(SPELL_FLURRY_OF_CLAWS_CHANNEL);

                    _events.ScheduleEvent(EVENT_FLURRY_OF_CLAWS, 16s, 18s);
                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

// Flurry of Claws - 80365
class spell_silverpine_flurry_of_claws : public AuraScript
{
    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        PreventDefaultAction();

        GetCaster()->CastSpell(GetTarget(), GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
    }

    void Register() override
    {
        OnEffectPeriodic.Register(&spell_silverpine_flurry_of_claws::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Forsaken Trooper - 44791, 44792
struct npc_silverpine_forsaken_trooper : public ScriptedAI
{
    npc_silverpine_forsaken_trooper(Creature* creature) : ScriptedAI(creature), _spellId(0), _randomSpellIndex(0) { }

    void Reset() override
    {
        _events.Reset();

        if (urand(0, 1) == 0)
            Talk(TALK_0);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_SPELL_DAMAGE, 5s, 8s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_SPELL_DAMAGE:
                {
                    _spellId = me->GetCreatureTemplate()->spells[_randomSpellIndex];

                    if (!_spellId)
                    {
                        _randomSpellIndex = 0;

                        _spellId = me->GetCreatureTemplate()->spells[_randomSpellIndex];
                    }

                    if (!_spellId)
                        break;

                    if (++_randomSpellIndex > 8)
                        _randomSpellIndex = 0;

                    DoCastVictim(_spellId);

                    _events.ScheduleEvent(EVENT_SPELL_DAMAGE, 5s, 8s);
                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    uint32 _spellId;
    uint8 _randomSpellIndex;
};

enum QuestTheWarchiefCometh
{
    QUEST_THE_WARCHIEF_COMETH           = 26965,

    NPC_LADY_SYLVANAS_WINDRUNNER_44365  = 44365,
    NPC_AGATHA_44608                    = 44608,
    NPC_GRAND_EXECUTOR_MORTUUS          = 44615,
    NPC_MALE_FALLEN_HUMAN               = 44592,
    NPC_FEMALE_FALLEN_HUMAN             = 44593,
    NPC_PORTAL_FROM_ORGRIMMAR           = 44630,
    NPC_GARROSH_HELLSCREAM              = 44629,
    NPC_HIGH_WARLORD_CROMUSH            = 44640,
    NPC_HELLSCREEMS_ELITE               = 44636,
    NPC_QUEST_MONSTER_CREDIT            = 44629,
    NPC_FORSAKEN_WARHORSE_UNPHASED      = 73595,

    SPELL_RAISE_FORSAKEN                = 83173,
    SPELL_AIR_REVENANT_ENTRANCE         = 55761,
    SPELL_SIMPLE_TELEPORT               = 12980,
    SPELL_WELCOME_TO_SILVERPINE_CREDIT  = 83384,

    EVENT_AGATHA_RAISE_FORSAKEN         = 201,
    EVENT_TALK_SEQUENCE                 = 301,
    EVENT_SUMMON_PORTAL                 = 401,
    EVENT_SUMMON_GARROSH,

    ACTION_START_WALKING                = 1,
    ACTION_START_ANIM                   = 101,
    EVENT_START_ANIM_1                  = 102,

    DATA_ANIMKIT_SYLV_1                 = 595,
    DATA_ANIMKIT_SYLV_2                 = 606,
    DATA_ANIMKIT_SYLV_3                 = 609,
    DATA_ANIMKIT_GARROSH_1              = 662,
    DATA_ANIMKIT_GARROSH_2              = 595,
    DATA_ANIMKIT_FALLEN_HUMAN           = 721,

    PLAYER_GUID                         = 99999,

    MOVE_CROMUSH_TO_SYLVANAS            = 5405701,
    MOVE_CROMUSH_TO_HOME                = 5405702,
    MOVE_GARROSH_TO_HOME                = 5405703,

    WAYPOINT_CROMUSH_01                 = 446402,
    WAYPOINT_CROMUSH_02                 = 446403,
    WAYPOINT_GARROSH                    = 446290
};

Position const ElitePos[16] =
{
    { 1387.90f, 1029.71f, 53.21853f, 2.827433f },
    { 1389.79f, 1024.51f, 53.20833f, 2.827433f },
    { 1388.05f, 1026.91f, 53.20833f, 2.827433f },
    { 1388.16f, 1020.88f, 53.25523f, 2.827433f },
    { 1385.79f, 1025.99f, 53.22593f, 2.827433f },
    { 1386.69f, 1023.26f, 53.24393f, 2.827433f },
    { 1384.33f, 1022.04f, 53.28123f, 2.827433f },
    { 1391.10f, 1027.73f, 53.20483f, 2.827433f },

    { 1359.10f, 1046.55f, 52.97053f, 5.253441f },
    { 1360.89f, 1051.81f, 53.19793f, 5.253441f },
    { 1360.75f, 1048.84f, 53.12893f, 5.253441f },
    { 1364.43f, 1053.62f, 53.29343f, 5.253441f },
    { 1363.08f, 1048.15f, 53.22223f, 5.253441f },
    { 1364.08f, 1050.84f, 53.29163f, 5.253441f },
    { 1366.69f, 1050.31f, 53.34203f, 5.253441f },
    { 1357.85f, 1050.12f, 52.99823f, 5.253441f }
};

Position const PortalPos[3] =
{
    { 1358.62f, 1054.72f, 53.1200f, 0.0f },
    { 1393.27f, 1021.20f, 53.2225f, 0.0f },
    { 1404.71f, 1063.73f, 60.5617f, 0.0f }
};

Position const GarroshPos = { 1402.45f, 1061.62f, 60.56173f, 3.926991f };

Position const CromushPos = { 1404.71f, 1063.73f, 60.5617f, 2.827433f };

// Grand Executor Mortuus - 44615
struct npc_silverpine_grand_executor_mortuus : public ScriptedAI
{
    npc_silverpine_grand_executor_mortuus(Creature* creature) : ScriptedAI(creature), _summons(me), _eventStarted(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _garroshGUID = ObjectGuid::Empty;
        _cromushGUID = ObjectGuid::Empty;
        _sylvanasGUID = ObjectGuid::Empty;
        _agathaGUID = ObjectGuid::Empty;
        _warhorseGUID = ObjectGuid::Empty;
    }

    void QuestAccept(Player* /*player*/, const Quest* quest) override
    {
        if (quest->GetQuestId() == QUEST_THE_WARCHIEF_COMETH)
        {
            if (_eventStarted)
                return;

            DoAction(ACTION_START_ANIM);

            _eventStarted = true;
        }
    }

    void Reset() override
    {
        _events.Reset();
        _garroshGUID = ObjectGuid::Empty;
        _cromushGUID = ObjectGuid::Empty;
        _sylvanasGUID = ObjectGuid::Empty;
        _agathaGUID = ObjectGuid::Empty;
        _warhorseGUID = ObjectGuid::Empty;
        _summons.clear();
        _eventStarted = false;
    }

    void JustSummoned(Creature* summon) override
    {
        _summons.Summon(summon);

        switch (summon->GetEntry())
        {
            case NPC_GARROSH_HELLSCREAM:
            {
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);

                _garroshGUID = summon->GetGUID();
                break;
            }

            case NPC_HELLSCREEMS_ELITE:
            {
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
                break;
            }

            default:
                break;
        }
    }

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case ACTION_START_ANIM:
            {
                if (Creature* sylvanas = me->FindNearestCreature(NPC_LADY_SYLVANAS_WINDRUNNER_44365, 100.0f))
                {
                    if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA_44608, 100.0f))
                    {
                        _sylvanasGUID = sylvanas->GetGUID();
                        _agathaGUID = agatha->GetGUID();

                        if (Creature* warhorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_UNPHASED, 100.0f))
                            _warhorseGUID = warhorse->GetGUID();

                        _events.ScheduleEvent(EVENT_START_ANIM_1, 250ms);
                    }
                }

                break;
            }

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
                case EVENT_START_ANIM_1:
                {
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE, 1s);
                    _events.ScheduleEvent(EVENT_SUMMON_PORTAL, 4s);
                    _events.ScheduleEvent(EVENT_SUMMON_GARROSH, 7s + 500ms);
                    break;
                }

                case EVENT_SUMMON_PORTAL:
                {
                    SummonPortalsFromOrgrimmar();
                    break;
                }

                case EVENT_SUMMON_GARROSH:
                {
                    SummonGarroshAndHisEliteGuards();
                    break;
                }

                case EVENT_TALK_SEQUENCE:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 1, 8s);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 25, 4s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 1:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->GetMotionMaster()->MoveJump(1378.65f, 1044.23f, 53.8389f, 5.51524f, 15.595897f, 15.595897f);

                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                        cromush->SetVisible(true);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 2, 3s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 2:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                            warhorse->SetFacingTo(3.924652f);

                        if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        {
                            garrosh->SetFacingToObject(sylvanas);
                            garrosh->PlayOneShotAnimKitId(DATA_ANIMKIT_GARROSH_1);
                            garrosh->AI()->Talk(TALK_0);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 3, 12s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 3:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(3.9444442f);
                        garrosh->AI()->Talk(TALK_1);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 4, 7s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 4:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                            warhorse->SetFacingTo(2.4260077f);

                        sylvanas->AI()->Talk(TALK_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 5, 5s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 5:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                            warhorse->SetFacingTo(3.7350047f);

                        sylvanas->PlayOneShotAnimKitId(DATA_ANIMKIT_SYLV_1);
                        sylvanas->AI()->Talk(TALK_3);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 6, 16s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 6:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_4);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 7, 4s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 7:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 8, 3s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 8:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_5);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 9, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 9:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_6);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 10, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 10:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->PlayOneShotAnimKitId(DATA_ANIMKIT_SYLV_2);
                        sylvanas->AI()->Talk(TALK_7);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 11, 9s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 11:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_8);

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN, 3s);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(1, 1364.02f, 1028.54f, 66.99143f, false, 6.0f);

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 1, 2s + 500ms);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 1:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(2, 1368.65f, 1032.19f, 63.3033f, false, 1.271437f);

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 2, 5s);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 2:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                        agatha->CastSpell(me, SPELL_RAISE_FORSAKEN);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 12, 10s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 12:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                        cromush->AI()->Talk(TALK_0);

                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(3, 1364.02f, 1028.54f, 55.9914f, false, 0.8775906f);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 13, 3s + 500ms);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 24, 10s);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 26, 3s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 13:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->PlayOneShotAnimKitId(DATA_ANIMKIT_SYLV_3);
                        garrosh->AI()->Talk(TALK_3);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 14, 13s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 14:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->PlayOneShotAnimKitId(DATA_ANIMKIT_SYLV_3);
                        sylvanas->AI()->Talk(TALK_9);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 15, 10s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 15:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->AI()->Talk(TALK_4);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 16, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 16:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->AI()->Talk(TALK_5);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 17, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 17:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                        sylvanas->AI()->Talk(TALK_10);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 18, 5s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 18:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(5.51524f);
                        garrosh->AI()->Talk(TALK_6);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 19, 5s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 19:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                        {
                            garrosh->SetFacingToObject(cromush);
                            garrosh->PlayOneShotAnimKitId(DATA_ANIMKIT_GARROSH_2);
                            garrosh->AI()->Talk(TALK_7);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 20, 14s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 20:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                    {
                        if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        {
                            cromush->SetFacingToObject(garrosh);
                            cromush->AI()->Talk(TALK_1);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 21, 2s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 21:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        garrosh->SetFacingTo(5.6199603f);
                        garrosh->PlayOneShotAnimKitId(DATA_ANIMKIT_GARROSH_2);
                        garrosh->AI()->Talk(TALK_8);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 22, 7s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 22:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                        cromush->GetMotionMaster()->MovePath(WAYPOINT_CROMUSH_02, false);

                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->GetMotionMaster()->MovePath(WAYPOINT_GARROSH, false);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 23, 11s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 23:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        garrosh->CastSpell(garrosh, SPELL_WELCOME_TO_SILVERPINE_CREDIT, true);

                    DespawnGarroshAndHisEliteGuards();

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 28, 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 24:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                        agatha->SetFacingTo(0.855211f);

                    break;
                }

                case EVENT_TALK_SEQUENCE + 25:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        sylvanas->AI()->Talk(TALK_1);

                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                            warhorse->SetFacingTo(0.808979f);
                    }

                    break;
                }

                case EVENT_TALK_SEQUENCE + 26:
                {
                    Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 27, 3s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 27:
                {
                    std::list<Creature*> maleforsaken;
                    std::list<Creature*> femaleforsaken;
                    GetCreatureListWithEntryInGrid(maleforsaken, me, NPC_MALE_FALLEN_HUMAN, 100.0f);
                    GetCreatureListWithEntryInGrid(femaleforsaken, me, NPC_FEMALE_FALLEN_HUMAN, 100.0f);

                    for (std::list<Creature*>::const_iterator itr = maleforsaken.begin(); itr != maleforsaken.end(); ++itr)
                        (*itr)->AI()->DoAction(ACTION_START_WALKING);

                    for (std::list<Creature*>::const_iterator itr = femaleforsaken.begin(); itr != femaleforsaken.end(); ++itr)
                        (*itr)->AI()->DoAction(ACTION_START_WALKING);

                    break;
                }

                case EVENT_TALK_SEQUENCE + 28:
                {
                    _summons.DespawnAll();

                    Reset();

                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void SummonPortalsFromOrgrimmar()
    {
        for (auto pos : PortalPos)
            me->SummonCreature(NPC_PORTAL_FROM_ORGRIMMAR, pos, TEMPSUMMON_TIMED_DESPAWN, 300s);

        std::list<Creature*> portals;
        GetCreatureListWithEntryInGrid(portals, me, NPC_PORTAL_FROM_ORGRIMMAR, 100.0f);

        for (std::list<Creature*>::const_iterator itr = portals.begin(); itr != portals.end(); ++itr)
            (*itr)->CastSpell(*itr, SPELL_AIR_REVENANT_ENTRANCE);
    }

    void SummonGarroshAndHisEliteGuards()
    {
        for (auto pos : ElitePos)
            me->SummonCreature(NPC_HELLSCREEMS_ELITE, pos, TEMPSUMMON_TIMED_DESPAWN, 300s);

        me->SummonCreature(NPC_GARROSH_HELLSCREAM, GarroshPos, TEMPSUMMON_TIMED_DESPAWN, 300s);

        if (Creature* cromush = me->SummonCreature(NPC_HIGH_WARLORD_CROMUSH, CromushPos, TEMPSUMMON_TIMED_DESPAWN, 300s))
        {
            _cromushGUID = cromush->GetGUID();

            cromush->GetMotionMaster()->MovePath(WAYPOINT_CROMUSH_01, false);
        }
    }

    void DespawnGarroshAndHisEliteGuards()
    {
        for (std::list<ObjectGuid>::const_iterator itr = _summons.begin(); itr != _summons.end(); itr++)
        {
            if (Creature* npc = ObjectAccessor::GetCreature(*me, (*itr)))
                npc->CastSpell(npc, SPELL_SIMPLE_TELEPORT);
        }
    }

private:
    EventMap _events;
    ObjectGuid _garroshGUID;
    ObjectGuid _cromushGUID;
    ObjectGuid _sylvanasGUID;
    ObjectGuid _agathaGUID;
    ObjectGuid _warhorseGUID;
    SummonList _summons;
    bool _eventStarted;
};

enum SpellRaiseForsaken83173
{
    NPC_FALLEN_HUMAN_44592          = 44592,
    NPC_FALLEN_HUMAN_44593          = 44593,

    SPELL_INVISIBLE                 = 78351,
};

// Raise Forsaken - 83173
class spell_silverpine_raise_forsaken_83173 : public SpellScript
{
    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        std::list<uint32>entrys;

        entrys.push_back(NPC_FALLEN_HUMAN_44592);
        entrys.push_back(NPC_FALLEN_HUMAN_44593);

        targets.remove_if(IsNotInEntryList(entrys));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_silverpine_raise_forsaken_83173::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_silverpine_raise_forsaken_83173_aura : public AuraScript
{
    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* unit = GetTarget())
        {
            if (Creature* fallenHuman = unit->ToCreature())
            {
                fallenHuman->SetAIAnimKitId(DATA_ANIMKIT_FALLEN_HUMAN);

                fallenHuman->SetDisableGravity(true);
                fallenHuman->SetCanFly(true);

                fallenHuman->GetMotionMaster()->MovePoint(1, fallenHuman->GetPositionX(), fallenHuman->GetPositionY(), fallenHuman->GetPositionZ() + frand(3.5f, 5.5f), false, 1.5f);
            }
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* unit = GetTarget())
        {
            if (Creature* fallenHuman = unit->ToCreature())
            {
                fallenHuman->SetDisableGravity(false);
                fallenHuman->SetCanFly(false);

                // The sniff doesn't really tell whether they fall or just move back to home position
                fallenHuman->GetMotionMaster()->MoveFall();

                fallenHuman->SetAIAnimKitId(0);

                fallenHuman->AddAura(SPELL_INVISIBLE, fallenHuman);
            }
        }
    }

    void Register() override
    {
        OnEffectApply.Register(&spell_silverpine_raise_forsaken_83173_aura::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove.Register(&spell_silverpine_raise_forsaken_83173_aura::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

enum SpellForsakenTrooperMasterScriptHighCommand
{
    SPELL_FORSAKEN_TROOPER_MALE_01_HC       = 83150,
    SPELL_FORSAKEN_TROOPER_MALE_02_HC       = 83163,
    SPELL_FORSAKEN_TROOPER_MALE_03_HC       = 83164,
    SPELL_FORSAKEN_TROOPER_MALE_04_HC       = 83165,
    SPELL_FORSAKEN_TROOPER_FEMALE_01_HC     = 83152,
    SPELL_FORSAKEN_TROOPER_FEMALE_02_HC     = 83166,
    SPELL_FORSAKEN_TROOPER_FEMALE_03_HC     = 83167,
    SPELL_FORSAKEN_TROOPER_FEMALE_04_HC     = 83168,

    DISPLAY_MALE_01_HC                      = 33978,
    DISPLAY_MALE_02_HC                      = 33980,
    DISPLAY_MALE_03_HC                      = 33979,
    DISPLAY_MALE_04_HC                      = 33981,
    DISPLAY_FEMALE_01_HC                    = 33982,
    DISPLAY_FEMALE_02_HC                    = 33983,
    DISPLAY_FEMALE_03_HC                    = 33984,
    DISPLAY_FEMALE_04_HC                    = 33985
};

// Forsaken Trooper Master Script (Forsaken High Command) - 83149
class spell_silverpine_forsaken_trooper_masterscript_high_command : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* unit = GetCaster())
        {
            if (Creature* fallenHuman = unit->ToCreature())
            {
                if (fallenHuman->GetDisplayId() == DISPLAY_MALE_01_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_01_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_MALE_02_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_02_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_MALE_03_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_03_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_MALE_04_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_04_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_FEMALE_01_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_01_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_FEMALE_02_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_02_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_FEMALE_03_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_03_HC, true);

                if (fallenHuman->GetDisplayId() == DISPLAY_FEMALE_04_HC)
                    fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_04_HC, true);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_forsaken_trooper_masterscript_high_command::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum FallenHumanActions
{
    SPELL_FEIGNED                                       = 80636,
    SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_HIGH_COMMAND    = 83149,

    EVENT_MOVE                                          = 1,
    EVENT_TRANSFORM                                     = 3,
    EVENT_FACING                                        = 4,
    EVENT_EMOTE                                         = 5,

    DATA_EMOTE_SALUTE                                   = 66,

    WAYPOINT_OPTION1                                    = 445920,
    WAYPOINT_OPTION2                                    = 445921
};

// Fallen Human - 44592, 44593
struct npc_silverpine_fallen_human : public ScriptedAI
{
    npc_silverpine_fallen_human(Creature* creature) : ScriptedAI(creature), _done1(false), _done2(false) {}

    void Reset() override
    {
        _done1 = false;
        _done2 = false;

        _events.Reset();

        me->AddAura(SPELL_FEIGNED, me);

        me->RemoveAura(SPELL_INVISIBLE);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
        me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
        me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
    }

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case EVENT_MOVE:
            {
                if (_done2)
                    return;

                if (urand(0, 1) == 0)
                    me->GetMotionMaster()->MovePath(WAYPOINT_OPTION1, false);
                else
                    me->GetMotionMaster()->MovePath(WAYPOINT_OPTION2, false);

                me->DespawnOrUnsummon(35s);

                _done2 = true;
                break;
            }

            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (me->HasAura(SPELL_INVISIBLE))
            _events.ScheduleEvent(EVENT_TRANSFORM, 1s);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TRANSFORM:
                {
                    if (_done1)
                        return;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);

                    me->RemoveAura(SPELL_FEIGNED);

                    me->CastSpell(me, SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_HIGH_COMMAND, true);

                    _events.ScheduleEvent(EVENT_FACING, 1s + 500ms);

                    _done1 = true;
                    break;
                }

                case EVENT_FACING:
                {
                    me->SetFacingTo(0.706837f);

                    _events.ScheduleEvent(EVENT_EMOTE, 2s + 500ms);
                    break;
                }

                case EVENT_EMOTE:
                {
                    me->HandleEmoteCommand(DATA_EMOTE_SALUTE);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    bool _done1;
    bool _done2;
};

enum QuestIteratingUponSuccess
{
    QUEST_ITERATING_UPON_SUCCESS        = 26998,

    NPC_VILE_FIN_ORACLE                 = 1908,
    NPC_BAT_HANDLER_MAGGOTBREATH        = 44825,
    NPC_FORSAKEN_BAT                    = 44821,

    SPELL_SUMMON_BAT                    = 83584,
    SPELL_BLIGHT_CONCOCTION             = 83573,
    SPELL_GO_HOME                       = 83594,

    EVENT_CHECK_FINISH                  = 101,
    EVENT_START_MOVEMENT,
    EVENT_GO_HOME2,
    EVENT_GO_HOME,

    ACTION_OPTION_ID                    = 1,
    ACTION_GO_HOME                      = 2,

    DATA_POINT_FINISHED                 = 5,
    DATA_POINT_START_QUEST              = 6,

    DATA_WAYPOINT_BAT_ARRIVE            = 448210,
    DATA_WAYPOINT_BAT_CIRCLE            = 448211,
    DATA_WAYPOINT_BAT_HOME              = 448212
};

// Bat Handler Maggotbreath - 44825
struct npc_silverpine_bat_handler_maggotbreath : public ScriptedAI
{
    npc_silverpine_bat_handler_maggotbreath(Creature* creature) : ScriptedAI(creature) { }

    bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
    {
        switch (gossipListId)
        {
            case ACTION_OPTION_ID:
            {
                if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                {
                    player->CastSpell(player, SPELL_SUMMON_BAT);

                    me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);

                    Talk(TALK_0);

                    CloseGossipMenuFor(player);
                }

                break;
            }

            default:
                break;
        }

        return false;
    }
};

// Forsaken Bat - 44821
struct npc_silverpine_forsaken_bat : public VehicleAI
{
    npc_silverpine_forsaken_bat(Creature* creature) : VehicleAI(creature), _goingIsland(true), _goingHome(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;

        SetupActionBar();
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    _playerGUID = player->GetGUID();
            }
        }
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (apply)
        {
            if (Player* player = passenger->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                {
                    player->KilledMonsterCredit(NPC_BAT_HANDLER_MAGGOTBREATH);

                    me->SetCanFly(true);
                    me->SetDisableGravity(true);

                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    me->GetMotionMaster()->MovePath(DATA_WAYPOINT_BAT_ARRIVE, false);

                    _events.ScheduleEvent(EVENT_CHECK_FINISH, 500ms);
                }
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        switch (pathId)
        {
            case DATA_WAYPOINT_BAT_ARRIVE:
            {
                if (waypointId == 7)
                {
                    _goingIsland = false;

                    me->GetMotionMaster()->MovePath(DATA_WAYPOINT_BAT_CIRCLE, false);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_0, player);
                }

                break;
            }

            case DATA_WAYPOINT_BAT_CIRCLE:
            {
                if (waypointId == 31)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                            me->GetMotionMaster()->MovePath(DATA_WAYPOINT_BAT_CIRCLE, false);
                    }
                }

                break;
            }

            case DATA_WAYPOINT_BAT_HOME:
            {
                if (waypointId == 6)
                {
                    me->GetVehicleKit()->RemoveAllPassengers();

                    me->DespawnOrUnsummon();
                }

                break;
            }

            default:
                break;
        }
    }

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case ACTION_GO_HOME:
            {
                if (!_goingHome && !_goingIsland)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        _goingHome = true;

                        me->GetMotionMaster()->Clear();

                        me->GetMotionMaster()->MovePath(DATA_WAYPOINT_BAT_HOME, false);

                        Talk(TALK_1, player);
                    }
                }

                break;
            }

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
                case EVENT_CHECK_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetReqKillOrCastCurrentCount(QUEST_ITERATING_UPON_SUCCESS, NPC_VILE_FIN_ORACLE) == 50)
                            player->CastSpell(me, SPELL_GO_HOME, true);
                        else
                            _events.ScheduleEvent(EVENT_CHECK_FINISH, 500ms);
                    }

                    break;
                }

                default:
                    break;
            }
        }
    }

    void SetupActionBar()
    {
        me->m_spells[0] = SPELL_BLIGHT_CONCOCTION;
        me->m_spells[1] = SPELL_GO_HOME;
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _goingIsland;
    bool _goingHome;
};

// Go Home - 83594
class spell_silverpine_go_home : public SpellScript
{
    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetHitUnit())
            {
                if (target->GetEntry() == NPC_FORSAKEN_BAT)
                    target->GetAI()->DoAction(ACTION_GO_HOME);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_go_home::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Vile Fin Tidehunter - 1768, Vile Fin Oracle - 1908
struct npc_silverpine_vilefine_murlocks : public ScriptedAI
{
    npc_silverpine_vilefine_murlocks(Creature* creature) : ScriptedAI(creature)
    {
        Initilize();
    }

    void Initilize()
    {
        if (!me->IsInCombat())
            me->SetSpeed(UnitMoveType::MOVE_WALK, 7.4f);
        else
            me->SetSpeed(UnitMoveType::MOVE_WALK, 1.4f);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

enum QuestWaitingToExsanguinate
{
    QUEST_WAITING_TO_EXSANGUINATE               = 27045,

    NPC_DEATHSTALKER_RANE_YORICK                = 44882,
    NPC_ARMOIRE_IN_ROOM                         = 44894,
    NPC_ARMOIRE_SUMMONED                        = 44893,
    NPC_LORD_DARIUS_CROWLEY                     = 44883,
    NPC_PACKLEADER_IVAR_BLOODFANG               = 44884,
    NPC_KILLCREDIT_DEATHSTALKER_YORICK          = 44882,

    SPELL_SUMMON_HIDING_SPOT                    = 83756,
    SPELL_HIDE_IN_ARMOIRE                       = 83788,
    SPELL_STEALTH                               = 34189,
    SPELL_PERMANENT_FEIGN_DEATH                 = 29266,
    SPELL_EJECT_PASSENGER_1                     = 80743,
    SPELL_SUMMON_CROWLEY                        = 83752,
    SPELL_SUMMON_BLOODFANG                      = 83753,
    SPELL_SUMMON_CROWLEY_BLOODFANG_MASTER       = 83762,
    SPELL_ARMOIRE_CAMERA_A                      = 83763,
    SPELL_ARMOIRE_CAMERA_B                      = 83764,
    SPELL_ARMOIRE_CAMERA_C                      = 83768,
    SPELL_ARMOIRE_CAMERA_D                      = 83769,
    SPELL_CONVERSATION_TRIGGER_01A              = 83773,
    SPELL_CONVERSATION_TRIGGER_01B              = 83774,
    SPELL_CONVERSATION_TRIGGER_01C              = 83782,
    SPELL_CONVERSATION_TRIGGER_02A              = 83775,
    SPELL_CONVERSATION_TRIGGER_02B              = 83777,
    SPELL_CONVERSATION_TRIGGER_02C              = 83779,
    SPELL_CONVERSATION_TRIGGER_02D              = 83955,
    SPELL_CONVERSATION_TRIGGER_03               = 83787,
    SPELL_CONVERSATION_TRIGGER_04               = 83784,
    SPELL_REVERSE_RIDE_VEHICLE                  = 83781,
    SPELL_EJECT_PASSENGER_01                    = 80743,
    SPELL_KILL_CREDIT_YORICK                    = 83786,
    SPELL_DESPAWN_ALL                           = 83770,

    EVENT_START_ANIM                            = 100,
    EVENT_WAIT_ON_PLAYER                        = 102,
    EVENT_HIDE,
    EVENT_SET_FACE_TO_BLOODFANG,
    EVENT_RANE_LAST_MOVE,
    EVENT_START_ANIMATION                       = 201,
    EVENT_ACTION                                = 300,
    EVENT_CAMERA_A                              = 400,
    EVENT_CAMERA_B                              = 401,
    EVENT_TALK_TO_PLAYER                        = 700,
    EVENT_FINISH                                = 800,

    DATA_WAYPOINT_UP                            = 448820,
    DATA_WAYPOINT_HIDE                          = 448821,
    DATA_WAYPOINT_DARIUS                        = 448830,
    DATA_WAYPOINT_IVAR                          = 448840,
    DATA_WAYPOINT_IVAR_RANE_01                  = 448841,
    DATA_WAYPOINT_IVAR_RANE_02                  = 448842,
    DATA_WAYPOINT_IVAR_RANE_03                  = 448843,
    DATA_WAYPOINT_IVAR_EXIT                     = 448844,
    DATA_WAYPOINT_DARIUS_EXIT                   = 448831,

    MOVEPOINT_HIDDEN_PLACE                      = 1234,
    MOVEPOINT_RANA                              = 1230,

    ACTION_RANE_JUMP_DEATH                      = 2,
    ACTION_RANE_SKIP_PATH,
    ACTION_MOVE_TO_RANA                         = 500,

    MOVEPOINT_ARMOIRE                           = 15
};

// Deathstalker - 44789, 44790
struct npc_silverpine_deathstalker : public ScriptedAI
{
    npc_silverpine_deathstalker(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        me->SetPowerType(POWER_ENERGY);
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 100, true);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

// Abandoned Outhouse - 205143
struct go_silverpine_abandoned_outhouse : public GameObjectAI
{
    go_silverpine_abandoned_outhouse(GameObject* go) : GameObjectAI(go) { }

    enum eData
    {
        QUEST_WAITING_TO_EXSANGUINATE = 27045,

        SPELL_SUMMON_DEATHSTALKER_YORICK = 83751
    };

    void QuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_WAITING_TO_EXSANGUINATE)
            player->CastSpell(player, SPELL_SUMMON_DEATHSTALKER_YORICK, true);
    }
};

Position const YorickReady = { 1313.7f, 1211.99f, 58.5f };
Position const YorickDeath = { 1295.52f, 1206.58f, 58.501f };

// Deathstalker Rane Yorick - 44882
struct npc_silverpine_deathstalker_rane_yorick : public ScriptedAI
{
    npc_silverpine_deathstalker_rane_yorick(Creature* creature) : ScriptedAI(creature), _playerArrived(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _armoireGUID = ObjectGuid::Empty;
        _bloodfangGUID = ObjectGuid::Empty;

        me->SetPowerType(POWER_ENERGY);
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 100, true);
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
            _playerGUID = player->GetGUID();

        _events.ScheduleEvent(EVENT_START_ANIM, 1s);
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOINT_UP && waypointId == 15)
            _events.ScheduleEvent(EVENT_WAIT_ON_PLAYER, 1s);

        if (pathId == DATA_WAYPOINT_HIDE && waypointId == 2)
        {
            me->SetFacingTo(4.6425757f);
            me->CastSpell(me, SPELL_STEALTH);
        }
    }

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case ACTION_RANE_JUMP_DEATH:
            {
                me->SetDisableGravity(true);

                _events.ScheduleEvent(EVENT_RANE_LAST_MOVE, 10ms);
                break;
            }

            case ACTION_RANE_SKIP_PATH:
            {
                _events.Reset();

                _events.ScheduleEvent(EVENT_SET_FACE_TO_BLOODFANG, 4s);

                me->GetMotionMaster()->Clear();

                me->NearTeleportTo(YorickReady, false);

                me->CastSpell(me, SPELL_STEALTH);
                break;
            }

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
                case EVENT_START_ANIM:
                {
                    Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_START_ANIM + 1, 1s + 500ms);
                    break;
                }

                case EVENT_START_ANIM + 1:
                {
                    me->GetMotionMaster()->MovePath(DATA_WAYPOINT_UP, false);
                    break;
                }

                case EVENT_WAIT_ON_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetDistance2d(me) <= 5.0f && !_playerArrived)
                        {
                            _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 1s);
                            _playerArrived = true;
                        }
                        else
                            _events.ScheduleEvent(EVENT_WAIT_ON_PLAYER, 1s);
                    }

                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_HIDE, 3s);
                    _events.ScheduleEvent(EVENT_SET_FACE_TO_BLOODFANG, 3s);
                    break;
                }

                case EVENT_HIDE:
                {
                    me->GetMotionMaster()->MovePath(DATA_WAYPOINT_HIDE, false);
                    break;
                }

                case EVENT_SET_FACE_TO_BLOODFANG:
                {
                    if (!_bloodfangGUID)
                    {
                        if (Creature* ivar = me->FindNearestCreature(NPC_PACKLEADER_IVAR_BLOODFANG, 25.0f))
                            _bloodfangGUID = ivar->GetGUID();
                    }

                    if (!_armoireGUID)
                    {
                        if (Creature* armoire = me->FindNearestCreature(NPC_ARMOIRE_SUMMONED, 30.0f))
                        {
                            _armoireGUID = armoire->GetGUID();

                            armoire->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());
                        }
                    }

                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        me->SetFacingToObject(ivar);
                    break;
                }

                case EVENT_RANE_LAST_MOVE:
                {
                    me->GetMotionMaster()->MoveJump(YorickDeath, 10.0f, 10.0f);

                    _events.ScheduleEvent(EVENT_RANE_LAST_MOVE + 1, 2s);
                    break;
                }

                case EVENT_RANE_LAST_MOVE + 1:
                {
                    me->SetDisableGravity(false);

                    me->AddAura(SPELL_PERMANENT_FEIGN_DEATH, me);

                    me->DespawnOrUnsummon(15s);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _armoireGUID;
    ObjectGuid _bloodfangGUID;
    bool _playerArrived;
};

// Armoire - 44893
struct npc_silverpine_armoire : public VehicleAI
{
    npc_silverpine_armoire(Creature* creature) : VehicleAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _raneGUID = ObjectGuid::Empty;
        _crowleyGUID = ObjectGuid::Empty;
        _bloodfangGUID = ObjectGuid::Empty;
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_WAITING_TO_EXSANGUINATE) == QUEST_STATUS_INCOMPLETE)
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    _playerGUID = player->GetGUID();

                if (Creature* rane = player->FindNearestCreature(NPC_DEATHSTALKER_RANE_YORICK, 100.0f, true))
                {
                    _raneGUID = rane->GetGUID();

                    if (rane->GetOwner() == player)
                    {
                        if (me->GetDistance2d(rane) > 2.0f)
                            rane->GetAI()->DoAction(ACTION_RANE_SKIP_PATH);
                    }
                }
            }
        }
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (apply)
        {
            if (Player* player = passenger->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_WAITING_TO_EXSANGUINATE) == QUEST_STATUS_INCOMPLETE)
                {
                    _events.ScheduleEvent(EVENT_START_ANIMATION, 2s);
                    return;
                }
            }
        }
        else
        {
            if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                darius->DespawnOrUnsummon();

            if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                ivar->DespawnOrUnsummon();

            if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                rane->DespawnOrUnsummon();

            me->DespawnOrUnsummon(1s);
        }
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        switch (id)
        {
            case PLAYER_GUID:
            {
                _playerGUID = guid;
                break;
            }

            case NPC_DEATHSTALKER_RANE_YORICK:
            {
                _raneGUID = guid;
                break;
            }

            case NPC_LORD_DARIUS_CROWLEY:
            {
                _crowleyGUID = guid;
                break;
            }

            case NPC_PACKLEADER_IVAR_BLOODFANG:
            {
                _bloodfangGUID = guid;
                break;
            }

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
                case EVENT_START_ANIMATION:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        player->CastSpell(player, SPELL_SUMMON_CROWLEY_BLOODFANG_MASTER, true);

                        _events.ScheduleEvent(EVENT_START_ANIMATION + 1, 1s);
                    }
                    break;
                }

                case EVENT_START_ANIMATION + 1:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        darius->GetMotionMaster()->MovePath(DATA_WAYPOINT_DARIUS, false);

                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->GetMotionMaster()->MovePath(DATA_WAYPOINT_IVAR, false);

                    _events.ScheduleEvent(EVENT_ACTION, 5s);
                    break;
                }

                case EVENT_ACTION:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        {
                            darius->SetFacingToObject(ivar);
                            ivar->SetFacingToObject(darius);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 2s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        darius->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_CAMERA_B, 6s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 1, 6s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 1:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_CAMERA_A, 6s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 2, 6s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 2:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        darius->AI()->Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 3, 12s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 3:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        darius->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_CAMERA_B, 7s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 4, 7s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 4:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 5, 7s + 300ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 5:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 6, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 6:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_3);

                    _events.ScheduleEvent(EVENT_CAMERA_A, 9s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 7, 9s + 800ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 7:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        darius->AI()->Talk(TALK_3);

                    _events.ScheduleEvent(EVENT_CAMERA_B, 3s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 8, 3s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 8:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_4);

                    _events.ScheduleEvent(EVENT_ACTION + 1, 2s + 500ms);
                    break;
                }

                case EVENT_ACTION + 1:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->GetMotionMaster()->MovePath(DATA_WAYPOINT_IVAR_RANE_01, false);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 9, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 9:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_5);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 10, 5s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 10:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_6);

                    _events.ScheduleEvent(EVENT_ACTION + 2, 6s);
                    break;
                }

                case EVENT_ACTION + 2:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->GetMotionMaster()->MovePath(DATA_WAYPOINT_IVAR_RANE_02, false);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 11, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 11:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_7);

                    _events.ScheduleEvent(EVENT_ACTION + 3, 4s);
                    break;
                }

                case EVENT_ACTION + 3:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                        {
                            rane->RemoveAura(SPELL_STEALTH);
                            ivar->CastSpell(rane, SPELL_REVERSE_RIDE_VEHICLE, true);
                            rane->EnterVehicle(ivar, 0);
                        }
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 4, 1s);
                    break;
                }

                case EVENT_ACTION + 4:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->GetMotionMaster()->MovePath(DATA_WAYPOINT_IVAR_RANE_03, false);

                    _events.ScheduleEvent(EVENT_ACTION + 5, 3s);
                    break;
                }

                case EVENT_ACTION + 5:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->SetFacingTo(3.054326f);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 12, 500ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 12:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_8);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 13, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 13:
                {
                    if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                        rane->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_ACTION + 6, 3s);
                    break;
                }

                case EVENT_ACTION + 6:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                        {
                            ivar->CastSpell(rane, SPELL_EJECT_PASSENGER_01, false);
                            rane->GetAI()->DoAction(ACTION_RANE_JUMP_DEATH);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 14, 1s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 14:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        {
                            ivar->SetFacingToObject(darius);
                            ivar->AI()->Talk(TALK_9);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 15, 5s + 500ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 15:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        ivar->AI()->Talk(TALK_10);

                    _events.ScheduleEvent(EVENT_CAMERA_A, 5s + 300ms);
                    _events.ScheduleEvent(EVENT_ACTION + 7, 6s);
                    break;
                }

                case EVENT_ACTION + 7:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                            ivar->GetMotionMaster()->MovePath(DATA_WAYPOINT_IVAR_EXIT, false);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 8, 3s);
                    _events.ScheduleEvent(EVENT_FINISH, 15s);
                    break;
                }

                case EVENT_ACTION + 8:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                            darius->GetMotionMaster()->MovePath(DATA_WAYPOINT_DARIUS_EXIT, false);
                    }

                    _events.ScheduleEvent(EVENT_FINISH, 4s);
                    break;
                }

                case EVENT_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                            darius->CastSpell(player, SPELL_KILL_CREDIT_YORICK, false);

                        player->GetMotionMaster()->Clear();

                        player->RemoveAura(SPELL_HIDE_IN_ARMOIRE);

                        player->ExitVehicle(0);
                    }

                    _events.ScheduleEvent(EVENT_FINISH + 1, 1s);
                    break;
                }

                case EVENT_FINISH + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->DespawnOrUnsummon();
                    break;
                }

                case EVENT_CAMERA_A:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        player->CastSpell(player, SPELL_ARMOIRE_CAMERA_A, true);
                    break;
                }

                case EVENT_CAMERA_B:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        player->CastSpell(player, SPELL_ARMOIRE_CAMERA_B, true);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _raneGUID;
    ObjectGuid _crowleyGUID;
    ObjectGuid _bloodfangGUID;
};

// Lord Darius Crowley - 44883
struct npc_silverpine_lord_darius_crowley_exhanguinate : public ScriptedAI
{
    npc_silverpine_lord_darius_crowley_exhanguinate(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _armoireGUID = ObjectGuid::Empty;
        _bloodfangGUID = ObjectGuid::Empty;
        _events.Reset();
    }

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        FindAllGuid();
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOINT_DARIUS_EXIT && waypointId == 2)
            me->DespawnOrUnsummon();
    }

    void FindAllGuid()
    {
        if (!_bloodfangGUID)
        {
            if (Creature* ivar = me->FindNearestCreature(NPC_PACKLEADER_IVAR_BLOODFANG, 30.0f))
                _bloodfangGUID = ivar->GetGUID();
        }

        if (!_armoireGUID)
        {
            if (Creature* armoire = me->FindNearestCreature(NPC_ARMOIRE_SUMMONED, 30.0f))
            {
                _armoireGUID = armoire->GetGUID();

                armoire->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _armoireGUID;
    ObjectGuid _bloodfangGUID;
};

// Packleader Ivar Bloodfang - 44884
struct npc_silverpine_packleader_ivar_bloodfang_exhanguinate : public ScriptedAI
{
    npc_silverpine_packleader_ivar_bloodfang_exhanguinate(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _armoireGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* armoire = me->FindNearestCreature(NPC_ARMOIRE_SUMMONED, 30.0f))
        {
            _armoireGUID = armoire->GetGUID();

            armoire->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOINT_IVAR_EXIT && waypointId == 3)
            me->DespawnOrUnsummon();
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _armoireGUID;
};

// Eject Passenger 1 - 80743
class spell_silverpine_eject_passenger_1 : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetHitUnit()->IsVehicle())
        {
            Unit* rider = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(0);

            GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(rider);
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_eject_passenger_1::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum ForsakenRearGuardQuests
{
    QUEST_STEEL_THUNDER                         = 27069,
    QUEST_LOST_IN_THE_DARKNESS                  = 27093,
    QUEST_ITS_ONLY_POISONOUS_IF_YOU_INGEST_IT   = 27088,

    NPC_SALTY_ROCKA                             = 45498,
    NPC_SALTY_GORGAR                            = 45497,
    NPC_DRUNKEN_ORC_SEA_DOG                     = 44913,
    NPC_WARLORD_TOROK                           = 44917,
    NPC_ORC_SEA_PUP                             = 44914,
    NPC_ORC_CRATE                               = 44915,
    NPC_MUTANT_BUSH_CHICKEN                     = 44935,
    NPC_FOREST_ETTIN                            = 44367,
    NPC_WEBBED_VICTIM                           = 44941,
    NPC_ORC_SEA_DOG                             = 44942,
    NPC_BLOODFANG_SCAVENGER                     = 44547,
    NPC_RABID_DOG                               = 1766,
    NPC_GIANT_RABBID_BEAR                       = 1797,
    NPC_SKITTERWEB_STALKER                      = 44908,

    SPELL_SUMMON_SEA_PUP                        = 83839,
    SPELL_SUMMON_ORC_CRATE                      = 83835,
    SPELL_PICK_UP_ORC_CRATE                     = 83838,
    SPELL_DESPAWN_SUMMONS                       = 83840,
    SPELL_KILL_CREDIT_SEA_DOG_CRATE             = 83843,
    SPELL_BUSH_EXPLOSION                        = 83903,
    SPELL_FREE_WEBBED_VICTIM1                   = 83919,
    SPELL_FREE_WEBBED_VICTIM2                   = 83921,
    SPELL_FREE_WEBBED_VICTIM3                   = 83927,
    SPELL_SICK                                  = 83885,
    SPELL_SEA_PUP_TRIGGER                       = 83865,
    SPELL_DESPAWN_ALL_SUMMONS                   = 83935,
    SPELL_SKITTERWEB                            = 83827,
    SPELL_SUMMNON_SPIDERLINGS                   = 87084,
    SPELL_VENOM_SPLASH                          = 79607,

    ACTION_DELIVER_CRATES                       = 2,
    ACTION_EXPLODE                              = 3,
    ACTION_GRAB_CHICKEN                         = 4,

    EVENT_CHECK_PLAYER                          = 100,
    EVENT_TALK_COOLDOWN                         = 101,
    EVENT_REMOVE_PROTECTION                     = 102,
    EVENT_JUST_SUMMONED                         = 104,
    EVENT_TALK                                  = 200,
    EVENT_CHECK_PLAYER_ALIVE                    = 300,
    EVENT_CHECK_TALK                            = 400,
    EVENT_EXPLODE                               = 500,
    EVENT_CHECK_ETTIN                           = 501,
    EVENT_AGGRO_SPIDER                          = 800,
    EVENT_SUMMON_SPIDERLINGS                    = 900,
    EVENT_VENOM_SPLASH                          = 901,
    EVENT_TALK_CHOOSE                           = 1000,

    DATA_TOROK_ANIMKIT                          = 594,
    DATA_SPIDER_ANIMKIT1                        = 865,
    DATA_SPIDER_ANIMKIT2                        = 866,

    AREA_FORSAKEN_REAR_GUARD                    = 5386
};

// "Salty" Rocka - 45498
struct npc_silverpine_salty_rocka : public ScriptedAI
{
    npc_silverpine_salty_rocka(Creature* creature) : ScriptedAI(creature), _playerNear(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _gorgarGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_CHECK_TALK, 1s);
    }


    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
            _playerGUID = who->GetGUID();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_TALK:
                {
                    CheckForGorgor();

                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetDistance2d(me) < 20.0f && !_playerNear)
                        {
                            _events.ScheduleEvent(EVENT_TALK_CHOOSE, 1s);
                            _events.ScheduleEvent(EVENT_TALK_COOLDOWN, 180s);

                            _playerNear = true;
                        }
                        else
                        {
                            _playerNear = false;
                            _events.ScheduleEvent(EVENT_CHECK_TALK, 1s);
                        }
                    }

                    break;
                }

                case EVENT_TALK_CHOOSE:
                {
                    if (roll_chance_i(50))
                    {
                        if (roll_chance_i(50))
                        {
                            if (roll_chance_i(50))
                                _events.ScheduleEvent(EVENT_TALK, 1s);
                            else
                                _events.ScheduleEvent(EVENT_TALK + 10, 1s);
                        }
                        else
                        {
                            if (roll_chance_i(50))
                                _events.ScheduleEvent(EVENT_TALK + 20, 1s);
                            else
                                _events.ScheduleEvent(EVENT_TALK + 30, 1s);
                        }
                    }
                    else
                        _events.ScheduleEvent(EVENT_TALK + 40, 1s);

                    break;
                }

                case EVENT_TALK:
                {
                    Talk(TALK_0);
                    _events.ScheduleEvent(EVENT_TALK + 1, 20s);
                    break;
                }

                case EVENT_TALK + 1:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                        gorgar->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_TALK + 2, 20s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    Talk(TALK_1);
                    break;
                }

                case EVENT_TALK + 10:
                {
                    Talk(TALK_2);
                    _events.ScheduleEvent(EVENT_TALK + 11, 20s);
                    break;
                }

                case EVENT_TALK + 11:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                        gorgar->AI()->Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_TALK + 12, 20s);
                    break;
                }

                case EVENT_TALK + 12:
                {
                    Talk(TALK_3);
                    break;
                }

                case EVENT_TALK + 20:
                {
                    Talk(TALK_4);
                    _events.ScheduleEvent(EVENT_TALK + 21, 20s);
                    break;
                }

                case EVENT_TALK + 21:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                        gorgar->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_TALK + 22, 20s);
                    break;
                }

                case EVENT_TALK + 22:
                {
                    Talk(TALK_5);
                    break;
                }

                case EVENT_TALK + 30:
                {
                    Talk(TALK_6);
                    _events.ScheduleEvent(EVENT_TALK + 31, 20s);
                    break;
                }

                case EVENT_TALK + 31:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                        gorgar->AI()->Talk(TALK_3);

                    _events.ScheduleEvent(EVENT_TALK + 32, 20s);
                    break;
                }

                case EVENT_TALK + 32:
                {
                    Talk(TALK_7);
                    break;
                }

                case EVENT_TALK + 40:
                {
                    Talk(TALK_8);
                    _events.ScheduleEvent(EVENT_TALK + 1, 20s);
                    break;
                }

                case EVENT_TALK + 41:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                        gorgar->AI()->Talk(TALK_4);

                    _events.ScheduleEvent(EVENT_TALK + 2, 20s);
                    break;
                }

                case EVENT_TALK + 42:
                {
                    Talk(TALK_9);
                    break;
                }

                case EVENT_TALK_COOLDOWN:
                {
                    Reset();
                    break;
                }

                default:
                    break;
            }
        }
    }

    void CheckForGorgor()
    {
        if (!_gorgarGUID)
        {
            if (Creature* gorgar = me->FindNearestCreature(NPC_SALTY_GORGAR, 50.0f))
                _gorgarGUID = gorgar->GetGUID();
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _gorgarGUID;
    bool _playerNear;
};

// Apothecary Wormcrud - 44912
struct npc_silverpine_apothecary_wormcrud : public ScriptedAI
{
    npc_silverpine_apothecary_wormcrud(Creature* creature) : ScriptedAI(creature), _drunkenOrcSeaDog(), _orcSeaDogList(), _playerNear(false), _done(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_CHECK_TALK, 1s);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
            _playerGUID = who->GetGUID();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_TALK:
                {
                    if (!_done)
                        CheckForSeaOrcs();

                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetDistance2d(me) < 20.0f && !_playerNear)
                        {
                            _events.ScheduleEvent(EVENT_TALK, 5s);
                            _events.ScheduleEvent(EVENT_TALK_COOLDOWN, 150s);

                            _playerNear = true;
                        }
                        else
                        {
                            _playerNear = false;
                            _events.ScheduleEvent(EVENT_CHECK_TALK, 5s);
                        }
                    }

                    break;
                }

                case EVENT_TALK:
                {
                    if (Creature* orcsea1 = ObjectAccessor::GetCreature(*me, _drunkenOrcSeaDog[0]))
                        orcsea1->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_TALK + 1, 12s);
                    break;
                }

                case EVENT_TALK + 1:
                {
                    if (Creature* orcsea2 = ObjectAccessor::GetCreature(*me, _drunkenOrcSeaDog[1]))
                        orcsea2->AI()->Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_TALK + 2, 12s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    if (Creature* orcsea3 = ObjectAccessor::GetCreature(*me, _drunkenOrcSeaDog[2]))
                        orcsea3->AI()->Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_TALK + 3, 12s);
                    break;
                }

                case EVENT_TALK + 3:
                {
                    Talk(TALK_0);
                    break;
                }

                case EVENT_TALK_COOLDOWN:
                {
                    Reset();
                    break;
                }

                default:
                    break;
            }
        }
    }

    void CheckForSeaOrcs()
    {
        me->GetCreatureListWithEntryInGrid(_orcSeaDogList, NPC_DRUNKEN_ORC_SEA_DOG, 5.0f);

        uint8 i = 0;

        for (auto seaorc : _orcSeaDogList)
            _drunkenOrcSeaDog[i++] = seaorc->GetGUID();

        _done = true;
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    std::array<ObjectGuid, 3> _drunkenOrcSeaDog;
    std::vector<Creature*> _orcSeaDogList;
    bool _playerNear;
    bool _done;
};

// Admiral Hatchet - 44916
struct npc_silverpine_admiral_hatchet : public ScriptedAI
{
    npc_silverpine_admiral_hatchet(Creature* creature) : ScriptedAI(creature), _playerNear(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _torokGUID = ObjectGuid::Empty;
    }

    bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 /*gossipListId*/) override
    {
        if (player->GetQuestStatus(QUEST_STEEL_THUNDER) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->HasAura(SPELL_SEA_PUP_TRIGGER))
                player->RemoveAura(SPELL_SEA_PUP_TRIGGER);

            player->CastSpell(player, SPELL_SEA_PUP_TRIGGER);

            CloseGossipMenuFor(player);

            return true;
        }

        return false;
    }

    void QuestReward(Player* /*player*/, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_STEEL_THUNDER)
        {
            if (Creature* pup = me->FindNearestCreature(NPC_ORC_SEA_PUP, 25.0f))
                pup->GetAI()->DoAction(ACTION_DELIVER_CRATES);
        }

        if (quest->GetQuestId() == QUEST_LOST_IN_THE_DARKNESS)
        {
            me->GetCreatureListWithEntryInGrid(_seaDogList, NPC_ORC_SEA_DOG, 25.0f);

            for (Creature* seadog : _seaDogList)
            {
                if (seadog->GetOwner()->ToPlayer()->IsQuestRewarded(QUEST_LOST_IN_THE_DARKNESS))
                    seadog->DespawnOrUnsummon();
            }
        }
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_CHECK_TALK, 1s);

        me->SetPowerType(Powers::POWER_ENERGY);
        me->SetMaxPower(Powers::POWER_ENERGY, 100);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
            _playerGUID = who->GetGUID();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_TALK:
                {
                    if (me->GetAreaId() == AREA_FORSAKEN_REAR_GUARD)
                    {
                        CheckForTorok();

                        if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        {
                            if (player->GetDistance2d(me) < 20.0f && !_playerNear)
                            {
                                _events.ScheduleEvent(EVENT_TALK + 1, 1s);
                                _events.ScheduleEvent(EVENT_TALK_COOLDOWN, 230s);

                                _playerNear = true;
                            }
                            else
                            {
                                _playerNear = false;
                                _events.ScheduleEvent(EVENT_CHECK_TALK, 1s);
                            }
                        }
                    }

                    break;
                }

                case EVENT_TALK_COOLDOWN:
                {
                    Reset();
                    break;
                }

                case EVENT_TALK + 1:
                {
                    Talk(TALK_0);
                    _events.ScheduleEvent(EVENT_TALK + 2, 8s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    if (Creature* torok = ObjectAccessor::GetCreature(*me, _torokGUID))
                        torok->AI()->Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_TALK + 3, 6s);
                    break;
                }

                case EVENT_TALK + 3:
                {
                    Talk(TALK_1);
                    _events.ScheduleEvent(EVENT_TALK + 4, 6s);
                    break;
                }

                case EVENT_TALK + 4:
                {
                    if (Creature* torok = ObjectAccessor::GetCreature(*me, _torokGUID))
                    {
                        torok->PlayOneShotAnimKitId(DATA_TOROK_ANIMKIT);
                        torok->AI()->Talk(TALK_1);
                    }

                    break;
                }

                default:
                    break;
            }
        }
    }

    void CheckForTorok()
    {
        if (!_torokGUID)
        {
            if (Creature* torok = me->FindNearestCreature(NPC_WARLORD_TOROK, 30.0f))
                _torokGUID = torok->GetGUID();
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _torokGUID;
    std::list<Player*> _playerList;
    std::vector<Creature*> _seaDogList;
    bool _playerNear;
};

// Orc Sea Pup - 44914
struct npc_silverpine_orc_sea_pup : public ScriptedAI
{
    npc_silverpine_orc_sea_pup(Creature* creature) : ScriptedAI(creature), _isJustSummoned(true)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();

        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            _playerGUID = player->GetGUID();

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            me->GetMotionMaster()->MoveFollow(player, 3.0f, (float)M_PI / 2.0f, true, true, false);

            if (player->GetQuestStatus(QUEST_STEEL_THUNDER) == QUEST_STATUS_INCOMPLETE)
            {
                _events.ScheduleEvent(EVENT_JUST_SUMMONED, 1s);
                _events.ScheduleEvent(EVENT_REMOVE_PROTECTION, 1s + 500ms);
                _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 5s);

                int c = player->GetReqKillOrCastCurrentCount(QUEST_STEEL_THUNDER, NPC_ORC_CRATE);

                for (int i = 0; i < c; i++)
                    me->CastSpell(me, SPELL_SUMMON_ORC_CRATE, false);
            }
        }
    }

    void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
    {
        if (passenger->GetEntry() == NPC_ORC_CRATE)
        {
            if (apply)
            {
                if (!_isJustSummoned)
                    Talk(seatId + 1);
            }
            else
            {
                if (Creature* crate = passenger->ToCreature())
                    crate->DespawnOrUnsummon(6s);
            }
        }
    }

    void DoAction(int32 param) override
    {
        if (param == ACTION_DELIVER_CRATES)
        {
            _events.CancelEvent(EVENT_TALK_TO_PLAYER);

            Talk(TALK_6);

            me->GetVehicleKit()->RemoveAllPassengers();

            me->HandleEmoteCommand(427);

            me->DespawnOrUnsummon(6s);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_JUST_SUMMONED:
                {
                    Talk(TALK_0);
                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    if (me->GetVehicleKit()->IsVehicleInUse())
                        Talk(TALK_7);

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 45s, 65s);
                    break;
                }

                case EVENT_REMOVE_PROTECTION:
                {
                    _isJustSummoned = false;
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isJustSummoned;
};

// Orc Crate - 44915
struct npc_silverpine_orc_crate : public ScriptedAI
{
    npc_silverpine_orc_crate(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _orcGUID = ObjectGuid::Empty;
    }

    void IsSummonedBy(Unit* who) override
    {
        if (who->GetEntry() == NPC_ORC_SEA_PUP)
        {
            _orcGUID = who->GetGUID();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            if (Creature* orc = ObjectAccessor::GetCreature(*me, _orcGUID))
                me->CastSpell(orc, VEHICLE_SPELL_RIDE_HARDCODED, true);
        }
    }

private:
    ObjectGuid _orcGUID;
};

// Pick Up Orc Crate - 83838
class spell_silverpine_pick_up_orc_crate : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfi*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_SUMMON_ORC_CRATE,
                SPELL_KILL_CREDIT_SEA_DOG_CRATE
            });
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        GetHitUnit()->CastSpell(nullptr, SPELL_SUMMON_ORC_CRATE);
        GetHitUnit()->CastSpell(nullptr, SPELL_KILL_CREDIT_SEA_DOG_CRATE);
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_pick_up_orc_crate::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Sea Pup Trigger - 83865
class spell_silverpine_sea_pup_trigger : public SpellScript
{
    class IsNotPlayerGuid
    {
    public:
        explicit IsNotPlayerGuid(ObjectGuid guid) : _guid(guid) { }

        bool operator()(WorldObject* obj) const
        {
            if (Player* player = obj->ToPlayer())
                return player->GetGUID() != _guid;

            return true;
        }

    private:
        ObjectGuid _guid;
    };

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(IsNotPlayerGuid(GetCaster()->GetGUID()));
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* unit = GetCaster())
            if (Player* player = unit->ToPlayer())
                player->CastSpell(player, SPELL_SUMMON_SEA_PUP);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_silverpine_sea_pup_trigger::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget.Register(&spell_silverpine_sea_pup_trigger::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Mutant Bush Chicken - 44935
struct npc_silverpine_mutant_bush_chicken : public ScriptedAI
{
    npc_silverpine_mutant_bush_chicken(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

        _events.ScheduleEvent(EVENT_CHECK_ETTIN, 1s);
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_ITS_ONLY_POISONOUS_IF_YOU_INGEST_IT) == QUEST_STATUS_INCOMPLETE)
            {
                _playerGUID = player->GetGUID();

                if (Creature* ettin = me->FindNearestCreature(NPC_FOREST_ETTIN, 40.0f))
                    me->GetMotionMaster()->MoveFollow(ettin, 2.0f, float(M_PI * 2));
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_ETTIN:
                {
                    if (Creature* ettin = me->FindNearestCreature(NPC_FOREST_ETTIN, 3.0f))
                        _events.ScheduleEvent(EVENT_CHECK_ETTIN + 1, 1s);
                    else
                        _events.ScheduleEvent(EVENT_CHECK_ETTIN, 1s);

                    break;
                }

                case EVENT_CHECK_ETTIN + 1:
                {
                    if (Creature* ettin = me->FindNearestCreature(NPC_FOREST_ETTIN, 3.0f))
                    {
                        ettin->SetFacingToObject(me);

                        _events.ScheduleEvent(EVENT_CHECK_ETTIN + 2, 2s);
                    }

                    break;
                }

                case EVENT_CHECK_ETTIN + 2:
                {
                    if (Creature* ettin = me->FindNearestCreature(NPC_FOREST_ETTIN, 3.0f))
                        ettin->GetAI()->DoAction(ACTION_GRAB_CHICKEN);

                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

// Forest Ettin - 44367
struct npc_silverpine_forest_ettin : public ScriptedAI
{
    npc_silverpine_forest_ettin(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (passenger->GetEntry() == NPC_MUTANT_BUSH_CHICKEN)
        {
            if (apply)
                _events.ScheduleEvent(EVENT_EXPLODE, 4s);
        }
    }

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case ACTION_GRAB_CHICKEN:
            {
                if (Creature* chicken = me->FindNearestCreature(NPC_MUTANT_BUSH_CHICKEN, 5.0f))
                    chicken->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);

                break;
            }

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
                case EVENT_EXPLODE:
                {
                    if (Unit* chicken = me->GetVehicleKit()->GetPassenger(0))
                    {
                        chicken->CastSpell(me, SPELL_BUSH_EXPLOSION, true);

                        AttackStart(chicken->GetOwner());
                    }

                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    ObjectGuid _chickenGUID;
};

// Webbed Victim - 44941
struct npc_silverpine_webbed_victim_skitterweb : public ScriptedAI
{
    npc_silverpine_webbed_victim_skitterweb(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void JustDied(Unit* killer) override
    {
        if (Player* player = killer->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_LOST_IN_THE_DARKNESS) == QUEST_STATUS_INCOMPLETE)
            {
                _playerGUID = player->GetGUID();

                if (roll_chance_i(50))
                {
                    if (roll_chance_i(50))
                    {
                        if (roll_chance_i(50))
                        {
                            player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM1, true);

                            if (Creature* worgen = me->SummonCreature(NPC_BLOODFANG_SCAVENGER, me->GetPosition()))
                                worgen->Attack(player, true);
                        }
                        else
                            player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM1, true);
                    }
                    else
                    {
                        if (roll_chance_i(50))
                        {
                            player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM2, true);

                            if (Creature* bear = me->SummonCreature(NPC_GIANT_RABBID_BEAR, me->GetPosition()))
                                bear->Attack(player, true);
                        }
                        else
                        {
                            player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM2, true);

                            if (Creature* wolf = me->SummonCreature(NPC_RABID_DOG, me->GetPosition()))
                                wolf->Attack(player, true);
                        }
                    }
                }
                else
                {
                    player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM3, true);
                    player->KilledMonsterCredit(NPC_ORC_SEA_DOG, _playerGUID);
                }
            }
        }
    }

private:
    ObjectGuid _playerGUID;
};

// Orc Sea Dog - 44942
struct npc_silverpine_orc_sea_dog : public ScriptedAI
{
    npc_silverpine_orc_sea_dog(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();

        me->SetReactState(REACT_DEFENSIVE);
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            _playerGUID = player->GetGUID();

            me->GetMotionMaster()->MoveFollow(player, 4.0f, frand(1.57f, 4.71f), true, true, false);

            _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1s);
            _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 1s + 500ms);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->IsAlive() && player->IsInWorld() && player->hasQuest(QUEST_LOST_IN_THE_DARKNESS) && !player->IsQuestRewarded(QUEST_LOST_IN_THE_DARKNESS))
                            _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1s);
                    }
                    else
                        me->DespawnOrUnsummon();

                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    Talk(TALK_0);
                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

// Skitterweb Matriarch - 44906
struct npc_silverpine_skitterweb_matriarch : public ScriptedAI
{
    npc_silverpine_skitterweb_matriarch(Creature* creature) : ScriptedAI(creature), _alreadyPulled(false) { }

    void JustAppeared() override
    {
        _alreadyPulled = false;

        me->SetDisableGravity(true);
        me->SetHover(true);

        me->SetAIAnimKitId(DATA_SPIDER_ANIMKIT1);

        me->SetReactState(REACT_PASSIVE);

        me->GetCreatureListWithEntryInGrid(_stalkerList, NPC_SKITTERWEB_STALKER, 8.0f);
        for (Creature* stalker : _stalkerList)
            me->CastSpell(stalker, SPELL_SKITTERWEB, true);
    }

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        if (!_alreadyPulled)
        {
            _alreadyPulled = true;

            me->SetAIAnimKitId(0);
            me->PlayOneShotAnimKitId(DATA_SPIDER_ANIMKIT2);

            _events.ScheduleEvent(EVENT_AGGRO_SPIDER, 2s + 500ms);
        }
        else
        {
            _events.ScheduleEvent(EVENT_SUMMON_SPIDERLINGS, 2s);
            _events.ScheduleEvent(EVENT_VENOM_SPLASH, 4s, 7s);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_AGGRO_SPIDER:
                {
                    me->SetDisableGravity(false);
                    me->SetHover(false);

                    me->GetMotionMaster()->MoveFall();

                    _events.ScheduleEvent(EVENT_AGGRO_SPIDER + 1, 1s);
                    break;
                }

                case EVENT_AGGRO_SPIDER + 1:
                {
                    for (Creature* stalker : _stalkerList)
                        stalker->RemoveAura(SPELL_SKITTERWEB);

                    me->SetAIAnimKitId(1);

                    me->UpdateMovementFlags();

                    me->SetHomePosition(me->GetPosition());

                    _events.ScheduleEvent(EVENT_AGGRO_SPIDER + 2, 1s);
                    break;
                }

                case EVENT_AGGRO_SPIDER + 2:
                {
                    me->SetReactState(REACT_AGGRESSIVE);

                    _events.ScheduleEvent(EVENT_SUMMON_SPIDERLINGS, 2s);
                    _events.ScheduleEvent(EVENT_VENOM_SPLASH, 4s, 7s);
                    break;
                }

                case EVENT_SUMMON_SPIDERLINGS:
                {
                    me->CastSpell(me, SPELL_SUMMNON_SPIDERLINGS, false);
                    break;
                }

                case EVENT_VENOM_SPLASH:
                {
                    DoCastVictim(SPELL_VENOM_SPLASH);

                    _events.ScheduleEvent(EVENT_VENOM_SPLASH, 15s, 18s);
                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _alreadyPulled;
    std::vector<Creature*> _stalkerList;
};

enum FenrisIsleQuests
{
    QUEST_RISE_FORSAKEN                        = 27097,
    QUEST_NO_ESCAPE                            = 27099,
    QUEST_LORDAERON                            = 27098,

    NPC_AGATHA_FENRIS                          = 44951,
    NPC_HILLSBRAD_REFUGEE1                     = 44954,
    NPC_HILLSBRAD_REFUGEE2                     = 44966,
    NPC_FORSAKEN                               = 44959,
    NPC_FENRIS_KEEP_STALKER                    = 45032,
    NPC_FENRIS_KEEP_CAMERA                     = 45003,
    NPC_BLOODFANG_FENRIS                       = 44990,
    NPC_CROWLEY_FENRIS                         = 44989,
    NPC_PHIN_ODELIC                            = 44993,
    NPC_BARTOLO_GINSETTI                       = 44994,
    NPC_LOREMASTER_DIBBS                       = 44995,
    NPC_MAGISTRATE_HENRY_MALEB                 = 44996,
    NPC_CARETAKER_SMITHERS                     = 44997,
    NPC_SOPHIA_ZWOSKI                          = 45002,

    SPELL_BOND_OF_THE_VALKYR                   = 83979,
    SPELL_SUMMON_AGATHA                        = 83982,
    SPELL_DEATH_WALK                           = 85451,
    SPELL_AGATHA_BROADCAST                     = 83978,
    SPELL_NOTIFY_AGATHA                        = 83990,
    SPELL_RISE_FORSAKEN                        = 83993,
    SPELL_KILL_CREDIT_RISE                     = 83996,
    SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_FENRIS = 83997,
    SPELL_DETECT_INV_ZONE_4                    = 84184,
    SPELL_DOOMHOWL                             = 84012,
    SPELL_UNHOLY_DARKNESS                      = 84013,
    SPELL_UNHOLY_SMITE                         = 84014,
    SPELL_LORDAERON_MIGHT                      = 87104,
    SPELL_BROADCAST                            = 83978,
    SPELL_GENERAL_TRIGGER_84107                = 84107,
    SPELL_GENERAL_TRIGGER_84114                = 84114,
    SPELL_ARMORE_CAMERA_1                      = 84112,
    SPELL_RIDE_VEHICLE                         = 84109,

    SPELL_BLOOD_ODELIC                         = 84094,
    SPELL_BLOOD_BARTOLO                        = 84095,
    SPELL_BLOOD_DIBBS                          = 84096,
    SPELL_BLOOD_HENRY                          = 84097,
    SPELL_BLOOD_SMITHERS                       = 84098,
    SPELL_BLOOD_ZWOSKI                         = 84099,
    SPELL_FORCE_CAST_FENRIS_CAMERA             = 84113,
    SPELL_FORCE_SEAT_2                         = 84091,
    SPELL_SUMMON_BLOODFANG_FENRIS              = 84054,
    SPELL_SUMMON_CROWLEY_FENRIS                = 84055,
    SPELL_SUMMON_PHIN_ODELIC                   = 84056,
    SPELL_SUMMON_BARTOLO_GINSETTI              = 84057,
    SPELL_SUMMON_LOREMASTER_DIBBS              = 84058,
    SPELL_SUMMON_MAGISTRATE_HENRY              = 84059,
    SPELL_SUMMON_CARETAKER_SMITHERS            = 84060,
    SPELL_SUMMON_SOPHIA                        = 84061,
    SPELL_CONVERSATION_TRIGGER_84076           = 84076,
    SPELL_CONVERSATION_TRIGGER_84077           = 84077,
    SPELL_ARMORE_CAMERA_2                      = 84104,
    SPELL_GENERAL_TRIGGER_84102                = 84102,

    SPELL_SUMMON_SYLVANAS_AND_HORSE            = 84128,
    SPELL_SUMMON_FORSAKEN_WARHORSE             = 84164,

    EVENT_CHECK_QUEST                          = 101,
    EVENT_MASTER_RESET,
    EVENT_DRINKING,
    EVENT_MORPHING,
    EVENT_HOWLING,
    EVENT_MOVE_TO_STARTPOINT,
    EVENT_CHANGE_TO_SEAT_2,
    EVENT_TRIGGER_84102,
    EVENT_ANIMATION                            = 200,
    EVENT_CHAT_TO_PLAYER                       = 300,
    EVENT_CAST_AGGRO,
    EVENT_HEAL_COOLDOWN,
    EVENT_RUN,
    EVENT_LORDAERON_MIGHT,

    DATA_WAYPOINT_AGATHA                       = 4495101,
    DATA_SOUND_HOWLING                         = 17671
};

// Agatha - 44951
struct npc_silverpine_agatha_fenris : public ScriptedAI
{
    npc_silverpine_agatha_fenris(Creature* creature) : ScriptedAI(creature), _doomHoulDone(false), _healCD(false) { }

    void JustAppeared() override
    {
        me->GetMotionMaster()->Clear();
    }

    void IsSummonedBy(Unit* summoner) override
    {
        _playerGUID = summoner->GetGUID();

        if (summoner->GetGuardianPet())
            me->GetMotionMaster()->MoveFollow(summoner, 4.0f, (float)M_PI * 1.5f, false, true, false);
        else
            me->GetMotionMaster()->MoveFollow(summoner, 4.0f, (float)M_PI / 2.0f, false, true, false);

        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        _events.ScheduleEvent(EVENT_CHECK_PLAYER, 2s);

        me->SetReactState(REACT_DEFENSIVE);
    }

    void Reset() override
    {
        _events.Reset();

        me->GetMotionMaster()->Clear();

        if (me->GetOwner()->GetGuardianPet())
            me->GetMotionMaster()->MoveFollow(me->GetOwner(), 4.0f, (float)M_PI * 1.5f, false, true, false);
        else
            me->GetMotionMaster()->MoveFollow(me->GetOwner(), 4.0f, (float)M_PI / 2.0f, false, true, false);

        _targetGUID = ObjectGuid::Empty;

        if (Creature* stalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 25.0f))
            SetForQuest27099();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84114:
            {
                SetForQuest27099();
                break;
            }

            case SPELL_GENERAL_TRIGGER_84107:
            {
                _events.ScheduleEvent(EVENT_RUN, 1s);
                break;
            }

            default:
                break;
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == POINT_MOTION_TYPE && id == 1231)
            _events.ScheduleEvent(EVENT_RUN + 1, 100ms);
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOINT_AGATHA && waypointId == 42)
            _events.ScheduleEvent(EVENT_RUN + 4, 100ms);
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (apply && passenger->ToPlayer())
            _events.ScheduleEvent(EVENT_RUN + 3, 100ms);
    }

    void DamageTaken(Unit* attacker, uint32& /*damage*/) override
    {
        if (attacker->GetGUID() != _targetGUID)
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            {
                if (Unit* unit = player->GetVictim())
                {
                    if (unit->GetGUID() != _targetGUID)
                        JustEnteredCombat(unit);
                }
            }
        }
    }

    void JustEngagedWith(Unit* who) override
    {
        _doomHoulDone = false;

        _targetGUID = who->GetGUID();

        me->CastSpell(who, SPELL_UNHOLY_SMITE);

        _events.RescheduleEvent(EVENT_CAST_AGGRO, 2s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->IsAlive() || player->IsInWorld())
                        {
                            if (CheckQuestStatus(player))
                            {
                                if (!_healCD && player->GetHealthPct() < 90.0f)
                                {
                                    me->CastSpell(player, SPELL_UNHOLY_DARKNESS);

                                    _healCD = true;

                                    _events.ScheduleEvent(EVENT_HEAL_COOLDOWN, 4s);
                                }

                                if (Unit* unit = player->GetVictim())
                                {
                                    if (unit->GetGUID() != _targetGUID)
                                        JustEnteredCombat(unit);
                                }
                                else if (player->IsInCombat())
                                {
                                    if (Unit* unit = player->GetSelectedUnit())
                                    {
                                        if (unit->GetGUID() != _targetGUID)
                                            JustEnteredCombat(unit);
                                    }
                                }
                            }
                        }

                        _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1s);
                    }
                    break;
                }

                case EVENT_CAST_AGGRO:
                {
                    if (Creature* target = ObjectAccessor::GetCreature(*me, _targetGUID))
                    {
                        if (target->IsAlive())
                        {
                            if (target->GetEntry() == NPC_HILLSBRAD_REFUGEE1 || target->GetEntry() == NPC_HILLSBRAD_REFUGEE2)
                            {
                                if (!_doomHoulDone)
                                {
                                    me->CastSpell(me, SPELL_DOOMHOWL);

                                    _doomHoulDone = true;
                                    break;
                                }
                            }
                        }
                    }

                    _events.ScheduleEvent(EVENT_CAST_AGGRO, 3s);
                    break;
                }

                case EVENT_HEAL_COOLDOWN:
                {
                    _healCD = false;
                    break;
                }

                case EVENT_RUN:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        Position pos = player->GetPosition();

                        me->GetMotionMaster()->MovePoint(1231, pos);
                    }
                    break;
                }

                case EVENT_RUN + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        player->ExitVehicle();

                        me->CastSpell(player, SPELL_ARMORE_CAMERA_2, true);

                        Talk(TALK_3);
                    }

                    _events.ScheduleEvent(EVENT_RUN + 2, 1s + 500ms);
                    break;
                }

                case EVENT_RUN + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->CastSpell(player, SPELL_RIDE_VEHICLE);

                        player->EnterVehicle(me);

                        Talk(TALK_4);
                    }

                    _events.ScheduleEvent(EVENT_RUN + 3, 1s);
                    break;
                }

                case EVENT_RUN + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        player->KilledMonsterCredit(NPC_AGATHA_FENRIS);

                    me->SetDisableGravity(true);

                    me->GetMotionMaster()->MovePath(DATA_WAYPOINT_AGATHA, false);
                    break;
                }

                case EVENT_RUN + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->RemoveAura(SPELL_RIDE_VEHICLE);
                        player->ExitVehicle();
                    }

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveFollow(me->GetOwner(), 4.0f, (float)M_PI / 2.0f, false, true, false);

                    break;
                }
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    bool CheckQuestStatus(Player* player)
    {
        if (player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_COMPLETE)
            return true;
        else if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_COMPLETE)
            return true;

        return false;
    }

    void SetForQuest27099()
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);

        me->GetMotionMaster()->MovePoint(1234, 982.57f, 671.04f, 77.298f);
        Talk(TALK_2);

        if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            player->RemoveAura(SPELL_BOND_OF_THE_VALKYR);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _targetGUID;
    bool _doomHoulDone;
    bool _healCD;
};

// Agatha Broadcast - 83978
class spell_silverpine_agatha_broadcast : public SpellScript
{
    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (caster)
        {
            _ownerGUID = caster->ToPlayer()->GetGUID();

            if (Creature* agatha = caster->FindNearestCreature(NPC_AGATHA_FENRIS, 50.0f))
            {
                if (agatha->GetOwner() == caster)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*caster, _ownerGUID))
                        agatha->AI()->Talk(TALK_0, player);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_agatha_broadcast::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

private:
    ObjectGuid _ownerGUID;
};

// Notify Agatha - 83990
class spell_silverpine_notify_agatha : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (caster && target)
        {
            _ownerGUID = target->ToPlayer()->GetGUID();

            if (Creature* agatha = target->FindNearestCreature(NPC_AGATHA_FENRIS, 50.0f, true))
            {
                if (agatha->GetOwner() == target)
                {
                    agatha->CastSpell(caster, SPELL_RISE_FORSAKEN, false);

                    if (Player* player = ObjectAccessor::GetPlayer(*target, _ownerGUID))
                    {
                        if (roll_chance_i(50))
                            agatha->AI()->Talk(TALK_1, player);
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_notify_agatha::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }

private:
    ObjectGuid _ownerGUID;
};

/*
HACKFIX: The next spell is a masterscript which is cast by the creature once it dies, which uses 8 different spells that summon 8 different npcs (4 male, 4 female).
However, according to sniffs, each 4 of them share the same displayIds on their creature_template. This means that, for example, no matter what spell the male npc uses,
the summoned npc will have a random displayId, which may or may not match its displayId. I could modify their creature_templates so that each of them
has a unique displayId, which would fix the problem, but Funjoker would kill me. Basically, the only way to fix this for now is making a hackfix to ignore the spells,
and simply change their displayIds on summon until proper support for disabling displayId probabilities gets implemented into TrinityCore.
*/

enum SpellForsakenTrooperMasterScriptFenris
{
    SPELL_FORSAKEN_TROOPER_MALE_01_F                = 83998,
    SPELL_FORSAKEN_TROOPER_MALE_02_F                = 83999,
    SPELL_FORSAKEN_TROOPER_MALE_03_F                = 84000,
    SPELL_FORSAKEN_TROOPER_MALE_04_F                = 84001,
    SPELL_FORSAKEN_TROOPER_FEMALE_01_F              = 84002,
    SPELL_FORSAKEN_TROOPER_FEMALE_02_F              = 84003,
    SPELL_FORSAKEN_TROOPER_FEMALE_03_F              = 84004,
    SPELL_FORSAKEN_TROOPER_FEMALE_04_F              = 84005,

    DISPLAY_MALE_01_D_F                             = 33986, // negro sueter
    DISPLAY_MALE_02_D_F                             = 33987, // lila claro
    DISPLAY_MALE_03_D_F                             = 33988, // lila oscuro
    DISPLAY_MALE_04_D_F                             = 33989, // lila toga
    DISPLAY_FEMALE_01_D_F                           = 33991, // verde sueter
    DISPLAY_FEMALE_02_D_F                           = 33992, // marron sueter
    DISPLAY_FEMALE_03_D_F                           = 33993, // negro sueter
    DISPLAY_FEMALE_04_D_F                           = 33994,  // amarillo toga

    DISPLAY_MALE_01_F                               = 33978, // negro sueter
    DISPLAY_MALE_02_F                               = 33979, // lila claro
    DISPLAY_MALE_03_F                               = 33980, // lila oscuro
    DISPLAY_MALE_04_F                               = 33981, // lila toga
    DISPLAY_FEMALE_01_F                             = 33982, // verde sueter
    DISPLAY_FEMALE_02_F                             = 33983, // marron sueter
    DISPLAY_FEMALE_03_F                             = 33984, // negro sueter
    DISPLAY_FEMALE_04_F                             = 33985  // amarillo toga
};

// Forsaken Trooper Master Script (Fenris Isle) - 83997
class spell_silverpine_forsaken_trooper_masterscript_fenris : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* unit = GetCaster())
        {
            if (Creature* hillsbradRefugee = unit->ToCreature())
            {
                if (hillsbradRefugee->GetDisplayId() == DISPLAY_MALE_01_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_MALE_01_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_MALE_02_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_MALE_02_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_MALE_03_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_MALE_03_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_MALE_04_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_MALE_04_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_FEMALE_01_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_FEMALE_01_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_FEMALE_02_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_FEMALE_02_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_FEMALE_03_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_FEMALE_03_F, true);

                if (hillsbradRefugee->GetDisplayId() == DISPLAY_FEMALE_04_F)
                    hillsbradRefugee->CastSpell(hillsbradRefugee, SPELL_FORSAKEN_TROOPER_FEMALE_04_F, true);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_forsaken_trooper_masterscript_fenris::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Hillsbrad Refugee - 44954, 44966
struct npc_silverpine_hillsbrad_refugee : public ScriptedAI
{
    npc_silverpine_hillsbrad_refugee(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_RISE_FORSAKEN:
            {
                me->CastSpell(me, SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_FENRIS, true);

                me->DespawnOrUnsummon(2s);
                break;
            }

            default:
                break;
        }
    }

    void JustDied(Unit* killer) override
    {
        if (Player* player = killer->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_INCOMPLETE)
                me->CastSpell(killer, SPELL_NOTIFY_AGATHA, true);
        }
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_LORDAERON_MIGHT, 3s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LORDAERON_MIGHT:
                {
                    me->CastSpell(me, SPELL_LORDAERON_MIGHT, false);

                    _events.RescheduleEvent(EVENT_LORDAERON_MIGHT, 15s, 18s);
                    break;
                }

                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

// Forsaken Trooper (Male) - 44958, 44959, 44960, 44961
struct npc_silverpine_forsaken_trooper_male_fenris : public ScriptedAI
{
    npc_silverpine_forsaken_trooper_male_fenris(Creature* creature) : ScriptedAI(creature) {}

    void IsSummonedBy(Unit* summoner) override
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);

        if (summoner->GetDisplayId() == DISPLAY_MALE_01_D_F)
            me->SetDisplayId(DISPLAY_MALE_01_F);

        if (summoner->GetDisplayId() == DISPLAY_MALE_02_D_F)
            me->SetDisplayId(DISPLAY_MALE_02_F);

        if (summoner->GetDisplayId() == DISPLAY_MALE_03_D_F)
            me->SetDisplayId(DISPLAY_MALE_03_F);

        if (summoner->GetDisplayId() == DISPLAY_MALE_04_D_F)
            me->SetDisplayId(DISPLAY_MALE_04_F);

        if (roll_chance_i(50))
            Talk(TALK_0);
    }

    void Reset() override
    {
        _events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

// Forsaken Trooper (Female) - 44962, 44963, 44964, 44965
struct npc_silverpine_forsaken_trooper_female_fenris : public ScriptedAI
{
    npc_silverpine_forsaken_trooper_female_fenris(Creature* creature) : ScriptedAI(creature) {}

    void IsSummonedBy(Unit* summoner) override
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);

        if (summoner->GetDisplayId() == DISPLAY_FEMALE_01_D_F)
            me->SetDisplayId(DISPLAY_FEMALE_01_F);

        if (summoner->GetDisplayId() == DISPLAY_FEMALE_02_D_F)
            me->SetDisplayId(DISPLAY_FEMALE_02_F);

        if (summoner->GetDisplayId() == DISPLAY_FEMALE_03_D_F)
            me->SetDisplayId(DISPLAY_FEMALE_03_F);

        if (summoner->GetDisplayId() == DISPLAY_FEMALE_04_D_F)
            me->SetDisplayId(DISPLAY_FEMALE_04_F);

        if (roll_chance_i(50))
            Talk(TALK_0);
    }

    void Reset() override
    {
        _events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

// Silverpine Forest - 130
class playerScript_silverpine_zone : public PlayerScript
{
public:
    playerScript_silverpine_zone() : PlayerScript("playerScript_silverpine_zone") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        // HACK FIX: we need this to make sure the player summons the NPC after logging in. Since the aura is based on spell_area, adding the aura again won't summon the NPCs
        // because the player still has the aura on log off (this kind of behaviour should be implemented at some point, simply adding the aura as not kept after logging off won't make it work).
        if (player->GetZoneId() == 130) // Silverpine Forest
        {
            if (player->GetQuestStatus(QUEST_STEEL_THUNDER) == QUEST_STATUS_INCOMPLETE)
            {
                if (player->HasAura(SPELL_SUMMON_SEA_PUP))
                {
                    player->RemoveAura(SPELL_SUMMON_SEA_PUP);

                    player->CastSpell(player, SPELL_SUMMON_SEA_PUP, true);
                }
                else
                    player->CastSpell(player, SPELL_SUMMON_SEA_PUP, true);
            }

            if (player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_INCOMPLETE)
            {
                if (player->HasAura(SPELL_SUMMON_AGATHA))
                {
                    player->RemoveAura(SPELL_SUMMON_AGATHA);

                    player->CastSpell(player, SPELL_BOND_OF_THE_VALKYR, true);
                    player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
                }
                else
                {
                    player->CastSpell(player, SPELL_BOND_OF_THE_VALKYR, true);
                    player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
                }

            }

            if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE)
            {
                if (player->HasAura(SPELL_SUMMON_AGATHA))
                {
                    player->RemoveAura(SPELL_SUMMON_AGATHA);

                    player->CastSpell(player, SPELL_BOND_OF_THE_VALKYR, true);
                    player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
                }
                else
                {
                    player->CastSpell(player, SPELL_BOND_OF_THE_VALKYR, true);
                    player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
                }
            }
        }
    }
};

// Fenris Keep Stalker - 45032
struct npc_silverpine_fenris_stalker : public ScriptedAI
{
    npc_silverpine_fenris_stalker(Creature* creature) : ScriptedAI(creature), _animationIsStarted(false) { }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_CHECK_QUEST, 1s);
        _animationIsStarted = false;
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84107:
            {
                _events.RescheduleEvent(EVENT_MASTER_RESET, 30s);
                break;
            }

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
                case EVENT_CHECK_QUEST:
                {
                    if (!_animationIsStarted)
                    {
                        if (Player* player = me->SelectNearestPlayer(25.0f))
                        {
                            if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE)
                            {
                                _playerGUID = player->GetGUID();

                                _animationIsStarted = true;

                                _events.ScheduleEvent(EVENT_START_ANIMATION, 1s);
                                _events.ScheduleEvent(EVENT_MASTER_RESET, 250s);
                            }
                        }
                    }
                    else
                        _events.ScheduleEvent(EVENT_CHECK_QUEST, 1s);

                    break;
                }

                case EVENT_MASTER_RESET:
                {
                    Reset();
                    break;
                }

                case EVENT_START_ANIMATION:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_FORCE_CAST_FENRIS_CAMERA, true);

                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _animationIsStarted;
};

// Fenris Keep Camera - 45003
struct npc_silverpine_fenris_camera : public ScriptedAI
{
    npc_silverpine_fenris_camera(Creature* creature) : ScriptedAI(creature), _summons(me)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        me->SetVisible(false);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void JustSummoned(Creature* summon) override
    {
        switch (summon->GetEntry())
        {
            _summons.Summon(summon);

            case NPC_BLOODFANG_FENRIS:
            case NPC_CROWLEY_FENRIS:
                break;

            default:
                SetCreatureValues(summon);
                break;
        }

        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
    {
        if (apply && seatId == 0)
            _events.ScheduleEvent(EVENT_MOVE_TO_STARTPOINT, 100ms);

        if (apply && seatId == 1)
            passenger->SetFacingTo(0.0f);

        if (!apply && seatId == 1)
        {
            _events.ScheduleEvent(EVENT_FINISH, 100ms);
            me->DespawnOrUnsummon(2s);
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == POINT_MOTION_TYPE && id == 1230)
            _events.ScheduleEvent(EVENT_CHANGE_TO_SEAT_2, 1s);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84102:
            {
                if (Creature* camera = me->FindNearestCreature(NPC_FENRIS_KEEP_CAMERA, 50.0f))
                {
                    if (Creature* crowley = me->FindNearestCreature(NPC_CROWLEY_FENRIS, 50.0f))
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                            camera->CastSpell(crowley, SPELL_ARMORE_CAMERA_1, true);
                    }
                }

                break;
            }

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
                case EVENT_MOVE_TO_STARTPOINT:
                {
                    me->GetMotionMaster()->MovePoint(1230, 980.7f, 689.14f, 76.9f);

                    _events.ScheduleEvent(EVENT_START_ANIMATION, 500ms);
                    break;
                }

                case EVENT_CHANGE_TO_SEAT_2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_FORCE_SEAT_2, true);

                    _events.ScheduleEvent(EVENT_TRIGGER_84102, 1s + 500ms);
                    break;
                }

                case EVENT_TRIGGER_84102:
                {
                    if (Creature* stalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 25.0f))
                        me->CastSpell(stalker, SPELL_GENERAL_TRIGGER_84102, true);
                    break;
                }

                case EVENT_START_ANIMATION:
                {
                    me->SummonCreature(NPC_CROWLEY_FENRIS, 994.562f, 691.186f, 74.8984f, 0.018045f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_BLOODFANG_FENRIS, 994.53f, 687.74f, 74.8984f, 0.018045f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_PHIN_ODELIC, 1002.52f, 693.027f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_BARTOLO_GINSETTI, 1002.76f, 687.195f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_LOREMASTER_DIBBS, 1002.8f, 684.256f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_MAGISTRATE_HENRY_MALEB, 1000.67f, 689.759f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_CARETAKER_SMITHERS, 1002.7f, 695.775f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);
                    me->SummonCreature(NPC_SOPHIA_ZWOSKI, 1004.53f, 688.619f, 76.1922f, 3.1636f, TEMPSUMMON_MANUAL_DESPAWN, 0, _playerGUID);

                    break;
                }

                case EVENT_FINISH:
                {
                    _summons.DespawnAll();
                    break;
                }

                default:
                    break;
            }
        }
    }

    void SetCreatureValues(Creature* npc)
    {
        Position pos = npc->GetPosition();
        pos.SetOrientation(3.33f);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    SummonList _summons;
};

// Lord Darius Crowley - 44989
struct npc_silverpine_crowley_fenris : public ScriptedAI
{
    npc_silverpine_crowley_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84102:
            {
                _events.ScheduleEvent(EVENT_ANIMATION, 500ms);
                break;
            }

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
                case EVENT_ANIMATION + 0:
                {
                    Talk(TALK_0);
                    _events.ScheduleEvent(EVENT_ANIMATION + 1, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 1:
                {
                    Talk(TALK_1);
                    _events.ScheduleEvent(EVENT_ANIMATION + 2, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 2:
                {
                    Talk(TALK_2);
                    _events.ScheduleEvent(EVENT_ANIMATION + 3, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 3:
                {
                    Talk(TALK_3);
                    _events.ScheduleEvent(EVENT_ANIMATION + 4, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 4:
                {
                    Talk(TALK_4);
                    _events.ScheduleEvent(EVENT_ANIMATION + 5, 6s + 100ms);
                    break;
                }

                case EVENT_ANIMATION + 5:
                {
                    Talk(TALK_5);
                    _events.ScheduleEvent(EVENT_ANIMATION + 6, 9s + 500ms);
                    break;
                }

                case EVENT_ANIMATION + 6:
                {
                    me->CastSpell(me, SPELL_CONVERSATION_TRIGGER_84076, true);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

// Phin Odelic - 44993
struct npc_silverpine_odelic_fenris : public ScriptedAI
{
    npc_silverpine_odelic_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_ODELIC))
                me->RemoveAura(SPELL_BLOOD_ODELIC);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_ODELIC);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isWorgen;
};

// Bartolo Ginsetti - 44994
struct npc_silverpine_bartolo_fenris : public ScriptedAI
{
    npc_silverpine_bartolo_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_BARTOLO))
                me->RemoveAura(SPELL_BLOOD_BARTOLO);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_BARTOLO);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isWorgen;
};

// Loremaster Dibbs - 44995
struct npc_silverpine_loremaster_fenris : public ScriptedAI
{
    npc_silverpine_loremaster_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_DIBBS))
                me->RemoveAura(SPELL_BLOOD_DIBBS);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_DIBBS);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    uint64 _playerGUID;
    bool _isWorgen;
};

// Magistrate Henry Maleb - 44996
struct npc_silverpine_henry_fenris : public ScriptedAI
{
    npc_silverpine_henry_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84076:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    me->CastSpell(player, SPELL_ARMORE_CAMERA_1, true);

                _events.ScheduleEvent(EVENT_ANIMATION, 500ms);
                break;
            }

            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_HENRY))
                me->RemoveAura(SPELL_BLOOD_HENRY);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ANIMATION:
                {
                    Talk(TALK_0);
                    _events.ScheduleEvent(EVENT_ANIMATION + 1, 10s + 800ms);
                    break;
                }

                case EVENT_ANIMATION + 1:
                {
                    Talk(TALK_1);
                    _events.ScheduleEvent(EVENT_ANIMATION + 2, 3s + 500ms);
                    break;
                }

                case EVENT_ANIMATION + 2:
                {
                    me->CastSpell(me, SPELL_CONVERSATION_TRIGGER_84077, true);
                    break;
                }

                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_HENRY);
                    me->CastSpell(me, SPELL_GENERAL_TRIGGER_84107, true);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isWorgen;
};

// Caretaker Smithers - 44997
struct npc_silverpine_caretaker_fenris : public ScriptedAI
{
    npc_silverpine_caretaker_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_SMITHERS))
                me->RemoveAura(SPELL_BLOOD_SMITHERS);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_SMITHERS);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isWorgen;
};

// Sophia Zwoski - 45002
struct npc_silverpine_sophia_fenris : public ScriptedAI
{
    npc_silverpine_sophia_fenris(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _isWorgen = false;
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 100ms);
                break;
            }

            default:
                break;
        }

        if (!_isWorgen)
        {
            if (me->HasAura(SPELL_BLOOD_ZWOSKI))
                me->RemoveAura(SPELL_BLOOD_ZWOSKI);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 3s);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;
                    me->CastSpell(me, SPELL_BLOOD_ZWOSKI);
                    _events.ScheduleEvent(EVENT_HOWLING, 1s + 500ms);
                    break;
                }

                case EVENT_HOWLING:
                {
                    me->PlayDirectSound(DATA_SOUND_HOWLING);
                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    bool _isWorgen;
};

enum LordaeronQuest
{
    QUEST_TO_FORSAKEN_HIGH_COMMAND      = 27290,

    NPC_SYLVANAS_LORDAERON              = 45051,
    NPC_FORSAKEN_WARHORSE_SYLVANAS      = 45057,
    NPC_FORSAKEN_WARHORSE_PLAYER        = 45041,
    NPC_ORC_MOVER                       = 45589,
    NPC_ORC_DEMOLISHER_LORDAERON        = 45635,
    NPC_DREADGUARD_LORDAREON            = 45588,
    NPC_ARTHURA_FHC                     = 44610,

    SPELL_SUMMON_LORDAERON_ACTORS       = 84127, // NYI, cast by player on player
    SPELL_RIDE_SYLVANAS_HORSE           = 84166,
    SPELL_LORDAERON_AURA                = 84189, // NYI
    SPELL_KILL_ME                       = 84180, // Cast by Worgen every
    SPELL_DREADGUARD_SALUTE_PERIODIC    = 84200, // Cast by Sylvanas
    SPELL_DREADGUARD_SALUTE             = 84199, 
    SPELL_LORDAERON_COMPLETE            = 84185, // Cast by Sylvanas on player 
    SPELL_FADE_TO_BLACK                 = 89092, // Cast by player on player
    SPELL_APPLY_INVIS_ZONE_1            = 83231,
    SPELL_APPLY_INVIS_ZONE_4            = 84183,
    SPELL_FLIGHT_OF_THE_VALKYR          = 84695,

    EVENT_CHECK_PLAYER_STATUS           = 501,
    EVENT_RESET_HEARTSTRIKE             = 702,

    EVENT_WARHORSE_DESPAWN              = 546,
    EVENT_CAST_CAMERA                   = 547,
    EVENT_RIDE_WARHORSE                 = 548,
    EVENT_START_EMOTE                   = 549,
    EVENT_FINISH_RIDE                   = 550,

    DATA_LORDAERON_RIDE                 = 54,
    DATA_FINISH_RIDE                    = 55,
    DATA_EMOTE_PREPARE_BOW              = 384,
    DATA_ANIMKIT_DREADGUARD             = 898,
    DATA_WAYPOIHT_WARHORSE              = 450570,
    DATA_ORC_LEADER_PATHID              = 4494200,
    DATA_ARTHURA_PATHID                 = 453180
};

/* DOCUMENTATION ON THIS QUEST:

* It seems all of the actors included during this quest are temporary summons which can only be seen by the player. They're summoned through SPELL_SUMMON_LORDAERON_ACTORS.
* There are missing spawns so it's not completely implemented yet.
*/

// Lady Sylvanas Windrunner - 44365
struct npc_silverpine_sylvanas_fhc : public ScriptedAI
{
    npc_silverpine_sylvanas_fhc(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
    {
        if (me->GetPhaseShift().HasPhase(DEFAULT_PHASE) || me->GetPhaseShift().HasPhase(PHASE_FORSAKEN_HIGH_COMMAND_INTRODUCTION))
        {
            if (Creature* warhorse = me->SummonCreature(NPC_FORSAKEN_WARHORSE_UNPHASED, me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (me->HasAura(SPELL_APPLY_INVIS_ZONE_1))
                    warhorse->AddAura(SPELL_APPLY_INVIS_ZONE_1, warhorse);
                else if (me->HasAura(SPELL_APPLY_INVIS_ZONE_4))
                    warhorse->AddAura(SPELL_APPLY_INVIS_ZONE_4, warhorse);

                me->EnterVehicle(warhorse, 0);

                warhorse->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }
        }
    }

    void QuestAccept(Player* player, const Quest* quest) override
    {
        if (quest->GetQuestId() == QUEST_RISE_FORSAKEN)
        {
            player->CastSpell(player, SPELL_BOND_OF_THE_VALKYR, true); // This triggers Agatha's broadcast every 60s.
            player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
        }
        else if (quest->GetQuestId() == QUEST_LORDAERON)
        {
            player->CastSpell(player, SPELL_SUMMON_SYLVANAS_AND_HORSE, true);
            player->CastSpell(player, SPELL_SUMMON_FORSAKEN_WARHORSE, true);
            player->CastSpell(player, SPELL_LORDAERON_AURA, true);
            player->CastSpell(player, SPELL_SUMMON_LORDAERON_ACTORS, true);

            player->NearTeleportTo(1383.0f, 1038.63f, 54.3172f, 3.9f);
        }
        else if (quest->GetQuestId() == QUEST_TO_FORSAKEN_HIGH_COMMAND)
            player->CastSpell(player, SPELL_FLIGHT_OF_THE_VALKYR, true);
    }

    void QuestReward(Player* player, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_NO_ESCAPE)
        {
            player->RemoveAura(SPELL_BOND_OF_THE_VALKYR);
            player->RemoveAura(SPELL_SUMMON_AGATHA);
        }
    }

    void Reset() override
    {
        _events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

Position const ForsakenTrooper[3] =
{
    { 1072.83f, 1270.05f, 43.02f, 3.16124f },
    { 1013.34f, 1313.89f, 42.85f, 3.30653f },
    { 969.38f, 1361.34f, 47.302f, 3.53822f }
};

Position const WorgenRenegade[3] =
{
    { 1072.83f, 1270.05f, 43.02f, 3.16124f },
    { 1013.34f, 1313.89f, 42.85f, 3.30653f },
    { 969.38f, 1361.34f, 47.302f, 3.53822f }
};

Position const OrcDemolisherPos[3] =
{
    { 1072.83f, 1270.05f, 43.02f, 3.16124f },
    { 1013.34f, 1313.89f, 42.85f, 3.30653f },
    { 969.38f, 1361.34f, 47.302f, 3.53822f }
};

Position const SeaOrcPos[9] =
{
    { 1076.48f, 1253.85f, 45.929f, 2.43872f },
    { 1077.14f, 1263.65f, 43.694f, 1.59048f },
    { 1070.36f, 1263.61f, 45.013f, 1.56299f },

    { 1014.53f, 1307.78f, 43.935f, 1.54728f },
    { 1003.97f, 1315.32f, 44.549f, 4.10375f },
    { 1002.05f, 1338.77f, 44.450f, 3.85635f },

    { 971.74f, 1354.18f, 46.846f, 3.46365f },
    { 964.22f, 1366.20f, 46.887f, 5.15618f },
    { 969.18f, 1368.70f, 47.393f, 5.03445f },
};

Position const SeaOrcLeaderPos = { 1010.04f, 1301.17f, 45.891f, 2.53684f };

Position const DreadguardPos[16] =
{
    { 1387.90f, 1029.71f, 53.21853f, 2.827433f },
    { 1389.79f, 1024.51f, 53.20833f, 2.827433f },
    { 1388.05f, 1026.91f, 53.20833f, 2.827433f },
    { 1388.16f, 1020.88f, 53.25523f, 2.827433f },
    { 1385.79f, 1025.99f, 53.22593f, 2.827433f },
    { 1386.69f, 1023.26f, 53.24393f, 2.827433f },
    { 1384.33f, 1022.04f, 53.28123f, 2.827433f },
    { 1391.10f, 1027.73f, 53.20483f, 2.827433f }
};

// Forsaken Warhorse (Player) - 45041
struct npc_silverpine_warhorse_player_lordaeron : public ScriptedAI
{
    npc_silverpine_warhorse_player_lordaeron(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
        _playerGUID = ObjectGuid::Empty;
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            _playerGUID = player->GetGUID();

            player->EnterVehicle(me);
        }
    }

    void SetData(uint32 id, uint32 /*value*/) override
    {
        switch (id)
        {
            case DATA_FINISH_RIDE:
            {
                _events.ScheduleEvent(EVENT_WARHORSE_DESPAWN, 0ms);
                break;
            }

            default:
                break;
        }
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (apply && passenger->GetTypeId() == TYPEID_PLAYER)
        {
            me->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);

            if (Creature* sylvanas = me->FindNearestCreature(NPC_SYLVANAS_LORDAERON, 10.0f, true))
                sylvanas->AI()->SetData(DATA_LORDAERON_RIDE, DATA_LORDAERON_RIDE);

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_WARHORSE_DESPAWN:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_FADE_TO_BLACK, true);

                    me->GetVehicleKit()->RemoveAllPassengers();

                    me->DespawnOrUnsummon(1s);

                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

// Forsaken Warhorse (Sylvanas) - 45057
struct npc_silverpine_warhorse_sylvanas_lordaeron : public ScriptedAI
{
    npc_silverpine_warhorse_sylvanas_lordaeron(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _sylvanasGUID = ObjectGuid::Empty;
        _playerGUID = ObjectGuid::Empty;
        _playerHorseGUID = ObjectGuid::Empty;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (Creature* sylvanas = me->FindNearestCreature(NPC_SYLVANAS_LORDAERON, 10.0f))
                _sylvanasGUID = sylvanas->GetGUID();

            if (Player* player = who->ToPlayer())
                _playerGUID = player->GetGUID();

            if (Creature* playerhorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_PLAYER, 10.0f))
                _playerHorseGUID = playerhorse->GetGUID();
        }
    }

    void PassengerBoarded(Unit* /*passenger*/, int8 seatId, bool apply) override
    {
        if (!apply)
            return;

        if (seatId == 0)
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_WAYPOIHT_WARHORSE)
        {
            if (waypointId == 42)
                me->GetMotionMaster()->MovePoint(1, 499.185f, 1549.9855f, 129.094f, false, 3.5f);
        }
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (type == POINT_MOTION_TYPE && pointId == 1)
        {
            if (Creature* playerhorse = ObjectAccessor::GetCreature(*me, _playerHorseGUID))
            {
                playerhorse->AI()->SetData(DATA_FINISH_RIDE, DATA_FINISH_RIDE);

                me->SetFacingTo(4.40509f);
                me->StopMoving();

                if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    sylvanas->AI()->SetData(DATA_FINISH_RIDE, DATA_FINISH_RIDE);

                _events.ScheduleEvent(EVENT_CHECK_FINISH, 1s + 500ms);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetQuestStatus(QUEST_LORDAERON) == QUEST_STATUS_COMPLETE)
                            me->DespawnOrUnsummon();
                    }

                    break;
                }

            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _playerHorseGUID;
    ObjectGuid _sylvanasGUID;
};

// Lady Sylvanas Windrunner - 45051
struct npc_silverpine_sylvanas_lordaeron : public ScriptedAI
{
    npc_silverpine_sylvanas_lordaeron(Creature* creature) : ScriptedAI(creature), _heartstrikeCD(false)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
        _playerHorseGUID = ObjectGuid::Empty;
        _sylvanasHorseGUID = ObjectGuid::Empty;
    }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* sylvanashorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_SYLVANAS, 10.0f))
        {
            _sylvanasHorseGUID = sylvanashorse->GetGUID();

            me->EnterVehicle(sylvanashorse, 0);

            sylvanashorse->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);

            _events.ScheduleEvent(EVENT_RIDE_WARHORSE, 500ms);
        }

        me->CastSpell(me, SPELL_DREADGUARD_SALUTE_PERIODIC, true);
    }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_KILL_ME:
            {
                if (_heartstrikeCD || me->HasUnitState(UNIT_STATE_CASTING))
                    me->HandleEmoteCommand(0);
                else
                {
                    me->HandleEmoteCommand(DATA_EMOTE_PREPARE_BOW);

                    if (caster->ToUnit()->GetEntry() == NPC_WORGEN_RENEGATE)
                        me->CastSpell(caster, SPELL_HEARTSTRIKE, false);

                    _heartstrikeCD = true;

                    _events.ScheduleEvent(EVENT_RESET_HEARTSTRIKE, 3s);
                }

                break;
            }

            default:
                break;
        }
    }

    void SetData(uint32 id, uint32 /*value*/) override
    {
        switch (id)
        {
            case DATA_FINISH_RIDE:
            {
                _events.ScheduleEvent(EVENT_FINISH_RIDE, 0ms);
                break;
            }

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
                case EVENT_RIDE_WARHORSE:
                {
                    if (Creature* playerhorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_PLAYER, 10.0f))
                    {
                        _playerHorseGUID = playerhorse->GetGUID();

                        if (Creature* sylvanashorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_SYLVANAS, 10.0f))
                        {
                            sylvanashorse->GetMotionMaster()->MovePath(DATA_WAYPOIHT_WARHORSE, false);
                            playerhorse->EnterVehicle(sylvanashorse, 1);
                        }
                    }

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 6s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_0);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 1, 4s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_1);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 2, 4s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_2);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 3, 7s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        Talk(TALK_3);
                        me->AddAura(SPELL_DETECT_INV_ZONE_4, player);
                    }

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 4, 4s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_4);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 5, 7s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 5:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_5);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 6, 7s + 600ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 6:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_6);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 7, 8s + 600ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 7:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_7);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 8, 9s + 600ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 8:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_8);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 9, 9s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 9:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_9);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 10, 5s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 10:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_10);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 11, 4s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 11:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_11);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 12, 8s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 12:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_12);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 13, 6s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 13: // And though
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_13);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 14, 10s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 14: // With Arthas
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_14);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 15, 10s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 15: // From
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_15);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 16, 8s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 16: // Our goal
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_16);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 17, 9s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 17: // Lich dead
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_17);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 18, 5s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 18:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_18);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 19, 7s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 19: // But
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_19);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 20, 10s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 20: // I will never
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_20);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 21, 4s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 21: // Lordaeron
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_21);
                    break;
                }

                case EVENT_FINISH_RIDE:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_LORDAERON_COMPLETE, true);
                    break;
                }

                case EVENT_RESET_HEARTSTRIKE:
                {
                    _heartstrikeCD = false;
                    break;
                }

                default:
                    break;
                }
        }

        UpdateVictim();
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _playerHorseGUID;
    ObjectGuid _sylvanasHorseGUID;
    bool _heartstrikeCD;
};

// Dreadguard - 45588
struct npc_silverpine_dreadguard_lordaeron : public ScriptedAI
{
    npc_silverpine_dreadguard_lordaeron(Creature* creature) : ScriptedAI(creature), _done(false) { }

    void Reset() override
    {
        _events.Reset();
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_DREADGUARD_SALUTE:
            {
                if (_done)
                    return;

                me->SetAIAnimKitId(DATA_ANIMKIT_DREADGUARD);

                _events.ScheduleEvent(EVENT_START_EMOTE, 5s);

                _done = true;
                break;
            }

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
                case EVENT_START_EMOTE:
                {
                    if (_done)
                        _done = false;

                    me->SetAIAnimKitId(0);

                    break;
                }

                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    bool _done;
};

// Summon Lordaeron Actors - 84127
class spell_silverpine_summon_lordaeron_actors : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            _playerGUID = caster->GetGUID();

            for (auto pos : OrcDemolisherPos)
                caster->SummonCreature(NPC_ORC_DEMOLISHER_LORDAERON, pos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID);

            for (auto pos : SeaOrcPos)
                caster->SummonCreature(NPC_ORC_MOVER, pos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID);

            if (Creature* leaderOrc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID))
            {
                leaderOrc->GetMotionMaster()->MovePath(DATA_ORC_LEADER_PATHID, true);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID))
                    orc->GetMotionMaster()->MoveFollow(leaderOrc, 3.0f, 0.0f, false, true, false);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID))
                    orc->GetMotionMaster()->MoveFollow(leaderOrc, 6.0f, 0.0f, false, true, false);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID))
                    orc->GetMotionMaster()->MoveFollow(leaderOrc, 9.0f, 0.0f, false, true, false);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos, TEMPSUMMON_TIMED_DESPAWN, 120s, 0, _playerGUID))
                    orc->GetMotionMaster()->MoveFollow(leaderOrc, 12.0f, 0.0f, false, true, false);
            }

            for (auto pos : DreadguardPos)
                caster->SummonCreature(NPC_DREADGUARD_LORDAREON, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_summon_lordaeron_actors::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }

private:
    ObjectGuid _playerGUID;
};

enum SepulcherQuests
{
    QUEST_EXCISING_THE_TAINT        = 27181,

    SPELL_STALKING                  = 86237,
    SPELL_SHADOWSTEP                = 79864,
    SPELL_KILL_ME_QUEST             = 86559,

};

// Bloodfang Stalker - 45195
struct npc_silverpine_bloodfang_stalker : public ScriptedAI
{
    npc_silverpine_bloodfang_stalker(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        me->SetPowerType(POWER_ENERGY);
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 100, true);
    }

    void Reset() override
    {
        me->CastSpell(me, SPELL_STALKING, true);
        me->CastSpell(me, SPELL_KILL_ME_QUEST, true);
    }

    void JustEngagedWith(Unit* who) override
    {
        me->CastSpell(who, SPELL_SHADOWSTEP, false);

        me->RemoveAura(SPELL_STALKING);
        me->RemoveAura(SPELL_KILL_ME_QUEST);
    }

    void JustDied(Unit* killer) override
    {
        if (Player* player = killer->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_EXCISING_THE_TAINT) == QUEST_STATUS_NONE)
            {
                if (const Quest* ExcisingTheTaint = sObjectMgr->GetQuestTemplate(QUEST_EXCISING_THE_TAINT))
                    player->AddQuest(ExcisingTheTaint, nullptr);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

// Arthura - 45318
struct npc_arthura_sepulcher : public ScriptedAI
{
    npc_arthura_sepulcher(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _playerGUID = ObjectGuid::Empty;
    }

    void Reset() override
    {
        _events.Reset();
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            if (Vehicle* vehicle = me->GetVehicleKit())
            {
                _playerGUID = player->GetGUID();

                me->GetMotionMaster()->MovePath(DATA_ARTHURA_PATHID, false);
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == DATA_ARTHURA_PATHID)
        {
            if (waypointId == 1)
                Talk(TALK_0);

            if (waypointId == 20)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                {
                    player->KilledMonsterCredit(NPC_ARTHURA_FHC);

                    me->GetVehicleKit()->RemoveAllPassengers();

                    me->DespawnOrUnsummon(3s);
                }
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

/* Gilneas Part */

void AddSC_silverpine_forest()
{
    new playerScript_silverpine_zone();

    RegisterCreatureAI(npc_silverpine_horde_hauler);

    RegisterCreatureAI(npc_silverpine_worgen_renegade);
    RegisterCreatureAI(npc_silverpine_forsaken_trooper);
    RegisterSpellScript(spell_silverpine_flurry_of_claws);

    RegisterCreatureAI(npc_silverpine_grand_executor_mortuus);
    RegisterCreatureAI(npc_silverpine_fallen_human);
    RegisterSpellScript(spell_silverpine_forsaken_trooper_masterscript_high_command);
    RegisterSpellAndAuraScriptPair(spell_silverpine_raise_forsaken_83173, spell_silverpine_raise_forsaken_83173_aura);
    RegisterCreatureAI(npc_silverpine_bat_handler_maggotbreath);
    RegisterCreatureAI(npc_silverpine_forsaken_bat);
    RegisterSpellScript(spell_silverpine_go_home);
    RegisterCreatureAI(npc_silverpine_vilefine_murlocks);

    RegisterCreatureAI(npc_silverpine_deathstalker);
    RegisterGameObjectAI(go_silverpine_abandoned_outhouse);
    RegisterCreatureAI(npc_silverpine_deathstalker_rane_yorick);
    RegisterCreatureAI(npc_silverpine_armoire);
    RegisterCreatureAI(npc_silverpine_lord_darius_crowley_exhanguinate);
    RegisterCreatureAI(npc_silverpine_packleader_ivar_bloodfang_exhanguinate);
    RegisterSpellScript(spell_silverpine_eject_passenger_1);

    RegisterCreatureAI(npc_silverpine_salty_rocka);
    RegisterCreatureAI(npc_silverpine_apothecary_wormcrud);
    RegisterCreatureAI(npc_silverpine_admiral_hatchet);
    RegisterCreatureAI(npc_silverpine_orc_sea_pup);
    RegisterCreatureAI(npc_silverpine_orc_crate);
    RegisterSpellScript(spell_silverpine_pick_up_orc_crate);
    RegisterSpellScript(spell_silverpine_sea_pup_trigger);
    RegisterCreatureAI(npc_silverpine_mutant_bush_chicken);
    RegisterCreatureAI(npc_silverpine_forest_ettin);
    RegisterCreatureAI(npc_silverpine_webbed_victim_skitterweb);
    RegisterCreatureAI(npc_silverpine_orc_sea_dog);
    RegisterCreatureAI(npc_silverpine_skitterweb_matriarch);

    RegisterCreatureAI(npc_silverpine_sylvanas_fhc);
    RegisterCreatureAI(npc_silverpine_agatha_fenris);
    RegisterSpellScript(spell_silverpine_agatha_broadcast); 
    RegisterSpellScript(spell_silverpine_notify_agatha); 
    RegisterCreatureAI(npc_silverpine_hillsbrad_refugee);
    RegisterCreatureAI(npc_silverpine_forsaken_trooper_male_fenris);
    RegisterCreatureAI(npc_silverpine_forsaken_trooper_female_fenris);
    RegisterSpellScript(spell_silverpine_forsaken_trooper_masterscript_fenris);
    RegisterCreatureAI(npc_silverpine_fenris_stalker);
    RegisterCreatureAI(npc_silverpine_fenris_camera);
    RegisterCreatureAI(npc_silverpine_crowley_fenris);
    RegisterCreatureAI(npc_silverpine_odelic_fenris);
    RegisterCreatureAI(npc_silverpine_bartolo_fenris);
    RegisterCreatureAI(npc_silverpine_loremaster_fenris);
    RegisterCreatureAI(npc_silverpine_henry_fenris);
    RegisterCreatureAI(npc_silverpine_caretaker_fenris);
    RegisterCreatureAI(npc_silverpine_sophia_fenris);

    RegisterSpellScript(spell_silverpine_summon_lordaeron_actors);
    RegisterCreatureAI(npc_silverpine_warhorse_player_lordaeron);
    RegisterCreatureAI(npc_silverpine_warhorse_sylvanas_lordaeron);
    RegisterCreatureAI(npc_silverpine_sylvanas_lordaeron);
    RegisterCreatureAI(npc_silverpine_dreadguard_lordaeron);

    RegisterCreatureAI(npc_silverpine_bloodfang_stalker);
    RegisterCreatureAI(npc_arthura_sepulcher);
}
