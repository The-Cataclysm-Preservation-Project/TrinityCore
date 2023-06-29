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

#ifndef ResearchHistory_h__
#define ResearchHistory_h__

#include "Define.h"
#include "Optional.h"

struct ResearchProjectEntry;
enum class ResearchProjectRarity : uint32;

struct ResearchHistoryInfo
{
    ResearchHistoryInfo() = delete;
    ResearchHistoryInfo(ResearchProjectEntry const* projectEntry, uint32 completionCount, uint32 firstCompletionTime) :
        ProjectEntry(projectEntry), CompletionCount(completionCount), FirstCompletionTime(firstCompletionTime) { }

    ResearchProjectEntry const* ProjectEntry;
    uint32 CompletionCount;
    uint32 FirstCompletionTime;
};

struct ResearchHistory
{
    bool HasCompletedAllRareProjectsOfBranch(uint32 researchBranchRecId) const;
    bool HasCompletedProject(ResearchProjectEntry const* projectEntry) const;
    ResearchHistoryInfo* StoreProjectInHistory(ResearchProjectEntry const* projectEntry);

    std::unordered_map<uint32 /*researchBranchRectId*/, std::vector<ResearchHistoryInfo>> History;

private:
    ResearchHistoryInfo* GetHistoryInfoForProject(ResearchProjectEntry const* projectEntry);
};

#endif // ResearchHistory_h__
