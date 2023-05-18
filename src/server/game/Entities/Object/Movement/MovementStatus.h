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

#ifndef MovementStatus_h__
#define MovementStatus_h__

#include "ObjectGuid.h"
#include "Optional.h"
#include "Position.h"

enum MovementFlags : uint32;
enum MovementFlags2 : uint32;

struct MovementTransport
{
    ObjectGuid Guid;
    TaggedPosition<Position::XYZ> _Position;
    float Facing = 0.0f;
    uint8 VehicleSeatIndex = 0;
    uint32 MoveTime = 0;
    Optional<uint32> PrevMoveTime;
    Optional<int32> VehicleRecID;
};

struct MovementFallVelocity
{
    TaggedPosition<Position::XY> Direction;
    float Speed = 0.0f;
};

struct MovementFallOrLand
{
    uint32 Time = 0;
    float JumpVelocity = 0.0f;
    Optional<MovementFallVelocity> Velocity;
};

class TC_GAME_API MovementStatus
{
public:
    MovementStatus();

    ObjectGuid MoverGUID;
    TaggedPosition<Position::XYZ> _Position;
    uint32 MoveFlags0;
    uint16 MoveFlags1;
    uint32 MoveTime;
    uint32 MoveIndex;
    float Facing;
    float Pitch;
    float StepUpStartElevation;
    bool HasSpline;
    bool HeightChangeFailed;
    bool RemoteTimeValid;
    Optional<MovementTransport> Transport;
    Optional<MovementFallOrLand> Fall;

    bool HasMovementFlag0(MovementFlags flag) const { return (MoveFlags0 & flag) != 0; }
    bool HasMovementFlag1(MovementFlags2 flag) const { return (MoveFlags1 & flag) != 0; }
    void AddMovementFlag0(MovementFlags flag) { MoveFlags0 |= flag; }
    void AddMovementFlag1(MovementFlags2 flag) { MoveFlags1 |= flag; }
    void RemoveMovementFlag0(MovementFlags flag) { MoveFlags0 &= ~flag; }
    void RemoveMovementFlag1(MovementFlags2 flag) { MoveFlags1 &= ~flag; }
    bool HasAnyMovementFlags0() const { return MoveFlags0 != 0; }
    bool HasAnyMovementFlags1() const { return MoveFlags1 != 0; }

    Position const& GetPosition() const { return _Position.Pos; }
};

#endif // MovementStatus_h__
