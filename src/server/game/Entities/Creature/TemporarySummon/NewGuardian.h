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

#ifndef Guardian_h__
#define Guardian_h__

#include "NewTemporarySummon.h"

class TC_GAME_API NewGuardian : public NewTemporarySummon
{
public:
    explicit NewGuardian(SummonPropertiesEntry const* properties, Unit* owner, bool isWorldObject, bool isTotem);

    void AddToWorld() override;
    void Update(uint32 diff) override;

    // Stats handling
    bool UpdateAllStats() override;
    bool UpdateStats(Stats stat) override;
    void UpdateResistances(uint32 school) override;
    void UpdateArmor() override;
    void UpdateMaxHealth() override;
    void UpdateMaxPower(Powers power) override;
    void UpdateAttackPowerAndDamage(bool ranged = false) override;
    void UpdateDamagePhysical(WeaponAttackType attType) override;

    bool HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons) override;
    void HandlePostSummonActions() override;
    bool IsUsingRealStats() const { return _isUsingRealStats; }
    void InitializeStats();

private:
    void CastPassiveAuras();
    void UpdateScalingAuras();

    bool _isUsingRealStats;
    std::vector<uint32> _scalingAuras;
    Optional<Milliseconds> _scalingAuraUpdateTimer;
};

#endif // Guardian_h__
