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

void WorldPackets::Talent::LeanPreviewTalentsPet::Read()
{
    _worldPacket >> PetGUID;

    Talents.resize(_worldPacket.read<uint32>());
    for (TalentInfo& talent : Talents)
    {
        _worldPacket >> talent.TalentID;
        _worldPacket >> talent.Rank;
    }
}

WorldPacket const* WorldPackets::Talent::TalentInfoUpdate::Write()
{
    _worldPacket << bool(PetTalents);
    _worldPacket << uint32(UnspentPoints);
    if (PetTalents)
    {
        _worldPacket << uint8(PetTalent.size());
        for (TalentInfo const& talentData : PetTalent)
        {
            _worldPacket << uint32(talentData.TalentID);
            _worldPacket << uint8(talentData.Rank);
        }
    }
    else
    {

    }

    return &_worldPacket;
}
