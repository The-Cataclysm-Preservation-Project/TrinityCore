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

#ifndef ReforgePackets_h__
#define ReforgePackets_h__

#include "ObjectGuid.h"
#include "Packet.h"

namespace WorldPackets
{
    namespace Reforge
    {
        class ReforgeResult final : public ServerPacket
        {
        public:
            ReforgeResult() : ServerPacket(SMSG_REFORGE_RESULT, 1) { }

            WorldPacket const* Write() override;

            bool Success = false;
        };

        class ReforgeItem final : public ClientPacket
        {
        public:
            ReforgeItem(WorldPacket&& packet) : ClientPacket(CMSG_REFORGE_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid ReforgerGUID;
            uint32 SlotNum = 0;
            uint32 ContainerId = 0;
            int32 Enchantment = 0;
        };

        class SetForgeMaster final : public ServerPacket
        {
        public:
            SetForgeMaster() : ServerPacket(SMSG_FORGE_MASTER_SET, 8) { }

            WorldPacket const* Write() override;

            ObjectGuid ReforgerGUID;
        };
    }
}

#endif // ReforgePackets_h__
