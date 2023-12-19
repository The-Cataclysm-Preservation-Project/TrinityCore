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

#ifndef DEF_BASTION_OF_TWILIGHT_H
#define DEF_BASTION_OF_TWILIGHT_H

#include "CreatureAIImpl.h"

namespace BastionOfTwilight
{
constexpr char const* DataHeader = "BoT";
#define BoTScriptName "instance_bastion_of_twilight"

uint32 const EncounterCountNormal = 4;
uint32 const EncounterCountHeroic = 5;

enum BoTDataTypes
{
    // Encounter Types
    DATA_HALFUS_WYRMBREAKER             = 0,
    DATA_THERALION_AND_VALIONA          = 1,
    DATA_ASCENDANT_COUNCIL              = 2,
    DATA_CHOGALL                        = 3,
    DATA_SINESTRA                       = 4,

    // Creature Types
    DATA_PROTO_BEHEMOTH                 = 5,
    DATA_THERALION                      = 6,
    DATA_VALIONA                        = 7,
    DATA_IGNACIOUS                      = 8,
    DATA_FELUDIUS                       = 9,
    DATA_TERRASTRA                      = 10,
    DATA_ARION                          = 11,
    DATA_ELEMENTIUM_MONSTROSITY         = 12,
    DATA_ASCENDANT_COUNCIL_CONTROLLER   = 13,
    DATA_CORRUPTION                     = 14,
    DATA_CHOGALL_EVENT                  = 15,

    // GameObject Types
    DATA_GRIM_BATOL_RAID_TRAP_DOOR      = 16,

    // Areatriggers
    DATA_AT_HALFUS_WYRMBREAKER_INTRO    = 17,
    DATA_AT_THERALION_AND_VALIONA_INTRO = 18,
    DATA_AT_ASCENDANT_COUNCIL_INTRO_1   = 19,
    DATA_AT_ASCENDANT_COUNCIL_INTRO_2   = 20,
    DATA_AT_ASCENDANT_COUNCIL_INTRO_3   = 21,
    DATA_AT_CHOGALL_INTRO               = 22,

    // Encounter Related
    /*Halfus Wyrmbreaker*/
    DATA_ACTIVE_DRAGON_FLAGS,
    DATA_SLATE_DRAGON,
    DATA_NETHER_SCION,
    DATA_STORM_RIDER,
    DATA_TIME_WARDEN,
    DATA_WHELP_CAGE,

    /*Theralion and Valiona*/
    DATA_RANDOM_VALIONA_DUMMY,
    DATA_COLLAPSING_TWILIGHT_PORTAL_COUNT,
    DATA_VALIONA_AURA_DUMMY,

    /*Cho'Gall*/
    DATA_FULL_HEROIC_ID
};

enum BoTCreatures
{
    // Bosses
    BOSS_HALFUS_WYRMBREAKER             = 44600,
    BOSS_THERALION                      = 45993,
    BOSS_VALIONA                        = 45992,
    BOSS_IGNACIOUS                      = 43686,
    BOSS_FELUDIUS                       = 43687,
    BOSS_TERRASTRA                      = 43689,
    BOSS_ARION                          = 43688,
    BOSS_ELEMENTIUM_MONSTROSITY         = 43735,
    BOSS_CHOGALL                        = 43324,
    BOSS_SINESTRA                       = 45213,

    // Encounter related
    /*Halfus Wyrmbreaker*/
    NPC_PROTO_BEHEMOTH                  = 44687,
    NPC_NETHER_SCION                    = 44645,
    NPC_NETHER_SCION_ENCOUNTER          = 44828,
    NPC_SLATE_DRAGON                    = 44652,
    NPC_SLATE_DRAGON_ENCOUNTER          = 44829,
    NPC_STORM_RIDER                     = 44650,
    NPC_STORM_RIDER_ENCOUNTER           = 44826,
    NPC_TIME_WARDEN                     = 44797,
    NPC_TIME_WARDEN_ENCOUNTER           = 44653,
    NPC_ORPHANED_EMERALD_WELP           = 44641,
    NPC_SPIKE                           = 44765,

    /*Theralion and Valiona*/
    NPC_THERALION_FLIGHT_TARGET_STALKER = 46364,
    NPC_CONVECTIVE_FLAMES               = 46588,
    NPC_DAZZLING_DESTRUCTION_STALKER    = 46374,
    NPC_FABULOUS_FLAMES                 = 46448,
    NPC_COLLAPSING_TWILIGHT_PORTAL      = 46301,
    NPC_VALIONA_DUMMY                   = 46147,
    NPC_UNSTABLE_TWILIGHT               = 46304,
    NPC_TWILIGHT_SENTRY                 = 50008,
    NPC_TWILIGHT_RIFT                   = 50014,

    /*Ascendant Council*/
    NPC_ASCENDANT_COUNCIL_CONTROLLER    = 43691,
    NPC_WATER_BOMB                      = 44201,
    NPC_INFERNO_RUSH                    = 47501,
    NPC_FROZEN_ORB                      = 49518,
    NPC_LIQUID_ICE                      = 45452,

    /*Cho'Gall*/
    NPC_FIRE_PORTAL                     = 43393,
    NPC_FIRE_ELEMENTAL                  = 43406,
    NPC_CORRUPTION                      = 43999,
    NPC_MALFORMATION                    = 43888,
    NPC_SPIKED_TENTACLE_TRIGGER         = 50265,

    // Generic Creatures
    NPC_CHOGALL                         = 46965,
    NPC_INVISIBLE_STALKER               = 42098,
    NPC_TWILIGHT_PHASE_SHIFTER          = 45267
};

enum BoTGameObjects
{
    GO_HALFUS_ENTRANCE                  = 205222,
    GO_HALFUS_EXIT                      = 205223,
    GO_DRAGON_SIBLINGS_DOOR_ENTRANCE    = 205224,
    GO_DRAGON_SIBLINGS_DOOR_EXIT        = 205225,
    GO_ASCENDANT_COUNCIL_ENTRANCE       = 205226,
    GO_ASCENDANT_COUNCIL_EXIT           = 205227,
    GO_CHOGALL_ENTRANCE                 = 205228,
    GO_WHELP_CAGE                       = 205087,
    GO_WHELP_CAGE_BASE                  = 205088,
    GO_GRIM_BATOL_RAID_TRAP_DOOR        = 205898
};

enum BoTActions
{
    // Halfus Wyrmbreaker
    ACTION_DRAGON_FREED                     = 0,

    // Theralion and Valiona
    ACTION_START_ARGUMENT_INTRO             = 1,

    // Cho'Gall (Non-Boss version)
    ACTION_TALK_HALFUS_WYRMBREAKER_INTRO    = 1,
    ACTION_TALK_HALFUS_WYRMBREAKER_DEAD     = 2,

    // Cho'Gall (Boss)
    ACTION_TALK_THERALION_AND_VALIONA_INTRO = 1,
    ACTION_TALK_THERALION_AND_VALIONA_DEAD  = 2,
    ACTION_TALK_ASCENDANT_COUNCIL_INTRO_1   = 3,
    ACTION_TALK_ASCENDANT_COUNCIL_INTRO_2   = 4,
    ACTION_TALK_ASCENDANT_COUNCIL_INTRO_3   = 5,
    ACTION_TALK_CHOGALL_INTRO               = 6
};

enum BoTEvents
{
    EVENT_CAST_DANCING_FLAMES = 1,
    EVENT_CHOGALL_TALK_THERALION_AND_VALIONA_DEAD,
};

enum BoTSpells
{
    SPELL_DANCING_FLAMES_VISUAL = 83962,
    SPELL_BERSERK               = 26662
};

enum BoTWorldStates
{
    // Halfus Wyrmbreaker
    WORLD_STATE_ID_THE_ONLY_ESCAPE                      = 5607,

    // Ascendant Council
    WORLD_STATE_ID_ELEMENTARY                           = 5621,

    // Cho'Gall
    WORLD_STATE_ID_THE_ABYSS_WILL_GAZE_BACK_INTO_YOU    = 5659
};

enum BoTHalfusDragonFlags : uint8
{
    DRAGON_FLAG_NETHER_SCION    = 0x01,
    DRAGON_FLAG_SLATE_DRAGON    = 0x02,
    DRAGON_FLAG_STORM_RIDER     = 0x04,
    DRAGON_FLAG_TIME_WARDEN     = 0x08,
    DRAGON_FLAG_EMERALD_WHELPS  = 0x10,

    DRAGON_FLAG_ALL_ACTIVE = DRAGON_FLAG_NETHER_SCION | DRAGON_FLAG_SLATE_DRAGON | DRAGON_FLAG_STORM_RIDER | DRAGON_FLAG_TIME_WARDEN | DRAGON_FLAG_EMERALD_WHELPS
};

template<class AI>
AI* GetBastionOfTwilightAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, BoTScriptName);
}

#define RegisterBastionOfTwilightCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetBastionOfTwilightAI)

template<class AI>
AI* GetBastionOfTwilightAI(GameObject* go)
{
    return GetInstanceAI<AI>(go, BoTScriptName);
}
}

#endif
