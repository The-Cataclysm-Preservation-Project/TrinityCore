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

#ifndef TempoarySummon_h__
#define TempoarySummon_h__

#include "Creature.h"

struct SummonPropertiesEntry;

// This time sepcifies when a summon will despawn after dying from expiration
static constexpr Milliseconds SummonExpirationCorpseDespawnTime = 15s;

class TC_GAME_API NewTemporarySummon : public Creature
{
public:
    explicit NewTemporarySummon(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isTotem);
    virtual ~NewTemporarySummon() { }

    // Overriden methods of Creature class
    void AddToWorld() override;
    void RemoveFromWorld() override;
    void Update(uint32 diff) override;

    // Handles everything that needs to be done before the summon is being added to the world (assigning slot, selecting level, etc)
    virtual bool HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons);
    // Handles everything that needs to be done after the summon has been added to the world (casting passive auras, calling AI hooks, etc)
    virtual void HandlePostSummonActions();

    // Sets the summon duration before it will expire and either die or despawn
    void SetSummonDuration(Milliseconds duration) { _summonDuration = duration; }
    // Returns the remaining duration in milliseconds until the summon expires and dies or despawns
    Milliseconds GetRemainingSummonDuration() const { return _summonDuration; }

    // Marks the summon as permanent so it will longer die or despawn upon expiration
    void SetPermanentSummon(bool apply) { _isPermanentSummon = apply; }

    // Unsummons the summon after a specified amount of time.
    virtual void Unsummon(Milliseconds timeUntilDespawn = 0ms);

    // Returns a pointer to the internally stored summoner that has summoned this creature. Unrelated to UNIT_FIELD_SUMMONEDBY. If the creature has not been added to a map yet this method will return nullptr!
    Unit* GetInternalSummoner() const;
    // Returns the internally stored ObjectGuid of the summoner. Unrelated to UNIT_FIELD_SUMMONEDBY
    ObjectGuid GetInternalSummonerGUID() const { return _internalSummonerGUID; }

    // Returns true if the summon is suposed to despawn when its summoner has died
    bool ShouldDespawnOnSummonerDeath() const;
    // Returns true if the summon is suposed to despawn when its summoner is a player and has logged out
    virtual bool ShouldDespawnOnSummonerLogout() const;
    // Returns true of the summon is suposed to despawn when it has expire
    bool ShouldDespawnOnExpiration() const;
    // Returns true if the summon is suposed to follow the summoner. SpawnGroups are Blizzard's internal term for our CreatureGroups which are currently not supported for players
    virtual bool ShouldJoinSummonerSpawnGroupAfterCreation() const;
    // Returns true if the summon is suposed to follow the summoner without joining its SpawnGroup. This mechanic is often being used by companions (formerly known as minipets)
    bool ShouldFollowSummonerAfterCreation() const;

protected:
    SummonPropertiesEntry const* _summonProperties;
    ObjectGuid _internalSummonerGUID;
    Milliseconds _summonDuration;
    bool _isPermanentSummon;
    SummonPropertiesSlot _summonSlot;
    Optional<Milliseconds> _summonDistanceCheckTimer;
};

#endif // PhasingHandler_h__
