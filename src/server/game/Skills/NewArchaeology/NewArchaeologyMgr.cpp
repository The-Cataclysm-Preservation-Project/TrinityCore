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

#include "NewArchaeologyMgr.h"
#include "NewArchaeology.h"
#include "Containers.h"
#include "Log.h"

NewArchaeologyMgr* NewArchaeologyMgr::instance()
{
    static NewArchaeologyMgr instance;
    return &instance;
}

void NewArchaeologyMgr::Load()
{
    TC_LOG_INFO("server.loading", "Initializing Archaeology research projects...");
    InitializeResearchProjects();
    TC_LOG_INFO("server.loading", "Loading Archaeology research sites...");
    LoadResearchSites();
    TC_LOG_INFO("server.loading", "Loading Archaeology research site finds...");
    LoadResearchSiteFinds();
    TC_LOG_INFO("server.loading", "Loading Archaeology research project requirements...");
    LoadResearchProjectRequirements();
}

void NewArchaeologyMgr::InitializeResearchProjects()
{
    for (ResearchProjectEntry const* researchProject : sResearchProjectStore)
    {
        // ResearchBranches::Other is a debug branch that must be skipped
        if (static_cast<ResearchBranches>(researchProject->ResearchBranchID) == ResearchBranches::Other)
            continue;

        _researchProjectsForBranch[researchProject->ResearchBranchID].push_back(researchProject);
    }
}

void NewArchaeologyMgr::LoadResearchSites()
{
    uint32 oldMSTime = getMSTime();

    //                                               0      1
    QueryResult result = WorldDatabase.Query("SELECT RecID, ResearchBranchRecID FROM research_sites");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 research site definitions. DB table `research_sites` is empty.");
        return;
    }

    uint32 count = 0u;
    do
    {
        Field* fields = result->Fetch();
        uint32 researchSiteRecId = fields[0].GetUInt32();
        ResearchSiteEntry const* siteEntry = sResearchSiteStore.LookupEntry(researchSiteRecId);
        if (!siteEntry)
        {
            TC_LOG_INFO("server.loading", "Research site (ID: %u) defined in `research_sites` does not exist in dbc files. Skipped.", researchSiteRecId);
            continue;
        }

        // Store the continent map IDs here for auto-generated update field offsets later
        _researchSiteContinents.insert(siteEntry->MapID);

        uint32 researchBranchRecId = fields[1].GetUInt32();
        if (!sResearchBranchStore.LookupEntry(researchBranchRecId) || static_cast<ResearchBranches>(researchBranchRecId) == ResearchBranches::Other)
        {
            TC_LOG_INFO("server.loading", "Research branch (ID: %u) defined in `research_sites` does not exist in dbc files or is invalid. Skipped.", researchBranchRecId);
            continue;
        }

        _researchSitesForBranch[researchBranchRecId].push_back(siteEntry);

        // Store the research branch for the continent to speed up the generation of research sites later
        _researchBranchesForContinent[siteEntry->MapID].insert(researchBranchRecId);

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u research site definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void NewArchaeologyMgr::LoadResearchSiteFinds()
{
    uint32 oldMSTime = getMSTime();

    //                                               0                    1
    QueryResult result = WorldDatabase.Query("SELECT ResearchBranchRecID, FindGameObjectID FROM research_site_finds");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 research site find definitions. DB table `research_site_finds` is empty.");
        return;
    }

    uint32 count = 0u;
    do
    {
        Field* fields = result->Fetch();
        uint32 researchBranchRecId = fields[0].GetUInt32();
        if (!sResearchBranchStore.LookupEntry(researchBranchRecId) || static_cast<ResearchBranches>(researchBranchRecId) == ResearchBranches::Other)
        {
            TC_LOG_INFO("server.loading", "Research branch (ID: %u) defined in `research_site_finds` does not exist in dbc files or is invalid. Skipped.", researchBranchRecId);
            continue;
        }

        uint32 findGameObjectId = fields[1].GetUInt32();
        if (!sObjectMgr->GetGameObjectTemplate(findGameObjectId))
        {
            TC_LOG_INFO("server.loading", "GameObject find (ID: %u) defined in `research_site_finds` does not exist. Skipped.", researchBranchRecId);
            continue;
        }

        _researchSiteFindsForBranch[researchBranchRecId] = findGameObjectId;

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u research site find gameobject definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void NewArchaeologyMgr::LoadResearchProjectRequirements()
{
    uint32 oldMSTime = getMSTime();

    //                                               0                     1
    QueryResult result = WorldDatabase.Query("SELECT ResearchProjectRecID, RequiredSkillValue FROM research_project_requirements");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 research project requirements. DB table `research_project_requirements` is empty.");
        return;
    }

    uint32 count = 0u;
    do
    {
        Field* fields = result->Fetch();
        uint32 researchProjectRecId = fields[0].GetUInt32();
        if (!sResearchProjectStore.LookupEntry(researchProjectRecId))
        {
            TC_LOG_INFO("server.loading", "Research project (ID: %u) defined in `research_project_requirements` does not exist in dbc files or is invalid. Skipped.", researchProjectRecId);
            continue;
        }

        _researchProjectRequirements[researchProjectRecId] = fields[1].GetUInt32();

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u research project requirements in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

std::unordered_set<uint32> const* NewArchaeologyMgr::GetResearchBranchesForContinent(uint32 continentId) const
{
    return Trinity::Containers::MapGetValuePtr(_researchBranchesForContinent, continentId);
}

std::vector<ResearchSiteEntry const*>* NewArchaeologyMgr::GetResearchSitesForBranch(uint32 researchBranchRecId)
{
    return Trinity::Containers::MapGetValuePtr(_researchSitesForBranch, researchBranchRecId);
}

std::vector<ResearchProjectEntry const*>* NewArchaeologyMgr::GetResearchProjectsForBranch(uint32 researchBranchRecId)
{
    return Trinity::Containers::MapGetValuePtr(_researchProjectsForBranch, researchBranchRecId);
}

uint8 NewArchaeologyMgr::GetResearchSiteFieldIndexForContinent(uint32 continentId, uint8 researchSiteIndex) const
{
    uint8 index = 0u;
    for (uint32 continent : _researchSiteContinents)
    {
        if (continentId == continent)
            return index * MAX_RESEARCH_SITES_PER_CONTINENT + researchSiteIndex;
        else
            ++index;
    }

    return 0u;
}

uint8 NewArchaeologyMgr::GetResearchBranchFieldIndex(ResearchBranches researchBranch) const
{
    uint8 index = 0u;
    for (ResearchBranchEntry const* branchEntry : sResearchBranchStore)
    {
        ResearchBranches branch = static_cast<ResearchBranches>(branchEntry->ID);
        if (branch == ResearchBranches::Other)
            continue;

        if (static_cast<ResearchBranches>(branchEntry->ID) == researchBranch)
            return index;
        else
            ++index;
    }

    return 0u;
}

ResearchBranches NewArchaeologyMgr::GetResearchBranchForCurrency(uint32 currencyID) const
{
    for (ResearchBranchEntry const* branchEntry : sResearchBranchStore)
        if (branchEntry->CurrencyID == currencyID)
            return static_cast<ResearchBranches>(branchEntry->ID);

    return ResearchBranches::None;
}

uint32 NewArchaeologyMgr::GetRequiredSkillValueForResearchProject(uint32 researchProjectRec) const
{
    auto itr = _researchProjectRequirements.find(researchProjectRec);
    if (itr == _researchProjectRequirements.end())
        return 0u;

    return itr->second;
}
