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

#ifndef TalentPackets_h__
#define TalentPackets_h__

#include "Packet.h"
#include "DBCEnums.h"
#include "ObjectGuid.h"
#include "PacketUtilities.h"

namespace WorldPackets
{
    namespace Talent
    {
        struct TalentInfo
        {
            uint32 TalentID = 0;
            uint32 Rank = 0;
        };

        class LearnPreviewTalents final : public ClientPacket
        {
        public:
            LearnPreviewTalents(WorldPacket&& packet) : ClientPacket(CMSG_LEARN_PREVIEW_TALENTS, std::move(packet)) { }

            void Read() override;

            int32 TalentTab = 0;
            Array<TalentInfo, 70> Talents;
        };
    }
}

#endif // TalentPackets_h__
