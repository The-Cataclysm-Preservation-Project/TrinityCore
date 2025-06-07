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

#include "GameTables.h"
#include "DBCStores.h"
#include "DBCStructure.h"

GameTable<GtNpcTotalHpEntry> sNpcTotalHpGameTable[MAX_EXPANSIONS];

/*static*/ float GameTables::GetSpellScalingValue(uint8 level, UnitClass unitClass)
{
    if (level > GT_MAX_LEVEL)
        return 0.0f;

    GtSpellScalingEntry const* gt = sGtSpellScalingStore.LookupEntry((unitClass - 1) * 100 + level -1);
    if (!gt)
        return 0.0f;

    return gt->value;
}

/*float*/ float GameTables::GetNpcTotalHpValue(uint8 level, UnitClass unitClass, Expansions expansion)
{
    if (level > GT_MAX_LEVEL || expansion >= MAX_EXPANSIONS)
        return 0.0f;

    return GetGameTableColumnForClass(sNpcTotalHpGameTable[expansion].GetRow(level), unitClass);
}
