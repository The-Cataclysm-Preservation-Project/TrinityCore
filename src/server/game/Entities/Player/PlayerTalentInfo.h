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

#ifndef __PlayerTalentInfo_h_
#define __PlayerTalentInfo_h_

#include "Define.h"
#include "SharedDefines.h"

struct PlayerTalent;

struct TalentSpecInfo
{
    std::unordered_map<uint32, PlayerTalent> Talents;
    std::array<uint32, MAX_GLYPH_SLOT_INDEX> Glyphs = { };
    uint32 TalentTree = 0;
};

struct PlayerTalentInfo
{
    PlayerTalentInfo() = default;

    std::array<TalentSpecInfo, MAX_TALENT_SPECS> SpecInfo = { };
    uint32 FreeTalentPoints = 0;
    uint32 UsedTalentCount = 0;
    uint32 QuestRewardedTalentCount = 0;
    uint32 ResetTalentsCost = 0;
    time_t ResetTalentsTime = 0;
    uint8 ActiveSpec = 0;
    uint8 SpecsCount = 0;
};

#endif // __PlayerTalentInfo_h_
