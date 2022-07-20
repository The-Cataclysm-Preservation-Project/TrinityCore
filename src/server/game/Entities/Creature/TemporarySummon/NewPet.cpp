
#include "NewPet.h"
#include "CharmInfo.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "PetPackets.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "TalentPackets.h"

NewPet::NewPet(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isClassPet, uint32 creatureId, uint8 slot) :
    NewGuardian(properties, summoner, isWorldObject, false), _isClassPet(isClassPet), _petModeFlags(PetModeFlags::None)
{
    m_unitTypeMask |= UNIT_MASK_PET;
    InitCharmInfo();

    // Create or load player pet data for summons which can be controlled
    if (summoner && summoner->IsPlayer() && (IsClassPet() || properties->GetFlags().HasFlag(SummonPropertiesFlags::SavePetAutocast)))
    {
        Player* player = summoner->ToPlayer();
        if (PlayerPetData* playerPetData = GetOrCreatePlayerPetData(player, slot, creatureId))
            SetPlayerPetDataKey(slot, creatureId);
    }
}

void NewPet::AddToWorld()
{
    // Setting the guid before adding to world to reduce building unnecessary object update packets
    SetSummonerGUID(GetInternalSummonerGUID());

    NewGuardian::AddToWorld();

    if (Unit* summoner = GetInternalSummoner())
        summoner->SetActivelyControlledSummon(this, true);
}

void NewPet::RemoveFromWorld()
{
    if (Unit* summoner = GetInternalSummoner())
        summoner->SetActivelyControlledSummon(this, false);

    NewGuardian::RemoveFromWorld();
}

bool NewPet::HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons)
{
    if (!NewGuardian::HandlePreSummonActions(summoner, creatureLevel, maxSummons))
        return false;

    // Pets are initialized with REACT_ASSIST by default
    SetReactState(REACT_ASSIST);

    // Initialize the action bar after initializing the stats
    m_charmInfo->InitPetActionBar();
    LearnAvailableSpellsForCurrentLevel();

    // initialize pet behavior
    if (_playerPetDataKey.has_value() && summoner && summoner->IsPlayer())
    {
        PlayerPetData* playerPetData = summoner->ToPlayer()->GetPlayerPetData(_playerPetDataKey->first, _playerPetDataKey->second);
        if (!playerPetData)
            return false;

        playerPetData->DisplayId = GetNativeDisplayId();
        playerPetData->CreatedBySpellId = GetUInt32Value(UNIT_CREATED_BY_SPELL);

        GetCharmInfo()->SetPetNumber(playerPetData->PetNumber, IsClassPet());

        if (IsHunterPet())
            LoadTalents(playerPetData->Talents);

        GetCharmInfo()->LoadPetActionBar(playerPetData->ActionBar, this);

        for (auto const& itr : playerPetData->SpellStates)
        {
            if (!HasSpell(itr.SpellId))
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr.SpellId);
            if (!spellInfo)
                continue;

            if (itr.ActiveState == ACT_ENABLED || itr.ActiveState == ACT_DISABLED)
                ToggleAutocast(spellInfo, itr.ActiveState == ACT_ENABLED);
        }

        for (auto const& itr : playerPetData->Cooldowns)
            GetSpellHistory()->AddCooldown(itr.SpellId, 0, itr.CooldownEnd, itr.CategoryId, itr.CategoryEnd);

        if (IsClassPet())
        {
            SetName(playerPetData->Name);
            ApplySavedAuras(playerPetData);
            if (playerPetData->DeclinedNames.has_value())
                _declinedNames = std::make_unique<DeclinedName>(*playerPetData->DeclinedNames);
        }

        SetReactState(playerPetData->ReactState);
        if (playerPetData->Status != PlayerPetDataStatus::New)
        {
            if (IsHunterPet())
            {
                if (playerPetData->SavedHealth != 0)
                    SetHealth(playerPetData->SavedHealth);
                else
                    setDeathState(JUST_DIED);
            }
            else
                SetFullHealth();
        }
        else
            SetFullHealth();

        if (IsHunterPet())
        {
            // @todo: This is a workarround for us not properly supporting permanent pets yet. Hunter pet's are meant to last permanently until resurrected or the player leaves the world.
            SetCorpseDelay(DAY);

            // Hunter pets have some special settings
            SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_ABANDONED);
            if (!playerPetData->HasBeenRenamed)
                SetByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED);
        }

        if (summoner->IsMounted())
            DisablePetActions(true);
    }
    else
        SetFullHealth();

    // Set the pet's visbility distance to normal (100yds) which is the threshold when a pet is being dismissed due to being too far away
    SetVisibilityDistanceOverride(VisibilityDistanceType::Normal);

    return true;
}

void NewPet::HandlePostSummonActions()
{
    NewGuardian::HandlePostSummonActions();

    if (IsHunterPet())
    {
        SendTalentsInfoUpdateToSummoner();
        CastSpell(nullptr, SPELL_PET_ENERGIZE);
    }
    else
        CastSpell(nullptr, SPELL_SUMMON_HEAL);
}

bool NewPet::CanBeDismissed() const
{
    if (_summonProperties && _summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::CannotDismissPet))
        return false;

    return true;
}

void NewPet::Dismiss(bool permanent /*= true*/)
{
    if (IsClassPet() || _playerPetDataKey.has_value())
    {
        Unit* summoner = GetInternalSummoner();
        if (summoner && summoner->IsPlayer())
        {
            Player* player = summoner->ToPlayer();
            // Update the pet data before unsummoning the pet
            if (_playerPetDataKey.has_value())
                UpdatePlayerPetData(player->GetPlayerPetData(_playerPetDataKey->first, _playerPetDataKey->second));

            // If the pet has been despawned while dead, we will mark the pet as inactive
            if (IsClassPet())
            {
                if (permanent)
                    player->SetActiveClassPetDataKey(std::nullopt);
                else
                    player->SetActiveClassPetDataKey(_playerPetDataKey);
            }

            // Warlock pets do play dismiss sounds when releasing them permanently
            if (IsClassPet() && permanent && player->getClass() == CLASS_WARLOCK)
            {
                if (CreatureDisplayInfoEntry const* creatureDisplay = sCreatureDisplayInfoStore.LookupEntry(GetDisplayId()))
                {
                    WorldPackets::Pet::PetDismissSound dismissSound;
                    dismissSound.ModelID = creatureDisplay->ModelID;
                    dismissSound.ModelPosition = GetPosition();
                    player->SendDirectMessage(dismissSound.Write());
                }
            }
        }
    }

    NewGuardian::Unsummon();
}

void NewPet::Unsummon(Milliseconds timeUntilDespawn /*= 0ms*/)
{
    if (timeUntilDespawn > 0ms)
    {
        m_Events.AddEventAtOffset([&]() { Unsummon(); }, timeUntilDespawn);
        return;
    }

    Dismiss();
}

PlayerPetData* NewPet::GetOrCreatePlayerPetData(Player* summoner, uint8 slot, uint32 creatureId)
{
    if (!summoner)
        return nullptr;

    PlayerPetData* petData = summoner->GetPlayerPetData(slot, creatureId);
    if (!petData && creatureId) // only create new pet data for non-hunter pets. Hunter pets do not have a creatureId so consequently we do not want to use it here.
        petData = summoner->CreatePlayerPetData(slot, creatureId);

    return petData;
}

bool NewPet::Create(ObjectGuid::LowType guidlow, Map* map, uint32 entry, uint32 petNumber)
{
    ASSERT(map);
    SetMap(map);

    Object::_Create(guidlow, petNumber, HighGuid::Pet);

    m_spawnId = guidlow;
    m_originalEntry = entry;

    if (!InitEntry(entry))
        return false;

    // Force regen flag for player pets, just like we do for players themselves
    SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
    SetSheath(SHEATH_STATE_MELEE);
    SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
    SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, GameTime::GetGameTime());

    // Patch 4.1.0 (2011-04-26): Pets will now level with hunters in the same way warlock pets currently do.
    SetInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
    SetInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, -1);

    GetThreatManager().Initialize();

    return true;
}

void NewPet::SynchronizeLevelWithSummoner()
{
    if (!IsClassPet())
        return;

    Unit* summoner = GetInternalSummoner();
    if (!summoner || getLevel() == summoner->getLevel())
        return;

    SetLevel(summoner->getLevel());
    InitializeStats();

    LearnAvailableSpellsForCurrentLevel();

    if (IsHunterPet())
        SendTalentsInfoUpdateToSummoner();
}

bool NewPet::HasSpell(uint32 spellId) const
{
    // Class pets use a own spell container which is being updated during runtime
    auto itr = _spells.find(spellId);
    return itr != _spells.end() && itr->second.State != PETSPELL_REMOVED;
}

void NewPet::ToggleAutocast(SpellInfo const* spellInfo, bool apply)
{
    if (!spellInfo || !spellInfo->IsAutocastable())
        return;

    auto itr = _spells.find(spellInfo->Id);
    if (itr == _spells.end())
        return;

    if (apply)
    {
        auto it = std::find(_autoCastSpells.begin(), _autoCastSpells.end(), spellInfo->Id);
        if (it == _autoCastSpells.end())
            _autoCastSpells.push_back(spellInfo->Id);

        if (itr->second.Active != ACT_ENABLED)
        {
            itr->second.Active = ACT_ENABLED;
            if (itr->second.State != PETSPELL_NEW)
                itr->second.State = PETSPELL_CHANGED;
        }
    }
    else
    {
        auto it = std::find(_autoCastSpells.begin(), _autoCastSpells.end(), spellInfo->Id);
        if (it != _autoCastSpells.end())
            _autoCastSpells.erase(it);

        if (itr->second.Active != ACT_DISABLED)
        {
            itr->second.Active = ACT_DISABLED;
            if (itr->second.State != PETSPELL_NEW)
                itr->second.State = PETSPELL_CHANGED;
        }
    }
}

void NewPet::SetPlayerPetDataKey(uint8 slot, uint32 creatureId)
{
    _playerPetDataKey = std::make_pair(slot, creatureId);
}

void NewPet::UpdatePlayerPetData(PlayerPetData* petData)
{
    if (!petData)
        return;

    petData->SavedHealth = GetHealth();
    petData->SavedPower = GetPower(GetPowerType());
    petData->LastSaveTime = static_cast<uint32>(GameTime::GetGameTime());
    petData->ReactState = _disableActionsOriginalReactState.value_or(GetReactState());
    petData->Slot = _playerPetDataKey->first;
    petData->Name = GetName();
    petData->ActionBar = GenerateActionBarDataString();
    petData->Talents = GenenerateTalentsDataString();
    if (_declinedNames)
        petData->DeclinedNames = *_declinedNames.get();
    else
        petData->DeclinedNames.reset();

    if (IsHunterPet())
        petData->HasBeenRenamed = !HasByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED);

    if (IsClassPet())
    {
        GetSpellHistory()->StoreSpellHistoryEntries(petData->Cooldowns);
        StoreAppliedAuras(petData);
    }

    petData->SpellStates.clear();
    for (auto const& itr : _spells)
    {
        if (itr.second.Active == ACT_ENABLED || itr.second.Active == ACT_DISABLED)
        {
            PlayerPetDataSpellState& spellState = petData->SpellStates.emplace_back();
            spellState.SpellId = itr.first;
            spellState.ActiveState = itr.second.Active;
        }
    }

    if (petData->Status != PlayerPetDataStatus::New)
        petData->Status = PlayerPetDataStatus::Changed;
}

void NewPet::LearnTalent(uint32 talentId, uint32 rank)
{
    if (rank >= MAX_TALENT_RANK)
        return;

    TalentEntry const* talentEntry = sTalentStore.LookupEntry(talentId);
    if (!talentEntry)
        return;

    TalentTabEntry const* talentTabEntry = sTalentTabStore.LookupEntry(talentEntry->TabID);
    if (!talentTabEntry || talentTabEntry->ClassMask != 0)
        return;

    _talents[talentId] = rank;

    LearnSpell(talentEntry->SpellRank[rank]);
}

void NewPet::LoadTalents(std::string const& dataString)
{
    if (dataString.empty())
        return;

    Tokenizer tokens(dataString, ' ');

    // Talents are stored in pairs (Id and rank) so ensure that there is always a pair
    if (tokens.size() == 0 || (tokens.size() % 2) != 0)
        return;

    for (Tokenizer::const_iterator itr = tokens.begin(); itr != tokens.end();)
    {
        uint32 talentId = atoi(*itr);
        ++itr;
        uint32 rank = atoi(*itr);

        if (rank <= MAX_PET_TALENT_RANK)
            LearnTalent(talentId, rank);

        ++itr;
    }
}

void NewPet::SendTalentsInfoUpdateToSummoner()
{
    Unit const* summoner = GetInternalSummoner();
    if (!summoner || !summoner->IsPlayer())
        return;

    WorldPackets::Talent::TalentInfoUpdate packet;
    packet.PetTalents = true;
    packet.UnspentPoints = CalculateTalentPointsForLevel() - GetSpentTalentPoints();
    for (auto const& learnedTalents : _talents)
    {
        WorldPackets::Talent::TalentInfo& talent = packet.PetTalent.emplace_back();
        talent.TalentID = learnedTalents.first;
        talent.Rank = learnedTalents.second;
    }

    summoner->ToPlayer()->SendDirectMessage(packet.Write());
}

void NewPet::SetDeclinedNames(DeclinedName&& declinedNames)
{
    _declinedNames = std::make_unique<DeclinedName>(std::move(declinedNames));
}

void NewPet::ResetTalents()
{
    if (_talents.empty())
        return;

    for (auto const& pair : _talents)
    {
        TalentEntry const* talentEntry = sTalentStore.LookupEntry(pair.first);
        if (!talentEntry)
            return;

        UnlearnSpell(talentEntry->SpellRank[pair.second], false);
    }

    if (Unit* summoner = GetInternalSummoner())
        if (Player* player = summoner->ToPlayer())
            player->SendPetSpellsMessage(this);

    _talents.clear();
    SendTalentsInfoUpdateToSummoner();
}

void NewPet::DisablePetActions(bool disable)
{
    if (_petModeFlags.HasFlag(PetModeFlags::DisableActions) == disable)
        return;

    if (disable)
        _petModeFlags |= PetModeFlags::DisableActions;
    else
        _petModeFlags &= ~PetModeFlags::DisableActions;

    ASSERT(GetCharmInfo());

    // Store the original react state before we change it
    if (disable)
        _disableActionsOriginalReactState = GetReactState();
    else
        if (_disableActionsOriginalReactState.has_value())
            SetReactState(*_disableActionsOriginalReactState);

    if (IsInWorld())
    {
        WorldPackets::Pet::SPetMode packet;
        packet.PetGUID = GetGUID();
        packet.CommandState = GetCharmInfo()->GetCommandState();
        packet.ReactState = GetReactState();
        packet.Flag = _petModeFlags.AsUnderlyingType();
        if (Unit const* summoner = GetInternalSummoner())
            if (Player const* player = summoner->ToPlayer())
                player->SendDirectMessage(packet.Write());
    }

    if (disable)
    {
        SetReactState(REACT_PASSIVE);
        GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);
    }

}

uint32 NewPet::GetPetAutoSpellOnPos(uint8 pos) const
{
    if (pos >= _autoCastSpells.size())
        return 0;
    else
        return _autoCastSpells[pos];
}

// ------------- private methods

void NewPet::SendSpellLearnedToSummoner(uint32 spellId)
{
    Unit const* summoner = GetInternalSummoner();
    if (!summoner || !summoner->IsPlayer())
        return;

    summoner->ToPlayer()->SendDirectMessage(WorldPackets::Pet::PetLearnedSpell(spellId).Write());
}

void NewPet::SendSpellUnlearnedToSummoner(uint32 spellId)
{
    Unit const* summoner = GetInternalSummoner();
    if (!summoner || !summoner->IsPlayer())
        return;

    summoner->ToPlayer()->SendDirectMessage(WorldPackets::Pet::PetUnlearnedSpell(spellId).Write());
}

void NewPet::LearnAvailableSpellsForCurrentLevel()
{
    if (!IsClassPet())
    {
        // Non-class pets do have their spells stored in creature_template so we draw our data from there
        for (uint32 spell : m_spells)
            if (spell != 0)
                LearnSpell(spell);
        return;
    }

    if (PetLevelupSpellSet const* levelupSpells = GetCreatureTemplate()->family ? sSpellMgr->GetPetLevelupSpellList(GetCreatureTemplate()->family) : nullptr)
    {
        // PetLevelupSpellSet ordered by levels
        for (auto const& itr : *levelupSpells)
        {
            if (itr.first > getLevel())
                UnlearnSpell(itr.second, true);
            else
                LearnSpell(itr.second);
        }
    }

    int32 petSpellsId = GetCreatureTemplate()->PetSpellDataId ? -(int32)GetCreatureTemplate()->PetSpellDataId : GetEntry();

    // default spells
    if (PetDefaultSpellsEntry const* defSpells = sSpellMgr->GetPetDefaultSpellsEntry(petSpellsId))
    {
        for (uint32 spellId : defSpells->spellid)
        {
            if (!spellId)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (!spellInfo)
                continue;

            if (spellInfo->SpellLevel > getLevel())
                UnlearnSpell(spellInfo->Id, true);
            else
                LearnSpell(spellInfo->Id);
        }
    }

    if (Unit* summoner = GetInternalSummoner())
        if (Player* player = summoner->ToPlayer())
            player->SendPetSpellsMessage(this);
}

void NewPet::ApplySavedAuras(PlayerPetData* petData)
{
    uint32 timediff = uint32(GameTime::GetGameTime() - petData->LastSaveTime);

    for (PlayerPetDataAura& petAura : petData->Auras)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(petAura.SpellId);
        if (!spellInfo) // skip non-existing/outdated auras. They will be pruned from DB at the next save cycle
            continue;

        int32 remainingDuration = petAura.RemainingDuration;
        // negative effects should continue counting down after logout
        if (remainingDuration != -1 && (!spellInfo->IsPositive() || spellInfo->HasAttribute(SPELL_ATTR4_AURA_EXPIRES_OFFLINE)))
        {
            if (remainingDuration / IN_MILLISECONDS <= int32(timediff))
                continue;

            remainingDuration -= timediff * IN_MILLISECONDS;
        }

        uint8 remainingCharges = petAura.RemainingCharges;
        // prevent wrong values of remaincharges
        if (spellInfo->ProcCharges)
        {
            if (remainingCharges <= 0 || remainingCharges > spellInfo->ProcCharges)
                remainingCharges = spellInfo->ProcCharges;
        }
        else
            remainingCharges = 0;

        AuraCreateInfo createInfo(spellInfo, petAura.EffectMask, this);
        createInfo
            .SetCasterGUID(petAura.CasterGuid.IsEmpty() ? GetGUID() : petAura.CasterGuid)
            .SetBaseAmount(petAura.BaseAmount.data());

        if (Aura* aura = Aura::TryCreate(createInfo))
        {
            if (!aura->CanBeSaved())
            {
                aura->Remove();
                continue;
            }

            aura->SetLoadedState(petAura.MaxDuration, remainingDuration, remainingCharges, petAura.StackAmount, petAura.RecalculateMask, petAura.CritChance, petAura.ApplyResilience, petAura.Amount.data());
            aura->ApplyForTargets();
        }
    }
}

void NewPet::StoreAppliedAuras(PlayerPetData* petData)
{
    petData->Auras.clear();

    for (auto const& pair : m_ownedAuras)
    {
        Aura* aura = pair.second;
        if (!aura->CanBeSaved())
            continue;

        PlayerPetDataAura& petAura = petData->Auras.emplace_back();

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (aura->GetEffect(i))
            {
                petAura.BaseAmount[i] = aura->GetEffect(i)->GetBaseAmount();
                petAura.Amount[i] = aura->GetEffect(i)->GetAmount();
                petAura.EffectMask |= (1 << i);
                if (aura->GetEffect(i)->CanBeRecalculated())
                    petAura.RecalculateMask |= (1 << i);
            }
        }

        // Store auras that have been casted by ourselves as empty because when we are summoned the next time, we will have a different ObjectGuid
        petAura.CasterGuid = aura->GetCasterGUID() == GetGUID() ? ObjectGuid::Empty : aura->GetCasterGUID();
        petAura.SpellId = aura->GetId();
        petAura.StackAmount = aura->GetStackAmount();
        petAura.RemainingCharges = aura->GetCharges();
        petAura.MaxDuration = aura->GetMaxDuration();
        petAura.RemainingDuration = aura->GetDuration();
        petAura.CritChance = aura->GetCritChance();
        petAura.ApplyResilience = aura->CanApplyResilience();
    }
}

bool NewPet::LearnSpell(uint32 spellId)
{
    if (!AddSpell(spellId))
        return false;

    if (IsInWorld())
        SendSpellLearnedToSummoner(spellId);

    return true;
}

bool NewPet::UnlearnSpell(uint32 spellId, bool learnPreviousRank)
{
    if (!RemoveSpell(spellId, learnPreviousRank))
        return false;

    if (IsInWorld())
        SendSpellUnlearnedToSummoner(spellId);

    return true;
}

bool NewPet::AddSpell(uint32 spellId, ActiveStates active /*= ACT_DECIDE*/, PetSpellState state /*= PETSPELL_NEW*/, PetSpellType type /*= PETSPELL_NORMAL*/)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return false;

    auto const itr = _spells.find(spellId);
    if (itr != _spells.end())
    {
        if (itr->second.State == PETSPELL_REMOVED)
            state = PETSPELL_CHANGED;
        else
        {
            if (state == PETSPELL_UNCHANGED && itr->second.State != PETSPELL_UNCHANGED)
            {
                // can be in case spell loading but learned at some previous spell loading
                itr->second.State = PETSPELL_UNCHANGED;

                if (active == ACT_ENABLED)
                    ToggleAutocast(spellInfo, true);
                else if (active == ACT_DISABLED)
                    ToggleAutocast(spellInfo, false);
            }

            return false;
        }
    }

    PetSpell newspell;
    newspell.State = state;
    newspell.Type = type;

    if (active == ACT_DECIDE) // active was not used before, so we save it's autocast/passive state here
    {
        if (spellInfo->IsAutocastable())
            newspell.Active = ACT_ENABLED;
        else
            newspell.Active = ACT_PASSIVE;
    }
    else
        newspell.Active = active;

    // talent: unlearn all other talent ranks (high and low)
    if (TalentSpellPos const* talentPos = sDBCManager.GetTalentSpellPos(spellId))
    {
        if (TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentPos->talent_id))
        {
            for (uint8 i = 0; i < MAX_TALENT_RANK; ++i)
            {
                // skip learning spell and no rank spell case
                uint32 rankSpellId = talentInfo->SpellRank[i];
                if (!rankSpellId || rankSpellId == spellId)
                    continue;

                // skip unknown ranks
                if (!HasSpell(rankSpellId))
                    continue;
                RemoveSpell(rankSpellId, false);
            }
        }
    }
    else if (spellInfo->IsRanked())
    {
        for (auto const& itr : _spells)
        {
            if (itr.second.State == PETSPELL_REMOVED)
                continue;

            SpellInfo const* oldRankSpellInfo = sSpellMgr->GetSpellInfo(itr.first);

            if (!oldRankSpellInfo)
                continue;

            if (spellInfo->IsDifferentRankOf(oldRankSpellInfo))
            {
                // replace by new high rank
                if (spellInfo->IsHighRankOf(oldRankSpellInfo))
                {
                    newspell.Active = itr.second.Active;

                    if (newspell.Active == ACT_ENABLED)
                        ToggleAutocast(oldRankSpellInfo, false);

                    UnlearnSpell(itr.first, false);
                    break;
                }
                // ignore new lesser rank
                else
                    return false;
            }
        }
    }

    _spells[spellId] = newspell;

    if (spellInfo->IsPassive() && (!spellInfo->CasterAuraState || HasAuraState(AuraStateType(spellInfo->CasterAuraState))))
        CastSpell(this, spellId, true);
    else if (!spellInfo->HasAttribute(SPELL_ATTR4_NOT_IN_SPELLBOOK))
        m_charmInfo->AddSpellToActionBar(spellInfo);

    if (newspell.Active == ACT_ENABLED)
        ToggleAutocast(spellInfo, true);

    return true;
}

bool NewPet::RemoveSpell(uint32 spellId, bool learnPreviousRank)
{
    auto itr = _spells.find(spellId);
    if (itr == _spells.end())
        return false;

    if (itr->second.State == PETSPELL_REMOVED)
        return false;

    if (itr->second.State == PETSPELL_NEW)
        _spells.erase(itr);
    else
        itr->second.State = PETSPELL_REMOVED;

    RemoveAurasDueToSpell(spellId);

    if (learnPreviousRank)
    {
        if (uint32 prevSpellId = sSpellMgr->GetPrevSpellInChain(spellId))
            LearnSpell(prevSpellId);
        else
            learnPreviousRank = false;
    }

    return true;
}

std::string NewPet::GenerateActionBarDataString() const
{
    std::ostringstream ss;

    for (uint32 i = ACTION_BAR_INDEX_START; i < ACTION_BAR_INDEX_END; ++i)
    {
        ss << uint32(m_charmInfo->GetActionBarEntry(i)->GetType()) << ' '
            << uint32(m_charmInfo->GetActionBarEntry(i)->GetAction()) << ' ';
    }

    return ss.str();
}

std::string NewPet::GenenerateTalentsDataString() const
{
    std::ostringstream ss;

    for (auto const& talent : _talents)
        ss << uint32(talent.first) << ' ' << uint32(talent.second) << ' ';

    return ss.str();
}

uint32 NewPet::CalculateTalentPointsForLevel() const
{
    uint32 points = 0;
    if (getLevel() >= 20)
        points = 1 + ((getLevel() - 20) / 4);

    if (Unit* summoner = GetInternalSummoner())
        points += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_PET_TALENT_POINTS);

    return points;
}

uint32 NewPet::GetSpentTalentPoints() const
{
    uint32 points = 0;
    for (auto const& pair : _talents)
        points += pair.second + 1;

    return points;
}
