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

#ifndef ResearchSite_h__
#define ResearchSite_h__

#include "Define.h"
#include <array>

// The client allows up to 16 research sites in its update fields. As of 4.3.4.15595 we only have 4 continents to care about which makes 4 sites per continent
// In later expansions they have turned the update field into an dynamic value so this limit is due to technical limitations
constexpr uint8 const MAX_RESEARCH_SITES_PER_CONTINENT = 4u;

// Default value for research site finds. Blizzard has never changed it over the years but we might turn it into a worldserver config value one day for custom settings
constexpr uint8 const MAX_FINDS_PER_RESEARCH_SITE = 3u;

struct ResearchSite
{
    void Set(uint32 recId, uint32 remainingFinds);

    uint32 RecID = 0u;
    uint32 RemainingFinds = 0u;
};

struct ResearchSites
{
    void SetActiveResearchSite(uint8 index, uint32 researchSiteRecId);
    bool GetNextUnusedResearchSiteIndex(uint8& index) const;

    bool HasUnusedResearchSite() const;
    bool HasResearchSiteActive(uint32 researchSiteRecId) const;

    ResearchSite& GetResearchSite(uint8 index) { return _storage[index]; }

private:
    std::array<ResearchSite, MAX_RESEARCH_SITES_PER_CONTINENT> _storage = { };
};

#endif // ResearchSite_h__
