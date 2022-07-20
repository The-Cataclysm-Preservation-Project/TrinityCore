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

#ifndef PetPackets_h__
#define PetPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "Position.h"
#include "UnitDefines.h"

namespace WorldPackets
{
    namespace Pet
    {
        class PetDismissSound final : public ServerPacket
        {
        public:
            PetDismissSound() : ServerPacket(SMSG_PET_DISMISS_SOUND, 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 ModelID = 0;
            TaggedPosition<Position::XYZ> ModelPosition;
        };

        class PetGuids final : public ServerPacket
        {
        public:
            PetGuids() : ServerPacket(SMSG_PET_GUIDS, 4) { }

            WorldPacket const* Write() override;

            std::vector<ObjectGuid> PetGUIDs;
        };

        class SPetMode final : public ServerPacket
        {
        public:
            SPetMode() : ServerPacket(SMSG_PET_MODE, 8 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid PetGUID;
            ReactStates ReactState = REACT_PASSIVE;
            CommandStates CommandState = COMMAND_STAY;
            uint16 Flag = 0;
        };

        class PetActionFeedback final : public ServerPacket
        {
        public:
            PetActionFeedback() : ServerPacket(SMSG_PET_ACTION_FEEDBACK, 1) { }

            WorldPacket const* Write() override;

            int32 SpellID = 0;
            uint8 Response = 0;
        };

        class PetActionSound final : public ServerPacket
        {
        public:
            PetActionSound() : ServerPacket(SMSG_PET_ACTION_SOUND, 8 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            int32 Action = 0;
        };

        class PetAdded final : public ServerPacket
        {
        public:
            PetAdded() : ServerPacket(SMSG_PET_ADDED, 4 + 4 + 1 + 4 + 4) { }

            WorldPacket const* Write() override;

            std::string Name;
            int32 CreatureID = 0;
            int32 Level = 0;
            int32 PetSlot = 0;
            uint32 PetNumber = 0;
            uint8 Flags = 0;
        };

        struct PetSpellCooldown
        {
            int32 SpellID = 0;
            int32 Duration = 0;
            int32 CategoryDuration = 0;
            uint16 Category = 0;
        };

        class PetSpellsMessage final : public ServerPacket
        {
        public:
            PetSpellsMessage() : ServerPacket(SMSG_PET_SPELLS) { }

            WorldPacket const* Write() override;

            ObjectGuid PetGUID;
            uint16 _CreatureFamily = 0; ///< @see enum CreatureFamily
            uint32 TimeLimit = 0;
            uint8 ReactState = 0;
            uint8 CommandState = 0;
            uint16 Flag = 0;

            std::array<uint32, 10> ActionButtons = { };
            std::vector<uint32> Actions;
            std::vector<PetSpellCooldown> Cooldowns;
        };

        class PetAction final : public ClientPacket
        {
        public:
            PetAction(WorldPacket&& packet) : ClientPacket(CMSG_PET_ACTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
            ObjectGuid TargetGUID;
            TaggedPosition<Position::XYZ> ActionPosition;
            uint32 Action = 0;
        };

        class DismissCritter final : public ClientPacket
        {
        public:
            DismissCritter(WorldPacket&& packet) : ClientPacket(CMSG_DISMISS_CRITTER, std::move(packet)) { }

            void Read() override;

            ObjectGuid CritterGUID;
        };

        class PetLearnedSpell final : public ServerPacket
        {
        public:
            PetLearnedSpell(uint32 spellId) : ServerPacket(SMSG_PET_LEARNED_SPELL, 4), SpellID(spellId) { }

            WorldPacket const* Write() override;

            uint32 SpellID = 0;
        };

        class PetUnlearnedSpell final : public ServerPacket
        {
        public:
            PetUnlearnedSpell(uint32 spellId) : ServerPacket(SMSG_PET_REMOVED_SPELL, 4), SpellID(spellId) { }

            WorldPacket const* Write() override;

            uint32 SpellID = 0;
        };

        struct PetStableInfo
        {
            uint32 PetSlot = 0;
            uint32 PetNumber = 0;
            uint32 CreatureID = 0;
            uint32 DisplayID = 0;
            uint32 ExperienceLevel = 0;
            std::string PetName;
            uint8 PetFlags = 0;
        };

        class SPetStableList final : public ServerPacket
        {
        public:
            SPetStableList() : ServerPacket(OpcodeServer(MSG_LIST_STABLED_PETS)) { }

            WorldPacket const* Write() override;

            std::vector<PetStableInfo> Pets;
            ObjectGuid StableMaster;
            uint8 StableSlots = 0;
        };

        class CPetStableList final : public ClientPacket
        {
        public:
            CPetStableList(WorldPacket&& packet) : ClientPacket(OpcodeClient(MSG_LIST_STABLED_PETS), std::move(packet)) { }

            void Read() override;

            ObjectGuid StableMaster;
        };

        class PetStableResult final : public ServerPacket
        {
        public:
            PetStableResult() : ServerPacket(SMSG_STABLE_RESULT, 1) { }

            WorldPacket const* Write() override;

            uint8 Result = 0;
        };

        class SetPetSlot final : public ClientPacket
        {
        public:
            SetPetSlot(WorldPacket&& packet) : ClientPacket(CMSG_SET_PET_SLOT, std::move(packet)) { }

            void Read() override;

            ObjectGuid StableMaster;
            uint32 PetNumber = 0;
            uint8 DestSlot = 0;
        };

        class PetAbandon final : public ClientPacket
        {
        public:
            PetAbandon(WorldPacket&& packet) : ClientPacket(CMSG_PET_ABANDON, std::move(packet)) { }

            void Read() override;

            ObjectGuid Pet;
        };

        class PetSetAction final : public ClientPacket
        {
        public:
            PetSetAction(WorldPacket&& packet) : ClientPacket(CMSG_PET_SET_ACTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
            uint32 Action = 0;
            uint32 Index = 0;
        };

        class PetTameFailure final : public ServerPacket
        {
        public:
            PetTameFailure() : ServerPacket(SMSG_PET_TAME_FAILURE, 1) { }

            WorldPacket const* Write() override;

            uint8 Result = 0;
        };

        class PetSpellAutocast final : public ClientPacket
        {
        public:
            PetSpellAutocast(WorldPacket&& packet) : ClientPacket(CMSG_PET_SPELL_AUTOCAST, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
            int32 SpellID = 0;
            bool AutocastEnabled = false;
        };

        class PetSlotUpdated final : public ServerPacket
        {
        public:
            PetSlotUpdated() : ServerPacket(SMSG_PET_SLOT_UPDATED, 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 PetNumberA = 0;
            int32 PetSlotA = 0;
            int32 PetNumberB = 0;
            int32 PetSlotB = 0;
        };

        class PetStopAttack final : public ClientPacket
        {
        public:
            PetStopAttack(WorldPacket&& packet) : ClientPacket(CMSG_PET_STOP_ATTACK, std::move(packet)) { }

            void Read() override;

            ObjectGuid PetGUID;
        };

        struct PetRenameData
        {
            ObjectGuid PetGUID;
            bool HasDeclinedNames = false;
            std::string NewName;
            DeclinedName DeclinedNames;
        };

        class PetRename final : public ClientPacket
        {
        public:
            PetRename(WorldPacket&& packet) : ClientPacket(CMSG_PET_RENAME, std::move(packet)) { }

            void Read() override;

            PetRenameData RenameData;
        };

        class PetNameInvalid final : public ServerPacket
        {
        public:
            PetNameInvalid() : ServerPacket(SMSG_PET_NAME_INVALID, 8 + 1 + RenameData.NewName.size()) { }

            WorldPacket const* Write() override;

            PetRenameData RenameData;
            uint8 Result = 0;
        };
    }
}

#endif // PetPackets_h__
