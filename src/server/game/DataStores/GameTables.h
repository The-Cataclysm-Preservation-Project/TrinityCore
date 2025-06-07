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

#ifndef __GAMETABLES_H
#define __GAMETABLES_H

#include "Define.h"

enum UnitClass : uint8;

struct GtNpcTotalHpEntry
{
    float Warrior = 0.0f;
    float Paladin = 0.0f;
    float Hunter = 0.0f;
    float Rogue = 0.0f;
    float Priest = 0.0f;
    float DeathKnight = 0.0f;
    float Shaman = 0.0f;
    float Mage = 0.0f;
    float Warlock = 0.0f;
    float Monk = 0.0f;
    float Druid = 0.0f;
};

struct GtNpcDamageByClassEntry
{
    float Warrior = 0.0f;
    float Paladin = 0.0f;
    float Hunter = 0.0f;
    float Rogue = 0.0f;
    float Priest = 0.0f;
    float DeathKnight = 0.0f;
    float Shaman = 0.0f;
    float Mage = 0.0f;
    float Warlock = 0.0f;
    float Monk = 0.0f;
    float Druid = 0.0f;
};

template<class T>
class GameTable
{
public:
    T const* GetRow(uint32 row) const
    {
        if (row >= _data.size())
            return nullptr;

        return &_data[row];
    }

    std::size_t GetTableRowCount() const { return _data.size(); }

    void SetData(std::vector<T> data) { _data = std::move(data); }

private:
    std::vector<T> _data;
};

class GameTables
{
public:
    // Helper Methods
    static float GetSpellScalingValue(uint8 level, UnitClass unitClass);
    static float GetNpcTotalHpValue(uint8 level, UnitClass unitClass, Expansions expansion);
};

TC_GAME_API extern GameTable<GtNpcTotalHpEntry> sNpcTotalHpGameTable[MAX_EXPANSIONS];
TC_GAME_API extern GameTable<GtNpcDamageByClassEntry> sNpcDamageByClassGameTable[MAX_EXPANSIONS];

template<class T>
inline float GetGameTableColumnForClass(T const* row, int32 class_)
{
    switch (class_)
    {
        case CLASS_WARRIOR:
            return row->Warrior;
        case CLASS_PALADIN:
            return row->Paladin;
        case CLASS_HUNTER:
            return row->Hunter;
        case CLASS_ROGUE:
            return row->Rogue;
        case CLASS_PRIEST:
            return row->Priest;
        case CLASS_DEATH_KNIGHT:
            return row->DeathKnight;
        case CLASS_SHAMAN:
            return row->Shaman;
        case CLASS_MAGE:
            return row->Mage;
        case CLASS_WARLOCK:
            return row->Warlock;
        //case CLASS_MONK:
        //    return row->Monk;
        case CLASS_DRUID:
            return row->Druid;
        default:
            break;
    }

    return 0.0f;
}

#endif // __GAMETABLES_H
