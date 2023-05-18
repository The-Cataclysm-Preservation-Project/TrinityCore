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

#ifndef MoveStateChange_h__
#define MoveStateChange_h__

#include "Optional.h"
#include "Position.h"

struct TC_GAME_API KnockBackInfo
{
    float HorzSpeed = 0.0f;
    TaggedPosition<Position::XY> Direction;
    float InitVertSpeed = 0.0f;
};

struct TC_GAME_API CollisionHeightInfo
{
    float Height = 0.0f;
    uint8 Reason = 0;
};

struct TC_GAME_API MoveStateChange
{
    uint16 MessageID = 0;
    uint32 SequenceIndex = 0;
    Optional<float> Speed;
    Optional<KnockBackInfo> KnockBack;
    Optional<int32> VehicleRecID;
    Optional<CollisionHeightInfo> CollisionHeight;
};

#endif // MoveStateChange_h__
