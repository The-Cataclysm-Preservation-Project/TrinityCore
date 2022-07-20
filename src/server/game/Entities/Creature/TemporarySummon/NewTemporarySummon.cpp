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

#include "NewTemporarySummon.h"
#include "DBCStores.h"
#include "CreatureAI.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "TotemPackets.h"

NewTemporarySummon::NewTemporarySummon(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isTotem) :
    Creature(isWorldObject), _summonProperties(properties), _summonDuration(0ms), _isPermanentSummon(false), _summonSlot(SummonPropertiesSlot::None)
{
    if (summoner)
        _internalSummonerGUID = summoner->GetGUID();

    m_unitTypeMask |= UNIT_MASK_SUMMON;
    if (isTotem)
        m_unitTypeMask |= UNIT_MASK_TOTEM;
}

void NewTemporarySummon::AddToWorld()
{
    if (IsInWorld())
        return;

    Creature::AddToWorld();

    Unit* summoner = GetInternalSummoner();
    if (summoner)
    {
        summoner->AddSummonGUID(GetGUID());
        if (_summonSlot != SummonPropertiesSlot::None)
        {
            // Unsummon previous summon in the slot that we are about to occupy
            if (NewTemporarySummon* summon = summoner->GetSummonInSlot(_summonSlot))
                summon->Unsummon();

            summoner->AddSummonGUIDToSlot(GetGUID(), _summonSlot);

            if (_summonSlot == SummonPropertiesSlot::Critter)
                summoner->SetCritterGUID(GetGUID());
        }
    }
}

void NewTemporarySummon::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    if (_summonProperties)
    {
        if (Unit* summoner = GetInternalSummoner())
        {
            summoner->RemoveSummonGUID(GetGUID());
            if (_summonSlot != SummonPropertiesSlot::None)
            {
                summoner->RemoveSummonGUIDFromSlot(GetGUID(), _summonSlot);
                if (_summonSlot == SummonPropertiesSlot::Critter && summoner->GetCritterGUID() == GetGUID())
                    summoner->SetCritterGUID(ObjectGuid::Empty);
            }
        }
    }

    Creature::RemoveFromWorld();
}

void NewTemporarySummon::Update(uint32 diff)
{
    Creature::Update(diff);

    if (_summonDuration >= Milliseconds(diff))
        _summonDuration -= Milliseconds(diff);
    else
        _summonDuration = 0s;

    // Make sure that the summon is within the summoner's distance when its following the player
    if (_summonDistanceCheckTimer.has_value())
    {
        if (*_summonDistanceCheckTimer <= Milliseconds(diff))
        {
            Unit* summoner = GetInternalSummoner();
            if (!summoner || GetExactDist(summoner) > MAX_SUMMON_DISTANCE)
            {
                Unsummon();
                return;
            }

            _summonDistanceCheckTimer = 1s;
        }
        else
            *_summonDistanceCheckTimer -= Milliseconds(diff);
    }

    // The summon has been marked as permanent so we will not proceed with any of the expiration mechanics
    if (_isPermanentSummon)
        return;

    if (_summonDuration <= 0s)
    {
        // When a summon expires it usually dies unless the summon property flags expects us to despawn the summon right away.
        if (ShouldDespawnOnExpiration())
        {
            Unsummon();
            return;
        }

        KillSelf(false);
    }
}

bool NewTemporarySummon::HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons)
{
    if (!_summonProperties)
        return true;

    if (_summonProperties->Faction != 0)
        SetFaction(_summonProperties->Faction);

    if (!_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::UseCreatureLevel) && creatureLevel > 0)
        SetLevel(creatureLevel);

    if (!summoner)
        return true;

    SetGuidValue(OBJECT_FIELD_DATA, summoner->GetGuidValue(OBJECT_FIELD_DATA));

    if (_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::UseSummonerFaction))
        SetFaction(summoner->GetFaction());

    _summonSlot = static_cast<SummonPropertiesSlot>(_summonProperties->Slot);

    // Only players are allowed to summon creatures in quest slots
    if (_summonSlot == SummonPropertiesSlot::Quest && !summoner->IsPlayer())
        return false;

    if (_summonSlot == SummonPropertiesSlot::AnyAvailableTotem)
    {
        // We have to select an unoccupied totem slot that can be used. As of 4.3.4.15595 this mechanic is only being used by Wild Mushrooms
        // If there is no empty slot, we are going to select the eldest summon of our kind and replace it
        constexpr int8 firstTotemSlot = AsUnderlyingType(SummonPropertiesSlot::Totem1);
        // first attempt: check for empty totem slots within the allowed range
        for (uint8 i = 0; i < maxSummons; ++i)
        {
            SummonPropertiesSlot slot = static_cast<SummonPropertiesSlot>(i + firstTotemSlot);
            if (!summoner->HasSummonInSlot(slot))
            {
                _summonSlot = slot;
                break;
            }
        }

        // all eligible slots are occupied. So try to find a related summon and replace it if possible
        if (_summonSlot == SummonPropertiesSlot::AnyAvailableTotem)
        {
            Optional<std::pair<SummonPropertiesSlot, Milliseconds>> fallbackSlot;
            for (uint8 i = 0; i < maxSummons; ++i)
            {
                SummonPropertiesSlot slot = static_cast<SummonPropertiesSlot>(i + firstTotemSlot);

                NewTemporarySummon const* summon = summoner->GetSummonInSlot(slot);
                if (summon->GetUInt32Value(UNIT_CREATED_BY_SPELL) == GetUInt32Value(UNIT_CREATED_BY_SPELL))
                    if (!fallbackSlot.has_value() || fallbackSlot->second >= summon->GetRemainingSummonDuration())
                        fallbackSlot = std::make_pair(slot, summon->GetRemainingSummonDuration());
            }

            if (fallbackSlot.has_value())
                _summonSlot = fallbackSlot->first;
            else // There is no slot that we can use right now so skip summon creation
                return false;
        }
    }

    // The summon slot is now fully initialized so we can proceed with the slot specific actions
    switch (_summonSlot)
    {
        case SummonPropertiesSlot::Totem1:
        case SummonPropertiesSlot::Totem2:
        case SummonPropertiesSlot::Totem3:
        case SummonPropertiesSlot::Totem4:
            if (summoner->IsPlayer())
            {
                // SMSG_TOTEM_CREATED must be sent to the client before adding the summon to world and destroying other totems
                WorldPackets::Totem::TotemCreated totemCreated;
                totemCreated.Duration = int32(_summonDuration.count());
                totemCreated.Slot = uint8(AsUnderlyingType(_summonSlot) - AsUnderlyingType(SummonPropertiesSlot::Totem1));
                totemCreated.SpellID = GetUInt32Value(UNIT_CREATED_BY_SPELL);
                totemCreated.Totem = GetGUID();
                summoner->ToPlayer()->SendDirectMessage(totemCreated.Write());
            }
            // There are some creatures which also go into totem slot but are no real totems. In this case we do not want to override the display Id
            if (uint32 totemModel = summoner->GetModelForTotem(PlayerTotemType(_summonProperties->ID)))
                SetDisplayId(totemModel);
            break;
        default:
            break;
    }

    return true;
}

void NewTemporarySummon::HandlePostSummonActions()
{
    if (Unit* summoner = GetInternalSummoner())
    {
        if (summoner->IsCreature() && summoner->IsAIEnabled())
            if (CreatureAI* ai = summoner->ToCreature()->AI())
                ai->JustSummoned(this);

        if (IsAIEnabled())
            AI()->IsSummonedBy(summoner);

        if (_summonProperties)
        {
            if (_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::AttackSummoner))
                EngageWithTarget(summoner);

            if (_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::HelpWhenSummonedInCombat) && summoner->IsInCombat())
            {
                Unit* victim = nullptr;
                if (summoner->IsPlayer())
                    victim = summoner->GetVictim();
                else if (summoner->IsCreature() && summoner->IsAIEnabled())
                    victim = summoner->GetThreatManager().GetCurrentVictim();

                if (victim)
                    EngageWithTarget(victim);
            }
        }
    }

    // Totem slot summons despawn instantly after they have died
    if (AsUnderlyingType(_summonSlot) >= AsUnderlyingType(SummonPropertiesSlot::Totem1) && AsUnderlyingType(_summonSlot) <= AsUnderlyingType(SummonPropertiesSlot::Totem4))
        SetDespawnInstantlyAfterDeath(true);

    // If a summon is suposed to follow its summoner, make sure that it stays within its distance
    if (ShouldJoinSummonerSpawnGroupAfterCreation() || ShouldFollowSummonerAfterCreation() || IsGuardian())
        _summonDistanceCheckTimer = 1s;

    // Mark all temporary summons as active to keep updating duration and distance checks
    if (_summonDistanceCheckTimer.has_value() || !_isPermanentSummon)
        setActive(true);
}

void NewTemporarySummon::Unsummon(Milliseconds timeUntilDespawn /*= 0ms*/)
{
    if (timeUntilDespawn > 0ms)
    {
        m_Events.AddEventAtOffset([&]() { Unsummon(); }, timeUntilDespawn);
        return;
    }

    if (Unit* summoner = GetInternalSummoner())
        if (summoner->IsCreature() && summoner->IsAIEnabled())
            if (CreatureAI* ai = summoner->ToCreature()->AI())
                ai->SummonedCreatureDespawn(this);

    AddObjectToRemoveList();
}

Unit* NewTemporarySummon::GetInternalSummoner() const
{
    if (_internalSummonerGUID.IsEmpty())
        return nullptr;

    return ObjectAccessor::GetUnit(*this, _internalSummonerGUID);
}

bool NewTemporarySummon::ShouldDespawnOnSummonerDeath() const
{
    if (_summonProperties && _summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::DespawnOnSummonerDeath))
        return true;

    // Totem slot summons will also despawn when the summoner has died
    if (AsUnderlyingType(_summonSlot) >= AsUnderlyingType(SummonPropertiesSlot::Totem1) && AsUnderlyingType(_summonSlot) <= AsUnderlyingType(SummonPropertiesSlot::Totem4))
        return true;

    return false;
}

bool NewTemporarySummon::ShouldDespawnOnSummonerLogout() const
{
    if (_summonProperties && _summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::DespawnOnSummonerLogout))
        return true;

    // Summons that are stored in a slot will be despawned when the summoner logs out
    return _summonSlot != SummonPropertiesSlot::None;
}

bool NewTemporarySummon::ShouldDespawnOnExpiration() const
{
    if (_summonProperties && _summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::DespawnWhenExpired))
        return true;

    return false;
}

bool NewTemporarySummon::ShouldJoinSummonerSpawnGroupAfterCreation() const
{
    if (!_summonProperties)
        return false;

    if (_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::JoinSummonerSpawnGroup))
        return true;

    return false;
}

bool NewTemporarySummon::ShouldFollowSummonerAfterCreation() const
{
    return _summonSlot == SummonPropertiesSlot::Critter;
}
