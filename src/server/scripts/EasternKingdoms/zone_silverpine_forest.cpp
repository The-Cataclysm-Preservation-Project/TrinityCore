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

enum SilverpineVehicleSeats
{
    SEAT_HAULER_PLAYER           = 2,
    SEAT_HAULER_TROOPER_0        = 3,
    SEAT_HAULER_TROOPER_1        = 4,
    SEAT_HAULER_TROOPER_2        = 5,
    SEAT_HAULER_TROOPER_3        = 6,
    SEAT_HAULER_TROOPER_4        = 7,
    SEAT_HAULER_TROOPER_5        = 8,

    SEAT_BLOODFANG               = 0,
    SEAT_FORSAKEN_BAT            = 0,
    SEAT_FENRIS_CAMERA           = 0,
    SEAT_FENRIS_CAMERA_FORCE     = 1,
    SEAT_WARHORSE_PLAYER         = 0,
    SEAT_WARHORSE_SYLVANAS       = 0,
    SEAT_WARHORSE_PLAYER_BOARDED = 1,
};

enum SilverpineAnimKits
{
    ANIMKIT_RESET                = 0,
    ANIMKIT_INTERACT             = 1,
    ANIMKIT_SYLV_1               = 595,
    ANIMKIT_SYLV_2               = 606,
    ANIMKIT_SYLV_3               = 609,
    ANIMKIT_GARROSH_1            = 662,
    ANIMKIT_GARROSH_2            = 595,
    ANIMKIT_FALLEN_HUMAN         = 721,
    ANIMKIT_TOROK                = 594,
    ANIMKIT_SPIDER_1             = 865,
    ANIMKIT_SPIDER_2             = 866
};

enum SilverpinePhases
{
    PHASE_FORSAKEN_HIGH_COMMAND_INTRODUCTION            = 264, 
    PHASE_WAITING_TO_EXSANGUINATE                       = 265,
    PHASE_NO_ESCAPE                                     = 266,
    PHASE_FOR_LORDAERON                                 = 267, // There seems to be a PhaseGroup (2491)
    PHASE_THE_WATERS_RUN_RED                            = 273, 
    PHASE_GILNEAS_ACT_I                                 = 274, // There seems to be a PhaseGroup (2489)
    PHASE_GILNEAS_ACT_II                                = 275, // There seems to be a PhaseGroup (2490)
    PHASE_GILNEAS_ACT_III                               = 276,
    PHASE_THE_FORSAKEN_FRONT_I                          = 277, 
    PHASE_THE_FORSAKEN_FRONT_II                         = 278, 
    PHASE_AMBER_MILL_I                                  = 279,  // There seems to be a PhaseGroup (2492, 2496)
    PHASE_AMBER_MILL_II                                 = 280,  // There seems to be a PhaseGroup (2493)
    PHASE_AMBER_MILL_III                                = 281,  // There seems to be a PhaseGroup (2494)
    PHASE_THE_FORSAKEN_FRONT_III                        = 284, 
    PHASE_THE_FORSAKEN_FRONT_IV                         = 289  
};

enum SilverpineTransports
{
    NPC_HORDE_ENGINEER_HAULER               = 44734,
    NPC_SUBDUED_FOREST_ETTIN_HAULER         = 44737,
    NPC_FORSAKEN_TROOPER_F                  = 44732,
    NPC_FORSAKEN_TROOPER_M                  = 44733,
    NPC_HORDE_ENGINEER_COFFIN               = 46559,
    NPC_SUBDUED_FOREST_ETTIN_COFFIN         = 46560,

    SPELL_CHAIN_RIGHT_HAULER                = 83467,
    SPELL_CHAIN_LEFT_HAULER                 = 83464,
    SPELL_CHAIN_RIGHT_COFFIN                = 86803,
    SPELL_CHAIN_LEFT_COFFIN                 = 86805,
    SPELL_ANIMKIT_HORDE_ENGINEER            = 91298,
    SPELL_EJECT_PASSENGERS_3_8              = 83477,

    EVENT_TO_FORSAKEN_HIGH_COMMAND          = 1,
    EVENT_TO_THE_SEPULCHER                  = 2,
    EVENT_TO_THE_FORSAKEN_FRONT             = 3,
    EVENT_TO_DESPAWN                        = 4,
    EVENT_YELL_ON_FORSAKEN_HIGH             = 5,
    EVENT_YELL_ON_SEPULCHER                 = 6,
    EVENT_YELL_ON_FORSAKEN_FRONT            = 7,

    TALK_HAULER_BOARDED                     = 0,
    TALK_ON_FORSAKEN_HIGH                   = 1,
    TALK_ON_SEPULCHER                       = 2,
    TALK_ON_FORSAKEN_FRONT                  = 3,

    PATH_FROM_NORTH_TO_SOUTH                = 447310, 
    PATH_FROM_SOUTH_TO_NORTH                = 447640,

    WAYPOINT_ON_FORSAKEN_HIGH               = 10,
    WAYPOINT_ON_SEPULCHER                   = 34,
    WAYPOINT_ON_FORSAKEN_FRONT              = 69,
    WAYPOINT_ON_DESPAWN_POINT_SOUTH         = 72,
    WAYPOINT_ON_DESPAWN_POINT_NORTH         = 68
};

// Eject Passengers 3-8 - 83477
class spell_silverpine_eject_passengers_3_8 : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetHitUnit()->IsVehicle())
        {
            Unit* passenger2 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_PLAYER);
            Unit* passenger3 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_0);
            Unit* passenger4 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_1);
            Unit* passenger5 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_2);
            Unit* passenger6 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_3);
            Unit* passenger7 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_4);
            Unit* passenger8 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_HAULER_TROOPER_5);

            if (GetHitUnit()->IsAIEnabled())
            {
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger2);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger3);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger4);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger5);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger6);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger7);
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger8);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_eject_passengers_3_8::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Horde Hauler - 44731
struct npc_silverpine_horde_hauler : public ScriptedAI
{
    npc_silverpine_horde_hauler(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
    {
        if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f))
        {
            if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN_HAULER, 15.0f))
            {
                if (engineer->IsAIEnabled())
                {
                    engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT_HAULER, true);
                    engineer->CastSpell(ettin, SPELL_CHAIN_LEFT_HAULER, true);
                    engineer->CastSpell(engineer, SPELL_ANIMKIT_HORDE_ENGINEER, true);
                }

                _events.ScheduleEvent(EVENT_TO_FORSAKEN_HIGH_COMMAND, 8s);
            }
        }
    }

    void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
    {
        if (!apply)
            return;

        _playerGUID = passenger->GetGUID();

        if (seatId == SEAT_HAULER_PLAYER)
        {
            if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f, true))
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                {
                    if (engineer->IsAIEnabled())
                        engineer->AI()->Talk(TALK_HAULER_BOARDED, player);
                }
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_FROM_NORTH_TO_SOUTH)
        {
            if (waypointId == WAYPOINT_ON_FORSAKEN_HIGH)
            {
                _events.ScheduleEvent(EVENT_TO_THE_SEPULCHER, 15s);
                _events.ScheduleEvent(EVENT_YELL_ON_FORSAKEN_HIGH, 1s);
            }

            if (waypointId == WAYPOINT_ON_SEPULCHER)
            {
                _events.ScheduleEvent(EVENT_TO_THE_FORSAKEN_FRONT, 15s);
                _events.ScheduleEvent(EVENT_YELL_ON_SEPULCHER, 1s);
            }

            if (waypointId == WAYPOINT_ON_FORSAKEN_FRONT)
            {
                _events.ScheduleEvent(EVENT_TO_DESPAWN, 15s);
                _events.ScheduleEvent(EVENT_YELL_ON_FORSAKEN_FRONT, 1s);
            }

            if (waypointId == WAYPOINT_ON_DESPAWN_POINT_SOUTH)
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
                    me->GetMotionMaster()->MovePath(PATH_FROM_NORTH_TO_SOUTH, false);

                    break;
                }

                case EVENT_YELL_ON_FORSAKEN_HIGH:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f, true))
                    {
                        if (engineer->IsAIEnabled())
                            engineer->AI()->Talk(TALK_ON_FORSAKEN_HIGH);
                    }

                    break;
                }

                case EVENT_TO_THE_SEPULCHER:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN_HAULER, 15.0f))
                        {
                            if (engineer->IsAIEnabled())
                            {
                                engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT_HAULER, true);
                                engineer->CastSpell(ettin, SPELL_CHAIN_LEFT_HAULER, true);
                                engineer->CastSpell(engineer, SPELL_ANIMKIT_HORDE_ENGINEER, true);
                            }
                        }
                    }

                    break;
                }

                case EVENT_YELL_ON_SEPULCHER:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f, true))
                    {
                        if (engineer->IsAIEnabled())
                            engineer->AI()->Talk(TALK_ON_SEPULCHER);
                    }

                    break;
                }

                case EVENT_TO_THE_FORSAKEN_FRONT:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN_HAULER, 15.0f))
                        {
                            if (engineer->IsAIEnabled())
                            {
                                engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT_HAULER, true);
                                engineer->CastSpell(ettin, SPELL_CHAIN_LEFT_HAULER, true);
                                engineer->CastSpell(engineer, SPELL_ANIMKIT_HORDE_ENGINEER, true);
                            }
                        }
                    }

                    break;
                }

                case EVENT_YELL_ON_FORSAKEN_FRONT:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f, true))
                    {
                        if (engineer->IsAIEnabled())
                            engineer->AI()->Talk(TALK_ON_FORSAKEN_FRONT);
                    }

                    DoCastSelf(SPELL_EJECT_PASSENGERS_3_8);

                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    break;
                }

                case EVENT_TO_DESPAWN:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_HAULER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN_HAULER, 15.0f))
                        {
                            if (engineer->IsAIEnabled())
                            {
                                engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT_HAULER, true);
                                engineer->CastSpell(ettin, SPELL_CHAIN_LEFT_HAULER, true);
                                engineer->CastSpell(engineer, SPELL_ANIMKIT_HORDE_ENGINEER, true);
                            }
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

// Horde Coffin Hauler - 44764
struct npc_silverpine_horde_coffin_hauler : public ScriptedAI
{
    npc_silverpine_horde_coffin_hauler(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
    {
        if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER_COFFIN, 15.0f))
        {
            if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN_COFFIN, 15.0f))
            {
                if (engineer->IsAIEnabled())
                {
                    engineer->CastSpell(ettin, SPELL_CHAIN_RIGHT_COFFIN, true);
                    engineer->CastSpell(ettin, SPELL_CHAIN_LEFT_COFFIN, true);
                    engineer->CastSpell(engineer, SPELL_ANIMKIT_HORDE_ENGINEER, true);
                }

                _events.ScheduleEvent(EVENT_TO_DESPAWN, 8s);
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_FROM_SOUTH_TO_NORTH)
        {
            if (waypointId == WAYPOINT_ON_DESPAWN_POINT_NORTH)
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
                case EVENT_TO_DESPAWN:
                {
                    me->GetMotionMaster()->MovePath(PATH_FROM_SOUTH_TO_NORTH, false);

                    break;
                }
            }
        }
    }

private:
    EventMap _events;
};

enum QuestTheGilneasLiberationFront
{
    NPC_FORSAKEN_TROOPER1           = 44791,
    NPC_FORSAKEN_TROOPER2           = 44792,
    NPC_WORGEN_RENEGADE             = 44793,

    SPELL_HEARTSTRIKE               = 84182,
    SPELL_DARKENED                  = 73307,
    SPELL_KILL_ME_PERIODIC          = 84181,
    SPELL_FLURRY_OF_CLAWS_CHANNEL   = 80365,
    SPELL_FLURRY_OF_CLAWS           = 80367,

    EVENT_FLURRY_OF_CLAWS           = 102,
    EVENT_SPELL_DAMAGE,

    TALK_TROOPER_RESET              = 0
};

// Worgen Renegade - 44793
struct npc_silverpine_worgen_renegade : public ScriptedAI
{
    npc_silverpine_worgen_renegade(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();

        if (me->IsSummon())
            DoCastSelf(SPELL_KILL_ME_PERIODIC);

        me->SetReactState(REACT_AGGRESSIVE);
    }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (!caster)
            return;

        if (spell->Id == SPELL_HEARTSTRIKE)
            me->KillSelf();
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

        if (!me->IsSummon())
        {
            if (urand(0, 1) == 0)
                Talk(TALK_TROOPER_RESET);
        }
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

    EVENT_START_ANIM_1                  = 102,
    EVENT_AGATHA_RAISE_FORSAKEN         = 201,
    EVENT_TALK_SEQUENCE                 = 301,
    EVENT_SUMMON_PORTAL                 = 401,
    EVENT_SUMMON_GARROSH,

    ACTION_START_WALKING                = 1,
    ACTION_START_ANIM                   = 101,

    TALK_SYLVANAS_COMETH_0              = 0,
    TALK_SYLVANAS_COMETH_1              = 1,
    TALK_SYLVANAS_COMETH_2              = 2,
    TALK_SYLVANAS_COMETH_3              = 3,
    TALK_SYLVANAS_COMETH_4              = 4,
    TALK_SYLVANAS_COMETH_5              = 5,
    TALK_SYLVANAS_COMETH_6              = 6,
    TALK_SYLVANAS_COMETH_7              = 7,
    TALK_SYLVANAS_COMETH_8              = 8,
    TALK_SYLVANAS_COMETH_9              = 9,
    TALK_SYLVANAS_COMETH_10             = 10,
    TALK_GARROSH_COMETH_0               = 0,
    TALK_GARROSH_COMETH_1               = 1,
    TALK_GARROSH_COMETH_2               = 2,
    TALK_GARROSH_COMETH_3               = 3,
    TALK_GARROSH_COMETH_4               = 4,
    TALK_GARROSH_COMETH_5               = 5,
    TALK_GARROSH_COMETH_6               = 6,
    TALK_GARROSH_COMETH_7               = 7,
    TALK_GARROSH_COMETH_8               = 8,
    TALK_GARROSH_COMETH_9               = 9,
    TALK_GARROSH_COMETH_10              = 10,
    TALK_CROMUSH_COMETH_0               = 0,
    TALK_CROMUSH_COMETH_1               = 1,

    MOVE_CROMUSH_TO_SYLVANAS            = 5405701,
    MOVE_CROMUSH_TO_HOME                = 5405702,
    MOVE_GARROSH_TO_HOME                = 5405703,

    PATH_CROMUSH_01                     = 446402,
    PATH_CROMUSH_02                     = 446403,
    PATH_GARROSH                        = 446290,

    POINT_AGATHA_PRE_RISE_1             = 1,
    POINT_AGATHA_PRE_RISE_2             = 2,
    POINT_AGATHA_RISING                 = 3,
    POINT_AGATHA_RESET                  = 4
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

Position const GarroshJumpPos = { 1378.65f, 1044.23f, 53.8389f, 5.51524f };

Position const CromushPos = { 1404.71f, 1063.73f, 60.5617f, 2.827433f };

Position const AgathaPreRisePos = { 1364.02f, 1028.54f, 66.99143f };

Position const AgathaRisingPos = { 1368.65f, 1032.19f, 63.3033f };

Position const AgathaResetPos = { 1364.02f, 1028.54f, 55.9914f };

// Grand Executor Mortuus - 44615
struct npc_silverpine_grand_executor_mortuus : public ScriptedAI
{
    npc_silverpine_grand_executor_mortuus(Creature* creature) : ScriptedAI(creature), _summons(me), _eventStarted(false) {}

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
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_0);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 1, 8s);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 25, 4s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 1:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->GetMotionMaster()->MoveJump(GarroshJumpPos, 15.595897f, 15.595897f);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 2, 3s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 2:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                        {
                            if (warhorse->IsAIEnabled())
                                warhorse->SetFacingTo(3.924652f);
                        }

                        if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                        {
                            if (garrosh->IsAIEnabled())
                            {
                                garrosh->SetFacingToObject(sylvanas);
                                garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_1);
                                garrosh->AI()->Talk(TALK_GARROSH_COMETH_0);
                            }
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 3, 12s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 3:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                        {
                            garrosh->SetFacingTo(3.9444442f);
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_1);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 4, 7s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 4:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                        {
                            if (warhorse->IsAIEnabled())
                                warhorse->SetFacingTo(2.4260077f);
                        }

                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 5, 5s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 5:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                        {
                            if (warhorse->IsAIEnabled())
                                warhorse->SetFacingTo(3.7350047f);
                        }

                        if (sylvanas->IsAIEnabled())
                        {
                            sylvanas->PlayOneShotAnimKitId(ANIMKIT_SYLV_1);
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_3);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 6, 16s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 6:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_4);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 7, 4s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 7:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 8, 3s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 8:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_5);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 9, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 9:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_6);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 10, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 10:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                        {
                            sylvanas->PlayOneShotAnimKitId(ANIMKIT_SYLV_2);
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_7);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 11, 9s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 11:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_8);
                    }

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN, 3s);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        if (agatha->IsAIEnabled())
                            agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_PRE_RISE_1, AgathaPreRisePos, false, 6.0f);
                    }

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 1, 2s + 500ms);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 1:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        if (agatha->IsAIEnabled())
                            agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_PRE_RISE_2, AgathaRisingPos, false, 1.271437f);
                    }

                    _events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 2, 5s);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 2:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        if (agatha->IsAIEnabled())
                            agatha->CastSpell(agatha, SPELL_RAISE_FORSAKEN);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 12, 10s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 12:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                    {
                        if (cromush->IsAIEnabled())
                            cromush->AI()->Talk(TALK_CROMUSH_COMETH_0);
                    }

                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        if (agatha->IsAIEnabled())
                            agatha->GetMotionMaster()->MovePoint(POINT_AGATHA_RISING, AgathaResetPos, false, 0.8775906f);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 13, 3s + 500ms);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 24, 10s);
                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 26, 3s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 13:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                        {
                            garrosh->PlayOneShotAnimKitId(ANIMKIT_SYLV_3);
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_3);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 14, 13s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 14:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                        {
                            sylvanas->PlayOneShotAnimKitId(ANIMKIT_SYLV_3);
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_9);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 15, 10s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 15:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_4);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 16, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 16:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_5);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 17, 6s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 17:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_10);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 18, 5s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 18:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                        {
                            garrosh->SetFacingTo(5.51524f);
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_6);
                        }
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
                            if (garrosh->IsAIEnabled())
                            {
                                garrosh->SetFacingToObject(cromush);
                                garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_2);
                                garrosh->AI()->Talk(TALK_GARROSH_COMETH_7);
                            }
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
                            if (cromush->IsAIEnabled())
                            {
                                cromush->SetFacingToObject(garrosh);
                                cromush->AI()->Talk(TALK_CROMUSH_COMETH_1);
                            }
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 21, 2s + 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 21:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                        {
                            garrosh->SetFacingTo(5.6199603f);
                            garrosh->PlayOneShotAnimKitId(ANIMKIT_GARROSH_2);
                            garrosh->AI()->Talk(TALK_GARROSH_COMETH_8);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 22, 7s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 22:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, _cromushGUID))
                    {
                        if (cromush->IsAIEnabled())
                            cromush->GetMotionMaster()->MovePath(PATH_CROMUSH_02, false);
                    }

                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->GetMotionMaster()->MovePath(PATH_GARROSH, false);
                    }

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 23, 11s);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 23:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, _garroshGUID))
                    {
                        if (garrosh->IsAIEnabled())
                            garrosh->CastSpell(garrosh, SPELL_WELCOME_TO_SILVERPINE_CREDIT, true);
                    }

                    DespawnGarroshAndHisEliteGuards();

                    _events.ScheduleEvent(EVENT_TALK_SEQUENCE + 28, 500ms);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 24:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, _agathaGUID))
                    {
                        if (agatha->IsAIEnabled())
                            agatha->SetFacingTo(0.855211f);
                    }

                    break;
                }

                case EVENT_TALK_SEQUENCE + 25:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, _sylvanasGUID))
                    {
                        if (sylvanas->IsAIEnabled())
                            sylvanas->AI()->Talk(TALK_SYLVANAS_COMETH_1);

                        if (Creature* warhorse = ObjectAccessor::GetCreature(*me, _warhorseGUID))
                        {
                            if (warhorse->IsAIEnabled())
                                warhorse->SetFacingTo(0.808979f);
                        }
                    }

                    break;
                }

                case EVENT_TALK_SEQUENCE + 26:
                {
                    Talk(TALK_SYLVANAS_COMETH_0);

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
                    {
                        if ((*itr)->IsAIEnabled())
                            (*itr)->AI()->DoAction(ACTION_START_WALKING);
                    }

                    for (std::list<Creature*>::const_iterator itr = femaleforsaken.begin(); itr != femaleforsaken.end(); ++itr)
                    {
                        if ((*itr)->IsAIEnabled())
                            (*itr)->AI()->DoAction(ACTION_START_WALKING);
                    }

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
        {
            if ((*itr)->IsAIEnabled())
                (*itr)->CastSpell(*itr, SPELL_AIR_REVENANT_ENTRANCE);
        }
    }

    void SummonGarroshAndHisEliteGuards()
    {
        for (auto pos : ElitePos)
            me->SummonCreature(NPC_HELLSCREEMS_ELITE, pos, TEMPSUMMON_TIMED_DESPAWN, 300s);

        me->SummonCreature(NPC_GARROSH_HELLSCREAM, GarroshPos, TEMPSUMMON_TIMED_DESPAWN, 300s);

        if (Creature* cromush = me->SummonCreature(NPC_HIGH_WARLORD_CROMUSH, CromushPos, TEMPSUMMON_TIMED_DESPAWN, 300s))
        {
            _cromushGUID = cromush->GetGUID();

            if (cromush->IsAIEnabled())
                cromush->GetMotionMaster()->MovePath(PATH_CROMUSH_01, false);
        }
    }

    void DespawnGarroshAndHisEliteGuards()
    {
        for (std::list<ObjectGuid>::const_iterator itr = _summons.begin(); itr != _summons.end(); itr++)
        {
            if (Creature* npc = ObjectAccessor::GetCreature(*me, (*itr)))
            {
                if (npc->IsAIEnabled())
                    npc->CastSpell(npc, SPELL_SIMPLE_TELEPORT);
            }
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

    POINT_BEING_RISEN               = 1,
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
                if (fallenHuman->IsAIEnabled())
                {
                    fallenHuman->SetAIAnimKitId(ANIMKIT_FALLEN_HUMAN);

                    fallenHuman->SetDisableGravity(true);
                    fallenHuman->SetCanFly(true);

                    fallenHuman->GetMotionMaster()->MovePoint(POINT_BEING_RISEN, fallenHuman->GetPositionX(), fallenHuman->GetPositionY(), fallenHuman->GetPositionZ() + frand(3.5f, 5.5f), false, 1.5f);
                }
            }
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* unit = GetTarget())
        {
            if (Creature* fallenHuman = unit->ToCreature())
            {
                if (fallenHuman->IsAIEnabled())
                {
                    fallenHuman->SetDisableGravity(false);
                    fallenHuman->SetCanFly(false);

                    // The sniff doesn't really tell whether they fall or just move back to home position
                    fallenHuman->GetMotionMaster()->MoveFall();

                    fallenHuman->SetAIAnimKitId(ANIMKIT_RESET);

                    fallenHuman->AddAura(SPELL_INVISIBLE, fallenHuman);
                }
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
                if (fallenHuman->IsAIEnabled())
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

    PATH_FALLEN_HUMAN_1                                 = 445920,
    PATH_FALLEN_HUMAN_2                                 = 445921
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

        DoCastSelf(SPELL_FEIGNED);

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
                    me->GetMotionMaster()->MovePath(PATH_FALLEN_HUMAN_1, false);
                else
                    me->GetMotionMaster()->MovePath(PATH_FALLEN_HUMAN_2, false);

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

                    DoCastSelf(SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_HIGH_COMMAND);

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
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
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

    TALK_MAGGOTBREATH                   = 0,
    TALK_BAT_ARRIVED_TO_ISLE            = 0,
    TALK_BAT_GOING_HOME                 = 1,

    PATH_BAT_ARRIVE                     = 448210,
    PATH_BAT_CIRCLE                     = 448211,
    PATH_BAT_HOME                       = 448212,

    WAYPOINT_ARRIVED_ON_ISLE            = 7,
    WAYPOINT_LAST_CIRCLE                = 31,
    WAYPOINT_ARRIVED_HOME               = 6,

    DATA_ITERATING_UPON_SUCCESS_REQ     = 50
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

                    Talk(TALK_MAGGOTBREATH);

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

                    me->GetMotionMaster()->MovePath(PATH_BAT_ARRIVE, false);

                    _events.ScheduleEvent(EVENT_CHECK_FINISH, 500ms);
                }
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        switch (pathId)
        {
            case PATH_BAT_ARRIVE:
            {
                if (waypointId == WAYPOINT_ARRIVED_ON_ISLE)
                {
                    _goingIsland = false;

                    me->GetMotionMaster()->MovePath(PATH_BAT_CIRCLE, false);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_BAT_ARRIVED_TO_ISLE, player);
                }

                break;
            }

            case PATH_BAT_CIRCLE:
            {
                if (waypointId == WAYPOINT_LAST_CIRCLE)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                            me->GetMotionMaster()->MovePath(PATH_BAT_CIRCLE, false);
                    }
                }

                break;
            }

            case PATH_BAT_HOME:
            {
                if (waypointId == WAYPOINT_ARRIVED_HOME)
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

                        me->GetMotionMaster()->MovePath(PATH_BAT_HOME, false);

                        Talk(TALK_BAT_GOING_HOME, player);
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
                        if (player->GetReqKillOrCastCurrentCount(QUEST_ITERATING_UPON_SUCCESS, NPC_VILE_FIN_ORACLE) == DATA_ITERATING_UPON_SUCCESS_REQ)
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
                {
                    if (target->IsAIEnabled())
                        target->GetAI()->DoAction(ACTION_GO_HOME);
                }
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
        Initialize();
    }

    void Initialize()
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

    ACTION_RANE_JUMP_DEATH                      = 2,
    ACTION_RANE_SKIP_PATH,
    ACTION_MOVE_TO_RANA                         = 500,

    TALK_YORICK_EXSANGUINATE_SUMMON             = 0,
    TALK_YORICK_EXSANGUINATE_HIDE               = 1,
    TALK_YORICK_EXSANGUINATE_DEATH              = 2,
    TALK_DARIUS_EXSANGUINATE_0                  = 0,
    TALK_DARIUS_EXSANGUINATE_1                  = 1,
    TALK_DARIUS_EXSANGUINATE_2                  = 2,
    TALK_DARIUS_EXSANGUINATE_3                  = 3,
    TALK_DARIUS_EXSANGUINATE_4                  = 4,
    TALK_DARIUS_EXSANGUINATE_5                  = 5,
    TALK_DARIUS_EXSANGUINATE_6                  = 6,
    TALK_IVAR_EXSANGUINATE_0                    = 0,
    TALK_IVAR_EXSANGUINATE_1                    = 1,
    TALK_IVAR_EXSANGUINATE_2                    = 2,
    TALK_IVAR_EXSANGUINATE_3                    = 3,
    TALK_IVAR_EXSANGUINATE_4                    = 4,
    TALK_IVAR_EXSANGUINATE_5                    = 5,
    TALK_IVAR_EXSANGUINATE_6                    = 6,
    TALK_IVAR_EXSANGUINATE_7                    = 7,
    TALK_IVAR_EXSANGUINATE_8                    = 8,
    TALK_IVAR_EXSANGUINATE_9                    = 9,
    TALK_IVAR_EXSANGUINATE_10                   = 10,

    PATH_YORICK_UP                              = 448820,
    PATH_YORICK_HIDE                            = 448821,
    PATH_DARIUS_ENTER                           = 448830,
    PATH_IVAR_ENTER                             = 448840,
    PATH_IVAR_RANE_01                           = 448841,
    PATH_IVAR_RANE_02                           = 448842,
    PATH_IVAR_RANE_03                           = 448843,
    PATH_IVAR_EXIT                              = 448844,
    PATH_DARIUS_EXIT                            = 448831,

    WAYPOINT_CLOSE_TO_ARMOIRE                   = 15,
    WAYPOINT_NEXT_TO_ARMOIRE                    = 2,
    WAYPOINT_ON_DARIUS_DESPAWN                  = 2,
    WAYPOINT_ON_IVAR_DESPAWN                    = 3,

    POINT_HIDDEN_PLACE                          = 1234,
    POINT_RANA                                  = 1230,
    POINT_ARMOIRE                               = 15
};

// Deathstalker - 44789, 44790
struct npc_silverpine_deathstalker : public ScriptedAI
{
    npc_silverpine_deathstalker(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
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
    npc_silverpine_deathstalker_rane_yorick(Creature* creature) : ScriptedAI(creature), _playerArrived(false) { }

    void JustAppeared() override
    {
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
        if (pathId == PATH_YORICK_UP && waypointId == WAYPOINT_CLOSE_TO_ARMOIRE)
            _events.ScheduleEvent(EVENT_WAIT_ON_PLAYER, 1s);

        if (pathId == PATH_YORICK_HIDE && waypointId == WAYPOINT_NEXT_TO_ARMOIRE)
        {
            me->SetFacingTo(4.6425757f);

            DoCastSelf(SPELL_STEALTH);
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

                DoCastSelf(SPELL_STEALTH);
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
                    Talk(TALK_YORICK_EXSANGUINATE_SUMMON);

                    _events.ScheduleEvent(EVENT_START_ANIM + 1, 1s + 500ms);
                    break;
                }

                case EVENT_START_ANIM + 1:
                {
                    me->GetMotionMaster()->MovePath(PATH_YORICK_UP, false);
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
                    Talk(TALK_YORICK_EXSANGUINATE_HIDE);

                    _events.ScheduleEvent(EVENT_HIDE, 3s);
                    _events.ScheduleEvent(EVENT_SET_FACE_TO_BLOODFANG, 3s);
                    break;
                }

                case EVENT_HIDE:
                {
                    me->GetMotionMaster()->MovePath(PATH_YORICK_HIDE, false);
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

                            if (armoire->IsAIEnabled())
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

                    DoCastSelf(SPELL_PERMANENT_FEIGN_DEATH);

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
    npc_silverpine_armoire(Creature* creature) : VehicleAI(creature) { }

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
                        {
                            if (rane->IsAIEnabled())
                                rane->GetAI()->DoAction(ACTION_RANE_SKIP_PATH);
                        }
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
                    _events.ScheduleEvent(EVENT_START_ANIMATION, 2s);
            }
        }
        else
        {
            if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
            {
                if (darius->IsAIEnabled())
                    darius->DespawnOrUnsummon();
            }

            if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
            {
                if (ivar->IsAIEnabled())
                    ivar->DespawnOrUnsummon();
            }

            if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
            {
                if (rane->IsAIEnabled())
                    rane->DespawnOrUnsummon();
            }

            me->DespawnOrUnsummon(1s);
        }
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        switch (id)
        {
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
                    {
                        if (darius->IsAIEnabled())
                            darius->GetMotionMaster()->MovePath(PATH_DARIUS_ENTER, false);
                    }

                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->GetMotionMaster()->MovePath(PATH_IVAR_ENTER, false);
                    }

                    _events.ScheduleEvent(EVENT_ACTION, 5s);
                    break;
                }

                case EVENT_ACTION:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        {
                            if (darius->IsAIEnabled())
                                darius->SetFacingToObject(ivar);

                            if (ivar->IsAIEnabled())
                                ivar->SetFacingToObject(darius);
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER, 2s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (darius->IsAIEnabled())
                            darius->AI()->Talk(TALK_DARIUS_EXSANGUINATE_0);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_B, 6s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 1, 6s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 1:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_0);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_A, 6s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 2, 6s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 2:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (darius->IsAIEnabled())
                            darius->AI()->Talk(TALK_DARIUS_EXSANGUINATE_1);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 3, 12s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 3:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (darius->IsAIEnabled())
                            darius->AI()->Talk(TALK_DARIUS_EXSANGUINATE_2);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_B, 7s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 4, 7s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 4:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_1);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 5, 7s + 300ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 5:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 6, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 6:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_3);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_A, 9s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 7, 9s + 800ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 7:
                {
                    if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                    {
                        if (darius->IsAIEnabled())
                            darius->AI()->Talk(TALK_DARIUS_EXSANGUINATE_3);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_B, 3s);
                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 8, 3s + 700ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 8:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_4);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 1, 2s + 500ms);
                    break;
                }

                case EVENT_ACTION + 1:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->GetMotionMaster()->MovePath(PATH_IVAR_RANE_01, false);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 9, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 9:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_5);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 10, 5s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 10:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_6);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 2, 6s);
                    break;
                }

                case EVENT_ACTION + 2:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->GetMotionMaster()->MovePath(PATH_IVAR_RANE_02, false);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 11, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 11:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_7);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 3, 4s);
                    break;
                }

                case EVENT_ACTION + 3:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                        {
                            if (rane->IsAIEnabled())
                                rane->RemoveAura(SPELL_STEALTH);

                            if (ivar->IsAIEnabled())
                                ivar->CastSpell(rane, SPELL_REVERSE_RIDE_VEHICLE, true);
                        }
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 4, 1s);
                    break;
                }

                case EVENT_ACTION + 4:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->GetMotionMaster()->MovePath(PATH_IVAR_RANE_03, false);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 5, 3s);
                    break;
                }

                case EVENT_ACTION + 5:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->SetFacingTo(3.054326f);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 12, 500ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 12:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_8);
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 13, 3s);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 13:
                {
                    if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                    {
                        if (rane->IsAIEnabled())
                            rane->AI()->Talk(TALK_YORICK_EXSANGUINATE_DEATH);
                    }

                    _events.ScheduleEvent(EVENT_ACTION + 6, 3s);
                    break;
                }

                case EVENT_ACTION + 6:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (Creature* rane = ObjectAccessor::GetCreature(*me, _raneGUID))
                        {
                            if (ivar->IsAIEnabled())
                                ivar->CastSpell(rane, SPELL_EJECT_PASSENGER_01, false);

                            if (rane->IsAIEnabled())
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
                            if (ivar->IsAIEnabled())
                            {
                                ivar->SetFacingToObject(darius);
                                ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_9);
                            }
                        }
                    }

                    _events.ScheduleEvent(EVENT_TALK_TO_PLAYER + 15, 5s + 500ms);
                    break;
                }

                case EVENT_TALK_TO_PLAYER + 15:
                {
                    if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                    {
                        if (ivar->IsAIEnabled())
                            ivar->AI()->Talk(TALK_IVAR_EXSANGUINATE_10);
                    }

                    _events.ScheduleEvent(EVENT_CAMERA_A, 5s + 300ms);
                    _events.ScheduleEvent(EVENT_ACTION + 7, 6s);
                    break;
                }

                case EVENT_ACTION + 7:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                        {
                            if (ivar->IsAIEnabled())
                                ivar->GetMotionMaster()->MovePath(PATH_IVAR_EXIT, false);
                        }
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
                        {
                            if (darius->IsAIEnabled())
                                darius->GetMotionMaster()->MovePath(PATH_DARIUS_EXIT, false);
                        }
                    }

                    _events.ScheduleEvent(EVENT_FINISH, 4s);
                    break;
                }

                case EVENT_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                        {
                            if (darius->IsAIEnabled())
                                darius->CastSpell(player, SPELL_KILL_CREDIT_YORICK, false);
                        }

                        player->GetMotionMaster()->Clear();

                        player->RemoveAura(SPELL_HIDE_IN_ARMOIRE);

                        player->ExitVehicle();
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
                    {
                        if (Creature* darius = ObjectAccessor::GetCreature(*me, _crowleyGUID))
                            player->CastSpell(darius, SPELL_ARMOIRE_CAMERA_A, true);
                    }
                    break;
                }

                case EVENT_CAMERA_B:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* ivar = ObjectAccessor::GetCreature(*me, _bloodfangGUID))
                            player->CastSpell(ivar, SPELL_ARMOIRE_CAMERA_B, true);
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
    ObjectGuid _raneGUID;
    ObjectGuid _crowleyGUID;
    ObjectGuid _bloodfangGUID;
};

// Lord Darius Crowley - 44883
struct npc_silverpine_lord_darius_crowley_exhanguinate : public ScriptedAI
{
    npc_silverpine_lord_darius_crowley_exhanguinate(Creature* creature) : ScriptedAI(creature) { }

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
        if (pathId == PATH_DARIUS_EXIT && waypointId == WAYPOINT_ON_DARIUS_DESPAWN)
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

                if (armoire->IsAIEnabled())
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
    npc_silverpine_packleader_ivar_bloodfang_exhanguinate(Creature* creature) : ScriptedAI(creature) { }

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

            if (armoire->IsAIEnabled())
                armoire->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_IVAR_EXIT && waypointId == WAYPOINT_ON_IVAR_DESPAWN)
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
            Unit* passenger0 = GetHitUnit()->ToUnit()->GetVehicleKit()->GetPassenger(SEAT_BLOODFANG);

            if (GetHitUnit()->IsAIEnabled())
                GetHitUnit()->ToUnit()->GetVehicleKit()->RemovePassenger(passenger0);
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
    SPELL_SKITTERWEB                            = 83827,
    SPELL_SUMMNON_SPIDERLINGS                   = 87084,
    SPELL_VENOM_SPLASH                          = 79607,

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

    ACTION_DELIVER_CRATES                       = 2,
    ACTION_EXPLODE                              = 3,
    ACTION_GRAB_CHICKEN                         = 4,

    TALK_ROCKA_0                                = 0,
    TALK_ROCKA_1                                = 1,
    TALK_ROCKA_2                                = 2,
    TALK_ROCKA_3                                = 3,
    TALK_ROCKA_4                                = 4,
    TALK_ROCKA_5                                = 5,
    TALK_ROCKA_6                                = 6,
    TALK_ROCKA_7                                = 7,
    TALK_ROCKA_8                                = 8,
    TALK_ROCKA_9                                = 9,
    TALK_GORGAR_0                               = 0,
    TALK_GORGAR_1                               = 1,
    TALK_GORGAR_2                               = 2,
    TALK_GORGAR_3                               = 3,
    TALK_GORGAR_4                               = 4,
    TALK_GORGAR_5                               = 5,
    TALK_WORMCRUD_0                             = 0,
    TALK_ORCSEA_0                               = 0,
    TALK_ORCSEA_1                               = 1,
    TALK_ORCSEA_2                               = 2,
    TALK_HATCHET_0                              = 0,
    TALK_HATCHET_1                              = 1,
    TALK_TOROK_0                                = 0,
    TALK_TOROK_1                                = 1,
    TALK_TOROK_2                                = 2,
    TALK_ORC_PUP_SUMMONED                       = 0,
    TALK_ORC_PUP_DELIVER_CRATES                 = 6,
    TALK_ORC_PUP_WORN_OFF                       = 7,
    TALK_ORC_FREED                              = 0,

    AREA_FORSAKEN_REAR_GUARD                    = 5386
};

// "Salty" Rocka - 45498
struct npc_silverpine_salty_rocka : public ScriptedAI
{
    npc_silverpine_salty_rocka(Creature* creature) : ScriptedAI(creature), _playerNear(false) { }

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
                    Talk(TALK_ROCKA_0);

                    _events.ScheduleEvent(EVENT_TALK + 1, 20s);
                    break;
                }

                case EVENT_TALK + 1:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                    {
                        if (gorgar->IsAIEnabled())
                            gorgar->AI()->Talk(TALK_GORGAR_0);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 2, 20s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    Talk(TALK_ROCKA_1);
                    break;
                }

                case EVENT_TALK + 10:
                {
                    Talk(TALK_ROCKA_2);

                    _events.ScheduleEvent(EVENT_TALK + 11, 20s);
                    break;
                }

                case EVENT_TALK + 11:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                    {
                        if (gorgar->IsAIEnabled())
                            gorgar->AI()->Talk(TALK_GORGAR_1);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 12, 20s);
                    break;
                }

                case EVENT_TALK + 12:
                {
                    Talk(TALK_ROCKA_3);
                    break;
                }

                case EVENT_TALK + 20:
                {
                    Talk(TALK_ROCKA_4);

                    _events.ScheduleEvent(EVENT_TALK + 21, 20s);
                    break;
                }

                case EVENT_TALK + 21:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                    {
                        if (gorgar->IsAIEnabled())
                            gorgar->AI()->Talk(TALK_GORGAR_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 22, 20s);
                    break;
                }

                case EVENT_TALK + 22:
                {
                    Talk(TALK_ROCKA_5);
                    break;
                }

                case EVENT_TALK + 30:
                {
                    Talk(TALK_ROCKA_6);

                    _events.ScheduleEvent(EVENT_TALK + 31, 20s);
                    break;
                }

                case EVENT_TALK + 31:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                    {
                        if (gorgar->IsAIEnabled())
                            gorgar->AI()->Talk(TALK_GORGAR_3);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 32, 20s);
                    break;
                }

                case EVENT_TALK + 32:
                {
                    Talk(TALK_ROCKA_7);
                    break;
                }

                case EVENT_TALK + 40:
                {
                    Talk(TALK_ROCKA_8);

                    _events.ScheduleEvent(EVENT_TALK + 1, 20s);
                    break;
                }

                case EVENT_TALK + 41:
                {
                    if (Creature* gorgar = ObjectAccessor::GetCreature(*me, _gorgarGUID))
                    {
                        if (gorgar->IsAIEnabled())
                            gorgar->AI()->Talk(TALK_GORGAR_4);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 2, 20s);
                    break;
                }

                case EVENT_TALK + 42:
                {
                    Talk(TALK_ROCKA_9);
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
    npc_silverpine_apothecary_wormcrud(Creature* creature) : ScriptedAI(creature), _drunkenOrcSeaDog(), _orcSeaDogList(), _playerNear(false), _done(false) { }

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
                    {
                        if (orcsea1->IsAIEnabled())
                            orcsea1->AI()->Talk(TALK_ORCSEA_0);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 1, 12s);
                    break;
                }

                case EVENT_TALK + 1:
                {
                    if (Creature* orcsea2 = ObjectAccessor::GetCreature(*me, _drunkenOrcSeaDog[1]))
                    {
                        if (orcsea2->IsAIEnabled())
                            orcsea2->AI()->Talk(TALK_ORCSEA_1);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 2, 12s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    if (Creature* orcsea3 = ObjectAccessor::GetCreature(*me, _drunkenOrcSeaDog[2]))
                    {
                        if (orcsea3->IsAIEnabled())
                            orcsea3->AI()->Talk(TALK_ORCSEA_2);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 3, 12s);
                    break;
                }

                case EVENT_TALK + 3:
                {
                    Talk(TALK_WORMCRUD_0);
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
    npc_silverpine_admiral_hatchet(Creature* creature) : ScriptedAI(creature), _playerNear(false) { }

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
            {
                if (pup->IsAIEnabled())
                    pup->GetAI()->DoAction(ACTION_DELIVER_CRATES);
            }
        }

        if (quest->GetQuestId() == QUEST_LOST_IN_THE_DARKNESS)
        {
            me->GetCreatureListWithEntryInGrid(_seaDogList, NPC_ORC_SEA_DOG, 25.0f);

            for (Creature* seadog : _seaDogList)
            {
                if (seadog->IsAIEnabled())
                {
                    if (seadog->GetOwner()->ToPlayer()->IsQuestRewarded(QUEST_LOST_IN_THE_DARKNESS))
                        seadog->DespawnOrUnsummon();
                }
            }
        }
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
                    Talk(TALK_HATCHET_0);

                    _events.ScheduleEvent(EVENT_TALK + 2, 8s);
                    break;
                }

                case EVENT_TALK + 2:
                {
                    if (Creature* torok = ObjectAccessor::GetCreature(*me, _torokGUID))
                    {
                        if (torok->IsAIEnabled())
                            torok->AI()->Talk(TALK_TOROK_0);
                    }

                    _events.ScheduleEvent(EVENT_TALK + 3, 6s);
                    break;
                }

                case EVENT_TALK + 3:
                {
                    Talk(TALK_HATCHET_1);

                    _events.ScheduleEvent(EVENT_TALK + 4, 6s);
                    break;
                }

                case EVENT_TALK + 4:
                {
                    if (Creature* torok = ObjectAccessor::GetCreature(*me, _torokGUID))
                    {
                        if (torok->IsAIEnabled())
                        {
                            torok->PlayOneShotAnimKitId(ANIMKIT_TOROK);
                            torok->AI()->Talk(TALK_TOROK_1);
                        }
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
    npc_silverpine_orc_sea_pup(Creature* creature) : ScriptedAI(creature), _isJustSummoned(true) { }

    void JustAppeared() override
    {
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
                    DoCastSelf(SPELL_SUMMON_ORC_CRATE);
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
                {
                    if (crate->IsAIEnabled())
                        crate->DespawnOrUnsummon(6s);
                }
            }
        }
    }

    void DoAction(int32 param) override
    {
        if (param == ACTION_DELIVER_CRATES)
        {
            _events.CancelEvent(EVENT_TALK_TO_PLAYER);

            Talk(TALK_ORC_PUP_DELIVER_CRATES);

            me->GetVehicleKit()->RemoveAllPassengers();

            me->HandleEmoteCommand(EMOTE_STATE_FALL);

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
                    Talk(TALK_ORC_PUP_SUMMONED);
                    break;
                }

                case EVENT_TALK_TO_PLAYER:
                {
                    if (me->GetVehicleKit()->IsVehicleInUse())
                        Talk(TALK_ORC_PUP_WORN_OFF);

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
    npc_silverpine_orc_crate(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* who) override
    {
        if (who->GetEntry() == NPC_ORC_SEA_PUP)
        {
            _orcGUID = who->GetGUID();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            if (Creature* orc = ObjectAccessor::GetCreature(*me, _orcGUID))
            {
                if (orc->IsAIEnabled())
                    me->CastSpell(orc, VEHICLE_SPELL_RIDE_HARDCODED, true);
            }
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
    bool Validate(SpellInfo const* /*spellInfi*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_SUMMON_SEA_PUP
            });
    }

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
    npc_silverpine_mutant_bush_chicken(Creature* creature) : ScriptedAI(creature) { }

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
                        if (ettin->IsAIEnabled())
                        {
                            ettin->SetFacingToObject(me);

                            _events.ScheduleEvent(EVENT_CHECK_ETTIN + 2, 2s);
                        }
                    }

                    break;
                }

                case EVENT_CHECK_ETTIN + 2:
                {
                    if (Creature* ettin = me->FindNearestCreature(NPC_FOREST_ETTIN, 3.0f))
                    {
                        if (ettin->IsAIEnabled())
                            ettin->GetAI()->DoAction(ACTION_GRAB_CHICKEN);
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
                {
                    if (chicken->IsAIEnabled())
                        chicken->CastSpell(me, VEHICLE_SPELL_RIDE_HARDCODED, true);
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
                case EVENT_EXPLODE:
                {
                    if (Unit* chicken = me->GetVehicleKit()->GetPassenger(0))
                    {
                        if (chicken->IsAIEnabled())
                        {
                            chicken->CastSpell(me, SPELL_BUSH_EXPLOSION, true);

                            AttackStart(chicken->GetOwner());
                        }
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
    npc_silverpine_webbed_victim_skitterweb(Creature* creature) : ScriptedAI(creature) { }

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
                            {
                                if (worgen->IsAIEnabled())
                                    worgen->Attack(player, true);
                            }
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
                            {
                                if (bear->IsAIEnabled())
                                    bear->Attack(player, true);
                            }
                        }
                        else
                        {
                            player->CastSpell(me, SPELL_FREE_WEBBED_VICTIM2, true);

                            if (Creature* wolf = me->SummonCreature(NPC_RABID_DOG, me->GetPosition()))
                            {
                                if (wolf->IsAIEnabled())
                                    wolf->Attack(player, true);
                            }
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
    npc_silverpine_orc_sea_dog(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
    {
        me->SetPowerType(POWER_ENERGY);
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 100, true);
    }

    void Reset() override
    {
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
                    Talk(TALK_ORC_FREED);
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

        me->SetAIAnimKitId(ANIMKIT_SPIDER_1);

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

            me->SetAIAnimKitId(ANIMKIT_RESET);
            me->PlayOneShotAnimKitId(ANIMKIT_SPIDER_2);

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
                    {
                        if (stalker->IsAIEnabled())
                            stalker->RemoveAura(SPELL_SKITTERWEB);
                    }

                    me->SetAIAnimKitId(ANIMKIT_INTERACT);

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
                    DoCastSelf(SPELL_SUMMNON_SPIDERLINGS);
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
    NPC_WORGEN_SENTRY                          = 44987,
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
    SPELL_BATTLE_ROAR                          = 6507,
    SPELL_UNDYING_FRENZY                       = 80515,

    SPELL_SUMMON_FENRIS_ACTORS                 = 84053,
    SPELL_GENERAL_TRIGGER_84107                = 84107,
    SPELL_GENERAL_TRIGGER_84114                = 84114,
    SPELL_ARMORE_CAMERA_1                      = 84112,
    SPELL_ARMORE_CAMERA_FENRIS                 = 83768,
    SPELL_ARMORE_CAMERA_2                      = 84104,
    SPELL_ARMORE_CAMERA_3                      = 84103,
    SPELL_ARMORE_CAMERA_4                      = 84111,
    SPELL_RIDE_REVERSE_CAST_RIDE_VEHICLE       = 84109,
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
    SPELL_GENERAL_TRIGGER_84102                = 84102,
    SPELL_DESPAWN_ALL_SUMMONS_FENRIS           = 84066,
    SPELL_GENERAL_TRIGGER_84079                = 84079,

    SPELL_SUMMON_SYLVANAS_AND_HORSE            = 84128,
    SPELL_SUMMON_FORSAKEN_WARHORSE             = 84164,

    EVENT_CHECK_QUEST                          = 101,
    EVENT_MASTER_RESET,
    EVENT_DRINKING,
    EVENT_MORPHING,
    EVENT_MOVE_TO_STARTPOINT,
    EVENT_CHANGE_TO_SEAT_2,
    EVENT_TRIGGER_84102,
    EVENT_ANIMATION                            = 200,
    EVENT_CHAT_TO_PLAYER                       = 300,
    EVENT_CAST_AGGRO,
    EVENT_HEAL_COOLDOWN,
    EVENT_RUN,
    EVENT_LORDAERON_MIGHT,
    EVENT_TALK_REVIVE,
    EVENT_UNDYING_FRENZY,
    EVENT_HOWLING                              = 400,

    ACTION_ACTIVATE_SCENE,

    TALK_AGATHA_BROADCAST                      = 0,
    TALK_AGATHA_RISE_FORSAKEN                  = 1,
    TALK_AGATHA_PRE_EVENT                      = 2,
    TALK_AGATHA_POST_EVENT1                    = 3,
    TALK_AGATHA_POST_EVENT2                    = 4,
    TALK_SENTRY_WARNING                        = 0,
    TALK_CROWLEY_NO_ESCAPE_0                   = 0,
    TALK_CROWLEY_NO_ESCAPE_1                   = 1,
    TALK_CROWLEY_NO_ESCAPE_2                   = 2,
    TALK_CROWLEY_NO_ESCAPE_3                   = 3,
    TALK_CROWLEY_NO_ESCAPE_4                   = 4,
    TALK_CROWLEY_NO_ESCAPE_5                   = 5,
    TALK_CROWLEY_NO_ESCAPE_6                   = 6,
    TALK_HENRY_NO_ESCAPE_0                     = 0,
    TALK_HENRY_NO_ESCAPE_1                     = 1,

    PATH_AGATHA_TO_FORSAKEN                    = 449510,

    WAYPOINT_ARRIVED_TO_FORSAKEN               = 41,

    POINT_CAMERA_FRONTYARD                     = 665,
    POINT_AGATHA_BACK_FRONTYARD                = 1234,

    SOUND_HOWLING                              = 17671
};

Position const AgathaBackFrontyardPos = { 982.57f, 671.04f, 77.298f };

// Agatha - 44951
struct npc_silverpine_agatha_fenris : public VehicleAI
{
    npc_silverpine_agatha_fenris(Creature* creature) : VehicleAI(creature), _doomHoulDone(false), _healCD(false), _sceneStarted(false) { }

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
        _targetGUID = ObjectGuid::Empty;
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84114:
            {
                if (!_sceneStarted)
                    SetForQuest27099();
                break;
            }

            case SPELL_GENERAL_TRIGGER_84107:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    me->CastSpell(player, SPELL_ARMORE_CAMERA_1, true);
                break;
            }

            case SPELL_ARMORE_CAMERA_4:
            {
                _events.ScheduleEvent(EVENT_RUN + 1, 1s);
                break;
            }

            case SPELL_GENERAL_TRIGGER_84079:
            {
                _events.ScheduleEvent(EVENT_RUN + 2, 1s);
                break;
            }

            default:
                break;
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_AGATHA_TO_FORSAKEN && waypointId == WAYPOINT_ARRIVED_TO_FORSAKEN)
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
                            if (!_healCD && player->GetHealthPct() < 75.0f)
                            {
                                me->CastSpell(player, SPELL_UNHOLY_DARKNESS);

                                _healCD = true;

                                _events.ScheduleEvent(EVENT_HEAL_COOLDOWN, 8s);
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
                            if (target->GetEntry() == NPC_HILLSBRAD_REFUGEE1 || target->GetEntry() == NPC_HILLSBRAD_REFUGEE2 || target->GetEntry() == NPC_WORGEN_SENTRY)
                            {
                                if (!_doomHoulDone)
                                {
                                    DoCastVictim(SPELL_DOOMHOWL);

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

                case EVENT_RUN + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->SetFacingTo(3.159046f);

                        Talk(TALK_AGATHA_POST_EVENT1);
                    }

                    break;
                }

                case EVENT_RUN + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->CastSpell(player, SPELL_RIDE_REVERSE_CAST_RIDE_VEHICLE, true);

                        Talk(TALK_AGATHA_POST_EVENT2);

                        player->KilledMonsterCredit(NPC_AGATHA_FENRIS);

                        me->SetDisableGravity(true);
                        me->SetCanFly(true);

                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePath(PATH_AGATHA_TO_FORSAKEN, false);
                    }
                    break;
                }

                case EVENT_RUN + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        player->ExitVehicle();

                        _sceneStarted = false;

                        me->SetDisableGravity(false);
                        me->SetCanFly(false);

                        me->GetMotionMaster()->Clear();

                        if (player->GetGuardianPet())
                            me->GetMotionMaster()->MoveFollow(player, 4.0f, (float)M_PI * 1.5f, false, true, false);
                        else
                            me->GetMotionMaster()->MoveFollow(player, 4.0f, (float)M_PI / 2.0f, false, true, false);

                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetReactState(REACT_DEFENSIVE);
                    }

                    break;
                }
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void SetForQuest27099()
    {
        _sceneStarted = true;

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);

        me->GetMotionMaster()->Clear();

        me->GetMotionMaster()->MovePoint(POINT_AGATHA_BACK_FRONTYARD, AgathaBackFrontyardPos);

        Talk(TALK_AGATHA_PRE_EVENT);

        if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            player->RemoveAura(SPELL_BOND_OF_THE_VALKYR);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _targetGUID;
    bool _doomHoulDone;
    bool _healCD;
    bool _sceneStarted;
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
                    {
                        if (agatha->IsAIEnabled())
                            agatha->AI()->Talk(TALK_AGATHA_BROADCAST, player);
                    }
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
                    if (agatha->IsAIEnabled())
                        agatha->CastSpell(caster, SPELL_RISE_FORSAKEN, false);

                    if (Player* player = ObjectAccessor::GetPlayer(*target, _ownerGUID))
                    {
                        if (roll_chance_i(50))
                        {
                            if (agatha->IsAIEnabled())
                                agatha->AI()->Talk(TALK_AGATHA_RISE_FORSAKEN, player);
                        }
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

    TALK_REFUGEE_JUST_RISEN                         = 0,

    DISPLAY_MALE_01_D_F                             = 33986,
    DISPLAY_MALE_02_D_F                             = 33987,
    DISPLAY_MALE_03_D_F                             = 33988,
    DISPLAY_MALE_04_D_F                             = 33989,
    DISPLAY_FEMALE_01_D_F                           = 33991,
    DISPLAY_FEMALE_02_D_F                           = 33992,
    DISPLAY_FEMALE_03_D_F                           = 33993,
    DISPLAY_FEMALE_04_D_F                           = 33994,

    DISPLAY_MALE_01_F                               = 33978,
    DISPLAY_MALE_02_F                               = 33979,
    DISPLAY_MALE_03_F                               = 33980,
    DISPLAY_MALE_04_F                               = 33981,
    DISPLAY_FEMALE_01_F                             = 33982,
    DISPLAY_FEMALE_02_F                             = 33983,
    DISPLAY_FEMALE_03_F                             = 33984, 
    DISPLAY_FEMALE_04_F                             = 33985,

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
                if (hillsbradRefugee->IsAIEnabled())
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
                DoCastSelf(SPELL_FORSAKEN_TROOPER_MASTERSCRIPT_FENRIS);

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
        _events.ScheduleEvent(EVENT_LORDAERON_MIGHT, 3s, 8s);
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
                    DoCastSelf(SPELL_LORDAERON_MIGHT);

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
            _events.ScheduleEvent(EVENT_TALK_REVIVE, 1s);

        me->DespawnOrUnsummon(4s);
    }

    void Reset() override
    {
        _events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TALK_REVIVE:
                {
                    Talk(TALK_REFUGEE_JUST_RISEN);
                    break;
                }

                default:
                    break;
            }
        }
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
            _events.ScheduleEvent(EVENT_TALK_REVIVE, 1s);

        me->DespawnOrUnsummon(4s);
    }

    void Reset() override
    {
        _events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TALK_REVIVE:
                {
                    Talk(TALK_REFUGEE_JUST_RISEN);
                    break;
                }

                default:
                    break;
            }
        }
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

// Worgen Sentry - 44987
struct npc_silverpine_worgen_sentry : public ScriptedAI
{
    npc_silverpine_worgen_sentry(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (urand(0, 2) == 2)
            Talk(TALK_SENTRY_WARNING);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        DoCastSelf(SPELL_BATTLE_ROAR);

        _events.ScheduleEvent(EVENT_UNDYING_FRENZY, 3s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_UNDYING_FRENZY:
                {
                    DoCastVictim(SPELL_UNDYING_FRENZY);

                    _events.RescheduleEvent(EVENT_UNDYING_FRENZY, 10s, 12s);
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

// Undying Frenzy - 80515
class spell_silverpine_undying_frenzy : public AuraScript
{
    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        GetCaster()->CastSpell(GetTarget(), GetSpellInfo()->Effects[EFFECT_1].TriggerSpell, true);
    }

    void Register() override
    {
        OnEffectPeriodic.Register(&spell_silverpine_undying_frenzy::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// No Escape - 6230
class at_silverpine_no_escape : public AreaTriggerScript
{
public:
    at_silverpine_no_escape() : AreaTriggerScript("at_silverpine_no_escape") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*at*/) override
    {
        if (player->IsAlive())
        {
            if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE)
            {
                if (Creature* agatha = player->FindNearestCreature(NPC_AGATHA_FENRIS, 10.0f, true))
                {
                    if (agatha->GetOwner() == player)
                    {
                        if (agatha->IsAIEnabled())
                        {
                            if (Creature* fenrisStalker = player->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
                            {
                                if (fenrisStalker->IsAIEnabled())
                                    fenrisStalker->AI()->SetGUID(player->GetGUID());
                            }
                        }
                    }
                }
            }
        }

        return true;
    }
};

// Summon Fenris Actors - 84053
class spell_silverpine_summon_fenris_actors : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfi*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_SUMMON_BLOODFANG_FENRIS,
                SPELL_SUMMON_CROWLEY_FENRIS,
                SPELL_SUMMON_PHIN_ODELIC,
                SPELL_SUMMON_BARTOLO_GINSETTI,
                SPELL_SUMMON_LOREMASTER_DIBBS,
                SPELL_SUMMON_MAGISTRATE_HENRY,
                SPELL_SUMMON_CARETAKER_SMITHERS,
                SPELL_SUMMON_SOPHIA
            });
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            target->CastSpell(target, SPELL_SUMMON_BLOODFANG_FENRIS, true);
            target->CastSpell(target, SPELL_SUMMON_CROWLEY_FENRIS, true);
            target->CastSpell(target, SPELL_SUMMON_PHIN_ODELIC, true);
            target->CastSpell(target, SPELL_SUMMON_BARTOLO_GINSETTI, true);
            target->CastSpell(target, SPELL_SUMMON_LOREMASTER_DIBBS, true);
            target->CastSpell(target, SPELL_SUMMON_MAGISTRATE_HENRY, true);
            target->CastSpell(target, SPELL_SUMMON_CARETAKER_SMITHERS, true);
            target->CastSpell(target, SPELL_SUMMON_SOPHIA, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_silverpine_summon_fenris_actors::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Despawn All Summons - 84065
class spell_silverpine_despawn_all_summons_fenris : public SpellScript
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

        entrys.push_back(NPC_CROWLEY_FENRIS);
        entrys.push_back(NPC_BLOODFANG_FENRIS);
        entrys.push_back(NPC_PHIN_ODELIC);
        entrys.push_back(NPC_BARTOLO_GINSETTI);
        entrys.push_back(NPC_LOREMASTER_DIBBS);
        entrys.push_back(NPC_MAGISTRATE_HENRY_MALEB);
        entrys.push_back(NPC_CARETAKER_SMITHERS);
        entrys.push_back(NPC_SOPHIA_ZWOSKI);
        entrys.push_back(NPC_FENRIS_KEEP_CAMERA);

        targets.remove_if(IsNotInEntryList(entrys));
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetHitUnit()->IsSummon())
            GetHitUnit()->ToCreature()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_silverpine_despawn_all_summons_fenris::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget.Register(&spell_silverpine_despawn_all_summons_fenris::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

Position const NoEscapeStartPos = { 981.782f, 670.953f, 74.898f, 3.1887f };

// Fenris Keep Stalker - 45032
struct npc_silverpine_fenris_stalker : public ScriptedAI
{
    npc_silverpine_fenris_stalker(Creature* creature) : ScriptedAI(creature) { }

    void SetGUID(ObjectGuid const& guid, int32 /*id*/) override
    {
        if (!_playerGUID.IsEmpty())
            return;

        _playerGUID = guid;

        DoAction(ACTION_START_ANIM);
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

    void DoAction(int32 param) override
    {
        switch (param)
        {
            case ACTION_START_ANIM:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                {
                    me->CastSpell(player, SPELL_FORCE_CAST_FENRIS_CAMERA, true);
                    player->NearTeleportTo(NoEscapeStartPos, true);
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
                case EVENT_MASTER_RESET:
                {
                    Reset();
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

Position const CameraFrontyardPos = { 980.7f, 689.14f, 76.9f };

// Fenris Keep Camera - 45003
struct npc_silverpine_fenris_camera : public ScriptedAI
{
    npc_silverpine_fenris_camera(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            _playerGUID = player->GetGUID();

            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                player->EnterVehicle(me, SEAT_FENRIS_CAMERA);
        }

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84107:
            {
                _events.ScheduleEvent(EVENT_FINISH, 2s + 500ms);
                break;
            }

            default:
                break;
        }
    }

    void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
    {
        if (apply && seatId == SEAT_FENRIS_CAMERA)
        {
            _events.ScheduleEvent(EVENT_MOVE_TO_STARTPOINT, 10ms);

            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                me->CastSpell(player, SPELL_SUMMON_FENRIS_ACTORS, true);
        }

        if (apply && seatId == SEAT_FENRIS_CAMERA_FORCE)
            passenger->SetFacingTo(0.0f);

        if (!apply && seatId == SEAT_FENRIS_CAMERA_FORCE)
            _events.ScheduleEvent(EVENT_FINISH, 1s);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == POINT_MOTION_TYPE && id == POINT_CAMERA_FRONTYARD)
            _events.ScheduleEvent(EVENT_CHANGE_TO_SEAT_2, 500ms);
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
                    me->GetMotionMaster()->MovePoint(POINT_CAMERA_FRONTYARD, CameraFrontyardPos);
                    break;
                }

                case EVENT_CHANGE_TO_SEAT_2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_FORCE_SEAT_2, true);

                    _events.ScheduleEvent(EVENT_TRIGGER_84102, 2s);
                    break;
                }

                case EVENT_TRIGGER_84102:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (Creature* crowley = player->FindNearestCreature(NPC_CROWLEY_FENRIS, 50.0f))
                            player->CastSpell(player, SPELL_GENERAL_TRIGGER_84102, true);
                    }

                    break;
                }

                case EVENT_FINISH:
                {
                    if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA_FENRIS, 60.0f))
                        me->CastSpell(agatha, SPELL_GENERAL_TRIGGER_84079, true);

                    me->SetFacingTo(0.08278348f);

                    _events.ScheduleEvent(EVENT_FINISH + 1, 4s);
                    break;
                }

                case EVENT_FINISH + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_DESPAWN_ALL_SUMMONS_FENRIS, true);
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

// Lord Darius Crowley - 44989
struct npc_silverpine_crowley_fenris : public ScriptedAI
{
    npc_silverpine_crowley_fenris(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_GENERAL_TRIGGER_84102:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                {
                    me->CastSpell(player, SPELL_ARMORE_CAMERA_FENRIS, true);

                    Talk(TALK_CROWLEY_NO_ESCAPE_6);

                    _events.ScheduleEvent(EVENT_ANIMATION, 2s + 500ms);
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
                case EVENT_ANIMATION:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_0, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 1, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_1, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 2, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_2, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 3, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_3, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 4, 4s + 700ms);
                    break;
                }

                case EVENT_ANIMATION + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_4, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 5, 6s + 100ms);
                    break;
                }

                case EVENT_ANIMATION + 5:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_CROWLEY_NO_ESCAPE_5, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 6, 9s + 500ms);
                    break;
                }

                case EVENT_ANIMATION + 6:
                {
                    if (Creature* henry = me->FindNearestCreature(NPC_MAGISTRATE_HENRY_MALEB, 30.0f))
                        me->CastSpell(henry, SPELL_CONVERSATION_TRIGGER_84076, true);
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

// Packleader Ivar Bloodfang - 44990
struct npc_silverpine_bloodfang_fenris : public ScriptedAI
{
    npc_silverpine_bloodfang_fenris(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
};

// Phin Odelic - 44993
struct npc_silverpine_odelic_fenris : public ScriptedAI
{
    npc_silverpine_odelic_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_ODELIC);

                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
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
    npc_silverpine_bartolo_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_BARTOLO);

                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
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
    npc_silverpine_loremaster_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_DIBBS);

                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
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
    npc_silverpine_henry_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84076:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    me->CastSpell(player, SPELL_ARMORE_CAMERA_2, true);
                break;
            }

            case SPELL_ARMORE_CAMERA_3:
            {
                _events.ScheduleEvent(EVENT_ANIMATION, 1s + 300ms);
                break;
            }

            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_HENRY_NO_ESCAPE_0, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 1, 10s + 800ms);
                    break;
                }

                case EVENT_ANIMATION + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_HENRY_NO_ESCAPE_1, player);

                    _events.ScheduleEvent(EVENT_ANIMATION + 2, 2s);
                    break;
                }

                case EVENT_ANIMATION + 2:
                {
                    DoCastSelf(SPELL_CONVERSATION_TRIGGER_84077, true);
                    break;
                }

                case EVENT_DRINKING:
                {
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_HENRY);

                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);

                    _events.ScheduleEvent(EVENT_MORPHING + 2, 2s);
                    break;
                }

                case EVENT_MORPHING + 2:
                {
                    if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA_FENRIS, 60.0f))
                        me->CastSpell(agatha, SPELL_GENERAL_TRIGGER_84107, true);
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
    npc_silverpine_caretaker_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_SMITHERS);

                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
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
    npc_silverpine_sophia_fenris(Creature* creature) : ScriptedAI(creature), _isWorgen(false) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* fenrisStalker = me->FindNearestCreature(NPC_FENRIS_KEEP_STALKER, 50.0f, true))
            me->SetFacingToObject(fenrisStalker);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                _events.ScheduleEvent(EVENT_DRINKING, 1s);
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
                    _events.ScheduleEvent(EVENT_MORPHING, 1s + 500ms);
                    break;
                }

                case EVENT_MORPHING:
                {
                    _isWorgen = true;

                    DoCastSelf(SPELL_BLOOD_ZWOSKI);

                    _events.ScheduleEvent(EVENT_HOWLING, 1s + 500ms);
                    _events.ScheduleEvent(EVENT_MORPHING + 1, 1s);
                    break;
                }

                case EVENT_MORPHING + 1:
                {
                    me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
                    break;
                }

                case EVENT_HOWLING:
                {
                    me->PlayDirectSound(SOUND_HOWLING);
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

    SPELL_SUMMON_LORDAERON_ACTORS       = 84127,
    SPELL_RIDE_SYLVANAS_HORSE           = 84166,
    SPELL_LORDAERON_AURA                = 84189, // NYI, no idea what it is for
    SPELL_KILL_ME                       = 84180,
    SPELL_DREADGUARD_SALUTE_PERIODIC    = 84200,
    SPELL_DREADGUARD_SALUTE             = 84199, 
    SPELL_LORDAERON_COMPLETE            = 84185,
    SPELL_DESPAWN_ALL_SUMMONS_LORDAERON = 84173,
    SPELL_FADE_TO_BLACK                 = 89092,
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

    TALK_SYLVANAS_LORDAERON_0           = 0,
    TALK_SYLVANAS_LORDAERON_1           = 1,
    TALK_SYLVANAS_LORDAERON_2           = 2,
    TALK_SYLVANAS_LORDAERON_3           = 3,
    TALK_SYLVANAS_LORDAERON_4           = 4,
    TALK_SYLVANAS_LORDAERON_5           = 5,
    TALK_SYLVANAS_LORDAERON_6           = 6,
    TALK_SYLVANAS_LORDAERON_7           = 7,
    TALK_SYLVANAS_LORDAERON_8           = 8,
    TALK_SYLVANAS_LORDAERON_9           = 9,
    TALK_SYLVANAS_LORDAERON_10          = 10,
    TALK_SYLVANAS_LORDAERON_11          = 11,
    TALK_SYLVANAS_LORDAERON_12          = 12,
    TALK_SYLVANAS_LORDAERON_13          = 13,
    TALK_SYLVANAS_LORDAERON_14          = 14,
    TALK_SYLVANAS_LORDAERON_15          = 15,
    TALK_SYLVANAS_LORDAERON_16          = 16,
    TALK_SYLVANAS_LORDAERON_17          = 17,
    TALK_SYLVANAS_LORDAERON_18          = 18,
    TALK_SYLVANAS_LORDAERON_19          = 19,
    TALK_SYLVANAS_LORDAERON_20          = 20,
    TALK_SYLVANAS_LORDAERON_21          = 21,

    PATH_WARHORSE_TO_SEPULCHER          = 450570,
    PATH_ORC_LEADER1                    = 4494200,
    PATH_ORC_LEADER2                    = 4494201,
    PATH_ARTHURA_TO_GILNEAS             = 453180,

    WAYPOINT_ARRIVED_TO_SEPULCHER       = 42,
    WAYPOINT_JUST_TOOK_OFF              = 1,
    WAYPOINT_ARRIVED_TO_GILNEAS         = 41,

    POINT_CLOSE_TO_ZONE                 = 1,

    ANIMKIT_DREADGUARD_SALUTE           = 898,
};

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
                    warhorse->CastSpell(warhorse, SPELL_APPLY_INVIS_ZONE_1, true);
                else if (me->HasAura(SPELL_APPLY_INVIS_ZONE_4))
                    warhorse->CastSpell(warhorse, SPELL_APPLY_INVIS_ZONE_4, true);

                me->EnterVehicle(warhorse, SEAT_WARHORSE_SYLVANAS);

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
            Position pos = player->GetPosition();

            player->CastSpell(player, SPELL_SUMMON_SYLVANAS_AND_HORSE, true);
            player->CastSpell(player, SPELL_SUMMON_FORSAKEN_WARHORSE, true);
            player->NearTeleportTo(pos, false);
            player->CastSpell(player, SPELL_LORDAERON_AURA, true);
            player->CastSpell(player, SPELL_SUMMON_LORDAERON_ACTORS, true);
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

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
};

// Forsaken Warhorse (Player) - 45041
struct npc_silverpine_warhorse_player_lordaeron : public ScriptedAI
{
    npc_silverpine_warhorse_player_lordaeron(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            _playerGUID = player->GetGUID();

            player->EnterVehicle(me);
        }

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (apply && passenger->GetTypeId() == TYPEID_PLAYER)
        {
            me->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
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

Position const LordaeronFinish = { 499.185f, 1549.9855f, 129.094f };

// Forsaken Warhorse (Sylvanas) - 45057
struct npc_silverpine_warhorse_sylvanas_lordaeron : public ScriptedAI
{
    npc_silverpine_warhorse_sylvanas_lordaeron(Creature* creature) : ScriptedAI(creature) { }

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


        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetReactState(REACT_PASSIVE);
    }

    void PassengerBoarded(Unit* /*passenger*/, int8 seatId, bool apply) override
    {
        if (!apply)
            return;

        if (seatId == SEAT_WARHORSE_SYLVANAS)
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_WARHORSE_TO_SEPULCHER)
        {
            if (waypointId == WAYPOINT_ARRIVED_TO_SEPULCHER)
                me->GetMotionMaster()->MovePoint(POINT_CLOSE_TO_ZONE, LordaeronFinish, false, 3.5f);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);
    }

private:
    EventMap _events;
    ObjectGuid _playerGUID;
    ObjectGuid _playerHorseGUID;
    ObjectGuid _sylvanasGUID;
};

// Lady Sylvanas Windrunner (Lordaeron) - 45051
struct npc_silverpine_sylvanas_lordaeron : public ScriptedAI
{
    npc_silverpine_sylvanas_lordaeron(Creature* creature) : ScriptedAI(creature), _heartstrikeCD(false) { }

    void IsSummonedBy(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
            _playerGUID = player->GetGUID();

        if (Creature* sylvanashorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_SYLVANAS, 10.0f))
        {
            _sylvanasHorseGUID = sylvanashorse->GetGUID();

            me->EnterVehicle(sylvanashorse, SEAT_WARHORSE_SYLVANAS);

            if (sylvanashorse->IsAIEnabled())
                sylvanashorse->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);

            _events.ScheduleEvent(EVENT_RIDE_WARHORSE, 500ms);
        }

        DoCastSelf(SPELL_DREADGUARD_SALUTE_PERIODIC);
    }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_KILL_ME:
            {
                if (_heartstrikeCD || me->HasUnitState(UNIT_STATE_CASTING))
                    me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
                else
                {
                    me->HandleEmoteCommand(EMOTE_STATE_HOLD_BOW);

                    if (caster->ToUnit()->GetEntry() == NPC_WORGEN_RENEGADE)
                        me->CastSpell(caster, SPELL_HEARTSTRIKE, false);

                    _heartstrikeCD = true;

                    _events.ScheduleEvent(EVENT_RESET_HEARTSTRIKE, 2s);
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
                case EVENT_RIDE_WARHORSE:
                {
                    if (Creature* playerhorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_PLAYER, 10.0f))
                    {
                        _playerHorseGUID = playerhorse->GetGUID();

                        if (Creature* sylvanashorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_SYLVANAS, 10.0f))
                        {
                            if (playerhorse->IsAIEnabled())
                                playerhorse->EnterVehicle(sylvanashorse, SEAT_WARHORSE_PLAYER_BOARDED);
                        }
                    }

                    _events.ScheduleEvent(EVENT_RIDE_WARHORSE + 1, 1s);
                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 7s + 500ms);
                    break;
                }

                case EVENT_RIDE_WARHORSE + 1:
                {
                    if (Creature* sylvanashorse = me->FindNearestCreature(NPC_FORSAKEN_WARHORSE_SYLVANAS, 10.0f))
                    {
                        if (sylvanashorse->IsAIEnabled())
                            sylvanashorse->GetMotionMaster()->MovePath(PATH_WARHORSE_TO_SEPULCHER, false);
                    }

                    break;
                }

                case EVENT_CHAT_TO_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_0, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 1, 5s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_1, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 2, 5s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_2, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 3, 8s + 800ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_3, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 4, 5s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_4, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 5, 8s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 5:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_5, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 6, 8s + 900ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 6:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_6, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 7, 9s + 900ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 7:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_7, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 8, 10s + 900ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 8:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_8, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 9, 9s + 800ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 9:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_9, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 10, 5s + 800ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 10:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_10, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 11, 5s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 11:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_11, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 12, 9s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 12:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_12, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 13, 6s + 800ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 13:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_13, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 14, 11s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 14:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_14, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 15, 11s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 15:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_15, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 16, 9s + 300ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 16:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_16, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 17, 9s + 800ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 17:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_17, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 18, 5s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 18:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_18, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 19, 9s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 19:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_19, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 20, 12s + 500ms);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 20:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_20, player);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 21, 7s);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 21:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        Talk(TALK_SYLVANAS_LORDAERON_21, player);

                    _events.ScheduleEvent(EVENT_FINISH_RIDE, 10s);
                    break;
                }

                case EVENT_FINISH_RIDE:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->CastSpell(player, SPELL_LORDAERON_COMPLETE, true);

                    _events.ScheduleEvent(EVENT_FINISH_RIDE + 1, 100ms);
                    break;
                }

                case EVENT_FINISH_RIDE + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        player->CastSpell(player, SPELL_DESPAWN_ALL_SUMMONS_LORDAERON, true);
                        player->CastSpell(player, SPELL_FADE_TO_BLACK, true);
                    }

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

                me->SetAIAnimKitId(ANIMKIT_DREADGUARD_SALUTE);

                _events.ScheduleEvent(EVENT_START_EMOTE, 3s);

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

                    me->SetAIAnimKitId(ANIMKIT_RESET);

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

Position const ForsakenTrooperMPos[5] =
{
    { 1278.29f, 1053.83f, 54.284f, 3.16124f },
    { 1258.23f, 1078.28f, 52.430f, 3.30653f },
    { 1251.07f, 1161.13f, 51.974f, 3.53822f },
    { 1204.23f, 1174.21f, 46.824f, 3.53822f },
    { 1180.46f, 1227.42f, 54.059f, 3.53822f }
};

Position const ForsakenTrooperFPos[5] =
{
    { 1308.89f, 1059.138f, 54.598f, 3.16124f },
    { 1227.65f, 1107.85f, 50.456f, 3.30653f },
    { 1252.97f, 1148.88f, 49.575f, 3.53822f },
    { 1178.60f, 1200.06f, 48.753f, 3.53822f },
    { 1144.25f, 1153.30f, 48.406f, 3.53822f }
};

Position const WorgenRenegadePos[10] =
{
    { 1308.89f, 1059.138f, 54.598f, 3.16124f },
    { 1227.65f, 1107.85f, 50.456f, 3.30653f },
    { 1252.97f, 1148.88f, 49.575f, 3.53822f },
    { 1178.60f, 1200.06f, 48.753f, 3.53822f },
    { 1144.25f, 1153.30f, 48.406f, 3.53822f },
    { 1278.29f, 1053.83f, 54.284f, 3.16124f },
    { 1258.23f, 1078.28f, 52.430f, 3.30653f },
    { 1251.07f, 1161.13f, 51.974f, 3.53822f },
    { 1204.23f, 1174.21f, 46.824f, 3.53822f },
    { 1180.46f, 1227.42f, 54.059f, 3.53822f }
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

Position const SeaOrcLeaderPos1 = { 1010.04f, 1301.17f, 45.891f, 2.53684f };
Position const SeaOrcLeaderPos2 = { 1042.26f, 1337.02f, 37.903f, 3.68637f };

Position const DreadguardPos[27] =
{
    { 636.148f, 1317.93f, 83.6952f, 3.73749f },
    { 630.318f, 1326.25f, 84.0411f, 3.78069f },
    { 624.334f, 1335.32f, 84.9048f, 3.75712f },
    { 619.605f, 1345.29f, 85.8326f, 3.57648f },
    { 614.219f, 1357.1f, 86.9278f, 3.60397f },
    { 608.645f, 1369.23f, 88.0946f, 3.56863f },
    { 603.713f, 1380.07f, 89.3707f, 3.56863f },
    { 598.781f, 1390.9f, 90.9621f, 3.56863f },
    { 592.28f, 1408.5f, 93.6573f, 3.54592f },
    { 595.234f, 1399.99f, 92.2739f, 3.49581f },
    { 588.529f, 1386.8f, 91.2004f, 0.391692f },
    { 593.998f, 1375.77f, 89.6546f, 0.423108f },
    { 599.016f, 1364.63f, 88.6384f, 0.423108f },
    { 604.064f, 1353.47f, 87.6216f, 0.391692f },
    { 610.865f, 1340.42f, 86.6103f, 0.478085f },
    { 616.218f, 1331.02f, 86.0793f, 0.474158f },
    { 622.396f, 1321.63f, 85.0942f, 0.482012f },
    { 627.318f, 1312.74f, 84.2524f, 0.505574f },
    { 580.802f, 1404.8f, 94.3732f, 0.321859f },
    { 584.395f, 1397.61f, 93.0061f, 0.235466f },
    { 573.944f, 1436.88f, 100.719f, 4.72984f },
    { 573.306f, 1440.54f, 101.701f, 2.63545f },
    { 558.766f, 1426.97f, 99.5213f, 0.087266f },
    { 556.385f, 1430.23f, 100.23f, 1.50098f },
    { 574.674f, 1412.9f, 96.1487f, 5.48033f },
    { 588.505f, 1417.08f, 95.532f, 3.60485f },
    { 576.29f, 1412.34f, 95.9112f, 0.400425f }
};

// Summon Lordaeron Actors - 84127
class spell_silverpine_summon_lordaeron_actors : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            _playerGUID = caster->GetGUID();

            for (auto pos : ForsakenTrooperMPos)
                caster->SummonCreature(NPC_FORSAKEN_TROOPER1, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);

            for (auto pos : ForsakenTrooperFPos)
                caster->SummonCreature(NPC_FORSAKEN_TROOPER2, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);

            for (auto pos : WorgenRenegadePos)
                caster->SummonCreature(NPC_WORGEN_RENEGADE, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);

            for (auto pos : OrcDemolisherPos)
                caster->SummonCreature(NPC_ORC_DEMOLISHER_LORDAERON, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);

            for (auto pos : SeaOrcPos)
                caster->SummonCreature(NPC_ORC_MOVER, pos, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID);

            if (Creature* leaderOrc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos1, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
            {
                if (leaderOrc->IsAIEnabled())
                    leaderOrc->GetMotionMaster()->MovePath(PATH_ORC_LEADER1, true);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos1, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 3.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos1, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 6.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos1, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 9.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos1, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 12.0f, 0.0f, false, true, false);
                }
            }

            if (Creature* leaderOrc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos2, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
            {
                if (leaderOrc->IsAIEnabled())
                    leaderOrc->GetMotionMaster()->MovePath(PATH_ORC_LEADER2, true);

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos2, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 3.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos2, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 6.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos2, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 9.0f, 0.0f, false, true, false);
                }

                if (Creature* orc = caster->SummonCreature(NPC_ORC_MOVER, SeaOrcLeaderPos2, TEMPSUMMON_TIMED_DESPAWN, 300s, 0, _playerGUID))
                {
                    if (orc->IsAIEnabled())
                        orc->GetMotionMaster()->MoveFollow(leaderOrc, 12.0f, 0.0f, false, true, false);
                }

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

// Despawn All Summons - 84173
class spell_silverpine_despawn_all_summons_lordaeron : public SpellScript
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

        entrys.push_back(NPC_SYLVANAS_LORDAERON);
        entrys.push_back(NPC_FORSAKEN_WARHORSE_SYLVANAS);
        entrys.push_back(NPC_FORSAKEN_WARHORSE_PLAYER);

        targets.remove_if(IsNotInEntryList(entrys));
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (GetHitUnit()->IsSummon())
                GetHitUnit()->ToCreature()->DespawnOrUnsummon();
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect.Register(&spell_silverpine_despawn_all_summons_lordaeron::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget.Register(&spell_silverpine_despawn_all_summons_lordaeron::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum SepulcherQuests
{
    QUEST_EXCISING_THE_TAINT        = 27181,

    SPELL_STALKING                  = 86237,
    SPELL_SHADOWSTEP                = 79864,
    SPELL_KILL_ME_QUEST             = 86559,

    TALK_ARTHURA_FLIGHT             = 0,
};

// Bloodfang Stalker - 45195
struct npc_silverpine_bloodfang_stalker : public ScriptedAI
{
    npc_silverpine_bloodfang_stalker(Creature* creature) : ScriptedAI(creature) { }

    void JustAppeared() override
    {
        me->SetPowerType(POWER_ENERGY);
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 100, true);
    }

    void Reset() override
    {
        DoCastSelf(SPELL_STALKING);
        DoCastSelf(SPELL_KILL_ME_QUEST);
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
    npc_arthura_sepulcher(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            if (Vehicle* vehicle = me->GetVehicleKit())
            {
                _playerGUID = player->GetGUID();

                me->GetMotionMaster()->MovePath(PATH_ARTHURA_TO_GILNEAS, false);
            }
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_ARTHURA_TO_GILNEAS)
        {
            if (waypointId == WAYPOINT_JUST_TOOK_OFF)
                Talk(TALK_ARTHURA_FLIGHT);

            if (waypointId == WAYPOINT_ARRIVED_TO_GILNEAS)
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

    RegisterSpellScript(spell_silverpine_eject_passengers_3_8);
    RegisterCreatureAI(npc_silverpine_horde_hauler);
    RegisterCreatureAI(npc_silverpine_horde_coffin_hauler); 

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

    RegisterCreatureAI(npc_silverpine_worgen_sentry);
    RegisterSpellScript(spell_silverpine_undying_frenzy);
    new at_silverpine_no_escape();
    RegisterSpellScript(spell_silverpine_summon_fenris_actors);
    RegisterSpellScript(spell_silverpine_despawn_all_summons_fenris);
    RegisterCreatureAI(npc_silverpine_fenris_stalker);
    RegisterCreatureAI(npc_silverpine_fenris_camera);
    RegisterCreatureAI(npc_silverpine_crowley_fenris);
    RegisterCreatureAI(npc_silverpine_bloodfang_fenris);
    RegisterCreatureAI(npc_silverpine_odelic_fenris);
    RegisterCreatureAI(npc_silverpine_bartolo_fenris);
    RegisterCreatureAI(npc_silverpine_loremaster_fenris);
    RegisterCreatureAI(npc_silverpine_henry_fenris);
    RegisterCreatureAI(npc_silverpine_caretaker_fenris);
    RegisterCreatureAI(npc_silverpine_sophia_fenris);

    RegisterSpellScript(spell_silverpine_summon_lordaeron_actors);
    RegisterSpellScript(spell_silverpine_despawn_all_summons_lordaeron);
    RegisterCreatureAI(npc_silverpine_warhorse_player_lordaeron);
    RegisterCreatureAI(npc_silverpine_warhorse_sylvanas_lordaeron);
    RegisterCreatureAI(npc_silverpine_sylvanas_lordaeron);
    RegisterCreatureAI(npc_silverpine_dreadguard_lordaeron);

    RegisterCreatureAI(npc_silverpine_bloodfang_stalker);

    RegisterCreatureAI(npc_arthura_sepulcher);

}
