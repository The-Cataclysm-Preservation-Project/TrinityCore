
#include "ResearchHistory.h"
#include "NewArchaeologyMgr.h"
#include "DBCStores.h"
#include "DBCEnums.h"
#include "GameTime.h"

bool ResearchHistory::HasCompletedAllRareProjectsOfBranch(uint32 researchBranchRecId) const
{
    std::vector<ResearchProjectEntry const*>* projects = sNewArchaeologyMgr->GetResearchProjectsForBranch(researchBranchRecId);
    if (!projects)
        return false;

    uint32 maximumRares = std::count_if(projects->begin(), projects->end(), [](ResearchProjectEntry const* researchEntry)
    {
        return static_cast<ResearchProjectRarity>(researchEntry->Rarity) == ResearchProjectRarity::Rare;
    });

    auto itr = History.find(researchBranchRecId);
    if (itr == History.end())
        return false;

    uint32 completedRares = std::count_if(itr->second.begin(), itr->second.end(), [](ResearchHistoryInfo const& historyInfo)
    {
        return static_cast<ResearchProjectRarity>(historyInfo.ProjectEntry->Rarity) == ResearchProjectRarity::Rare;
    });

    return maximumRares == completedRares;
}

bool ResearchHistory::HasCompletedProject(ResearchProjectEntry const* projectEntry) const
{
    auto itr = History.find(projectEntry->ResearchBranchID);
    if (itr == History.end())
        return false;

    return std::find_if(itr->second.begin(), itr->second.end(), [projectEntry](ResearchHistoryInfo const& historyEntry) { return projectEntry->ID == historyEntry.ProjectEntry->ID;}) != itr->second.end();
}

ResearchHistoryInfo* ResearchHistory::StoreProjectInHistory(ResearchProjectEntry const* projectEntry)
{
    // We have completed the project before. Just update its completion counter and move on
    if (ResearchHistoryInfo* info = GetHistoryInfoForProject(projectEntry))
    {
        ++info->CompletionCount;
        return info;
    }

    ResearchHistoryInfo& itr = History[projectEntry->ResearchBranchID].emplace_back(projectEntry, 1, GameTime::GetGameTime());
    return &itr;
}

ResearchHistoryInfo* ResearchHistory::GetHistoryInfoForProject(ResearchProjectEntry const* projectEntry)
{
    auto itr = History.find(projectEntry->ResearchBranchID);
    if (itr == History.end())
        return nullptr;

    auto it = std::find_if(itr->second.begin(), itr->second.end(), [projectEntry](ResearchHistoryInfo const& historyEntry) { return projectEntry->ID == historyEntry.ProjectEntry->ID; });
    if (it == itr->second.end())
        return nullptr;

    return &(*it);
}
