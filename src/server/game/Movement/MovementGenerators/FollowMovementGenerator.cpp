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

#include "FollowMovementGenerator.h"
#include "CreatureAI.h"
#include "EventProcessor.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "PathGenerator.h"
#include "Pet.h"
#include "TemporarySummon.h"
#include "Unit.h"
#include "Util.h"
#include <optional>

// Helpers
inline UnitMoveType SelectSpeedType(uint32 moveFlags)
{
    if (moveFlags & MOVEMENTFLAG_FLYING)
    {
        if (moveFlags & MOVEMENTFLAG_BACKWARD)
            return MOVE_FLIGHT_BACK;
        else
            return MOVE_FLIGHT;
    }
    else if (moveFlags & MOVEMENTFLAG_SWIMMING)
    {
        if (moveFlags & MOVEMENTFLAG_BACKWARD)
            return MOVE_SWIM_BACK;
        else
            return MOVE_SWIM;
    }
    else if (moveFlags & MOVEMENTFLAG_WALKING)
        return MOVE_WALK;
    else if (moveFlags & MOVEMENTFLAG_BACKWARD)
        return MOVE_RUN_BACK;

    return MOVE_RUN;
}

inline bool IsTargetMoving(Unit const* target)
{
    return target->HasUnitMovementFlag(MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD | MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT) || !target->movespline->Finalized();
}

inline float GetVelocity(Unit* owner, Unit* target, bool catchUp)
{
    float targetSpeed = 0.f;
    float velocity = 0.f;
    if (target->IsPlayer())
        targetSpeed = target->GetSpeed(SelectSpeedType(target->m_movementInfo.GetMovementFlags()));
    else
    {
        if (!target->movespline->Finalized())
            targetSpeed = target->movespline->Velocity();
        else
            targetSpeed = target->GetSpeed(MOVE_RUN);
    }

    if (catchUp)
        velocity = targetSpeed;
    else
    {
        if (owner->IsFlying())
            velocity = owner->GetSpeed(MOVE_FLIGHT);
        else if (!owner->IsWalking())
            velocity = owner->GetSpeed(MOVE_RUN);
        else
            velocity = owner->GetSpeed(MOVE_WALK);

        // Owner might be able to walk faster than its target so we limit the speed
        //velocity = std::min<float>(velocity, targetSpeed);
    }

    return velocity;
}

static void ApplyCatchUpMod(Unit* owner, Position dest, float& velocity)
{
    float distance = owner->GetExactDist2d(dest);

    if (dest.HasInArc(float(M_PI), owner)) // owner is beyond default destination. throttle speed.
        AddPct(velocity, -((distance / velocity) * 100.f));
    else // owner is behind destination, catch up.
        AddPct(velocity, ((distance / velocity) * 100.f));
}

static void DoMovementInform(Unit* owner, Unit* target)
{
    if (owner->GetTypeId() != TYPEID_UNIT)
        return;

    if (UnitAI* ai = owner->GetAI())
        static_cast<CreatureAI*>(ai)->MovementInform(FOLLOW_MOTION_TYPE, target->GetGUID().GetCounter());
}

static void GetFollowOffsets(uint8 followerIndex, float& distance, float& relativeAngle)
{
    uint8 rowOffset = 0;

    if (followerIndex >= DEFAULT_ROW_FOLLOWERS)
        rowOffset = std::floor<uint8>((followerIndex - DEFAULT_ROW_FOLLOWERS) / MAX_FOLLOWERS_PER_ROW);
    else
    {
        // Default left and right follower slots
        distance = STRAIGHT_FOLLOW_DISTANCE;
        relativeAngle = followerIndex == 0 ? float(M_PI_2) : float(-M_PI_2);
        return;
    }

    // Further follower slots. Calculating a place in further rows
    uint8 rowIndex = (followerIndex - DEFAULT_ROW_FOLLOWERS) % MAX_FOLLOWERS_PER_ROW;

    // Blizzard assigns new followers in a left-right repeat pattern rather than just going from left to right
    bool left = rowIndex % 2;
    Position pos;
    pos.RelocateOffset({ 0.f, left ? -STRAIGHT_FOLLOW_DISTANCE : STRAIGHT_FOLLOW_DISTANCE });

    if (rowIndex <= 3)
    {
        pos.RelocateOffset({ -(STRAIGHT_FOLLOW_DISTANCE * rowOffset) });

        // Inner and outer positions
        float angle = float(M_PI);
        if (rowIndex <= 1)
            angle += left ? float(-(M_PI_4 / 2)) : float(M_PI_4 / 2);
        else
            angle += left ? float(M_PI_4 / 2) : float(-(M_PI_4 / 2));

        pos.m_positionX += std::cos(angle) * SIDE_FOLLOW_DISTANCE;
        pos.m_positionY += std::sin(angle) * SIDE_FOLLOW_DISTANCE;
    }
    else
    {
        // Middle positions
        pos.RelocateOffset({ -(STRAIGHT_FOLLOW_DISTANCE + STRAIGHT_FOLLOW_DISTANCE * rowOffset) });
    }

    distance = Position().GetExactDist2d(pos);
    relativeAngle = Position().GetRelativeAngle(pos);
}

FollowMovementGenerator::FollowMovementGenerator(Unit* target, Optional<float> distance, Optional<float> angle, bool joinFormation /*= true*/, bool catchUpToTarget /*= false*/) :
    _target(target), _joinFormation(joinFormation), _catchUpToTarget(catchUpToTarget)
{
    _distance = distance ? distance.get() : 0.f;
    _angle = angle ? angle.get() : 0.f;
}

FollowMovementGenerator::~FollowMovementGenerator() { }

void FollowMovementGenerator::Initialize(Unit* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW);

    // owner joins a organized follower formation.
    if (_joinFormation)
    {
        _target->AddFormationFollower(owner);
        UpdateFollowFormation();
    }

    _followMovementTimer.Reset(0);
    _events.ScheduleEvent(EVENT_ALLIGN_TO_TARGET, Milliseconds(ALLIGN_MOVEMENT_INTERVAL));
}

void FollowMovementGenerator::Finalize(Unit* owner)
{
    owner->ClearUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);

    if (_joinFormation && _target)
    {
        _target->RemoveFormationFollower(owner);
        UpdateFollowFormation();
    }
}

void FollowMovementGenerator::Reset(Unit* /*owner*/)
{
    _followMovementTimer.Reset(0);
    _events.ScheduleEvent(EVENT_ALLIGN_TO_TARGET, 1ms);
}

bool FollowMovementGenerator::Update(Unit* owner, uint32 diff)
{
    // Owner might be dead or gone
    if (!owner->IsAlive())
        return false;

    // Our target might have gone away
    if (!_target || !_target->IsInWorld())
        return false;

    // Follower cannot move at the moment
    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE) || owner->IsMovementPreventedByCasting())
    {
        owner->StopMoving();
        _followMovementTimer.Reset(0);
        return true;
    }

    // Default follow movement procedure when target is moving
    _followMovementTimer.Update(diff);
    if (_followMovementTimer.Passed())
    {
        if (IsTargetMoving(_target))
        {
            _events.Reset();
            LaunchMovement(owner);
            return true;
        }
        else if (owner->HasUnitState(UNIT_STATE_FOLLOW_MOVE))
        {
            owner->StopMoving();
            owner->ClearUnitState(UNIT_STATE_FOLLOW_MOVE);
            DoMovementInform(owner, _target);
            _events.ScheduleEvent(EVENT_ALLIGN_TO_TARGET, Milliseconds(ALLIGN_MOVEMENT_INTERVAL));
        }
    }
    _followMovementTimer.Reset(FOLLOW_MOVEMENT_INTERVAL);

    _events.Update(diff);
    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_ALLIGN_TO_TARGET:
            {
                Position pos = _target->GetPosition();
                _target->MovePositionToFirstCollision(pos, _distance, _angle);
                Movement::MoveSplineInit init (owner);
                init.MoveTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
                uint32 duration = std::max<uint32>(1, init.Launch());
                _events.ScheduleEvent(EVENT_ALLIGN_TO_FACING_DIRECTION, Milliseconds(duration + FOLLOW_MOVEMENT_INTERVAL));
                break;
            }
            case EVENT_ALLIGN_TO_FACING_DIRECTION:
                owner->SetFacingTo(_target->GetOrientation());
                break;
            default:
                break;
        }
    }

    /*
    // Target is currently not moving. Return to intended follow position after a short delay.
    if (!owner->HasUnitState(UNIT_STATE_FOLLOW_MOVE) && !_faceTarget)
    {
        _allignMovementTimer.Update(diff);
        _facingMovementTimer.Update(diff);

        if (_allignMovementTimer.Passed() && _returnState == RETURN_STATE_NONE)
        {
            // Return to our follow target
            Position dest = _target->GetPosition();
            _target->MovePositionToFirstCollision(dest, _range, _angle.RelativeAngle);

            // Launch a alligning spline when we are too far away from our follow target
            if (owner->GetExactDist2d(dest) > 1.f)
            {
                Movement::MoveSplineInit init(owner);
                init.MoveTo(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ());
                init.SetVelocity(_target->GetSpeed(SelectSpeedType(_target->GetUnitMovementFlags())));
                init.Launch();
                _returnState = RETURN_STATE_RETURNING;
                _facingMovementTimer.Reset(FOLLOW_MOVEMENT_INTERVAL);
            }
            else
                _returnState = RETURN_STATE_RETURNED;
        }

        if (_facingMovementTimer.Passed() && _returnState == RETURN_STATE_RETURNING)
        {
            if (owner->movespline->Finalized())
            {
                owner->SetFacingTo(_target->GetOrientation());
                _returnState = RETURN_STATE_RETURNED;
            }
            else
                _facingMovementTimer.Reset(FOLLOW_MOVEMENT_INTERVAL);
        }

        _followMovementTimer.Reset(0);
    }

    _followMovementTimer.Update(diff);
    if (_followMovementTimer.Passed())
    {
        // Target is currently moving, launch our next spline
        if (!_target->movespline->Finalized() || _target->isMoving())
            LaunchMovement(owner, false);
        else if (owner->HasUnitState(UNIT_STATE_FOLLOW_MOVE) && _target->movespline->Finalized() && !_target->isMoving())
        {
            // Owner is no longer moving. Prepare out allignment movement
            if (!_faceTarget)
            {
                owner->StopMoving();
                owner->ClearUnitState(UNIT_STATE_FOLLOW_MOVE);
                _allignMovementTimer.Reset(ALLING_MOVEMENT_INTERVAL);
                DoMovementInform(owner, _target);
            }
            else
                LaunchMovement(owner, true);
        }

        _followMovementTimer.Reset(FOLLOW_MOVEMENT_INTERVAL);
    }

    */
    return true;
}

void FollowMovementGenerator::UpdateFollowFormation()
{
    uint8 followSlot = 0;
    for (Unit* follower : _target->GetFormationFollowers())
    {
        for (uint8 slot = MOTION_SLOT_IDLE; slot < MAX_MOTION_SLOT; ++slot)
        {
            MovementGenerator* moveGen = follower->GetMotionMaster()->GetMotionSlot(MovementSlot(slot));
            if (!moveGen || moveGen->GetMovementGeneratorType() != FOLLOW_MOTION_TYPE)
                continue;

            if (FollowMovementGenerator* followMoveGen = static_cast<FollowMovementGenerator*>(moveGen))
                followMoveGen->UpdateFormationFollowOffsets(followSlot);
        }

        ++followSlot;
    }
}

void FollowMovementGenerator::UpdateFormationFollowOffsets(uint32 slot)
{
    // Do not update follow values for follow targets that are not part of the follow formation.
    if (!_joinFormation)
        return;

    GetFollowOffsets(slot, _distance, _angle);
};

void FollowMovementGenerator::LaunchMovement(Unit* owner)
{
    Position dest = _target->GetPosition();
    float offset = 0.f;

    // Strafe handling for player sidewards movement
    if (_target->IsPlayer())
    {
        if (_target->HasUnitMovementFlag(MOVEMENTFLAG_STRAFE_LEFT))
            offset = _target->HasUnitMovementFlag(MOVEMENTFLAG_FORWARD) ? float(M_PI_4) : float(M_PI_2);

        if (_target->HasUnitMovementFlag(MOVEMENTFLAG_STRAFE_RIGHT))
            offset = _target->HasUnitMovementFlag(MOVEMENTFLAG_FORWARD) ? -float(M_PI_4) : -float(M_PI_2);

        if (_target->HasUnitMovementFlag(MOVEMENTFLAG_BACKWARD))
            offset += float(M_PI);
    }

    // Let's start with a cheap base destination calculation
    dest.m_positionX += std::cos(Position::NormalizeOrientation(_target->GetOrientation() + _angle)) * _distance;
    dest.m_positionY += std::sin(Position::NormalizeOrientation(_target->GetOrientation() + _angle)) * _distance;
    dest.SetOrientation(dest.GetOrientation() + offset);

    // Calculate velocity based on target's speed values
    float velocity = GetVelocity(owner, _target, (_joinFormation || _catchUpToTarget));
    if (velocity == 0.f)
        return;

    // Calculate catchup speed values if owner follows its target in a formation
    if (_joinFormation || _catchUpToTarget)
        ApplyCatchUpMod(owner, dest, velocity);

    if (velocity <= 0.1f)
        return;

    // Predicting our follow destination
    dest.m_positionX += std::cos(Position::NormalizeOrientation(_target->GetOrientation() + offset)) * velocity;
    dest.m_positionY += std::sin(Position::NormalizeOrientation(_target->GetOrientation() + offset)) * velocity;

    // Now we calculate our actual destination data
    if (!owner->HasUnitState(UNIT_STATE_IGNORE_PATHFINDING))
    {
        float relativeAngle = _target->GetRelativeAngle(dest);
        float distance = _target->GetExactDist2d(dest);
        dest = _target->GetPosition();
        _target->MovePositionToFirstCollision(dest, distance, relativeAngle);
    }

    Movement::MoveSplineInit init(owner);
    init.MoveTo(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ());
    init.SetVelocity(velocity);
    init.Launch();

    owner->AddUnitState(UNIT_STATE_FOLLOW_MOVE);
}
