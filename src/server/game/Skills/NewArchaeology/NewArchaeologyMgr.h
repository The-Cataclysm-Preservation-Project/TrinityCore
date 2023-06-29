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

#ifndef ArchaeologyMgr_h__
#define ArchaeologyMgr_h__

#include "Define.h"

struct ResearchProjectEntry;
struct ResearchSiteEntry;

enum class ResearchBranches : uint32;

class TC_GAME_API NewArchaeologyMgr
{
private:
    NewArchaeologyMgr() { }
    ~NewArchaeologyMgr() { }

public:
    NewArchaeologyMgr(NewArchaeologyMgr const&) = delete;
    NewArchaeologyMgr(NewArchaeologyMgr&&) = delete;

    NewArchaeologyMgr& operator= (NewArchaeologyMgr const&) = delete;
    NewArchaeologyMgr& operator= (NewArchaeologyMgr&&) = delete;

    static NewArchaeologyMgr* instance();

    void Load();

    std::unordered_set<uint32> const& GetResearchSiteContinentIds() const { return _researchSiteContinents; };
    std::unordered_set<uint32> const* GetResearchBranchesForContinent(uint32 continentId) const;
    std::vector<ResearchSiteEntry const*>* GetResearchSitesForBranch(uint32 researchBranchRecId);
    std::vector<ResearchProjectEntry const*>* GetResearchProjectsForBranch(uint32 researchBranchRecId);
    uint8 GetResearchSiteFieldIndexForContinent(uint32 continentId, uint8 researchSiteIndex) const;
    uint8 GetResearchBranchFieldIndex(ResearchBranches researchBranch) const;
    ResearchBranches GetResearchBranchForCurrency(uint32 currencyID) const;
    uint32 GetRequiredSkillValueForResearchProject(uint32 recId) const;

private:
    std::unordered_map<uint32 /*researchBranchRecId*/, std::vector<ResearchSiteEntry const*>> _researchSitesForBranch;
    std::unordered_map<uint32 /*researchBranchRecId*/, std::vector<ResearchProjectEntry const*>> _researchProjectsForBranch;
    std::unordered_map<uint32 /*researchBranchRecId*/, uint32 /*findGameObjectId*/> _researchSiteFindsForBranch;
    std::unordered_map<uint32 /*researchProjectRecId*/, uint32 /*requiredkillValue*/> _researchProjectRequirements;
    std::unordered_set<uint32 /*continentID*/> _researchSiteContinents;
    std::unordered_map<uint32 /*continentID*/, std::unordered_set<uint32>> _researchBranchesForContinent;

    void InitializeResearchProjects();
    void LoadResearchSites();
    void LoadResearchSiteFinds();
    void LoadResearchProjectRequirements();
};

#define sNewArchaeologyMgr NewArchaeologyMgr::instance()

#endif
