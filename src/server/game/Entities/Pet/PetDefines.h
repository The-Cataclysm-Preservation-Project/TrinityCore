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

#ifndef TRINITYCORE_PET_DEFINES_H
#define TRINITYCORE_PET_DEFINES_H

// Obsolete
enum PetType
{
    SUMMON_PET              = 0,
    HUNTER_PET              = 1,
    MAX_PET_TYPE            = 4
};

static constexpr uint8 MAX_PET_STABLES = 20;

enum PetCallSpells
{
    SPELL_CALL_PET_1 = 883,
    SPELL_CALL_PET_2 = 83242,
    SPELL_CALL_PET_3 = 83243,
    SPELL_CALL_PET_4 = 83244,
    SPELL_CALL_PET_5 = 83245
};

static constexpr std::array<uint32, 5> PetCallSpellsArray = { SPELL_CALL_PET_1, SPELL_CALL_PET_2, SPELL_CALL_PET_3, SPELL_CALL_PET_4, SPELL_CALL_PET_5 };

enum PetStableSlot : uint8
{
    PET_SLOT_FIRST                  = 0,
    PET_SLOT_LAST                   = 25,
    PET_SLOT_FIRST_ACTIVE_SLOT      = PET_SLOT_FIRST,
    PET_SLOT_LAST_ACTIVE_SLOT       = 4,
    PET_SLOT_FIRST_STABLE_SLOT      = 5,
    PET_SLOT_LAST_STABLE_SLOT       = PET_SLOT_LAST,
    PET_SLOT_INACTIVE_STABLE_SLOTS  = 20
};

enum PetSpellState
{
    PETSPELL_UNCHANGED = 0,
    PETSPELL_CHANGED   = 1,
    PETSPELL_NEW       = 2,
    PETSPELL_REMOVED   = 3
};

enum PetSpellType
{
    PETSPELL_NORMAL = 0,
    PETSPELL_FAMILY = 1,
    PETSPELL_TALENT = 2
};

enum ActionFeedback
{
    FEEDBACK_NONE               = 0,
    FEEDBACK_PET_DEAD           = 1,
    FEEDBACK_NOTHING_TO_ATT     = 2,
    FEEDBACK_CANT_ATT_TARGET    = 3,
    FEEBDACK_CHARGE_HAS_NO_PATH = 4
};

enum PetTalk
{
    PET_TALK_SPECIAL_SPELL  = 0,
    PET_TALK_ATTACK         = 1
};

enum class PetStableResultCode : uint8
{
    NotEnoughMoney          = 1,
    InvalidSlot             = 3,
    StabledSucccessfully    = 8,
    UnstabledSucccessfully  = 9,
    UnlockedStableSlot      = 10,
    CannotControlExoticPets = 11,
    InternalError           = 12
};


enum class PlayerPetDataStatus : uint8
{
    UpToDate    = 0,
    New         = 1,
    Changed     = 2,
    Deleted     = 3
};

enum PetSummonSpellIds
{
    // Hunter Pets
    SPELL_PET_ENERGIZE              = 99289,

    // Warlock Minions
    SPELL_SUMMONING_DISORIENTATION  = 32752,

    // Pets
    SPELL_SUMMON_HEAL               = 36492 // serverside Spell
};

// Used by companions (minipets) and quest slot summons
constexpr float DEFAULT_FOLLOW_DISTANCE = 2.5f;
constexpr float DEFAULT_FOLLOW_DISTANCE_PET = 3.f;
constexpr float DEFAULT_FOLLOW_ANGLE = float(M_PI);

// The maximum distance that can be between a following summon and the player. If the player crosses this threshold the summon must be unsummoned/dismissed
constexpr float MAX_SUMMON_DISTANCE = 100.f;

using PlayerPetDataKey = std::pair<uint8 /*slot*/, uint32 /*creatureId*/>;

#endif
