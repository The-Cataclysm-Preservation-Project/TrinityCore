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

#ifndef SpellCastRequest_h__
#define SpellCastRequest_h__

#include "SpellPacketsCommon.h"

struct SpellCastRequestItemData
{
    SpellCastRequestItemData(uint8 bagSlot, uint8 slot, ObjectGuid castItem) :
        BagSlot(bagSlot), Slot(slot), CastItem(castItem) { }

    uint8 BagSlot = 0;
    uint8 Slot = 0;
    ObjectGuid CastItem;
};

struct PendingSpellCastRequest
{
    PendingSpellCastRequest(WorldPackets::Spells::SpellCastRequest&& castRequest, Optional<SpellCastRequestItemData> castItemData = { }) :
        CastRequest(castRequest), CastItemData(castItemData) { }

    WorldPackets::Spells::SpellCastRequest CastRequest;
    Optional<SpellCastRequestItemData> CastItemData;
};

#endif // SpellCastRequest_h__
