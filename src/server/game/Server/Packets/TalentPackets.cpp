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

#include "TalentPackets.h"

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Talent::TalentInfo const& talent)
{
    data << uint32(talent.TalentID);
    data << uint32(talent.Rank);
    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Talent::TalentInfo& talent)
{
    data >> talent.TalentID;
    data >> talent.Rank;
    return data;
}

void WorldPackets::Talent::LearnPreviewTalents::Read()
{
    _worldPacket >> TalentTab;
    Talents.resize(_worldPacket.read<uint32>());

    for (TalentInfo& talent : Talents)
        _worldPacket >> talent;
}
