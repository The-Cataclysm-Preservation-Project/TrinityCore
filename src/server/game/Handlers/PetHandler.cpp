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

#include "WorldSession.h"
#include "CharmInfo.h"
#include "Common.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "Group.h"
#include "Log.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "NewPet.h"
#include "PetPackets.h"
#include "PetAI.h"
#include "Player.h"
#include "QueryPackets.h"
#include "Spell.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "TalentPackets.h"
#include "Util.h"
#include "WorldPacket.h"

void WorldSession::HandleDismissCritter(WorldPackets::Pet::DismissCritter& packet)
{
    if (_player->GetCritterGUID() != packet.CritterGUID)
        return;

    if (NewTemporarySummon* summon = _player->GetSummonInSlot(SummonPropertiesSlot::Critter))
        if (summon->GetGUID() == packet.CritterGUID)
            summon->Unsummon();
}

inline void SendPetActionFeedbackToPlayer(Player* player, ActionFeedback response, uint32 spellId)
{
    WorldPackets::Pet::PetActionFeedback actionFeedback;
    actionFeedback.Response = response;
    actionFeedback.SpellID = spellId;
    player->SendDirectMessage(actionFeedback.Write());
}

inline void SendPetActionSoundToPlayer(Player* player, ObjectGuid const& petGuid, PetTalk response)
{
    WorldPackets::Pet::PetActionSound actionSound;
    actionSound.Action = response;
    actionSound.UnitGUID = petGuid;
    player->SendDirectMessage(actionSound.Write());
}

void HandlePetActionHelper(NewPet* pet, Player* owner, ObjectGuid targetGuid, uint32 actionValue, uint16 actionFlag, Position const& actionPosition)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
        return;

    switch (actionFlag)
    {
        case ACT_COMMAND:
            switch (actionValue)
            {
                case COMMAND_STAY:
                    charmInfo->SetCommandState(COMMAND_STAY);
                    break;
                case COMMAND_FOLLOW:
                    charmInfo->SetCommandState(COMMAND_FOLLOW);
                    break;
                case COMMAND_ATTACK:
                    if (Unit* target = ObjectAccessor::GetUnit(*pet, targetGuid))
                        charmInfo->SetCommandState(COMMAND_ATTACK, target);
                    break;
                case COMMAND_ABANDON:
                    // Despite its enum name, abandoning pets is done via extra Opcode. This here is merely dismissing pets
                    if (pet->CanBeDismissed())
                        pet->Dismiss();
                    break;
                case COMMAND_MOVE_TO:
                    charmInfo->SetCommandState(COMMAND_MOVE_TO, nullptr, &actionPosition);
                    break;
                default:
                    break;
            }
            break;
        case ACT_REACTION:
            switch (actionValue)
            {
                case REACT_PASSIVE:
                case REACT_DEFENSIVE:
                case REACT_ASSIST:
                    pet->SetReactState(static_cast<ReactStates>(actionValue));
                    break;
                // case REACT_AGGRESSIVE: // no longer available since 4.x
                default:
                    break;
            }
            break;
        case ACT_DISABLED:
        case ACT_PASSIVE:
        case ACT_ENABLED:
        {
            if (!pet->IsAlive())
            {
                SendPetActionFeedbackToPlayer(owner, FEEDBACK_PET_DEAD, 0);
                return;
            }

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(actionValue);
            if (!spellInfo)
                return;

            if (!pet->HasSpell(actionValue) || spellInfo->IsPassive())
                return;

            Unit* spellTarget = nullptr;

            if (targetGuid)
                spellTarget = ObjectAccessor::GetUnit(*owner, targetGuid);


            Spell* spell = new Spell(pet, spellInfo, TRIGGERED_NONE);
            SpellCastResult result = spell->CheckPetCast(spellTarget);

            if (result == SPELL_FAILED_BAD_IMPLICIT_TARGETS)
            {
                SendPetActionFeedbackToPlayer(owner, FEEDBACK_NOTHING_TO_ATT, actionValue);
                return;
            }

            if (result == SPELL_CAST_OK)
            {
                if (pet->IsClassPet() && actionFlag == ACT_PASSIVE)
                    SendPetActionSoundToPlayer(owner, pet->GetGUID(), PET_TALK_SPECIAL_SPELL);

                result = spell->prepare(spell->m_targets);
                if (result == SPELL_FAILED_NOPATH)
                    SendPetActionFeedbackToPlayer(owner, FEEBDACK_CHARGE_HAS_NO_PATH, actionValue);
                else if (result != SPELL_CAST_OK)
                    SendPetActionFeedbackToPlayer(owner, FEEDBACK_CANT_ATT_TARGET, actionValue);
            }
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandlePetAction(WorldPackets::Pet::PetAction& packet)
{
    if (_player->IsMounted())
        return;

    if (packet.PetGUID.IsEmpty() || packet.Action == 0)
        return;

    uint32 actionValue = packet.Action & 0xFFFFFF;
    uint8 actionFlags = (packet.Action >> 24) & 0xFF;

    NewTemporarySummon* pet = _player->GetSummonByGUID(packet.PetGUID);
    if (!pet || !pet->IsPet())
        return;

    // There are pet summons that consist of more than a single summon (Force of Nature for example) so we make sure that all related summons perform the action
    std::vector<NewPet*> pets;
    pets.push_back(pet->ToNewPet());
    for (ObjectGuid const& guid : _player->GetSummonGUIDs())
    {
        NewTemporarySummon* summon = _player->GetSummonByGUID(guid);
        if (!summon || !summon->IsPet() || summon == pet || summon->GetUInt32Value(UNIT_CREATED_BY_SPELL) != pet->GetUInt32Value(UNIT_CREATED_BY_SPELL))
            continue;

        pets.push_back(summon->ToNewPet());
    }

    for (NewPet* actionPet : pets)
        HandlePetActionHelper(actionPet, _player, packet.TargetGUID, actionValue, actionFlags, packet.ActionPosition);
}

void WorldSession::HandlePetStopAttack(WorldPackets::Pet::PetStopAttack& packet)
{
    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != packet.PetGUID || !pet->IsAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandlePetNameQuery(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network.opcode", "WORLD: Received CMSG_PET_NAME_QUERY");

    uint32 petnumber;
    ObjectGuid petguid;

    recvData >> petnumber;
    recvData >> petguid;

    SendPetNameQuery(petguid, petnumber);
}

void WorldSession::SendPetNameQuery(ObjectGuid petGuid, uint32 petNumber)
{
    WorldPackets::Query::QueryPetNameResponse packet;
    packet.PetID = petNumber;

    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != petGuid || pet->GetUInt32Value(UNIT_FIELD_PETNUMBER) != petNumber)
    {
        _player->SendDirectMessage(packet.Write());
        return;
    }

    packet.Timestamp = pet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);
    packet.Name = pet->GetName();
    if (DeclinedName const* declinedNames = pet->GetDeclinedNames())
    {
        packet.HasDeclined = true;
        packet.DeclinedNames = *declinedNames;
    }

    _player->SendDirectMessage(packet.Write());
}

bool WorldSession::CheckStableMaster(ObjectGuid guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetGUID())
    {
        if (!GetPlayer()->IsGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            TC_LOG_DEBUG("entities.player.cheat", "%s attempt open stable in cheating way.", guid.ToString().c_str());
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            TC_LOG_DEBUG("entities.player", "Stablemaster %s not found or you can't interact with him.", guid.ToString().c_str());
            return false;
        }
    }
    return true;
}

void WorldSession::HandlePetSetAction(WorldPackets::Pet::PetSetAction& packet)
{
    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != packet.PetGUID)
        return;

    std::vector<NewPet*> pets = { pet };
    for (ObjectGuid const& guid : _player->GetSummonGUIDs())
    {
        if (guid == packet.PetGUID)
            continue;

        NewTemporarySummon* summon = _player->GetSummonByGUID(guid);
        if (!summon || !summon->IsPet() || summon == pet || summon->GetUInt32Value(UNIT_CREATED_BY_SPELL) != pet->GetUInt32Value(UNIT_CREATED_BY_SPELL))
            continue;

        pets.push_back(summon->ToNewPet());
    }

    uint32 position = packet.Index;
    uint32 actionData = packet.Action;

    uint32 spell_id = UNIT_ACTION_BUTTON_ACTION(actionData);
    uint8 act_state = UNIT_ACTION_BUTTON_TYPE(actionData);

    for (NewPet* petControlled : pets)
    {
        CharmInfo* charmInfo = petControlled->GetCharmInfo();
        if (!charmInfo)
        {
            TC_LOG_ERROR("entities.pet", "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", petControlled->GetGUID().GetCounter(), petControlled->GetTypeId());
            continue;
        }

        //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
        if (!((act_state == ACT_ENABLED || act_state == ACT_DISABLED || act_state == ACT_PASSIVE) && spell_id && !petControlled->HasSpell(spell_id)))
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id))
            {
                //sign for autocast
                if (act_state == ACT_ENABLED)
                    petControlled->ToggleAutocast(spellInfo, true);
                //sign for no/turn off autocast
                else if (act_state == ACT_DISABLED)
                    petControlled->ToggleAutocast(spellInfo, false);
            }

            charmInfo->SetActionBar(position, spell_id, ActiveStates(act_state));
        }
    }
}

void WorldSession::HandlePetRename(WorldPackets::Pet::PetRename& packet)
{
    if (packet.RenameData.PetGUID.IsEmpty())
        return;

    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || !pet->IsHunterPet() || pet->GetGUID() != packet.RenameData.PetGUID || !pet->HasByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED))
        return;

    PetNameInvalidReason res = ObjectMgr::CheckPetName(packet.RenameData.NewName, GetSessionDbcLocale());
    if (res != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(res, packet.RenameData);
        return;
    }

    if (sObjectMgr->IsReservedName(packet.RenameData.NewName))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, packet.RenameData);
        return;
    }

    pet->SetName(packet.RenameData.NewName);
    pet->RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED);

    if (packet.RenameData.HasDeclinedNames)
    {
        std::wstring wname;
        if (!Utf8toWStr(packet.RenameData.NewName, wname))
            return;

        if (!ObjectMgr::CheckDeclinedNames(wname, packet.RenameData.DeclinedNames))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, packet.RenameData);
            return;
        }
    }

    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, static_cast<uint32>(GameTime::GetGameTime()));
    pet->SetDeclinedNames(std::move(packet.RenameData.DeclinedNames));
}

void WorldSession::HandlePetAbandon(WorldPackets::Pet::PetAbandon& packet)
{
    if (packet.Pet.IsEmpty() || !packet.Pet.IsPet() || !_player->IsInWorld())
        return;

    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != packet.Pet)
        return;

    _player->AbandonPet();
}

void WorldSession::HandlePetSpellAutocastOpcode(WorldPackets::Pet::PetSpellAutocast& packet)
{
    if (packet.PetGUID.IsEmpty() || packet.PetGUID.IsPlayer())
        return;

    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != packet.PetGUID)
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.SpellID);
    // do not add not learned spells/ passive spells
    if (!spellInfo || spellInfo->IsAutocastable() || !pet->HasSpell(spellInfo->Id))
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
        return;

    pet->ToggleAutocast(spellInfo, packet.AutocastEnabled);
    charmInfo->SetSpellAutocast(spellInfo, packet.AutocastEnabled);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network.opcode", "WORLD: Received CMSG_PET_CAST_SPELL");

    ObjectGuid guid;
    uint8  castCount;
    uint32 spellId;
    uint8  castFlags;

    recvPacket >> guid >> castCount >> spellId >> castFlags;

    TC_LOG_DEBUG("entities.pet", "WORLD: CMSG_PET_CAST_SPELL, %s, castCount: %u, spellId %u, castFlags %u", guid.ToString().c_str(), castCount, spellId, castFlags);

    Unit* caster = ObjectAccessor::GetUnit(*_player, guid);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        TC_LOG_ERROR("spells.pet", "WORLD: unknown PET spell id %i", spellId);
        return;
    }

    // do not cast not learned spells
    if (!caster->HasSpell(spellId) || spellInfo->IsPassive())
        return;

    SpellCastTargets targets;
    targets.Read(recvPacket, caster);
    HandleClientCastFlags(recvPacket, castFlags, targets);

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE);
    spell->m_cast_count = castCount;                    // probably pending spell cast
    spell->m_targets = targets;

    SpellCastResult result = spell->CheckPetCast(nullptr);

    if (result == SPELL_CAST_OK)
    {
        if (Creature* creature = caster->ToCreature())
        {
            if (Pet* pet = creature->ToPet())
            {
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetActionSound(PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(guid);
            }
        }

        spell->prepare(spell->m_targets);
    }
    else
    {
        spell->SendPetCastResult(result);

        if (!caster->GetSpellHistory()->HasCooldown(spellId))
            caster->GetSpellHistory()->ResetCooldown(spellId, true);

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::SendPetNameInvalid(uint32 error, WorldPackets::Pet::PetRenameData const& renameData)
{
    WorldPackets::Pet::PetNameInvalid packet;
    packet.Result = error;
    packet.RenameData = renameData;
    SendPacket(packet.Write());
}

void WorldSession::HandlePetLearnTalent(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 talentId, requestedRank;
    recvData >> guid >> talentId >> requestedRank;

    if (NewPet* pet = _player->GetActivelyControlledSummon())
        if (pet->GetGUID() == guid)
            pet->LearnTalent(talentId, requestedRank);
}

void WorldSession::HandleLearnPreviewTalentsPet(WorldPackets::Talent::LeanPreviewTalentsPet& packet)
{
    NewPet* pet = _player->GetActivelyControlledSummon();
    if (!pet || pet->GetGUID() != packet.PetGUID)
        return;

    for (WorldPackets::Talent::TalentInfo& talent : packet.Talents)
        pet->LearnTalent(talent.TalentID, talent.Rank);

    pet->SendTalentsInfoUpdateToSummoner();
    // After learning talents, we have to send out another pet spells message to show the added spells
    _player->SendPetSpellsMessage(pet);
}

void WorldSession::SendPetAdded(int32 petSlot, int32 petNumber, int32 creatureID, int32 level, std::string name)
{
    WorldPackets::Pet::PetAdded petAdded;
    petAdded.Level = level;
    petAdded.PetSlot = petSlot;
    petAdded.Flags = PET_STABLE_ACTIVE | (petSlot > PET_SLOT_LAST_ACTIVE_SLOT ? PET_STABLE_INACTIVE : 0);
    petAdded.CreatureID = creatureID;
    petAdded.Name = name;
    petAdded.PetNumber = petNumber;

    SendPacket(petAdded.Write());
}

void WorldSession::HandleRequestPetInfoOpcode(WorldPacket& /*recvData*/)
{
    /* Handle the packet CMSG_REQUEST_PET_INFO - Send when player has a pet, and does /reload
     * From: https://github.com/TrinityCore/TrinityCore/commit/6ebe1afeeccb847702c12e522bddaa7b5694dc38#comments
     * This packet is also sent when
     *     You're controlling a vehicle (Player::VehicleSpellInitialize)
     *     You have a unit charmed (Player::CharmSpellInitialize)
     *     You're possessing a unit (Player::PossessSpellInitialize)
     */

    if (NewPet* pet = _player->GetActivelyControlledSummon())
        _player->SendPetSpellsMessage(pet);

    if (Unit* charm = _player->GetCharmed())
    {
        if (charm->HasUnitState(UNIT_STATE_POSSESSED))
            _player->PossessSpellInitialize();
        else if (charm->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
            _player->VehicleSpellInitialize();
        else
            _player->CharmSpellInitialize();
    }
}

void WorldSession::HandleListStabledPetsOpcode(WorldPackets::Pet::CPetStableList& packet)
{
    if (!CheckStableMaster(packet.StableMaster))
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // remove mounts this fix bug where getting pet from stable while mounted deletes pet.
    if (GetPlayer()->IsMounted())
        GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);

    SendPetStableList(packet.StableMaster);
}

void WorldSession::SendPetStableList(ObjectGuid stableMasterGuid)
{
    WorldPackets::Pet::SPetStableList packet;
    packet.StableMaster = stableMasterGuid;
    packet.StableSlots = PET_SLOT_INACTIVE_STABLE_SLOTS;

    for (auto const& pair : _player->GetPlayerPetDataMap())
    {
        WorldPackets::Pet::PetStableInfo& stableInfo = packet.Pets.emplace_back();
        stableInfo.CreatureID = pair.second->TamedCreatureId;
        stableInfo.DisplayID = pair.second->DisplayId;
        stableInfo.ExperienceLevel = _player->getLevel();
        stableInfo.PetName = pair.second->Name;
        stableInfo.PetNumber = pair.second->PetNumber;
        stableInfo.PetSlot = pair.second->Slot;
        stableInfo.PetFlags = (pair.second->Slot <= PET_SLOT_LAST_ACTIVE_SLOT ? PET_STABLE_ACTIVE : (PET_STABLE_ACTIVE | PET_STABLE_INACTIVE));
    }

    SendPacket(packet.Write());
}

void WorldSession::SendStableResult(PetStableResultCode result)
{
    WorldPackets::Pet::PetStableResult packet;
    packet.Result = AsUnderlyingType(result);
    SendPacket(packet.Write());
}

void WorldSession::SendPetSlotUpdated(int32 petNumberA, int32 petSlotA, int32 petNumberB, int32 petSlotB)
{
    WorldPackets::Pet::PetSlotUpdated packet;
    packet.PetNumberA = petNumberA;
    packet.PetSlotA = petSlotA;
    packet.PetNumberB = petNumberB;
    packet.PetSlotB = petSlotB;

    SendPacket(packet.Write());
}

void WorldSession::HandleSetPetSlot(WorldPackets::Pet::SetPetSlot& packet)
{
    if (!CheckStableMaster(packet.StableMaster) || packet.DestSlot >= PET_SLOT_LAST_STABLE_SLOT)
        return;

    _player->SetPetSlot(packet.PetNumber, packet.DestSlot);
}

void WorldSession::HandleStableRevivePet(WorldPacket &/* recvData */)
{
    TC_LOG_DEBUG("network", "HandleStableRevivePet: Not implemented");
}
