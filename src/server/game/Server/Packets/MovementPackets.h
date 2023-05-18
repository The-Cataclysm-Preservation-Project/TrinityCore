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

#ifndef MovementPackets_h__
#define MovementPackets_h__

#include "Packet.h"
#include "Object.h"
#include "Optional.h"

class MovementStatus;

namespace Movement
{
    template<class index_type>
    class Spline;
    class MoveSpline;
}

namespace WorldPackets
{
    namespace Movement
    {
        struct MovementAck
        {
            MovementInfo Status;
            int32 AckIndex = 0;
        };

        struct ShipTransferPending
        {
            uint32 ID = 0;              ///< gameobject_template.entry of the transport the player is teleporting on
            int32 OriginMapID = -1;     ///< Map id the player is currently on (before teleport)
        };

        class TransferPending final : public ServerPacket
        {
        public:
            TransferPending() : ServerPacket(SMSG_TRANSFER_PENDING, 12) { }

            WorldPacket const* Write() override;

            int32 MapID = -1;
            Optional<ShipTransferPending> Ship;
            Optional<int32> TransferSpellID;
        };

        class SuspendToken final : public ServerPacket
        {
        public:
            SuspendToken() : ServerPacket(SMSG_SUSPEND_TOKEN, 4 + 1) { }

            WorldPacket const* Write() override;

            uint32 SequenceIndex = 1;
            uint8 Reason = 1;
        };

        class SuspendTokenResponse final : public ClientPacket
        {
        public:
            SuspendTokenResponse(WorldPacket&& packet) : ClientPacket(CMSG_SUSPEND_TOKEN_RESPONSE, std::move(packet)) { }

            void Read() override;

            uint32 SequenceIndex = 0;
        };

        class NewWorld final : public ServerPacket
        {
        public:
            NewWorld() : ServerPacket(SMSG_NEW_WORLD, 4 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 MapID = 0;
            Position Pos;
        };

        class MoveSetCollisionHeight final : public ServerPacket
        {
        public:
            MoveSetCollisionHeight(ObjectGuid moverGuid, float height, UpdateCollisionHeightReason reason, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_COLLISION_HEIGHT, 8 + 4 + 4+ 1),
                MoverGUID(moverGuid), Height(height), SequenceIndex(sequenceIndex), Reason(reason) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
            float Height = 0.0f;
            UpdateCollisionHeightReason Reason = UPDATE_COLLISION_HEIGHT_MOUNT;
        };

        struct MoveKnockBackSpeeds
        {
            float HorzSpeed = 0.0f;
            float VertSpeed = 0.0f;
        };

        class MoveKnockBack final : public ServerPacket
        {
        public:
            MoveKnockBack() : ServerPacket(SMSG_MOVE_KNOCK_BACK, 1 + 8 + 4 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            TaggedPosition<Position::XY> Direction;
            MoveKnockBackSpeeds Speeds;
            uint32 SequenceIndex = 0;
        };

        struct VehicleTeleport
        {
            uint8 VehicleSeatIndex = 0;
            bool VehicleExitVoluntary = false;
            bool VehicleExitTeleport = false;
        };

        class MoveTeleport final : public ServerPacket
        {
        public:
            MoveTeleport() : ServerPacket(OpcodeServer(MSG_MOVE_TELEPORT), 12 + 4 + 8 + 8 + 4) { }

            WorldPacket const* Write() override;

            TaggedPosition<Position::XYZ> Pos;
            Optional<VehicleTeleport> Vehicle;
            uint32 SequenceIndex = 0;
            ObjectGuid MoverGUID;
            Optional<ObjectGuid> TransportGUID;
            float Facing = 0.0f;
        };

        class MoveTeleportAck final : public ClientPacket
        {
        public:
            MoveTeleportAck(WorldPacket&& packet) : ClientPacket(OpcodeClient(MSG_MOVE_TELEPORT_ACK), std::move(packet)) { }

            void Read() override;

            ObjectGuid MoverGUID;
            int32 AckIndex = 0;
            int32 MoveTime = 0;
        };

        class SetActiveMover final : public ClientPacket
        {
        public:
            SetActiveMover(WorldPacket&& packet) : ClientPacket(CMSG_SET_ACTIVE_MOVER, std::move(packet)) { }

            void Read() override;

            ObjectGuid ActiveMover;
        };

        class MoveUpdateTeleport final : public ServerPacket
        {
        public:
            MoveUpdateTeleport() : ServerPacket(SMSG_MOVE_UPDATE_TELEPORT) { }

            WorldPacket const* Write() override;

            MovementInfo* Status = nullptr;
        };

        struct MonsterSplineJumpExtraData
        {
            float JumpGravity = 0.0f;
            uint32 StartTime = 0;
        };

        struct MonsterSplineAnimationExtraData
        {
            uint8 AnimTier = 0;
            uint32 TierTransStartTime = 0;
        };

        struct MovementSpline
        {
            uint32 Flags                = 0;    // Spline flags
            uint8 Face                  = 0;    // Movement direction (see MonsterMoveType enum)
            uint32 MoveTime             = 0;
            std::vector<TaggedPosition<Position::XYZ>> Points;   // Spline path
            int8 VehicleExitVoluntary   = 0;
            ObjectGuid TransportGUID;
            int8 VehicleSeat            = -1;
            std::vector<TaggedPosition<Position::PackedXYZ>> PackedDeltas;
            Optional<MonsterSplineJumpExtraData> JumpExtraData;
            Optional<MonsterSplineAnimationExtraData> Animation;
            float FaceDirection         = 0.0f;
            ObjectGuid FaceGUID;
            TaggedPosition<Position::XYZ> FaceSpot;
        };

        struct MovementMonsterSpline
        {
            uint32 ID = 0;
            TaggedPosition<Position::XYZ> Destination;
            bool CrzTeleport = false;
            MovementSpline Move;
        };

        class MonsterMove final : public ServerPacket
        {
        public:
            MonsterMove(bool onTransport) : ServerPacket(onTransport ? SMSG_ON_MONSTER_MOVE_TRANSPORT : SMSG_ON_MONSTER_MOVE) { }

            void InitializeSplineData(::Movement::MoveSpline const& moveSpline);

            WorldPacket const* Write() override;

            MovementMonsterSpline SplineData;
            ObjectGuid MoverGUID;
            TaggedPosition<Position::XYZ> Pos;
        };

        class FlightSplineSync final : public ServerPacket
        {
        public:
            FlightSplineSync() : ServerPacket(SMSG_FLIGHT_SPLINE_SYNC, 4 + 8) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
            float SplineDist = 0.0f;
        };

        class TransferAborted final : public ServerPacket
        {
        public:
            TransferAborted() : ServerPacket(SMSG_TRANSFER_ABORTED, 4 + 1 + 1) { }

            WorldPacket const* Write() override;

            uint32 MapID = 0;
            uint8 TransfertAbort = 0;
            uint8 Arg = 0;
        };

        class MoveSetActiveMover final : public ServerPacket
        {
        public:
            MoveSetActiveMover(ObjectGuid moverGuid) : ServerPacket(SMSG_MOVE_SET_ACTIVE_MOVER, 8), MoverGUID(moverGuid) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
        };

        class MoveSetVehicleRecID final : public ServerPacket
        {
        public:
            MoveSetVehicleRecID(ObjectGuid moverGuid) : ServerPacket(SMSG_MOVE_SET_VEHICLE_REC_ID, 8 + 4 + 4), MoverGUID(moverGuid) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
            int32 VehicleRecID = 0;
        };

        class MoveUpdate final : public ServerPacket
        {
        public:
            MoveUpdate(MovementStatus const& status) : ServerPacket(SMSG_MOVE_UPDATE), Status(status) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
        };

        class MoveUpdateKnockBack final : public ServerPacket
        {
        public:
            MoveUpdateKnockBack(MovementStatus const& status) : ServerPacket(SMSG_MOVE_UPDATE_KNOCK_BACK), Status(status) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
        };

        class MoveUpdateRunSpeed final : public ServerPacket
        {
        public:
            MoveUpdateRunSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_RUN_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateRunBackSpeed final : public ServerPacket
        {
        public:
            MoveUpdateRunBackSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_RUN_BACK_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateWalkSpeed final : public ServerPacket
        {
        public:
            MoveUpdateWalkSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_WALK_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateSwimSpeed final : public ServerPacket
        {
        public:
            MoveUpdateSwimSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_SWIM_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateSwimBackSpeed final : public ServerPacket
        {
        public:
            MoveUpdateSwimBackSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_SWIM_BACK_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateFlightSpeed final : public ServerPacket
        {
        public:
            MoveUpdateFlightSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_FLIGHT_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateFlightBackSpeed final : public ServerPacket
        {
        public:
            MoveUpdateFlightBackSpeed(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateTurnRate final : public ServerPacket
        {
        public:
            MoveUpdateTurnRate(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_TURN_RATE), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdatePitchRate final : public ServerPacket
        {
        public:
            MoveUpdatePitchRate(MovementStatus const& status, float speed) : ServerPacket(SMSG_MOVE_UPDATE_PITCH_RATE), Status(status), Speed(speed) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Speed = 0.0f;
        };

        class MoveUpdateCollisionHeight final : public ServerPacket
        {
        public:
            MoveUpdateCollisionHeight(MovementStatus const& status, float height) : ServerPacket(SMSG_MOVE_UPDATE_COLLISION_HEIGHT), Status(status), Height(height) { }

            WorldPacket const* Write() override;

            MovementStatus Status;
            float Height = 0.0f;
        };

        class MoveSetRunSpeed final : public ServerPacket
        {
        public:
            MoveSetRunSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_RUN_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex){ }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetRunBackSpeed final : public ServerPacket
        {
        public:
            MoveSetRunBackSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_RUN_BACK_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetWalkSpeed final : public ServerPacket
        {
        public:
            MoveSetWalkSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_WALK_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetSwimSpeed final : public ServerPacket
        {
        public:
            MoveSetSwimSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_SWIM_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetSwimBackSpeed final : public ServerPacket
        {
        public:
            MoveSetSwimBackSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_SWIM_BACK_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetFlightSpeed final : public ServerPacket
        {
        public:
            MoveSetFlightSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_FLIGHT_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetFlightBackSpeed final : public ServerPacket
        {
        public:
            MoveSetFlightBackSpeed(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_FLIGHT_BACK_SPEED, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetTurnRate final : public ServerPacket
        {
        public:
            MoveSetTurnRate(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_TURN_RATE, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetPitchRate final : public ServerPacket
        {
        public:
            MoveSetPitchRate(ObjectGuid moverGuid, float speed, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_PITCH_RATE, 8 + 4 + 4), MoverGUID(moverGuid), Speed(speed), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 0.0f;
            uint32 SequenceIndex = 0;
        };

        class MoveSetCanFly final : public ServerPacket
        {
        public:
            MoveSetCanFly(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_CAN_FLY, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveUnsetCanFly final : public ServerPacket
        {
        public:
            MoveUnsetCanFly(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_UNSET_CAN_FLY, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveDisableGravity final : public ServerPacket
        {
        public:
            MoveDisableGravity(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_GRAVITY_DISABLE, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveEnableGravity final : public ServerPacket
        {
        public:
            MoveEnableGravity(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_GRAVITY_ENABLE, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveSetWaterWalk final : public ServerPacket
        {
        public:
            MoveSetWaterWalk(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_WATER_WALK, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveSetLandWalk final : public ServerPacket
        {
        public:
            MoveSetLandWalk(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_LAND_WALK, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveSetFeaterFall final : public ServerPacket
        {
        public:
            MoveSetFeaterFall(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_FEATHER_FALL, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveSetNormalFall final : public ServerPacket
        {
        public:
            MoveSetNormalFall(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_NORMAL_FALL, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveSetHovering final : public ServerPacket
        {
        public:
            MoveSetHovering(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_HOVER, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveUnsetHovering final : public ServerPacket
        {
        public:
            MoveUnsetHovering(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_UNSET_HOVER, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveRoot final : public ServerPacket
        {
        public:
            MoveRoot(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_ROOT, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveUnroot final : public ServerPacket
        {
        public:
            MoveUnroot(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_UNROOT, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveEnableTransitionBetweenSwimAndFly final : public ServerPacket
        {
        public:
            MoveEnableTransitionBetweenSwimAndFly(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class MoveDisableTransitionBetweenSwimAndFly final : public ServerPacket
        {
        public:
            MoveDisableTransitionBetweenSwimAndFly(ObjectGuid moverGuid, uint32 sequenceIndex) : ServerPacket(SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY, 8 + 4), MoverGUID(moverGuid), SequenceIndex(sequenceIndex) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };
    }

    ByteBuffer& operator<<(ByteBuffer& data, Movement::MovementSpline const& movementSpline);
    ByteBuffer& operator<<(ByteBuffer& data, Movement::MovementMonsterSpline const& movementMonsterSpline);
}

#endif // MovementPackets_h__
