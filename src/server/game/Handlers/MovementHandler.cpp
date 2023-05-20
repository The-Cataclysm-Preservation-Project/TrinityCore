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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Log.h"
#include "Map.h"
#include "Corpse.h"
#include "DBCStores.h"
#include "Player.h"
#include "MapManager.h"
#include "MotionMaster.h"
#include "MovementGenerator.h"
#include "MovementPackets.h"
#include "MovementStructures.h"
#include "MovementStatus.h"
#include "MoveStateChange.h"
#include "Transport.h"
#include "Battleground.h"
#include "InstanceSaveMgr.h"
#include "Vehicle.h"
#include "GameTime.h"
#include "GameClient.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <G3D/Vector3.h>

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket & /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: got MSG_MOVE_WORLDPORT_ACK.");
    HandleMoveWorldportAck();
}

void WorldSession::HandleMoveWorldportAck()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    // get the teleport destination
    WorldLocation const& loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mEntry->IsDungeon())
        GetPlayer()->m_InstanceValid = true;

    Map* oldMap = GetPlayer()->GetMap();
    Map* newMap = GetPlayer()->GetTeleportDestInstanceId() ?
        sMapMgr->FindMap(loc.GetMapId(), *GetPlayer()->GetTeleportDestInstanceId()) :
        sMapMgr->CreateMap(loc.GetMapId(), GetPlayer());

    MovementInfo::TransportInfo transportInfo = GetPlayer()->m_movementInfo.transport;
    if (TransportBase* transport = GetPlayer()->GetTransport())
        transport->RemovePassenger(GetPlayer());

    if (GetPlayer()->IsInWorld())
    {
        TC_LOG_ERROR("network", "%s %s is still in world when teleported from map %s (%u) to new map %s (%u)", GetPlayer()->GetGUID().ToString().c_str(), GetPlayer()->GetName().c_str(), oldMap->GetMapName(), oldMap->GetId(), newMap ? newMap->GetMapName() : "Unknown", loc.GetMapId());
        oldMap->RemovePlayerFromMap(GetPlayer(), false);
    }

    // relocate the player to the teleport destination
    // the CannotEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!newMap || newMap->CannotEnter(GetPlayer()))
    {
        TC_LOG_ERROR("network", "Map %d (%s) could not be created for player %d (%s), porting player to homebind", loc.GetMapId(), newMap ? newMap->GetMapName() : "Unknown", GetPlayer()->GetGUID().GetCounter(), GetPlayer()->GetName().c_str());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    float z = loc.GetPositionZ() + GetPlayer()->GetHoverOffset();

    GetPlayer()->Relocate(loc.GetPositionX(), loc.GetPositionY(), z, loc.GetOrientation());
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();

    // move player between transport copies on each map
    if (Transport* newTransport = newMap->GetTransport(transportInfo.guid))
    {
        GetPlayer()->m_movementInfo.transport = transportInfo;
        newTransport->AddPassenger(GetPlayer());
    }

    if (!GetPlayer()->GetMap()->AddPlayerToMap(GetPlayer()))
    {
        TC_LOG_ERROR("network", "WORLD: failed to teleport player %s (%d) to map %d (%s) because of unknown reason!",
            GetPlayer()->GetName().c_str(), GetPlayer()->GetGUID().GetCounter(), loc.GetMapId(), newMap ? newMap->GetMapName() : "Unknown");
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (_player->InBattleground())
    {
        // cleanup setting if outdated
        if (!mEntry->IsBattlegroundOrArena())
        {
            // We're not in BG
            _player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(0);
        }
        // join to bg case
        else if (Battleground* bg = _player->GetBattleground())
        {
            if (_player->IsInvitedForBattlegroundInstance(_player->GetBattlegroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleground())
        {
            // short preparations to continue flight
            MovementGenerator* movementGenerator = GetPlayer()->GetMotionMaster()->top();
            movementGenerator->Initialize(GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    // resurrect character at enter into instance where his corpse exist after add to map

    if (mEntry->IsDungeon() && !GetPlayer()->IsAlive())
    {
        if (GetPlayer()->GetCorpseLocation().GetMapId() == mEntry->ID)
        {
            GetPlayer()->ResurrectPlayer(0.5f, false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    if (mEntry->IsDungeon())
    {
        // check if this instance has a reset time and send it to player if so
        Difficulty diff = newMap->GetDifficulty();
        if (MapDifficulty const* mapDiff = sDBCManager.GetMapDifficultyData(mEntry->ID, diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sInstanceSaveMgr->GetResetTimeFor(mEntry->ID, diff))
                {
                    uint32 timeleft = uint32(timeReset - GameTime::GetGameTime());
                    GetPlayer()->SendInstanceResetWarning(mEntry->ID, diff, timeleft, true);
                }
            }
        }

        // check if instance is valid
        if (!GetPlayer()->CheckInstanceValidity(false))
            GetPlayer()->m_InstanceValid = false;
    }

    // update zone immediately, otherwise leave channel will cause crash in mtmap
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);

    // honorless target
    if (GetPlayer()->pvpInfo.IsHostile)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // in friendly area
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleSuspendTokenResponse(WorldPackets::Movement::SuspendTokenResponse& /*suspendTokenResponse*/)
{
    if (!_player->IsBeingTeleportedFar())
        return;

    WorldLocation const& loc = GetPlayer()->GetTeleportDest();
    WorldPackets::Movement::NewWorld packet;
    packet.MapID = loc.GetMapId();
    packet.Pos = loc;
    SendPacket(packet.Write());

    _player->SendSavedInstances();
}

void WorldSession::HandleMoveTeleportAck(WorldPackets::Movement::MoveTeleportAck& packet)
{
    if (!IsRightUnitBeingMoved(packet.MoverGUID))
        return;

    GameClient* client = GetGameClient();
    Unit* mover = client->GetActivelyMovedUnit();
    Player* plMover = mover->ToPlayer();

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;


    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    WorldPackets::Movement::MoveUpdateTeleport moveUpdateTeleport;
    moveUpdateTeleport.Status = &plMover->m_movementInfo;
    moveUpdateTeleport.Status->pos.Relocate(dest);
    plMover->SendMessageToSet(moveUpdateTeleport.Write(), false);

    plMover->UpdatePosition(dest, true);
    plMover->SetFallInformation(0, plMover->GetPositionZ());

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if (old_zone != newzone)
    {
        // honorless target
        if (plMover->pvpInfo.IsHostile)
            plMover->CastSpell(plMover, 2479, true);

        // in friendly area
        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket& recvPacket)
{
    /* extract packet */
    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvPacket, &movementInfo);
    uint16 opcode = recvPacket.GetOpcode();

    HandleMovementOpcode(opcode, movementInfo);
}

void WorldSession::HandleMovementOpcode(uint16 opcode, MovementInfo& movementInfo)
{
    if (!IsRightUnitBeingMoved(movementInfo.guid))
        return;

    GameClient* client = GetGameClient();
    Unit* mover = client->GetActivelyMovedUnit();

    // there must always be a mover
    if (!mover)
        return;

    Player* plrMover = mover->ToPlayer();

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plrMover && plrMover->IsBeingTeleported())
        return;

    GetPlayer()->ValidateMovementInfo(&movementInfo);

    // prevent tampered movement data
    if (movementInfo.guid != mover->GetGUID())
    {
        TC_LOG_ERROR("network", "HandleMovementOpcodes: guid error");
        return;
    }
    if (!movementInfo.pos.IsPositionValid())
    {
        TC_LOG_ERROR("network", "HandleMovementOpcodes: Invalid Position");
        return;
    }

    // stop some emotes at player move
    if (plrMover && (plrMover->GetUInt32Value(UNIT_NPC_EMOTESTATE) != 0))
        plrMover->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);

    /* handle special cases */
    if (!movementInfo.transport.guid.IsEmpty())
    {
        // We were teleported, skip packets that were broadcast before teleport
        if (movementInfo.pos.GetExactDist2d(mover) > SIZE_OF_GRIDS)
            return;

        // transports size limited
        // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if (fabs(movementInfo.transport.pos.GetPositionX()) > 75.0f || fabs(movementInfo.transport.pos.GetPositionY()) > 75.0f || fabs(movementInfo.transport.pos.GetPositionZ()) > 75.0f)
            return;

        if (!Trinity::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.transport.pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.transport.pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.transport.pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.transport.pos.GetOrientation()))
            return;

        // if we boarded a transport, add us to it
        if (plrMover)
        {
            if (!plrMover->GetTransport())
            {
                if (GameObject* go = plrMover->GetMap()->GetGameObject(movementInfo.transport.guid))
                    if (TransportBase* transport = go->ToTransportBase())
                        transport->AddPassenger(plrMover);
            }
            else if (plrMover->GetTransport()->GetTransportGUID() != movementInfo.transport.guid)
            {
                plrMover->GetTransport()->RemovePassenger(plrMover);
                if (GameObject* go = plrMover->GetMap()->GetGameObject(movementInfo.transport.guid))
                {
                    if (TransportBase* transport = go->ToTransportBase())
                        transport->AddPassenger(plrMover);
                    else
                        movementInfo.ResetTransport();
                }
                else
                    movementInfo.ResetTransport();
            }
        }

        if (!mover->GetTransport() && !mover->GetVehicle())
            movementInfo.transport.Reset();
    }
    else if (plrMover && plrMover->GetTransport())                // if we were on a transport, leave
        plrMover->GetTransport()->RemovePassenger(plrMover);

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plrMover && !plrMover->IsInFlight())
        plrMover->HandleFall(movementInfo);

    // interrupt parachutes upon falling or landing in water
    if (opcode == MSG_MOVE_FALL_LAND || opcode == MSG_MOVE_START_SWIM || opcode == CMSG_MOVE_SET_CAN_FLY)
        mover->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::LandingOrFlight); // Parachutes

    /* process position-change */
    int64 movementTime = (int64)movementInfo.time + _timeSyncClockDelta;
    if (_timeSyncClockDelta == 0 || movementTime < 0 || movementTime > 0xFFFFFFFF)
    {
        TC_LOG_WARN("misc", "The computed movement time using clockDelta is erronous. Using fallback instead");
        movementInfo.time = GameTime::GetGameTimeMS();
    }
    else
    {
        movementInfo.time = (uint32)movementTime;
    }

    movementInfo.guid = mover->GetGUID();
    mover->m_movementInfo = movementInfo;

    // Some vehicles allow the passenger to turn by himself
    if (Vehicle* vehicle = mover->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(mover))
        {
            if (seat->Flags & VEHICLE_SEAT_FLAG_ALLOW_TURNING)
            {
                if (movementInfo.pos.GetOrientation() != mover->GetOrientation())
                {
                    mover->SetOrientation(movementInfo.pos.GetOrientation());
                    mover->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Turning);
                }
            }
        }
        return;
    }

    mover->UpdatePosition(movementInfo.pos);

    WorldPacket data(SMSG_MOVE_UPDATE);
    mover->WriteMovementInfo(data);
    mover->SendMessageToSet(&data, _player);

    if (plrMover)                                            // nothing is charmed, or player charmed
    {
        if (plrMover->IsSitState() && (movementInfo.flags & (MOVEMENTFLAG_MASK_MOVING | MOVEMENTFLAG_MASK_TURNING)))
            plrMover->SetStandState(UNIT_STAND_STATE_STAND);

        plrMover->UpdateFallInformationIfNeed(movementInfo, opcode);

        if (movementInfo.pos.GetPositionZ() < plrMover->GetMap()->GetMinHeight(plrMover->GetPhaseShift(), movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY()))
        {
            if (!(plrMover->GetBattleground() && plrMover->GetBattleground()->HandlePlayerUnderMap(_player)))
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                /// @todo discard movement packets after the player is rooted
                if (plrMover->IsAlive())
                {
                    plrMover->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_IS_OUT_OF_BOUNDS);
                    plrMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    // player can be alive if GM/etc
                    // change the death state to CORPSE to prevent the death timer from
                    // starting in the next player update
                    if (plrMover->IsAlive())
                        plrMover->KillPlayer();
                }
            }
        }

        if (opcode == MSG_MOVE_JUMP)
        {
            plrMover->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags2::Jump);
            Unit::ProcSkillsAndAuras(plrMover, nullptr, PROC_FLAG_JUMP, PROC_FLAG_NONE, PROC_SPELL_TYPE_MASK_ALL, PROC_SPELL_PHASE_NONE, PROC_HIT_NONE, nullptr, nullptr, nullptr);
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPackets::Movement::SetActiveMover& packet)
{
    GameClient* client = GetGameClient();

    // step 1: look at the list of units that this client is allowed to move. check if the client is allowed to even move the
    // unit that is mentioned in the packet. if not, either silently ignore, log this event or kick the client.
    if (!client->IsAllowedToMove(packet.ActiveMover))
    {
        // @todo log or kick or do nothing depending on configuration
        TC_LOG_DEBUG("entities.unit", "set active mover FAILED for client of player %s. GUID %s.", _player->GetName().c_str(), packet.ActiveMover.ToString().c_str());
        return;
    }

    // step 2:
    TC_LOG_DEBUG("entities.unit", "set active mover OK for client of player %s. GUID %s.", _player->GetName().c_str(), packet.ActiveMover.ToString().c_str());
    Unit* newActivelyMovedUnit = ObjectAccessor::GetUnit(*_player, packet.ActiveMover);
    client->SetActivelyMovedUnit(newActivelyMovedUnit);
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket &recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");

    MovementInfo movementInfo;
    _player->ReadMovementInfo(recvData, &movementInfo);

    GameClient* client = GetGameClient();
    if (client->GetActivelyMovedUnit() == nullptr || client->GetActivelyMovedUnit()->GetGUID() != movementInfo.guid)
    {
        TC_LOG_DEBUG("entities.unit", "unset active mover FAILED for client of player %s. GUID %s.", _player->GetName().c_str(), movementInfo.guid.ToString().c_str());
        return;
    }

    TC_LOG_DEBUG("entities.unit", "unset active mover OK for client of player %s. GUID %s.", _player->GetName().c_str(), movementInfo.guid.ToString().c_str());
    client->SetActivelyMovedUnit(nullptr);
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvData*/)
{
    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << uint64(GetPlayer()->GetGUID());

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recvData)
{
    if (!_player->IsAlive() || _player->IsInCombat())
        return;

    ObjectGuid summoner_guid;
    bool agree;
    recvData >> summoner_guid;
    recvData >> agree;

    _player->SummonIfPossible(agree);
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_MOVE_TIME_SKIPPED");

    ObjectGuid guid;
    uint32 time;
    recvData >> time;

    guid[5] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[5]);

    if (!IsRightUnitBeingMoved(guid))
        return;

    GameClient* client = GetGameClient();
    Unit* mover = client->GetActivelyMovedUnit();

    if (!mover)
    {
        TC_LOG_WARN("entities.player", "WorldSession::HandleMoveTimeSkippedOpcode wrong mover state from the unit moved by the player %s", GetPlayer()->GetGUID().ToString().c_str());
        return;
    }

    // prevent tampered movement data
    if (guid != mover->GetGUID())
    {
        TC_LOG_WARN("entities.player", "WorldSession::HandleMoveTimeSkippedOpcode wrong guid from the unit moved by the player %s", GetPlayer()->GetGUID().ToString().c_str());
        return;
    }

    mover->m_movementInfo.time += time;

    WorldPacket data(MSG_MOVE_TIME_SKIPPED, recvData.size());
    data << time;
    data << guid.WriteAsPacked();
    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleTimeSyncResp(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_TIME_SYNC_RESP");

    uint32 counter, clientTimestamp;
    recvData >> counter >> clientTimestamp;

    if (_pendingTimeSyncRequests.count(counter) == 0)
        return;

    uint32 serverTimeAtSent = _pendingTimeSyncRequests.at(counter);
    _pendingTimeSyncRequests.erase(counter);

    // time it took for the request to travel to the client, for the client to process it and reply and for response to travel back to the server.
    // we are going to make 2 assumptions:
    // 1) we assume that the request processing time equals 0.
    // 2) we assume that the packet took as much time to travel from server to client than it took to travel from client to server.
    uint32 roundTripDuration = getMSTimeDiff(serverTimeAtSent, recvData.GetReceivedTime());
    uint32 lagDelay = roundTripDuration / 2;

    /*
    clockDelta = serverTime - clientTime
    where
    serverTime: time that was displayed on the clock of the SERVER at the moment when the client processed the SMSG_TIME_SYNC_REQUEST packet.
    clientTime:  time that was displayed on the clock of the CLIENT at the moment when the client processed the SMSG_TIME_SYNC_REQUEST packet.

    Once clockDelta has been computed, we can compute the time of an event on server clock when we know the time of that same event on the client clock,
    using the following relation:
    serverTime = clockDelta + clientTime
    */
    int64 clockDelta = (int64)(serverTimeAtSent + lagDelay) - (int64)clientTimestamp;
    _timeSyncClockDeltaQueue.push_back(std::pair<int64, uint32>(clockDelta, roundTripDuration));
    ComputeNewClockDelta();
}

void WorldSession::ComputeNewClockDelta()
{
    // implementation of the technique described here: https://web.archive.org/web/20180430214420/http://www.mine-control.com/zack/timesync/timesync.html
    // to reduce the skew induced by dropped TCP packets that get resent.

    using namespace boost::accumulators;

    accumulator_set<uint32, features<tag::mean, tag::median, tag::variance(lazy)> > latencyAccumulator;

    for (auto pair : _timeSyncClockDeltaQueue)
        latencyAccumulator(pair.second);

    uint32 latencyMedian = static_cast<uint32>(std::round(median(latencyAccumulator)));
    uint32 latencyStandardDeviation = static_cast<uint32>(std::round(sqrt(variance(latencyAccumulator))));

    accumulator_set<int64, features<tag::mean> > clockDeltasAfterFiltering;
    uint32 sampleSizeAfterFiltering = 0;
    for (auto pair : _timeSyncClockDeltaQueue)
    {
        if (pair.second < latencyStandardDeviation + latencyMedian)
        {
            clockDeltasAfterFiltering(pair.first);
            sampleSizeAfterFiltering++;
        }
    }

    if (sampleSizeAfterFiltering != 0)
    {
        int64 meanClockDelta = static_cast<int64>(std::round(mean(clockDeltasAfterFiltering)));
        if (std::abs(meanClockDelta - _timeSyncClockDelta) > 25)
            _timeSyncClockDelta = meanClockDelta;
    }
    else if (_timeSyncClockDelta == 0)
    {
        std::pair<int64, uint32> back = _timeSyncClockDeltaQueue.back();
        _timeSyncClockDelta = back.first;
    }
}

void ExtractMovementStatusFromPacket(WorldPacket& recvPacket, MovementStatus& movementStatus, MoveStateChange* stateChange = nullptr)
{
    MovementStatusElements const* sequence = GetMovementStatusElementsSequence(static_cast<OpcodeClient>(recvPacket.GetOpcode()));
    if (!sequence)
    {
        TC_LOG_ERROR("network", "WorldSession::ExtractMovementStatusFromPacket: No movement sequence found for opcode %s", GetOpcodeNameForLogging(static_cast<OpcodeClient>(recvPacket.GetOpcode())).c_str());
        return;
    }

    bool hasMovementFlags0 = false;
    bool hasMovementFlags1 = false;
    bool hasFacing = false;
    bool hasMoveTime = false;
    bool hasPitch = false;
    bool hasStepUpStartElevation = false;

    for (; *sequence != MSEEnd; ++sequence)
    {
        MovementStatusElements const& element = *sequence;

        switch (element)
        {
            case MSEHasGuidByte0:
            case MSEHasGuidByte1:
            case MSEHasGuidByte2:
            case MSEHasGuidByte3:
            case MSEHasGuidByte4:
            case MSEHasGuidByte5:
            case MSEHasGuidByte6:
            case MSEHasGuidByte7:
                movementStatus.MoverGUID[element - MSEHasGuidByte0] = recvPacket.ReadBit();
                break;
            case MSEHasTransportGuidByte0:
            case MSEHasTransportGuidByte1:
            case MSEHasTransportGuidByte2:
            case MSEHasTransportGuidByte3:
            case MSEHasTransportGuidByte4:
            case MSEHasTransportGuidByte5:
            case MSEHasTransportGuidByte6:
            case MSEHasTransportGuidByte7:
                if (movementStatus.Transport.has_value())
                    movementStatus.Transport->Guid[element - MSEHasTransportGuidByte0] = recvPacket.ReadBit();
                break;
            case MSEGuidByte0:
            case MSEGuidByte1:
            case MSEGuidByte2:
            case MSEGuidByte3:
            case MSEGuidByte4:
            case MSEGuidByte5:
            case MSEGuidByte6:
            case MSEGuidByte7:
                recvPacket.ReadByteSeq(movementStatus.MoverGUID[element - MSEGuidByte0]);
                break;
            case MSETransportGuidByte0:
            case MSETransportGuidByte1:
            case MSETransportGuidByte2:
            case MSETransportGuidByte3:
            case MSETransportGuidByte4:
            case MSETransportGuidByte5:
            case MSETransportGuidByte6:
            case MSETransportGuidByte7:
                if (movementStatus.Transport.has_value())
                    recvPacket.ReadByteSeq(movementStatus.Transport->Guid[element - MSETransportGuidByte0]);
                break;
            case MSEHasMovementFlags:
                hasMovementFlags0 = !recvPacket.ReadBit();
                break;
            case MSEHasMovementFlags2:
                hasMovementFlags1 = !recvPacket.ReadBit();
                break;
            case MSEHasTimestamp:
                hasMoveTime = !recvPacket.ReadBit();
                break;
            case MSEHasOrientation:
                hasFacing = !recvPacket.ReadBit();
                break;
            case MSEHasTransportData:
                if (recvPacket.ReadBit())
                    movementStatus.Transport.emplace();
                break;
            case MSEHasTransportTime2:
                if (movementStatus.Transport.has_value())
                    if (recvPacket.ReadBit())
                        movementStatus.Transport->PrevMoveTime.emplace();
                break;
            case MSEHasVehicleId:
                if (movementStatus.Transport.has_value())
                    if (recvPacket.ReadBit())
                        movementStatus.Transport->VehicleRecID.emplace();
                break;
            case MSEHasPitch:
                hasPitch = !recvPacket.ReadBit();
                break;
            case MSEHasFallData:
                if (recvPacket.ReadBit())
                    movementStatus.Fall.emplace();
                break;
            case MSEHasFallDirection:
                if (movementStatus.Fall.has_value())
                    if (recvPacket.ReadBit())
                        movementStatus.Fall->Velocity.emplace();
                break;
            case MSEHasSplineElevation:
                hasStepUpStartElevation = !recvPacket.ReadBit();
                break;
            case MSEHasSpline:
                movementStatus.HasSpline = recvPacket.ReadBit();
                break;
            case MSEHasHeightChangeFailed:
                movementStatus.HeightChangeFailed = recvPacket.ReadBit();
                break;
            case MSEMovementFlags:
                if (hasMovementFlags0)
                    movementStatus.MoveFlags0 = recvPacket.ReadBits(30);
                break;
            case MSEMovementFlags2:
                if (hasMovementFlags1)
                    movementStatus.MoveFlags1 = recvPacket.ReadBits(12);
                break;
            case MSETimestamp:
                if (hasMoveTime)
                    recvPacket >> movementStatus.MoveTime;
                break;
            case MSEPositionX:
                recvPacket >> movementStatus._Position.Pos.m_positionX;
                break;
            case MSEPositionY:
                recvPacket >> movementStatus._Position.Pos.m_positionY;
                break;
            case MSEPositionZ:
                recvPacket >> movementStatus._Position.Pos.m_positionZ;
                break;
            case MSEOrientation:
                if (hasFacing)
                    recvPacket >> movementStatus.Facing;
                break;
            case MSETransportPositionX:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->_Position.Pos.m_positionX;
                break;
            case MSETransportPositionY:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->_Position.Pos.m_positionY;
                break;
            case MSETransportPositionZ:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->_Position.Pos.m_positionZ;
                break;
            case MSETransportOrientation:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->Facing;
                break;
            case MSETransportSeat:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->VehicleSeatIndex;
                break;
            case MSETransportTime:
                if (movementStatus.Transport.has_value())
                    recvPacket >> movementStatus.Transport->MoveTime;
                break;
            case MSETransportTime2:
                if (movementStatus.Transport.has_value() && movementStatus.Transport->PrevMoveTime.has_value())
                    movementStatus.Transport->PrevMoveTime = recvPacket.read<uint32>();
                break;
            case MSETransportVehicleId:
                if (movementStatus.Transport.has_value() && movementStatus.Transport->VehicleRecID.has_value())
                    movementStatus.Transport->VehicleRecID = recvPacket.read<uint32>();
                break;
            case MSEPitch:
                if (hasPitch)
                    movementStatus.Pitch = G3D::wrap(recvPacket.read<float>(), float(-M_PI), float(M_PI));
                break;
            case MSEFallTime:
                if (movementStatus.Fall.has_value())
                    recvPacket >> movementStatus.Fall->Time;
                break;
            case MSEFallVerticalSpeed:
                if (movementStatus.Fall.has_value())
                    recvPacket >> movementStatus.Fall->JumpVelocity;
                break;
            case MSEFallCosAngle:
                if (movementStatus.Fall.has_value() && movementStatus.Fall->Velocity.has_value())
                    recvPacket >> movementStatus.Fall->Velocity->Direction.Pos.m_positionX;
                break;
            case MSEFallSinAngle:
                if (movementStatus.Fall.has_value() && movementStatus.Fall->Velocity.has_value())
                    recvPacket >> movementStatus.Fall->Velocity->Direction.Pos.m_positionY;
                break;
            case MSEFallHorizontalSpeed:
                if (movementStatus.Fall.has_value() && movementStatus.Fall->Velocity.has_value())
                    recvPacket >> movementStatus.Fall->Velocity->Speed;
                break;
            case MSESplineElevation:
                if (hasStepUpStartElevation)
                    recvPacket >> movementStatus.StepUpStartElevation;
                break;
            case MSECounter:
                recvPacket >> movementStatus.MoveIndex;
                break;
            case MSEMoveStateChangeSpeed:
                if (stateChange)
                    stateChange->Speed = recvPacket.read<float>();
                else
                    recvPacket.read<float>();
                break;
            case MSEMoveStateChangeVehicleRecId:
                if (stateChange)
                    stateChange->VehicleRecID = recvPacket.read<int32>();
                else
                    recvPacket.read<int32>();
                break;
            case MSEMoveStateChangeCollisionHeightHeight:
                if (stateChange)
                {
                    if (!stateChange->CollisionHeight.has_value())
                        stateChange->CollisionHeight.emplace();
                    stateChange->CollisionHeight->Height = recvPacket.read<float>();
                }
                else
                    recvPacket.read<float>();
                break;
            case MSEMoveStateChangeCollisionHeightReason:
                if (stateChange)
                {
                    if (!stateChange->CollisionHeight.has_value())
                        stateChange->CollisionHeight.emplace();
                    stateChange->CollisionHeight->Reason = recvPacket.ReadBits(2);
                }
                else
                    recvPacket.ReadBits(2);
                break;
            default:
                break;
        }
    }
}

// Some lightweight packet spoofing checks. Each of the MSG_MOVE packets either add or remove a movement flag so we are going to check for the presence/absence of these
inline bool HasRequiredMovementFlagForPacket(uint16 opcode, MovementStatus& movementStatus)
{
    switch (opcode)
    {
        case MSG_MOVE_FALL_LAND:            return !movementStatus.HasMovementFlag0(MOVEMENTFLAG_FALLING);
        case MSG_MOVE_HEARTBEAT:            return movementStatus.HasAnyMovementFlags0();
        case MSG_MOVE_JUMP:                 return movementStatus.HasMovementFlag0(MOVEMENTFLAG_FALLING);
        case MSG_MOVE_SET_FACING:           return true; // @todo: investigate
        case MSG_MOVE_SET_PITCH:            return true; // @todo: investigate
        case MSG_MOVE_SET_RUN_MODE:         return true; // @todo: investigate
        case MSG_MOVE_SET_WALK_MODE:        return true; // @todo: investigate
        case MSG_MOVE_START_ASCEND:         return movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_FALLING));
        case MSG_MOVE_START_BACKWARD:       return movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_BACKWARD | MOVEMENTFLAG_FALLING));
        case MSG_MOVE_START_DESCEND:        return movementStatus.HasMovementFlag0(MOVEMENTFLAG_DESCENDING);
        case MSG_MOVE_START_FORWARD:        return movementStatus.HasMovementFlag0(MOVEMENTFLAG_FORWARD);
        case MSG_MOVE_START_PITCH_DOWN:     return movementStatus.HasMovementFlag0(MOVEMENTFLAG_PITCH_DOWN);
        case MSG_MOVE_START_PITCH_UP:       return movementStatus.HasMovementFlag0(MOVEMENTFLAG_PITCH_UP);
        case MSG_MOVE_START_STRAFE_LEFT:    return movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_PENDING_STRAFE_LEFT));
        case MSG_MOVE_START_STRAFE_RIGHT:   return movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_STRAFE_RIGHT | MOVEMENTFLAG_PENDING_STRAFE_RIGHT));
        case MSG_MOVE_START_SWIM:           return movementStatus.HasMovementFlag0(MOVEMENTFLAG_SWIMMING);
        case MSG_MOVE_START_TURN_LEFT:      return movementStatus.HasMovementFlag0(MOVEMENTFLAG_LEFT);
        case MSG_MOVE_START_TURN_RIGHT:     return movementStatus.HasMovementFlag0(MOVEMENTFLAG_RIGHT);
        case CMSG_MOVE_SET_CAN_FLY:         return movementStatus.HasMovementFlag0(MOVEMENTFLAG_CAN_FLY);
        default:
            return true;
    }
}

/*
    The MSG_MOVE packets are the most common packets received from the client. These are the ones that we have to check for credibility the most as we have to use most
    of their movement status data. This means we have to ensure that the flags in particular are valid before broadcasting player updates.
*/
void WorldSession::HandleClientAuthorativeMovementOpcode(WorldPacket& recvPacket)
{
    MovementStatus movementStatus;
    ExtractMovementStatusFromPacket(recvPacket, movementStatus);
    if (!ValidateMovementStatus(recvPacket.GetOpcode(), movementStatus))
    {
        // Zero tolerance for invalid packet data.
        //KickPlayer();
        TC_LOG_ERROR("network", "duh...");
        return;
    }

    ApplyMovementStatus(movementStatus);
}

void WorldSession::HandleMoveStateChangeAckOpcode(WorldPacket& recvPacket)
{
    MoveStateChange change;
    MovementStatus movementStatus;
    ExtractMovementStatusFromPacket(recvPacket, movementStatus, &change);
    if (!ValidateMovementStatus(recvPacket.GetOpcode(), movementStatus))
        return;

    GameClient* client = GetGameClient();

    // ACK handlers should call GameClient::IsAllowedToMove instead of WorldSession::IsRightUnitBeingMoved
    // because the ACK could be coming from a unit that is under the control of that client but is not the 'Active Mover' unit.
    // Example: Get a speed buff on yourself, then mount a vehicle before the end of the buff. When the buff expires,
    // a force message will be sent to the client regarding the player and the client is required to respond with an ACK.
    // But the vehicle will be the active mover unit at that time.
    if (!client->IsAllowedToMove(movementStatus.MoverGUID))
    {
        TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) tried to change a unit that is not under his control.", _player->GetName().c_str());
        return;
    }

    Unit* mover = ObjectAccessor::GetUnit(*_player, movementStatus.MoverGUID);
    if (!mover)
        return;

    MoveStateChange const* expectedChange = mover->GetExpectedMoveStateChange(recvPacket.GetOpcode());
    if (!expectedChange)
    {
        TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) tried to acknowledge a packet (%s) that we did not expect. Possible cheater or malformed packet.",
            _player->GetName().c_str(),
            GetOpcodeNameForLogging(OpcodeClient(recvPacket.GetOpcode())).c_str());
        return;
    }

    // Skip wrong or outdated ack messages. We do not log anything here because some scenarios trigger multiple acks for the same opcode in quick succession.
    if (expectedChange->SequenceIndex != movementStatus.MoveIndex)
        return;

    // Now we are ready to validate the information that we wanted
    bool hasRequiredMovementFlags = [&]()
    {
        switch (recvPacket.GetOpcode())
        {
            case CMSG_MOVE_FEATHER_FALL_ACK:                            return (expectedChange->MessageID == SMSG_MOVE_FEATHER_FALL) == movementStatus.HasMovementFlag0(MOVEMENTFLAG_FALLING_SLOW);
            case CMSG_MOVE_GRAVITY_DISABLE_ACK:                         return movementStatus.HasMovementFlag0(MOVEMENTFLAG_DISABLE_GRAVITY);
            case CMSG_MOVE_GRAVITY_ENABLE_ACK:                          return !movementStatus.HasMovementFlag0(MOVEMENTFLAG_DISABLE_GRAVITY);
            case CMSG_MOVE_HOVER_ACK:                                   return (expectedChange->MessageID == SMSG_MOVE_SET_HOVER) == movementStatus.HasMovementFlag0(MOVEMENTFLAG_HOVER);
            case CMSG_MOVE_SET_CAN_FLY_ACK:                             return (expectedChange->MessageID == SMSG_MOVE_SET_CAN_FLY) == movementStatus.HasMovementFlag0(MOVEMENTFLAG_CAN_FLY);
            case CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK: return (expectedChange->MessageID == SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY) == movementStatus.HasMovementFlag1(MOVEMENTFLAG2_CAN_SWIM_TO_FLY_TRANS);
            case CMSG_MOVE_WATER_WALK_ACK:                              return (expectedChange->MessageID == SMSG_MOVE_WATER_WALK) == movementStatus.HasMovementFlag0(MOVEMENTFLAG_WATERWALKING);
            case CMSG_FORCE_MOVE_ROOT_ACK:                              return movementStatus.HasMovementFlag0(MOVEMENTFLAG_ROOT);
            case CMSG_FORCE_MOVE_UNROOT_ACK:                            return !movementStatus.HasMovementFlag0(MOVEMENTFLAG_ROOT);
            default:
                return true;
        }

        return true;
    }();

    if (!hasRequiredMovementFlags)
    {
        TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) has not returned expected movement flags. Possible cheater or malformed packet.", _player->GetName().c_str());
        return;
    }

    switch (recvPacket.GetOpcode())
    {
        case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK:
        case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK:
        case CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK:
            if (!expectedChange->Speed.has_value() || !change.Speed.has_value() || std::fabs(*expectedChange->Speed - *change.Speed) > 0.01f)
            {
                TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) has returned an invalid speed change ack value. Possible cheater or malformed packet.", _player->GetName().c_str());
                return;
            }
            break;
        case CMSG_MOVE_SET_COLLISION_HEIGHT_ACK:
            if (!expectedChange->CollisionHeight.has_value() || !change.CollisionHeight.has_value()
                || std::fabs(expectedChange->CollisionHeight->Height - change.CollisionHeight->Height) > 0.01f
                || expectedChange->CollisionHeight->Reason != change.CollisionHeight->Reason)
            {
                TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) has returned invalid collision height ack data. Possible cheater or malformed packet.", _player->GetName().c_str());
                return;
            }
            break;
        case CMSG_SET_VEHICLE_REC_ID_ACK:
            if (!expectedChange->VehicleRecID.has_value() || !change.VehicleRecID.has_value() || *expectedChange->VehicleRecID != *change.VehicleRecID)
            {
                TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) has returned invalid vehicle rec Id ack data. Possible cheater or malformed packet.", _player->GetName().c_str());
                return;
            }
            break;
        case CMSG_MOVE_KNOCK_BACK_ACK:
        {
            bool isValidAck = [&]()
            {
                if (!expectedChange->KnockBack.has_value() || !movementStatus.Fall.has_value())
                    return false;

                if (!movementStatus.Fall->Velocity.has_value() && (expectedChange->KnockBack->Direction.Pos.GetPositionX() != 0.f
                    || expectedChange->KnockBack->Direction.Pos.GetPositionY() != 0.f
                    || expectedChange->KnockBack->HorzSpeed != 0.f))
                    return false;

                if (std::fabs(expectedChange->KnockBack->InitVertSpeed - movementStatus.Fall->JumpVelocity) > 0.01f)
                    return false;

                if (movementStatus.Fall->Velocity.has_value())
                {
                    if (std::fabs(movementStatus.Fall->Velocity->Speed - expectedChange->KnockBack->HorzSpeed) > 0.01f
                        || std::fabs(movementStatus.Fall->Velocity->Direction.Pos.GetPositionX() - expectedChange->KnockBack->Direction.Pos.GetPositionX()) > 0.01f
                        || std::fabs(movementStatus.Fall->Velocity->Direction.Pos.GetPositionY() - expectedChange->KnockBack->Direction.Pos.GetPositionY()) > 0.01f)
                        return false;
                }

                return true;
            }();

            if (!isValidAck)
            {
                TC_LOG_ERROR("network", "WorldSession::HandleMoveStateChangeAckOpcode player (%s) has returned an invalid knockback ack packet. Possible cheater or malformed packet.", _player->GetName().c_str());
                printf("Expected: VertSpeed = %f, HorzSpeed = %f, X: %f, Y: %f\n", expectedChange->KnockBack->InitVertSpeed, movementStatus.Fall->Velocity->Speed, expectedChange->KnockBack->Direction.Pos.GetPositionX(), expectedChange->KnockBack->Direction.Pos.GetPositionY());
                printf("Retrieve: VertSpeed = %f, HorzSpeed = %f, X: %f, Y: %f\n", movementStatus.Fall->JumpVelocity, expectedChange->KnockBack->HorzSpeed, movementStatus.Fall->Velocity->Direction.Pos.GetPositionX(), movementStatus.Fall->Velocity->Direction.Pos.GetPositionY());
                return;
            }
            break;
        }
        default:
            break;
    }

    ApplyMovementStatus(movementStatus, expectedChange);
}

bool WorldSession::ValidateMovementStatus(uint16 opcode, MovementStatus& movementStatus)
{
    // Invalid mover GUID
    if (movementStatus.MoverGUID.IsEmpty())
    {
        TC_LOG_ERROR("network", "WorldSession::ValidateMovementStatus: Player (%s) has sent a packet (Opcode: %s) with an empty MoverGUID.", _player->GetName().c_str(), GetOpcodeNameForLogging(OpcodeClient(opcode)).c_str());
        return false;
    }

    // Mover validation
    Unit* mover = GetGameClient()->GetActivelyMovedUnit();
    if (!mover || mover->GetGUID() != movementStatus.MoverGUID)
    {
        TC_LOG_ERROR("network", "WorldSession::ValidateMovementStatus: Player (%s) has sent a packet (Opcode: %s) with an mismatching mover.", _player->GetName().c_str(), GetOpcodeNameForLogging(OpcodeClient(opcode)).c_str());
        return false;
    }

    // Position validation
    if (!movementStatus.GetPosition().IsPositionValid())
    {
        TC_LOG_ERROR("network", "WorldSession::ValidateMovementStatus: Player (%s) has sent a packet (Opcode: %s) with an invalid Position.", _player->GetName().c_str(), GetOpcodeNameForLogging(OpcodeClient(opcode)).c_str());
        return false;
    }

    // Check for expected movement flags
    if (!HasRequiredMovementFlagForPacket(opcode, movementStatus))
    {
        TC_LOG_ERROR("network", "WorldSession::ValidateMovementStatus: Player (%s) has sent a packet (Opcode: %s) with missing expected movement flags.", _player->GetName().c_str(), GetOpcodeNameForLogging(OpcodeClient(opcode)).c_str());
        return false;
    }

    // Sanitize movement flags
    {
        if (!mover->GetCharmedOrSelf()->GetVehicleBase() || !(mover->GetCharmedOrSelf()->GetVehicle()->GetVehicleInfo()->Flags & VEHICLE_FLAG_FIXED_POSITION))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_ROOT);

        /*! This must be a packet spoofing attempt. MOVEMENTFLAG_ROOT sent from the client is not valid
            in conjunction with any of the moving movement flags such as MOVEMENTFLAG_FORWARD.
            It will freeze clients that receive this player's movement info.*/

        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_ROOT) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_MASK_MOVING))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_MASK_MOVING);

        //! Cannot hover without SPELL_AURA_HOVER
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_HOVER) && !mover->HasAuraType(SPELL_AURA_HOVER))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_HOVER);

        //! Cannot ascend and descend at the same time
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_ASCENDING) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_DESCENDING))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING));

        //! Cannot move left and right at the same time
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_LEFT) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_RIGHT))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_LEFT | MOVEMENTFLAG_RIGHT));

        //! Cannot strafe left and right at the same time
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_STRAFE_LEFT) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_STRAFE_RIGHT))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT));

        //! Cannot pitch up and down at the same time
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_PITCH_UP) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_PITCH_DOWN))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN));

        //! Cannot move forwards and backwards at the same time
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_FORWARD) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_BACKWARD))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD));

        //! Cannot walk on water without SPELL_AURA_WATER_WALK except for ghosts
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_WATERWALKING) && !mover->HasAuraType(SPELL_AURA_WATER_WALK) && !mover->HasAuraType(SPELL_AURA_GHOST))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_WATERWALKING);

        //! Cannot feather fall without SPELL_AURA_FEATHER_FALL
        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_FALLING_SLOW) && !mover->GetCharmedOrSelf()->HasAuraType(SPELL_AURA_FEATHER_FALL))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_FALLING_SLOW);

        /*! Cannot fly if no fly auras present. Exception is being a GM.
            Note that we check for account level instead of Player::IsGameMaster() because in some
            situations it may be feasable to use .gm fly on as a GM without having .gm on,
            e.g. aerial combat.*/

        //if (movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY)) && player->GetSession()->GetSecurity() == SEC_PLAYER &&
        if (movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY)) &&
            !mover->HasAuraType(SPELL_AURA_FLY) && !mover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
            movementStatus.RemoveMovementFlag0(MovementFlags(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY));

        if (movementStatus.HasMovementFlag0(MovementFlags(MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_CAN_FLY)) && movementStatus.HasMovementFlag0(MOVEMENTFLAG_FALLING))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_FALLING);

        if (movementStatus.HasMovementFlag0(MOVEMENTFLAG_SPLINE_ELEVATION) && G3D::fuzzyEq(movementStatus.StepUpStartElevation, 0.0f))
            movementStatus.RemoveMovementFlag0(MOVEMENTFLAG_SPLINE_ELEVATION);

        // Client first checks if spline elevation != 0, then verifies flag presence
        if (G3D::fuzzyNe(movementStatus.StepUpStartElevation, 0.0f))
            movementStatus.AddMovementFlag0(MOVEMENTFLAG_SPLINE_ELEVATION);
    }

    return true;
}

void WorldSession::ApplyMovementStatus(MovementStatus& movementStatus, MoveStateChange const* stateChange /*= nullptr*/)
{
    // WorldSession::ValidateMovementStatus made sure that we have a valid mover so we can go for a simple ptr check.
    Unit* mover = GetGameClient()->GetActivelyMovedUnit();
    if (!mover)
        return;

    // Correct move time
    int64 movementTime = (int64)movementStatus.MoveTime + _timeSyncClockDelta;
    if (_timeSyncClockDelta == 0 || movementTime < 0 || movementTime > 0xFFFFFFFF)
        movementStatus.MoveTime = GameTime::GetGameTimeMS();
    else
        movementStatus.MoveTime = (uint32)movementTime;

    // Apply position
    Position position = movementStatus.GetPosition();
    position.SetOrientation(movementStatus.Facing != 0.f ? movementStatus.Facing : mover->GetOrientation());
    mover->UpdatePosition(position);

    // Apply movement status
    mover->_movementStatus = movementStatus;

    // Send corresponding MOVE_UPDATE packet
    if (stateChange)
    {
        // Because Cataclysm is using randomized packet structures, we have to use cases here. For WoD onwards you should use one class for all speed changes
        switch (stateChange->MessageID)
        {
            case SMSG_MOVE_SET_RUN_SPEED:           mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateRunSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_RUN_BACK_SPEED:      mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateRunBackSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_WALK_SPEED:          mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateWalkSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_SWIM_SPEED:          mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateSwimSpeed(movementStatus, *stateChange->Speed).Write(), _player);  break;
            case SMSG_MOVE_SET_SWIM_BACK_SPEED:     mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateSwimBackSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_FLIGHT_SPEED:        mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateFlightSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_FLIGHT_BACK_SPEED:   mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateFlightBackSpeed(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_TURN_RATE:           mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateTurnRate(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_PITCH_RATE:          mover->SendMessageToSet(WorldPackets::Movement::MoveUpdatePitchRate(movementStatus, *stateChange->Speed).Write(), _player); break;
            case SMSG_MOVE_SET_COLLISION_HEIGHT:    mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateCollisionHeight(movementStatus, stateChange->CollisionHeight->Height).Write(), _player); break;
            case SMSG_MOVE_KNOCK_BACK:              mover->SendMessageToSet(WorldPackets::Movement::MoveUpdateKnockBack(movementStatus).Write(), _player); break;
            default:
                mover->SendMessageToSet(WorldPackets::Movement::MoveUpdate(movementStatus).Write(), _player);
                break;
        }
    }
    else
        mover->SendMessageToSet(WorldPackets::Movement::MoveUpdate(movementStatus).Write(), _player);
}
