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

#include "PetPackets.h"
#include "PetDefines.h"

WorldPacket const* WorldPackets::Pet::PetDismissSound::Write()
{
    _worldPacket << int32(ModelID);
    _worldPacket << ModelPosition;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetGuids::Write()
{
    _worldPacket << int32(PetGUIDs.size());
    for (ObjectGuid const& guid : PetGUIDs)
        _worldPacket << guid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::SPetMode::Write()
{
    _worldPacket << PetGUID;
    _worldPacket << uint8(ReactState);
    _worldPacket << uint8(CommandState);
    _worldPacket << uint16(Flag);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetActionFeedback::Write()
{
    _worldPacket << uint8(Response);

    switch (Response)
    {
        case FEEDBACK_NOTHING_TO_ATT:
        case FEEDBACK_CANT_ATT_TARGET:
            _worldPacket << int32(SpellID);
            break;
        default:
            break;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetActionSound::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << int32(Action);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetAdded::Write()
{
    _worldPacket << int32(Level);
    _worldPacket << int32(PetSlot);
    _worldPacket << uint8(Flags);
    _worldPacket << int32(CreatureID);
    _worldPacket << int32(PetNumber);
    _worldPacket.WriteBits(Name.length(), 8);
    _worldPacket << Name;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetSpellsMessage::Write()
{
    _worldPacket << PetGUID;
    if (PetGUID.IsEmpty())
        return &_worldPacket;

    _worldPacket << uint16(_CreatureFamily);
    _worldPacket << uint32(TimeLimit);
    _worldPacket << uint8(ReactState);
    _worldPacket << uint8(CommandState);
    _worldPacket << uint16(Flag);

    for (uint32 button : ActionButtons)
        _worldPacket << uint32(button);

    _worldPacket << uint8(Actions.size());
    for (uint32 action : Actions)
        _worldPacket << uint32(action);

    _worldPacket << uint8(Cooldowns.size());
    for (PetSpellCooldown const& cooldown : Cooldowns)
    {
        _worldPacket << uint32(cooldown.SpellID);
        _worldPacket << uint16(cooldown.Category);
        _worldPacket << int32(cooldown.Duration);
        _worldPacket << int32(cooldown.CategoryDuration);
    }

    return &_worldPacket;
}

void WorldPackets::Pet::PetAction::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> Action;
    _worldPacket >> TargetGUID;
    _worldPacket >> ActionPosition;
}

void WorldPackets::Pet::DismissCritter::Read()
{
    _worldPacket >> CritterGUID;
}

WorldPacket const* WorldPackets::Pet::PetLearnedSpell::Write()
{
    _worldPacket << uint32(SpellID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::PetUnlearnedSpell::Write()
{
    _worldPacket << uint32(SpellID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Pet::SPetStableList::Write()
{
    _worldPacket << StableMaster;
    _worldPacket << uint8(Pets.size());
    _worldPacket << uint8(StableSlots);

    for (PetStableInfo const& pet : Pets)
    {
        _worldPacket << uint32(pet.PetSlot);
        _worldPacket << uint32(pet.PetNumber);
        _worldPacket << uint32(pet.CreatureID);
        _worldPacket << uint32(pet.ExperienceLevel);
        _worldPacket << pet.PetName;
        _worldPacket << uint8(pet.PetFlags);
    }

    return &_worldPacket;
}

void WorldPackets::Pet::CPetStableList::Read()
{
    _worldPacket >> StableMaster;
}

WorldPacket const* WorldPackets::Pet::PetStableResult::Write()
{
    _worldPacket << uint8(Result);

    return &_worldPacket;
}

void WorldPackets::Pet::SetPetSlot::Read()
{
    _worldPacket >> PetNumber;
    _worldPacket >> DestSlot;

    StableMaster[3] = _worldPacket.ReadBit();
    StableMaster[2] = _worldPacket.ReadBit();
    StableMaster[0] = _worldPacket.ReadBit();
    StableMaster[7] = _worldPacket.ReadBit();
    StableMaster[5] = _worldPacket.ReadBit();
    StableMaster[6] = _worldPacket.ReadBit();
    StableMaster[1] = _worldPacket.ReadBit();
    StableMaster[4] = _worldPacket.ReadBit();

    _worldPacket.ReadByteSeq(StableMaster[5]);
    _worldPacket.ReadByteSeq(StableMaster[3]);
    _worldPacket.ReadByteSeq(StableMaster[1]);
    _worldPacket.ReadByteSeq(StableMaster[7]);
    _worldPacket.ReadByteSeq(StableMaster[4]);
    _worldPacket.ReadByteSeq(StableMaster[0]);
    _worldPacket.ReadByteSeq(StableMaster[6]);
    _worldPacket.ReadByteSeq(StableMaster[2]);
}

void WorldPackets::Pet::PetAbandon::Read()
{
    _worldPacket >> Pet;
}

void WorldPackets::Pet::PetSetAction::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> Index;
    _worldPacket >> Action;
}

WorldPacket const* WorldPackets::Pet::PetTameFailure::Write()
{
    _worldPacket << uint8(Result);

    return &_worldPacket;
}

void WorldPackets::Pet::PetSpellAutocast::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> SpellID;
    _worldPacket >> AutocastEnabled;
}

WorldPacket const* WorldPackets::Pet::PetSlotUpdated::Write()
{
    _worldPacket << int32(PetNumberA);
    _worldPacket << int32(PetSlotA);
    _worldPacket << int32(PetNumberB);
    _worldPacket << int32(PetSlotB);

    return &_worldPacket;
}

void WorldPackets::Pet::PetStopAttack::Read()
{
    _worldPacket >> PetGUID;
}

void WorldPackets::Pet::PetRename::Read()
{
    _worldPacket >> RenameData.PetGUID;
    _worldPacket >> RenameData.NewName;
    _worldPacket >> RenameData.HasDeclinedNames;

    if (RenameData.HasDeclinedNames)
        for (std::string& declinedName : RenameData.DeclinedNames.name)
            _worldPacket >> declinedName;
}

WorldPacket const* WorldPackets::Pet::PetNameInvalid::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << RenameData.NewName;
    _worldPacket << bool(RenameData.HasDeclinedNames);

    if (RenameData.HasDeclinedNames)
        for (std::string const& declinedName : RenameData.DeclinedNames.name)
            _worldPacket << declinedName;

    return &_worldPacket;
}
