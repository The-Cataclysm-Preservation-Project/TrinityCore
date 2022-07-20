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

#ifndef NewPet_h__
#define NewPet_h__

#include "NewGuardian.h"
#include "Optional.h"
#include "PlayerPetData.h"
#include "PetDefines.h"

struct PlayerPetData;

struct PetSpell
{
    ActiveStates Active = ACT_DECIDE;
    PetSpellState State = PETSPELL_NEW;
    PetSpellType Type = PETSPELL_NORMAL;
};

using PetSpellMap = std::unordered_map<uint32 /*spellId*/, PetSpell /*actionBarValues*/>;
using PetTalentMap = std::unordered_map<uint32 /*talentId*/, uint32 /*rank*/>;

class TC_GAME_API NewPet final : public NewGuardian
{
public:
    explicit NewPet(SummonPropertiesEntry const* properties, Unit* owner, bool isWorldObject, bool isClassPet, uint32 creatureId, uint8 slot);

    void AddToWorld() override;
    void RemoveFromWorld() override;

    bool HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons) override;
    void HandlePostSummonActions() override;

    bool ShouldDespawnOnSummonerLogout() const override { return true; }
    bool ShouldJoinSummonerSpawnGroupAfterCreation() const override { return true; }

    // Returns true if the pet is belongs to a specific class (Hunter Pets, Mage Water Elementals, DK Ghouls and Warlock Minions)
    bool IsClassPet() const { return _isClassPet; }
    // Returns true if the pet is a hunter class pet. This is the case when the pet has player pet data and is stored under creatureId = 0
    bool IsHunterPet() const { return _playerPetDataKey.has_value() && _playerPetDataKey->second == 0; }
    // Returns true if the pet has a player pet data key to acces a player's pet data
    bool HasPlayerPetDataKey() { return _playerPetDataKey.has_value(); }
    // Returns the player pet data map key for the element that is stored in summoner's player class
    Optional<PlayerPetDataKey> const& GetPlayerPetDataKey() const { return _playerPetDataKey; }
    // Returns true if the summoner is allowed to dismiss the pet via pet action
    bool CanBeDismissed() const;
    // Unsummons the pet. If permanent is set to true, some pets will play a dismiss sound (such as Warlock pets)
    void Dismiss(bool permanent = true);
    // Overriden method of TemporarySummon::Unsummon to ensure that Pet::Dismiss is always called.
    void Unsummon(Milliseconds timeUntilDespawn = 0ms) override;
    // Returns or creates player pet data. Does return nullptr when the summon is a hunter pet and no pet data is available
    PlayerPetData* GetOrCreatePlayerPetData(Player* summoner, uint8 slot, uint32 creatureId);

    bool Create(ObjectGuid::LowType guidlow, Map* map, uint32 entry, uint32 petNumber);

    // Synchronizes the pet's level with its summoner, updates the stats for the new level and learns new spells if available
    void SynchronizeLevelWithSummoner();
    // Returns true when the pet knows the given spell
    bool HasSpell(uint32 spellId) const override;
    // Enables or disables auto casting for the given spell
    void ToggleAutocast(SpellInfo const* spellInfo, bool apply);
    // Returns a map of a pet's known spells
    PetSpellMap const& GetSpells() const { return _spells; }
    // Sets the PlayerPetData map key for accessing the summoner's player pet data at any given time
    void SetPlayerPetDataKey(uint8 slot, uint32 creatureId);
    // Updates the provided pet data object with the pet's current data, such as action bars, talents and cooldowns. This method should be used whenever you have to store the data (like unsummoning and saving pets)
    void UpdatePlayerPetData(PlayerPetData* petData);
    // Learns the specified talent for the given rank and learns the according spell/passive aura
    void LearnTalent(uint32 talentId, uint32 rank);
    // Learns all talents that have been saved in the database
    void LoadTalents(std::string const& dataString);
    // Sends a pet talent info update packet to the summoner
    void SendTalentsInfoUpdateToSummoner();
    // Stores the declined names which have been retrieved in the CMSG_PET_RENAME packet to store them in database later 
    void SetDeclinedNames(DeclinedName&& declinedNames);
    // Returns a pointer to the stored declined names of the pet if there are any. Otherwise nullptr will be returned
    DeclinedName const* GetDeclinedNames() const { return _declinedNames.get(); }
    // Resets all talents that the pet has currently learned
    void ResetTalents();
    // Disables pets actions and toggles the command state to passive/follow
    void DisablePetActions(bool disable);
    // Returns the currently set pet mode flags
    EnumFlag<PetModeFlags> GetPetModeFlags() const { return _petModeFlags; }
    // Returns the number of auto-castable spells that the pet can use
    uint8 GetPetAutoSpellSize() const override { return _autoCastSpells.size(); }
    // Returns the spell Id that is stored within the _autoCastSpells vector at the given position
    uint32 GetPetAutoSpellOnPos(uint8 pos) const override;

private:
    void SendSpellLearnedToSummoner(uint32 spellId);
    void SendSpellUnlearnedToSummoner(uint32 spellId);
    void LearnAvailableSpellsForCurrentLevel();
    void ApplySavedAuras(PlayerPetData* petData);
    void StoreAppliedAuras(PlayerPetData* petData);
    bool LearnSpell(uint32 spellId);
    bool UnlearnSpell(uint32 spellId, bool learnPreviousRank);
    bool AddSpell(uint32 spellId, ActiveStates active = ACT_DECIDE, PetSpellState state = PETSPELL_NEW, PetSpellType type = PETSPELL_NORMAL);
    bool RemoveSpell(uint32 spellId, bool learnPreviousRank);
    std::string GenerateActionBarDataString() const;
    std::string GenenerateTalentsDataString() const;
    uint32 CalculateTalentPointsForLevel() const;
    uint32 GetSpentTalentPoints() const;

    bool _isClassPet;
    PetSpellMap _spells;
    Optional<PlayerPetDataKey> _playerPetDataKey;
    Optional<ReactStates> _disableActionsOriginalReactState;
    PetTalentMap _talents;
    std::unique_ptr<DeclinedName> _declinedNames;
    EnumFlag<PetModeFlags> _petModeFlags;
    std::vector<uint32 /*spellId*/> _autoCastSpells;
};

#endif // NewPet_h__
