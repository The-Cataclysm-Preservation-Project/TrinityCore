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

#ifndef Possessed_h__
#define Possessed_h__

#include "NewTemporarySummon.h"

struct SummonPropertiesEntry;

class TC_GAME_API NewPossessed : public NewTemporarySummon
{
public:
    explicit NewPossessed(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isTotem);

    void RemoveFromWorld() override;

    bool HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons) override;
    void HandlePostSummonActions() override;
};

#endif // Possessed_h__
