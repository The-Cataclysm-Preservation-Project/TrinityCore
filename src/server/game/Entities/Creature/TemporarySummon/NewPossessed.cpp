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

#include "NewPossessed.h"
#include "CharmInfo.h"
#include "DBCStores.h"

NewPossessed::NewPossessed(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isTotem) :
    NewTemporarySummon(properties, summoner, isWorldObject, isTotem)
{
    m_unitTypeMask |= UNIT_MASK_POSSESSED;
    InitCharmInfo();
}

void NewPossessed::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    RemoveCharmedBy(nullptr);
    NewTemporarySummon::RemoveFromWorld();
}

bool NewPossessed::HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons)
{
    if (!summoner)
        return false;

    if (!NewTemporarySummon::HandlePreSummonActions(summoner, creatureLevel, maxSummons))
        return false;

    SetCreatorGUID(summoner->GetGUID());
    SetLevel(summoner->getLevel());
    SetFaction(summoner->GetFaction());
    SetReactState(REACT_PASSIVE);

    return true;
}

void NewPossessed::HandlePostSummonActions()
{
    if (Unit* summoner = GetInternalSummoner())
    {
        if (!SetCharmedBy(summoner, CHARM_TYPE_POSSESS))
            ABORT();
    }
    else
        Unsummon();
}
