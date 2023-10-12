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

#include "ReforgePackets.h"

WorldPacket const* WorldPackets::Reforge::ReforgeResult::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Reforge::ReforgeItem::Read()
{
    _worldPacket >> Enchantment;
    _worldPacket >> SlotNum;
    _worldPacket >> ContainerId;

    ReforgerGUID[2] = _worldPacket.ReadBit();
    ReforgerGUID[6] = _worldPacket.ReadBit();
    ReforgerGUID[3] = _worldPacket.ReadBit();
    ReforgerGUID[4] = _worldPacket.ReadBit();
    ReforgerGUID[1] = _worldPacket.ReadBit();
    ReforgerGUID[0] = _worldPacket.ReadBit();
    ReforgerGUID[7] = _worldPacket.ReadBit();
    ReforgerGUID[5] = _worldPacket.ReadBit();

    _worldPacket.ReadByteSeq(ReforgerGUID[2]);
    _worldPacket.ReadByteSeq(ReforgerGUID[3]);
    _worldPacket.ReadByteSeq(ReforgerGUID[6]);
    _worldPacket.ReadByteSeq(ReforgerGUID[4]);
    _worldPacket.ReadByteSeq(ReforgerGUID[1]);
    _worldPacket.ReadByteSeq(ReforgerGUID[0]);
    _worldPacket.ReadByteSeq(ReforgerGUID[7]);
    _worldPacket.ReadByteSeq(ReforgerGUID[5]);
}

WorldPacket const* WorldPackets::Reforge::SetForgeMaster::Write()
{
    _worldPacket.WriteBit(ReforgerGUID[1]);
    _worldPacket.WriteBit(ReforgerGUID[3]);
    _worldPacket.WriteBit(ReforgerGUID[4]);
    _worldPacket.WriteBit(ReforgerGUID[2]);
    _worldPacket.WriteBit(ReforgerGUID[6]);
    _worldPacket.WriteBit(ReforgerGUID[0]);
    _worldPacket.WriteBit(ReforgerGUID[7]);
    _worldPacket.WriteBit(ReforgerGUID[6]);

    _worldPacket.WriteByteSeq(ReforgerGUID[6]);
    _worldPacket.WriteByteSeq(ReforgerGUID[2]);
    _worldPacket.WriteByteSeq(ReforgerGUID[3]);
    _worldPacket.WriteByteSeq(ReforgerGUID[4]);
    _worldPacket.WriteByteSeq(ReforgerGUID[5]);
    _worldPacket.WriteByteSeq(ReforgerGUID[0]);
    _worldPacket.WriteByteSeq(ReforgerGUID[7]);
    _worldPacket.WriteByteSeq(ReforgerGUID[1]);

    return &_worldPacket;
}
