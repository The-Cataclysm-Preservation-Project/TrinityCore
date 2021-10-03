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

#include "BattlegroundPackets.h"

WorldPacket const* WorldPackets::Battleground::PVPOptionsEnabled::Write()
{
    _worldPacket.WriteBit(1); // ignored by the client
    _worldPacket.WriteBit(WargameBattlegrounds);
    _worldPacket.WriteBit(1); // ignored by the client
    _worldPacket.WriteBit(RatedBattlegrounds);
    _worldPacket.WriteBit(RatedArenas);
    _worldPacket.FlushBits();
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::RatedBattlefieldInfo::Write()
{
    _worldPacket << int32(Brackets[2].WeeklyWon);       // BgWeeklyWins25vs25
    _worldPacket << int32(Brackets[2].WeeklyPlayed);    // BgWeeklyPlayed25vs25
    _worldPacket << int32(Brackets[1].WeeklyPlayed);    // BgWeeklyPlayed15vs15
    _worldPacket << int32(Brackets[1].SeasonPlayed);    // BgSeasonalPlayed15vs15
    _worldPacket << int32(Brackets[0].WeeklyWon);       // BgWeeklyWins10vs10
    _worldPacket << int32(Brackets[2].SeasonPlayed);    // BgSeasonalPlayed25vs25
    _worldPacket << int32(Brackets[1].SeasonWon);       // BgSeasonalWins15vs15
    _worldPacket << int32(Brackets[0].TotalWon);        // BgTotalWins10vs10
    _worldPacket << int32(Brackets[1].WeeklyWon);       // BgWeeklyWins15vs15
    _worldPacket << int32(Brackets[2].TotalWon);        // BgTotalWins25vs25
    _worldPacket << int32(Brackets[0].SeasonWon);       // BgSeasonalWins10vs10
    _worldPacket << int32(Brackets[1].TotalWon);        // BgTotalWins15vs15
    _worldPacket << int32(Brackets[0].SeasonPlayed);    // BgSeasonalPlayed10vs10
    _worldPacket << int32(Brackets[2].SeasonWon);       // BgSeasonalWins25vs25
    _worldPacket << int32(Brackets[0].TotalPlayed);     // BgTotalPlayed10vs10
    _worldPacket << int32(Brackets[0].WeeklyPlayed);    // BgWeeklyPlayed10vs10
    _worldPacket << int32(Brackets[2].TotalPlayed);     // BgTotalPlayed25vs25
    _worldPacket << int32(Brackets[1].TotalPlayed);     // BgTotalPlayed15vs15

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldRatedInfo::Write()
{
    _worldPacket << int32(Reward);
    _worldPacket << int8(Mode);
    _worldPacket << int32(PersonalRating);
    _worldPacket << int32(RewardPointsThisWeek);
    _worldPacket << int32(RatedMaxRewardPointsThisWeek);    // this and MaxRewardPointsThisWeek are weekly conquest limits but not sure which one is for rated BGs
    _worldPacket << int32(RatedRewardPointsThisWeek);
    _worldPacket << int32(0);                               // values are being sent in sniffs but this field is unused in the client and interface code
    _worldPacket << int32(MaxRewardPointsThisWeek);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldList::Write()
{
    _worldPacket << int32(ConquestBonusHoliday);
    _worldPacket << int32(ConquestBonusRandom);
    _worldPacket << int32(HonorBonusHolidayLoss);
    _worldPacket << int32(BattlemasterListID);
    _worldPacket << int32(HonorBonusRandomLoss);
    _worldPacket << int32(HonorBonusRandomWin);
    _worldPacket << int32(HonorBonusHolidayWin);
    _worldPacket << uint8(MaxLevel);
    _worldPacket << uint8(MinLevel);

    _worldPacket.WriteBit(BattlemasterGuid[0]);
    _worldPacket.WriteBit(BattlemasterGuid[1]);
    _worldPacket.WriteBit(BattlemasterGuid[7]);
    _worldPacket.WriteBit(HasHolidayWinToday);
    _worldPacket.WriteBit(HasRandomWinToday);
    _worldPacket.WriteBits(Battlefields.size(), 24);
    _worldPacket.WriteBit(BattlemasterGuid[6]);
    _worldPacket.WriteBit(BattlemasterGuid[4]);
    _worldPacket.WriteBit(BattlemasterGuid[2]);
    _worldPacket.WriteBit(BattlemasterGuid[3]);
    _worldPacket.WriteBit(IsRandomBG);
    _worldPacket.WriteBit(BattlemasterGuid[5]);
    _worldPacket.WriteBit(PvpAnywhere);
    _worldPacket.FlushBits();

    _worldPacket.WriteByteSeq(BattlemasterGuid[6]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[1]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[7]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[5]);

    for (int32 battlefield : Battlefields)
        _worldPacket << int32(battlefield);

    _worldPacket.WriteByteSeq(BattlemasterGuid[0]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[2]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[4]);
    _worldPacket.WriteByteSeq(BattlemasterGuid[3]);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusNone::Write()
{
    _worldPacket.WriteBit(Ticket.RequesterGuid[0]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[4]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[7]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[1]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[6]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[3]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[5]);
    _worldPacket.WriteBit(Ticket.RequesterGuid[2]);

    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[5]);
    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[6]);
    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[7]);
    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[2]);

    _worldPacket << uint32(Ticket.Type);

    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[3]);
    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[1]);

    _worldPacket << uint32(Ticket.Id);
    _worldPacket << int32(Ticket.Time);

    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[0]);
    _worldPacket.WriteByteSeq(Ticket.RequesterGuid[4]);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusNeedConfirmation::Write()
{
    _worldPacket << uint32(Hdr.InstanceID);
    _worldPacket << uint32(Timeout);
    _worldPacket << uint8(Hdr.RangeMax); // @todo: validate
    _worldPacket << uint32(Hdr.Ticket.Id);
    _worldPacket << int32(Hdr.Ticket.Time);
    _worldPacket << uint8(Hdr.RangeMin); // @todo: validate
    _worldPacket << uint32(Hdr.Ticket.Type);
    _worldPacket << uint32(Mapid);
    _worldPacket << uint8(Role); // Roles?

    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[5]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[2]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[1]);
    _worldPacket.WriteBit(Hdr.QueueID[2]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[4]);
    _worldPacket.WriteBit(Hdr.QueueID[6]);
    _worldPacket.WriteBit(Hdr.QueueID[3]);
    _worldPacket.WriteBit(Hdr.RegisteredMatch);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[7]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[3]);
    _worldPacket.WriteBit(Hdr.QueueID[7]);
    _worldPacket.WriteBit(Hdr.QueueID[0]);
    _worldPacket.WriteBit(Hdr.QueueID[4]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[6]);
    _worldPacket.WriteBit(Hdr.QueueID[1]);
    _worldPacket.WriteBit(Hdr.QueueID[5]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[0]);

    _worldPacket.FlushBits();

    _worldPacket.WriteByteSeq(Hdr.QueueID[6]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[5]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[7]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[2]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[0]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[7]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[4]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[1]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[0]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[4]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[1]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[5]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[3]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[6]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[2]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[3]);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusActive::Write()
{
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[2]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[7]);
    _worldPacket.WriteBit(Hdr.QueueID[7]);
    _worldPacket.WriteBit(Hdr.QueueID[1]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[5]);
    _worldPacket.WriteBit(ArenaFaction); // 0 = Horde, 1 = Alliance
    _worldPacket.WriteBit(Hdr.QueueID[0]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[1]);
    _worldPacket.WriteBit(Hdr.QueueID[3]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[6]);
    _worldPacket.WriteBit(Hdr.QueueID[5]);
    _worldPacket.WriteBit(Hdr.RegisteredMatch);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[4]);
    _worldPacket.WriteBit(Hdr.QueueID[6]);
    _worldPacket.WriteBit(Hdr.QueueID[4]);
    _worldPacket.WriteBit(Hdr.QueueID[2]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[3]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[0]);

    _worldPacket.FlushBits();

    _worldPacket.WriteByteSeq(Hdr.QueueID[4]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[5]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[5]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[1]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[6]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[3]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[7]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[6]);

    _worldPacket << int32(Hdr.Ticket.Time);
    _worldPacket << uint8(Hdr.TeamSize); // @todo: validate
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[4]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[1]);

    _worldPacket << uint32(Hdr.Ticket.Id);
    _worldPacket << uint8(Hdr.RangeMax); // @todo: validate
    _worldPacket << uint32(Hdr.Ticket.Type);
    _worldPacket << uint32(Mapid);
    _worldPacket << uint8(Hdr.RangeMin);
    _worldPacket << uint32(StartTimer); // @todo: validate

    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[2]);

    _worldPacket << uint32(ShutdownTimer); // @todo: validate

    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[0]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[3]);

    _worldPacket.WriteByteSeq(Hdr.QueueID[2]);

    _worldPacket << uint32(Hdr.InstanceID);

    _worldPacket.WriteByteSeq(Hdr.QueueID[0]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[7]);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusQueued::Write()
{
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[3]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[0]);
    _worldPacket.WriteBit(Hdr.QueueID[3]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[2]);
    _worldPacket.WriteBit(EligibleForMatchmaking);
    _worldPacket.WriteBit(SuspendedQueue); // @todo: validate
    _worldPacket.WriteBit(Hdr.QueueID[2]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[1]);
    _worldPacket.WriteBit(Hdr.QueueID[0]);
    _worldPacket.WriteBit(Hdr.QueueID[6]);
    _worldPacket.WriteBit(Hdr.QueueID[4]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[6]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[7]);
    _worldPacket.WriteBit(Hdr.QueueID[7]);
    _worldPacket.WriteBit(Hdr.QueueID[5]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[4]);
    _worldPacket.WriteBit(Hdr.Ticket.RequesterGuid[5]);
    _worldPacket.WriteBit(Hdr.RegisteredMatch);
    _worldPacket.WriteBit(AsGroup); // @todo: validate
    _worldPacket.WriteBit(Hdr.QueueID[1]);

    _worldPacket.FlushBits();

    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[0]);

    _worldPacket << uint32(Hdr.Ticket.Type);

    _worldPacket.WriteByteSeq(Hdr.QueueID[5]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[3]);

    _worldPacket << uint32(AverageWaitTime);

    _worldPacket.WriteByteSeq(Hdr.QueueID[7]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[1]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[2]);

    _worldPacket << uint8(0); // @todo: research

    _worldPacket.WriteByteSeq(Hdr.QueueID[4]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[2]);

    _worldPacket << uint8(0); // @todo: research

    _worldPacket.WriteByteSeq(Hdr.QueueID[6]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[7]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[3]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[6]);
    _worldPacket.WriteByteSeq(Hdr.QueueID[0]);

    _worldPacket << int32(Hdr.Ticket.Time);
    _worldPacket << uint32(Hdr.Ticket.Id);
    _worldPacket << uint8(Hdr.RangeMin);
    _worldPacket << uint32(WaitTime);

    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[1]);
    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[5]);

    _worldPacket << uint32(Hdr.InstanceID);

    _worldPacket.WriteByteSeq(Hdr.Ticket.RequesterGuid[4]);

    return &_worldPacket;
}
