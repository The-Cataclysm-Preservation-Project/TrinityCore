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

#ifndef BattlegroundPackets_h__
#define BattlegroundPackets_h__

#include "LFGPacketsCommon.h"
#include <array>

namespace WorldPackets
{
    namespace Battleground
    {
        class PVPOptionsEnabled final : public ServerPacket
        {
        public:
            PVPOptionsEnabled() : ServerPacket(SMSG_PVP_OPTIONS_ENABLED, 1) { }

            WorldPacket const* Write() override;

            bool RatedArenas = false;
            bool WargameBattlegrounds = false;
            bool RatedBattlegrounds = false;
        };

        struct ClientBracketInfo
        {
            int32 WeeklyPlayed = 0;
            int32 WeeklyWon = 0;
            int32 SeasonPlayed = 0;
            int32 SeasonWon = 0;
            int32 TotalPlayed = 0;
            int32 TotalWon = 0;
        };

        class RatedBattlefieldInfo final : public ServerPacket
        {
        public:
            RatedBattlefieldInfo() : ServerPacket(SMSG_RATED_BG_STATS, 72) { }

            WorldPacket const* Write() override;

            std::array<ClientBracketInfo, 3> Brackets;
        };

        class BattlefieldRatedInfo final : public ServerPacket
        {
        public:
            BattlefieldRatedInfo() : ServerPacket(SMSG_BATTLEFIELD_RATED_INFO, 21) { }

            WorldPacket const* Write() override;

            int32 Reward = 0;
            int32 PersonalRating = 0;
            int32 RatedMaxRewardPointsThisWeek = 0;
            int32 MaxRewardPointsThisWeek = 0;
            int32 RewardPointsThisWeek = 0;
            int32 RatedRewardPointsThisWeek = 0;
            int8 Mode = 0; // 0 = arena 2v2, 1 = arena 3v3, 2 = arena 5v5, 3 = rated battleground
        };

        class BattlefieldList final : public ServerPacket
        {
        public:
            BattlefieldList() : ServerPacket(SMSG_BATTLEFIELD_LIST, 35) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlemasterGuid;
            int32 BattlemasterListID = 0;
            int32 ConquestBonusRandom = 0;
            int32 ConquestBonusHoliday = 0;
            int32 HonorBonusRandomWin = 0;
            int32 HonorBonusRandomLoss = 0;
            int32 HonorBonusHolidayWin = 0;
            int32 HonorBonusHolidayLoss = 0;
            int32 HonorLossBonus = 0;
            int32 HonorWinBonus = 0;
            uint8 MinLevel = 0;
            uint8 MaxLevel = 0;
            std::vector<int32> Battlefields;
            bool PvpAnywhere = false;
            bool IsRandomBG = false;
            bool HasRandomWinToday = false;
            bool HasHolidayWinToday = false;
        };

        struct BattlefieldStatusHeader
        {
            WorldPackets::LFG::RideTicket Ticket;
            ObjectGuid QueueID;
            uint8 RangeMin = 0;
            uint8 RangeMax = 0;
            uint8 TeamSize = 0;
            uint32 InstanceID = 0;
            bool RegisteredMatch = false;
            bool TournamentRules = false;
        };

        class BattlefieldStatusNone final : public ServerPacket
        {
        public:
            BattlefieldStatusNone() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NONE, 8 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            WorldPackets::LFG::RideTicket Ticket;
        };

        class BattlefieldStatusNeedConfirmation final : public ServerPacket
        {
        public:
            BattlefieldStatusNeedConfirmation() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 4 + 4 + sizeof(BattlefieldStatusHeader) + 1) { }

            WorldPacket const* Write() override;

            uint8 Role = 0;
            uint32 Timeout = 0;
            uint32 Mapid = 0;
            BattlefieldStatusHeader Hdr;
        };

        class BattlefieldStatusActive final : public ServerPacket
        {
        public:
            BattlefieldStatusActive() : ServerPacket(SMSG_BATTLEFIELD_STATUS_ACTIVE, sizeof(BattlefieldStatusHeader) + 4 + 1 + 1 + 4 + 4) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Hdr;
            uint32 ShutdownTimer = 0;
            uint8 ArenaFaction = 0;
            uint32 StartTimer = 0;
            uint32 Mapid = 0;
        };

        class BattlefieldStatusQueued final : public ServerPacket
        {
        public:
            BattlefieldStatusQueued() : ServerPacket(SMSG_BATTLEFIELD_STATUS_QUEUED, 4 + sizeof(BattlefieldStatusHeader) + 1 + 1 + 1 + 4) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Hdr;
            uint32 WaitTime = 0;
            uint32 AverageWaitTime = 0;
            bool AsGroup = false;
            bool SuspendedQueue = false;
            bool EligibleForMatchmaking = false;
        };
    }

}

#endif // BattlegroundPackets_h__
