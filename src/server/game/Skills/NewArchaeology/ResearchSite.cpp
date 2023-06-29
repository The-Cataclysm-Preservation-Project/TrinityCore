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


#include "ResearchSite.h"

void ResearchSite::Set(uint32 recId, uint32 remainingFinds)
{
    RecID = recId;
    RemainingFinds = remainingFinds;
}

void ResearchSites::SetActiveResearchSite(uint8 index, uint32 researchSiteRecId)
{
    _storage[index].Set(researchSiteRecId, MAX_FINDS_PER_RESEARCH_SITE);
}

bool ResearchSites::GetNextUnusedResearchSiteIndex(uint8& index) const
{
    for (size_t i = 0; i < _storage.size(); ++i)
    {
        if (_storage[i].RecID == 0u)
        {
            index = i;
            return true;
        }

    }
    return false;
}

bool ResearchSites::HasUnusedResearchSite() const
{
    for (size_t i = 0; i < _storage.size(); ++i)
        if (_storage[i].RecID == 0u)
            return true;
    return false;
}

bool ResearchSites::HasResearchSiteActive(uint32 researchSiteRecId) const
{
    for (size_t i = 0; i < _storage.size(); ++i)
        if (_storage[i].RecID == researchSiteRecId)
            return true;
    return false;
}
