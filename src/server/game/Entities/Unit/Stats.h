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

#ifndef _Stats_h__
#define _Stats_h__

#include "Define.h"
#include "Util.h"
#include <array>

class Unit;
enum AuraType : uint32;

enum class StatType : int8
{
    // Primary Stats
    AllPrimaryStats2   = -2, // There is only one spell using this and it also states that it increases all stats
    AllPrimaryStats    = -1,
    Strength            = 0,
    Agility             = 1,
    Stamina             = 2,
    Intellect           = 3,
    Spirit              = 4,
    Max,
};

static constexpr std::array<StatType, AsUnderlyingType(StatType::Max)> AllPrimaryStats =
{
    StatType::Strength,
    StatType::Agility,
    StatType::Stamina,
    StatType::Intellect,
    StatType::Spirit
};

struct StatData
{
    int32 BaseStat = 0;
    int32 BaseStatModifier = 0;
    int32 TotalModifier = 0;
    double BasePctMultiplier = 1.0;
    double TotalPctMultiplier = 1.0;
};

class TC_GAME_API Stats
{
public:
    Stats() = delete;
    Stats(Stats const&) = delete;
    Stats& operator=(Stats const&) = delete;

    ~Stats();
    explicit Stats(Unit* owner);

    int32 GetBaseStatValue(StatType statType) const;
    void SetBaseStatValue(StatType statType, int32 value);

    void AddBaseStatModifier(StatType statType, int32 value);
    void RemoveBaseStatModifier(StatType statType, int32 value);

    void UpdateBaseStatMultiplier(StatType statType);
    void UpdateTotalStatModifier(StatType statType);
    void UpdateTotalStatMultiplier(StatType statType);

    StatType GetPrimaryStat() const;

private:
    Unit* _owner;
    std::array<StatData, AsUnderlyingType(StatType::Max)> _primaryStatData;

    void updateStat(StatType statType);
    void calculateStatValues(StatData const& statData, int32& statValue, int32& posStatValue, int32& negStatValue);
    float getAuraMultiplierForStatType(AuraType auraType, StatType statType) const;
    int32 getAuraModifierForStatType(AuraType auraType, StatType statType) const;

};

#endif  // _Stats_h__
