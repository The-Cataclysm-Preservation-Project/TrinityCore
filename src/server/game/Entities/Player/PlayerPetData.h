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

#ifndef PlayerPetData_h__
#define PlayerPetData_h__

#include "SpellHistory.h"
#include "PetDefines.h"
#include "UnitDefines.h"

struct PlayerPetDataSpellState
{
    uint32 SpellId = 0;
    uint8 ActiveState = 0;
};

struct PlayerPetDataAura
{
    std::array<int32, MAX_SPELL_EFFECTS> Amount = { };
    std::array<int32, MAX_SPELL_EFFECTS> BaseAmount = { };
    ObjectGuid CasterGuid;
    uint32 SpellId = 0;
    uint8 EffectMask = 0;
    uint8 RecalculateMask = 0;
    uint8 StackAmount = 0;
    int32 MaxDuration = 0;
    int32 RemainingDuration = 0;
    uint8 RemainingCharges = 0;
    float CritChance = 0.f;
    bool ApplyResilience = false;
};

struct PlayerPetData
{
    PlayerPetData() { }

    uint32 PetNumber = 0;
    uint32 CreatureId = 0;
    uint32 TamedCreatureId = 0;
    uint32 DisplayId = 0;
    uint32 SavedHealth = 0;
    uint32 SavedPower = 0;
    uint32 CreatedBySpellId = 0;
    uint32 LastSaveTime = 0;
    ReactStates ReactState = REACT_ASSIST;
    uint8 Slot = 0;
    bool HasBeenRenamed = false;
    bool IsActive = false;
    std::string Name;
    std::string ActionBar;
    std::string Talents;
    std::vector<SpellHistory::CooldownEntry> Cooldowns;
    std::vector<PlayerPetDataAura> Auras;
    std::vector<PlayerPetDataSpellState> SpellStates;
    Optional<DeclinedName> DeclinedNames;
    PlayerPetDataStatus Status = PlayerPetDataStatus::New;
};

#endif
