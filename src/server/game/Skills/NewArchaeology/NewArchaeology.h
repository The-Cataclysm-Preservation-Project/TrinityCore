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

#ifndef Archaeology_h__
#define Archaeology_h__

#include "Define.h"
#include "DatabaseEnvFwd.h"
#include "ResearchSite.h"
#include "ResearchHistory.h"

class Player;
struct ResearchProjectEntry;

enum class ResearchBranches : uint32;

namespace WorldPackets::Archaeology
{
    class SetupResearchHistory;
}

namespace WorldPackets::Spells
{
    struct SpellWeight;
}

// Aliases
using ResearchSitesMap = std::unordered_map<uint32 /*continentID*/, ResearchSites>;
using ResearchProjectMap = std::unordered_map<uint32 /*researchBranchRecId*/, ResearchProjectEntry const*>;

class TC_GAME_API NewArchaeology
{
public:
    NewArchaeology() = delete;
    NewArchaeology(NewArchaeology const&) = delete;
    NewArchaeology(NewArchaeology&&) = delete;

    NewArchaeology& operator= (NewArchaeology const&) = delete;
    NewArchaeology& operator= (NewArchaeology&&) = delete;

    NewArchaeology(Player* owner);
    ~NewArchaeology() { }

    void LoadResearchHistory(PreparedQueryResult result);
    void LoadResearchSites(PreparedQueryResult result);
    void LoadResearchProjects(PreparedQueryResult result);

    void BuildResearchHistoryPacket(WorldPackets::Archaeology::SetupResearchHistory& packet);

    void GenerateResearchSites();
    void GenerateResearchSitesForContinent(uint32 continentId);

    void StartResearchProjectForBranch(ResearchBranches researchBranch);
    bool HasResearchProjectActive(ResearchProjectEntry const* projectEntry) const;
    void CompleteResearchProject(uint32 researchProjectRecId, std::vector<WorldPackets::Spells::SpellWeight> const& weights);

private:
    Player* _owner;
    ResearchHistory _researchHistory;
    ResearchSitesMap _researchSites;
    ResearchProjectMap _researchProjects;
};

#endif
