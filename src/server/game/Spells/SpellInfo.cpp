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

#include "SpellInfo.h"
#include "Battleground.h"
#include "Corpse.h"
#include "Creature.h"
#include "DBCStores.h"
#include "GameClient.h"
#include "InstanceScript.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "Log.h"
#include "LootMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"
#include "Spell.h"
#include "SpellAuraDefines.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Vehicle.h"

uint32 GetTargetFlagMask(SpellTargetObjectTypes objType)
{
    switch (objType)
    {
        case TARGET_OBJECT_TYPE_DEST:
            return TARGET_FLAG_DEST_LOCATION;
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
            return TARGET_FLAG_DEST_LOCATION | TARGET_FLAG_UNIT;
        case TARGET_OBJECT_TYPE_CORPSE_ALLY:
            return TARGET_FLAG_CORPSE_ALLY;
        case TARGET_OBJECT_TYPE_CORPSE_ENEMY:
            return TARGET_FLAG_CORPSE_ENEMY;
        case TARGET_OBJECT_TYPE_CORPSE:
            return TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY;
        case TARGET_OBJECT_TYPE_UNIT:
            return TARGET_FLAG_UNIT;
        case TARGET_OBJECT_TYPE_GOBJ:
            return TARGET_FLAG_GAMEOBJECT;
        case TARGET_OBJECT_TYPE_GOBJ_ITEM:
            return TARGET_FLAG_GAMEOBJECT_ITEM;
        case TARGET_OBJECT_TYPE_ITEM:
            return TARGET_FLAG_ITEM;
        case TARGET_OBJECT_TYPE_SRC:
            return TARGET_FLAG_SOURCE_LOCATION;
        default:
            return TARGET_FLAG_NONE;
    }
}

SpellImplicitTargetInfo::SpellImplicitTargetInfo(uint32 target)
{
    _target = Targets(target);
}

bool SpellImplicitTargetInfo::IsArea() const
{
    return GetSelectionCategory() == TARGET_SELECT_CATEGORY_AREA || GetSelectionCategory() == TARGET_SELECT_CATEGORY_CONE;
}

SpellTargetSelectionCategories SpellImplicitTargetInfo::GetSelectionCategory() const
{
    return _data[_target].SelectionCategory;
}

SpellTargetReferenceTypes SpellImplicitTargetInfo::GetReferenceType() const
{
    return _data[_target].ReferenceType;
}

SpellTargetObjectTypes SpellImplicitTargetInfo::GetObjectType() const
{
    return _data[_target].ObjectType;
}

SpellTargetCheckTypes SpellImplicitTargetInfo::GetCheckType() const
{
    return _data[_target].SelectionCheckType;
}

SpellTargetDirectionTypes SpellImplicitTargetInfo::GetDirectionType() const
{
    return _data[_target].DirectionType;
}

float SpellImplicitTargetInfo::CalcDirectionAngle(SpellEffectInfo const& effectInfo) const
{
    switch (GetDirectionType())
    {
        case TARGET_DIR_FRONT:
            return 0.0f;
        case TARGET_DIR_BACK:
            return static_cast<float>(M_PI);
        case TARGET_DIR_RIGHT:
            return static_cast<float>(-M_PI/2);
        case TARGET_DIR_LEFT:
            return static_cast<float>(M_PI/2);
        case TARGET_DIR_FRONT_RIGHT:
            return static_cast<float>(-M_PI/4);
        case TARGET_DIR_BACK_RIGHT:
            return static_cast<float>(-3*M_PI/4);
        case TARGET_DIR_BACK_LEFT:
            return static_cast<float>(3*M_PI/4);
        case TARGET_DIR_FRONT_LEFT:
            return static_cast<float>(M_PI/4);
        case TARGET_DIR_RANDOM:
            return float(rand_norm())*static_cast<float>(2*M_PI);
        case TARGET_DIR_SUMMON:
            // This direction does alter its angle based on what is being summoned.
            // Creatures are being summoned on the left, gameobjects infront
            switch (effectInfo.Effect)
            {
                case SPELL_EFFECT_SUMMON_PET:
                case SPELL_EFFECT_SUMMON:
                    return static_cast<float>(M_PI / 2);
                default:
                    return 0.0f;
            }
        default:
            return 0.0f;
    }
}

Targets SpellImplicitTargetInfo::GetTarget() const
{
    return _target;
}

uint32 SpellImplicitTargetInfo::GetExplicitTargetMask(bool& srcSet, bool& dstSet) const
{
    uint32 targetMask = 0;
    if (GetTarget() == TARGET_DEST_TRAJ)
    {
        if (!srcSet)
            targetMask = TARGET_FLAG_SOURCE_LOCATION;
        if (!dstSet)
            targetMask |= TARGET_FLAG_DEST_LOCATION;
    }
    else
    {
        switch (GetReferenceType())
        {
            case TARGET_REFERENCE_TYPE_SRC:
                if (srcSet)
                    break;
                targetMask = TARGET_FLAG_SOURCE_LOCATION;
                break;
            case TARGET_REFERENCE_TYPE_DEST:
                if (dstSet)
                    break;
                targetMask = TARGET_FLAG_DEST_LOCATION;
                break;
            case TARGET_REFERENCE_TYPE_TARGET:
                switch (GetObjectType())
                {
                    case TARGET_OBJECT_TYPE_GOBJ:
                        targetMask = TARGET_FLAG_GAMEOBJECT;
                        break;
                    case TARGET_OBJECT_TYPE_GOBJ_ITEM:
                        targetMask = TARGET_FLAG_GAMEOBJECT_ITEM;
                        break;
                    case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
                    case TARGET_OBJECT_TYPE_UNIT:
                    case TARGET_OBJECT_TYPE_DEST:
                        switch (GetCheckType())
                        {
                            case TARGET_CHECK_ENEMY:
                                targetMask = TARGET_FLAG_UNIT_ENEMY;
                                break;
                            case TARGET_CHECK_ALLY:
                                targetMask = TARGET_FLAG_UNIT_ALLY;
                                break;
                            case TARGET_CHECK_PARTY:
                                targetMask = TARGET_FLAG_UNIT_PARTY;
                                break;
                            case TARGET_CHECK_RAID:
                                targetMask = TARGET_FLAG_UNIT_RAID;
                                break;
                            case TARGET_CHECK_PASSENGER:
                                targetMask = TARGET_FLAG_UNIT_PASSENGER;
                                break;
                            case TARGET_CHECK_RAID_CLASS:
                                [[fallthrough]];
                            default:
                                targetMask = TARGET_FLAG_UNIT;
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    switch (GetObjectType())
    {
        case TARGET_OBJECT_TYPE_SRC:
            srcSet = true;
            break;
        case TARGET_OBJECT_TYPE_DEST:
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
            dstSet = true;
            break;
        default:
            break;
    }
    return targetMask;
}

SpellImplicitTargetInfo::StaticData  SpellImplicitTargetInfo::_data[TOTAL_SPELL_TARGETS] =
{
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        //
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 1 TARGET_UNIT_CASTER
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 2 TARGET_UNIT_NEARBY_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 3 TARGET_UNIT_NEARBY_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 4 TARGET_UNIT_NEARBY_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 5 TARGET_UNIT_PET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 6 TARGET_UNIT_TARGET_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 7 TARGET_UNIT_SRC_AREA_ENTRY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 8 TARGET_UNIT_DEST_AREA_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 9 TARGET_DEST_HOME
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 10
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 11 TARGET_UNIT_SRC_AREA_UNK_11
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 12
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 13
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 14
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 15 TARGET_UNIT_SRC_AREA_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 16 TARGET_UNIT_DEST_AREA_ENEMY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 17 TARGET_DEST_DB
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 18 TARGET_DEST_CASTER
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 19
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 20 TARGET_UNIT_CASTER_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 21 TARGET_UNIT_TARGET_ALLY
    {TARGET_OBJECT_TYPE_SRC,  TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 22 TARGET_SRC_CASTER
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 23 TARGET_GAMEOBJECT_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 24 TARGET_UNIT_CONE_ENEMY_24
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 25 TARGET_UNIT_TARGET_ANY
    {TARGET_OBJECT_TYPE_GOBJ_ITEM, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT, TARGET_DIR_NONE},    // 26 TARGET_GAMEOBJECT_ITEM_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 27 TARGET_UNIT_MASTER
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 28 TARGET_DEST_DYNOBJ_ENEMY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 29 TARGET_DEST_DYNOBJ_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 30 TARGET_UNIT_SRC_AREA_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 31 TARGET_UNIT_DEST_AREA_ALLY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_SUMMON },     // 32 TARGET_DEST_CASTER_SUMMON
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 33 TARGET_UNIT_SRC_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 34 TARGET_UNIT_DEST_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 35 TARGET_UNIT_TARGET_PARTY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 36 TARGET_DEST_CASTER_UNK_36
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_LAST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 37 TARGET_UNIT_LASTTARGET_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 38 TARGET_UNIT_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 39 TARGET_DEST_CASTER_FISHING
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 40 TARGET_GAMEOBJECT_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 41 TARGET_DEST_CASTER_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 42 TARGET_DEST_CASTER_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 43 TARGET_DEST_CASTER_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 44 TARGET_DEST_CASTER_FRONT_LEFT
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 45 TARGET_UNIT_TARGET_CHAINHEAL_ALLY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 46 TARGET_DEST_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 47 TARGET_DEST_CASTER_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 48 TARGET_DEST_CASTER_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 49 TARGET_DEST_CASTER_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 50 TARGET_DEST_CASTER_LEFT
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 51 TARGET_GAMEOBJECT_SRC_AREA
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 52 TARGET_GAMEOBJECT_DEST_AREA
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 53 TARGET_DEST_TARGET_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 54 TARGET_UNIT_CONE_180_DEG_ENEMY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 55 TARGET_DEST_CASTER_FRONT_LEAP
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 56 TARGET_UNIT_CASTER_AREA_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 57 TARGET_UNIT_TARGET_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 58 TARGET_UNIT_NEARBY_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ALLY,     TARGET_DIR_FRONT},       // 59 TARGET_UNIT_CONE_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENTRY,    TARGET_DIR_FRONT},       // 60 TARGET_UNIT_CONE_ENTRY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID_CLASS, TARGET_DIR_NONE},      // 61 TARGET_UNIT_TARGET_AREA_RAID_CLASS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 62 TARGET_DEST_CASTER_GROUND
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 63 TARGET_DEST_TARGET_ANY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 64 TARGET_DEST_TARGET_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 65 TARGET_DEST_TARGET_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 66 TARGET_DEST_TARGET_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 67 TARGET_DEST_TARGET_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 68 TARGET_DEST_TARGET_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 69 TARGET_DEST_TARGET_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 70 TARGET_DEST_TARGET_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 71 TARGET_DEST_TARGET_FRONT_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 72 TARGET_DEST_CASTER_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 73 TARGET_DEST_CASTER_RADIUS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 74 TARGET_DEST_TARGET_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 75 TARGET_DEST_TARGET_RADIUS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 76 TARGET_DEST_CHANNEL_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 77 TARGET_UNIT_CHANNEL_TARGET
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 78 TARGET_DEST_DEST_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 79 TARGET_DEST_DEST_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 80 TARGET_DEST_DEST_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 81 TARGET_DEST_DEST_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 82 TARGET_DEST_DEST_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 83 TARGET_DEST_DEST_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 84 TARGET_DEST_DEST_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 85 TARGET_DEST_DEST_FRONT_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 86 TARGET_DEST_DEST_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 87 TARGET_DEST_DEST
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 88 TARGET_DEST_DYNOBJ_NONE
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_TRAJ,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 89 TARGET_DEST_TRAJ
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 90 TARGET_UNIT_TARGET_MINIPET
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 91 TARGET_DEST_DEST_RADIUS
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 92 TARGET_UNIT_SUMMONER
    {TARGET_OBJECT_TYPE_CORPSE, TARGET_REFERENCE_TYPE_SRC,   TARGET_SELECT_CATEGORY_AREA,   TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 93 TARGET_CORPSE_SRC_AREA_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 94 TARGET_UNIT_VEHICLE
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_PASSENGER, TARGET_DIR_NONE},       // 95 TARGET_UNIT_TARGET_PASSENGER
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 96 TARGET_UNIT_PASSENGER_0
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 97 TARGET_UNIT_PASSENGER_1
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 98 TARGET_UNIT_PASSENGER_2
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 99 TARGET_UNIT_PASSENGER_3
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 100 TARGET_UNIT_PASSENGER_4
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 101 TARGET_UNIT_PASSENGER_5
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 102 TARGET_UNIT_PASSENGER_6
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 103 TARGET_UNIT_PASSENGER_7
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 104 TARGET_UNIT_CONE_CASTER_TO_DEST_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 105 TARGET_UNIT_CASTER_AND_PASSENGERS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 106 TARGET_DEST_CHANNEL_CASTER
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 107 TARGET_DEST_NEARBY_ENTRY_2
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 108 TARGET_GAMEOBJECT_CONE_CASTER_TO_DEST_ENEMY
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ALLY,     TARGET_DIR_FRONT},       // 109 TARGET_GAMEOBJECT_CONE_CASTER_TO_DEST_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENTRY,    TARGET_DIR_FRONT},       // 110 TARGET_UNIT_CONE_CASTER_TO_DEST_ENTRY
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 111
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 112 TARGET_DEST_TARGET_ANY_2
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 113
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 114
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 115
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 116
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 117
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 118 TARGET_UNIT_TARGET_ALLY_OR_RAID
    {TARGET_OBJECT_TYPE_CORPSE, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,  TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 119 TARGET_CORPSE_SRC_AREA_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_SUMMONED, TARGET_DIR_NONE},        // 120 TARGET_UNIT_SELF_AND_SUMMONS
    {TARGET_OBJECT_TYPE_CORPSE, TARGET_REFERENCE_TYPE_NONE, TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 121 TARGET_CORPSE_TARGET_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_THREAT,   TARGET_DIR_NONE},        // 122 TARGET_UNIT_AREA_THREAT_LIST
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_TAP,      TARGET_DIR_NONE},        // 123 TARGET_UNIT_AREA_TAP_LIST
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 124 TARGET_UNIT_TARGET_TAP_LIST
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 125 TARGET_DEST_CASTER_GROUND_2
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE },       // 126 TARGET_UNIT_AREA_ENEMY
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 127
};

SpellEffectInfo::SpellEffectInfo(SpellEntry const* /*spellEntry*/, SpellInfo const* spellInfo, uint8 effIndex, SpellEffectEntry const* _effect)
{
    SpellScalingEntry const* scaling = spellInfo->GetSpellScaling();

    _spellInfo = spellInfo;
    _effIndex = _effect ? _effect->EffectIndex : effIndex;
    Effect = _effect ? _effect->Effect : 0;
    ApplyAuraName = _effect ? _effect->EffectAura : 0;
    AuraPeriod = _effect ? _effect->EffectAuraPeriod : 0;
    DieSides = _effect ? _effect->EffectDieSides : 0;
    RealPointsPerLevel = _effect ? _effect->EffectRealPointsPerLevel : 0.0f;
    BasePoints = _effect ? _effect->EffectBasePoints : 0;
    PointsPerComboPoint = _effect ? _effect->EffectPointsPerResource : 0.0f;
    Amplitude = _effect ? _effect->EffectAmplitude : 0.0f;
    DamageMultiplier = _effect ? _effect->EffectChainAmplitude : 0.0f;
    BonusMultiplier = _effect ? _effect->EffectBonusCoefficient : 0.0f;
    MiscValue = _effect ? _effect->EffectMiscValue : 0;
    MiscValueB = _effect ? _effect->EffectMiscValueB : 0;
    Mechanic = Mechanics(_effect ? _effect->EffectMechanic : 0);
    TargetA = SpellImplicitTargetInfo(_effect ? _effect->EffectImplicitTargetA : 0);
    TargetB = SpellImplicitTargetInfo(_effect ? _effect->EffectImplicitTargetB : 0);
    TargetARadiusEntry = _effect && _effect->EffectRadiusIndex ? sSpellRadiusStore.LookupEntry(_effect->EffectRadiusIndex) : nullptr;
    TargetBRadiusEntry = _effect && _effect->EffectRadiusMaxIndex ? sSpellRadiusStore.LookupEntry(_effect->EffectRadiusMaxIndex) : nullptr;
    ChainTarget = _effect ? _effect->EffectChainTargets : 0;
    ItemType = _effect ? _effect->EffectItemType : 0;
    TriggerSpell = _effect ? _effect->EffectTriggerSpell : 0;
    SpellClassMask = _effect ? _effect->EffectSpellClassMask : flag96(0);
    ImplicitTargetConditions = nullptr;

    Scaling.Coefficient = scaling ? scaling->Coefficient[_effIndex] : 0.0f;
    Scaling.Variance = scaling ? scaling->Variance[_effIndex] : 0.0f;
    Scaling.ComboPointsCoefficient = scaling ? scaling->ComboPointsCoefficient[_effIndex] : 0.0f;
}

bool SpellEffectInfo::IsEffect() const
{
    return Effect != 0;
}

bool SpellEffectInfo::IsEffect(SpellEffects effectName) const
{
    return Effect == effectName;
}

bool SpellEffectInfo::IsAura() const
{
    return (IsUnitOwnedAuraEffect() || Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA) && ApplyAuraName != 0;
}

bool SpellEffectInfo::IsAura(AuraType aura) const
{
    return IsAura() && ApplyAuraName == aura;
}

bool SpellEffectInfo::IsTargetingArea() const
{
    return TargetA.IsArea() || TargetB.IsArea();
}

bool SpellEffectInfo::IsAreaAuraEffect() const
{
    if (Effect == SPELL_EFFECT_APPLY_AREA_AURA_PARTY    ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID     ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_FRIEND   ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_ENEMY    ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_PET      ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_OWNER)
        return true;
    return false;
}

bool SpellEffectInfo::IsUnitOwnedAuraEffect() const
{
    return IsAreaAuraEffect() || Effect == SPELL_EFFECT_APPLY_AURA || Effect == SPELL_EFFECT_APPLY_AURA_2;
}

int32 SpellEffectInfo::CalcValue(WorldObject const* caster, int32 const* bp, Unit const* target) const
{
    float basePointsPerLevel = RealPointsPerLevel;
    int32 basePoints = bp ? *bp : BasePoints;
    float comboDamage = PointsPerComboPoint;

    Unit const* casterUnit = nullptr;
    if (caster)
        casterUnit = caster->ToUnit();

    // base amount modification based on spell lvl vs caster lvl
    if (Scaling.Coefficient != 0.0f)
    {
        int32 level = _spellInfo->SpellLevel;
        if (target && _spellInfo->HasAttribute(SPELL_ATTR8_USE_TARGETS_LEVEL_FOR_SPELL_SCALING))
            level = target->getLevel();
        else if (casterUnit)
            level = casterUnit->getLevel();

        if (_spellInfo->HasAttribute(SPELL_ATTR10_USE_SPELL_BASE_LEVEL_FOR_SCALING))
            level = _spellInfo->BaseLevel;

        float value = 0.0f;
        if (level > 0)
        {
            if (!_spellInfo->Scaling.Class)
                return 0;

            if (GtSpellScalingEntry const* gtScaling = sGtSpellScalingStore.LookupEntry(((_spellInfo->Scaling.Class > 0 ? _spellInfo->Scaling.Class : ((MAX_CLASSES - 1 /*last class*/) - _spellInfo->Scaling.Class)) - 1) * 100 + level - 1))
                value = gtScaling->value;

            if (level < _spellInfo->Scaling.CastTimeMaxLevel && _spellInfo->Scaling.CastTimeMax)
                value *= float(_spellInfo->Scaling.CastTimeMin + (level - 1) * (_spellInfo->Scaling.CastTimeMax - _spellInfo->Scaling.CastTimeMin) / (_spellInfo->Scaling.CastTimeMaxLevel - 1)) / float(_spellInfo->Scaling.CastTimeMax);

            if (level < _spellInfo->Scaling.NerfMaxLevel)
                value *= ((((1.0 - _spellInfo->Scaling.NerfFactor) * (level - 1)) / (_spellInfo->Scaling.NerfMaxLevel - 1)) + _spellInfo->Scaling.NerfFactor);
        }

        if (Scaling.ComboPointsCoefficient)
            comboDamage = Scaling.ComboPointsCoefficient * value;

        value *= Scaling.Coefficient;
        if (value >= 0.0f && value < 1.0f)
            value = 1.0f;

        if (Scaling.Variance)
        {
            float delta = fabs(Scaling.Variance * value * 0.5f);
            value += frand(-delta, delta);
        }

        basePoints = int32(round(value));
    }
    else
    {
        if (casterUnit && basePointsPerLevel != 0.0f)
        {
            int32 level = 1;
            if (target && _spellInfo->HasAttribute(SPELL_ATTR8_USE_TARGETS_LEVEL_FOR_SPELL_SCALING))
                level = target->getLevel();
            else if (caster && caster->IsUnit())
                level = casterUnit->getLevel();

            if (level > int32(_spellInfo->MaxLevel) && _spellInfo->MaxLevel > 0)
                level = int32(_spellInfo->MaxLevel);
            else if (level < int32(_spellInfo->BaseLevel))
                level = int32(_spellInfo->BaseLevel);
            if (!_spellInfo->IsPassive())
                level -= int32(_spellInfo->SpellLevel);
            basePoints += int32(level * basePointsPerLevel);
        }

        // roll in a range <1;EffectDieSides> as of patch 3.3.3
        int32 randomPoints = int32(DieSides);
        switch (randomPoints)
        {
            case 0: break;
            case 1: basePoints += 1; break;                     // range 1..1
            default:
            {
                // range can have positive (1..rand) and negative (rand..1) values, so order its for irand
                int32 randvalue = (randomPoints >= 1)
                    ? irand(1, randomPoints)
                    : irand(randomPoints, 1);

                basePoints += randvalue;
                break;
            }
        }
    }

    float value = float(basePoints);

    // random damage
    if (casterUnit)
    {
        // bonus amount from combo points
        if (_spellInfo->HasAttribute(SPELL_ATTR1_FINISHING_MOVE_DAMAGE) && casterUnit->IsMovedByClient() && comboDamage != 0.f)
            if (uint8 comboPoints = casterUnit->GetGameClientMovingMe()->GetBasePlayer()->GetComboPoints())
                value += comboDamage * comboPoints;

        value = casterUnit->ApplyEffectModifiers(_spellInfo, _effIndex, value);

        if (!casterUnit->IsControlledByPlayer() &&
            _spellInfo->SpellLevel && _spellInfo->SpellLevel != casterUnit->getLevel() &&
            !basePointsPerLevel && _spellInfo->HasAttribute(SPELL_ATTR0_SCALES_WITH_CREATURE_LEVEL))
        {
            bool canEffectScale = false;
            switch (Effect)
            {
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_DUMMY:
                case SPELL_EFFECT_POWER_DRAIN:
                case SPELL_EFFECT_HEALTH_LEECH:
                case SPELL_EFFECT_HEAL:
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_POWER_BURN:
                case SPELL_EFFECT_SCRIPT_EFFECT:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_FORCE_CAST_WITH_VALUE:
                case SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE:
                    canEffectScale = true;
                    break;
                default:
                    break;
            }

            switch (ApplyAuraName)
            {
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_DUMMY:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_DAMAGE_SHIELD:
                case SPELL_AURA_PROC_TRIGGER_DAMAGE:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_SCHOOL_ABSORB:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                    canEffectScale = true;
                    break;
                default:
                    break;
            }

            if (canEffectScale)
            {
                GtNPCManaCostScalerEntry const* spellScaler = sGtNPCManaCostScalerStore.LookupEntry(_spellInfo->SpellLevel - 1);
                GtNPCManaCostScalerEntry const* casterScaler = sGtNPCManaCostScalerStore.LookupEntry(casterUnit->getLevel() - 1);
                if (spellScaler && casterScaler)
                {
                    value *= casterScaler->ratio / spellScaler->ratio;
                    if (casterUnit->getLevel() > 80)
                        value *= (casterUnit->getLevel() - 80) * 4.4f; // Cataclysm creatures have a way higher jump in stats than previous expansions so we use this estimated value based on combat log packet research
                }
            }
        }
    }

    return uint32(std::floor(value + 0.5f));
}

int32 SpellEffectInfo::CalcBaseValue(int32 value) const
{
    if (DieSides == 0)
        return value;
    else
        return value - 1;
}

float SpellEffectInfo::CalcValueMultiplier(WorldObject* caster, Spell* spell) const
{
    float multiplier = Amplitude;
    if (Player* modOwner = (caster ? caster->GetSpellModOwner() : nullptr))
        modOwner->ApplySpellMod(_spellInfo->Id, SpellModOp::Amplitude, multiplier, spell);
    return multiplier;
}

float SpellEffectInfo::CalcDamageMultiplier(WorldObject* caster, Spell* spell) const
{
    float multiplierPercent = DamageMultiplier * 100.0f;
    if (Player* modOwner = (caster ? caster->GetSpellModOwner() : nullptr))
        modOwner->ApplySpellMod(_spellInfo->Id, SpellModOp::ChainAmplitude, multiplierPercent, spell);
    return multiplierPercent / 100.0f;
}

bool SpellEffectInfo::HasRadius(SpellTargetIndex targetIndex) const
{
    switch (targetIndex)
    {
        case SpellTargetIndex::TargetA:
            return TargetARadiusEntry != nullptr;
        case SpellTargetIndex::TargetB:
            return TargetBRadiusEntry != nullptr;
        default:
            return false;
    }
}

float SpellEffectInfo::CalcRadius(WorldObject* caster /*= nullptr*/, SpellTargetIndex targetIndex /*=SpellTargetIndex::TargetA*/, Spell* spell /*= nullptr*/) const
{
    // TargetA -> TargetARadiusEntry
    // TargetB -> TargetBRadiusEntry
    // Aura effects have TargetARadiusEntry == TargetBRadiusEntry (mostly)
    SpellRadiusEntry const* entry = TargetARadiusEntry;
    if (targetIndex == SpellTargetIndex::TargetB && HasRadius(targetIndex))
        entry = TargetBRadiusEntry;

    if (!entry)
        return 0.0f;

    float radius = entry->RadiusMin;
    if (caster)
    {
        if (Unit* casterUnit = caster->ToUnit())
            radius += entry->RadiusPerLevel * casterUnit->getLevel();

        radius = std::min(radius, entry->RadiusMax);

        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(_spellInfo->Id, SpellModOp::Radius, radius, spell);
    }

    return radius;
}

uint32 SpellEffectInfo::GetProvidedTargetMask() const
{
    return GetTargetFlagMask(TargetA.GetObjectType()) | GetTargetFlagMask(TargetB.GetObjectType());
}

uint32 SpellEffectInfo::GetMissingTargetMask(bool srcSet /*= false*/, bool dstSet /*= false*/, uint32 mask /*=0*/) const
{
    uint32 effImplicitTargetMask = GetTargetFlagMask(GetUsedTargetObjectType());
    uint32 providedTargetMask = GetProvidedTargetMask() | mask;

    // remove all flags covered by effect target mask
    if (providedTargetMask & TARGET_FLAG_UNIT_MASK)
        effImplicitTargetMask &= ~(TARGET_FLAG_UNIT_MASK);
    if (providedTargetMask & TARGET_FLAG_CORPSE_MASK)
        effImplicitTargetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK);
    if (providedTargetMask & TARGET_FLAG_GAMEOBJECT_ITEM)
        effImplicitTargetMask &= ~(TARGET_FLAG_GAMEOBJECT_ITEM | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_ITEM);
    if (providedTargetMask & TARGET_FLAG_GAMEOBJECT)
        effImplicitTargetMask &= ~(TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_GAMEOBJECT_ITEM);
    if (providedTargetMask & TARGET_FLAG_ITEM)
        effImplicitTargetMask &= ~(TARGET_FLAG_ITEM | TARGET_FLAG_GAMEOBJECT_ITEM);
    if (dstSet || providedTargetMask & TARGET_FLAG_DEST_LOCATION)
        effImplicitTargetMask &= ~(TARGET_FLAG_DEST_LOCATION);
    if (srcSet || providedTargetMask & TARGET_FLAG_SOURCE_LOCATION)
        effImplicitTargetMask &= ~(TARGET_FLAG_SOURCE_LOCATION);

    return effImplicitTargetMask;
}

SpellEffectImplicitTargetTypes SpellEffectInfo::GetImplicitTargetType() const
{
    return _data[Effect].ImplicitTargetType;
}

SpellTargetObjectTypes SpellEffectInfo::GetUsedTargetObjectType() const
{
    return _data[Effect].UsedTargetObjectType;
}

SpellEffectInfo::StaticData SpellEffectInfo::_data[TOTAL_SPELL_EFFECTS] =
{
    // implicit target type           used target object type
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 0
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 1 SPELL_EFFECT_INSTAKILL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 2 SPELL_EFFECT_SCHOOL_DAMAGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 3 SPELL_EFFECT_DUMMY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 4 SPELL_EFFECT_PORTAL_TELEPORT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 5 SPELL_EFFECT_TELEPORT_UNITS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 6 SPELL_EFFECT_APPLY_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 7 SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 8 SPELL_EFFECT_POWER_DRAIN
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 9 SPELL_EFFECT_HEALTH_LEECH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 10 SPELL_EFFECT_HEAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 11 SPELL_EFFECT_BIND
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 12 SPELL_EFFECT_PORTAL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 13 SPELL_EFFECT_RITUAL_BASE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 14 SPELL_EFFECT_RITUAL_SPECIALIZE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 15 SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 16 SPELL_EFFECT_QUEST_COMPLETE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 17 SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 18 SPELL_EFFECT_RESURRECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 19 SPELL_EFFECT_ADD_EXTRA_ATTACKS
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 20 SPELL_EFFECT_DODGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 21 SPELL_EFFECT_EVADE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 22 SPELL_EFFECT_PARRY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 23 SPELL_EFFECT_BLOCK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 24 SPELL_EFFECT_CREATE_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 25 SPELL_EFFECT_WEAPON
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 26 SPELL_EFFECT_DEFENSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 27 SPELL_EFFECT_PERSISTENT_AREA_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 28 SPELL_EFFECT_SUMMON
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 29 SPELL_EFFECT_LEAP
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 30 SPELL_EFFECT_ENERGIZE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 31 SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 32 SPELL_EFFECT_TRIGGER_MISSILE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ_ITEM}, // 33 SPELL_EFFECT_OPEN_LOCK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 34 SPELL_EFFECT_SUMMON_CHANGE_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 35 SPELL_EFFECT_APPLY_AREA_AURA_PARTY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 36 SPELL_EFFECT_LEARN_SPELL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 37 SPELL_EFFECT_SPELL_DEFENSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 38 SPELL_EFFECT_DISPEL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 39 SPELL_EFFECT_LANGUAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 40 SPELL_EFFECT_DUAL_WIELD
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 41 SPELL_EFFECT_JUMP
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 42 SPELL_EFFECT_JUMP_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 43 SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 44 SPELL_EFFECT_SKILL_STEP
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 45 SPELL_EFFECT_ADD_HONOR
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 46 SPELL_EFFECT_SPAWN
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 47 SPELL_EFFECT_TRADE_SKILL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 48 SPELL_EFFECT_STEALTH
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 49 SPELL_EFFECT_DETECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 50 SPELL_EFFECT_TRANS_DOOR
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 51 SPELL_EFFECT_FORCE_CRITICAL_HIT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 52 SPELL_EFFECT_GUARANTEE_HIT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 53 SPELL_EFFECT_ENCHANT_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 54 SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 55 SPELL_EFFECT_TAMECREATURE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 56 SPELL_EFFECT_SUMMON_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 57 SPELL_EFFECT_LEARN_PET_SPELL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 58 SPELL_EFFECT_WEAPON_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 59 SPELL_EFFECT_CREATE_RANDOM_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 60 SPELL_EFFECT_PROFICIENCY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 61 SPELL_EFFECT_SEND_EVENT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 62 SPELL_EFFECT_POWER_BURN
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 63 SPELL_EFFECT_THREAT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 64 SPELL_EFFECT_TRIGGER_SPELL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 65 SPELL_EFFECT_APPLY_AREA_AURA_RAID
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 66 SPELL_EFFECT_CREATE_MANA_GEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 67 SPELL_EFFECT_HEAL_MAX_HEALTH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 68 SPELL_EFFECT_INTERRUPT_CAST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 69 SPELL_EFFECT_DISTRACT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 70 SPELL_EFFECT_PULL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 71 SPELL_EFFECT_PICKPOCKET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 72 SPELL_EFFECT_ADD_FARSIGHT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 73 SPELL_EFFECT_UNTRAIN_TALENTS
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 74 SPELL_EFFECT_APPLY_GLYPH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 75 SPELL_EFFECT_HEAL_MECHANICAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 76 SPELL_EFFECT_SUMMON_OBJECT_WILD
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 77 SPELL_EFFECT_SCRIPT_EFFECT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 78 SPELL_EFFECT_ATTACK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 79 SPELL_EFFECT_SANCTUARY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 80 SPELL_EFFECT_ADD_COMBO_POINTS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 81 SPELL_EFFECT_CREATE_HOUSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 82 SPELL_EFFECT_BIND_SIGHT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 83 SPELL_EFFECT_DUEL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 84 SPELL_EFFECT_STUCK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 85 SPELL_EFFECT_SUMMON_PLAYER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 86 SPELL_EFFECT_ACTIVATE_OBJECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 87 SPELL_EFFECT_GAMEOBJECT_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 88 SPELL_EFFECT_GAMEOBJECT_REPAIR
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 89 SPELL_EFFECT_GAMEOBJECT_SET_DESTRUCTION_STATE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 90 SPELL_EFFECT_KILL_CREDIT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 91 SPELL_EFFECT_THREAT_ALL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 92 SPELL_EFFECT_ENCHANT_HELD_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 93 SPELL_EFFECT_FORCE_DESELECT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 94 SPELL_EFFECT_SELF_RESURRECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 95 SPELL_EFFECT_SKINNING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 96 SPELL_EFFECT_CHARGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 97 SPELL_EFFECT_CAST_BUTTON
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 98 SPELL_EFFECT_KNOCK_BACK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 99 SPELL_EFFECT_DISENCHANT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 100 SPELL_EFFECT_INEBRIATE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 101 SPELL_EFFECT_FEED_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 102 SPELL_EFFECT_DISMISS_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 103 SPELL_EFFECT_REPUTATION
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 104 SPELL_EFFECT_SUMMON_OBJECT_SLOT1
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 105 SPELL_EFFECT_SUMMON_OBJECT_SLOT2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 106 SPELL_EFFECT_SUMMON_OBJECT_SLOT3
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 107 SPELL_EFFECT_SUMMON_OBJECT_SLOT4
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 108 SPELL_EFFECT_DISPEL_MECHANIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 109 SPELL_EFFECT_SUMMON_DEAD_PET
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 110 SPELL_EFFECT_DESTROY_ALL_TOTEMS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 111 SPELL_EFFECT_DURABILITY_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 112 SPELL_EFFECT_112
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 113 SPELL_EFFECT_RESURRECT_NEW
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 114 SPELL_EFFECT_ATTACK_ME
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 115 SPELL_EFFECT_DURABILITY_DAMAGE_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ENEMY}, // 116 SPELL_EFFECT_SKIN_PLAYER_CORPSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 117 SPELL_EFFECT_SPIRIT_HEAL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 118 SPELL_EFFECT_SKILL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 119 SPELL_EFFECT_APPLY_AREA_AURA_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 120 SPELL_EFFECT_TELEPORT_GRAVEYARD
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 121 SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 122 SPELL_EFFECT_122
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 123 SPELL_EFFECT_SEND_TAXI
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 124 SPELL_EFFECT_PULL_TOWARDS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 125 SPELL_EFFECT_MODIFY_THREAT_PERCENT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 126 SPELL_EFFECT_STEAL_BENEFICIAL_BUFF
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 127 SPELL_EFFECT_PROSPECTING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 128 SPELL_EFFECT_APPLY_AREA_AURA_FRIEND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 129 SPELL_EFFECT_APPLY_AREA_AURA_ENEMY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 130 SPELL_EFFECT_REDIRECT_THREAT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 131 SPELL_EFFECT_PLAY_SOUND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 132 SPELL_EFFECT_PLAY_MUSIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 133 SPELL_EFFECT_UNLEARN_SPECIALIZATION
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 134 SPELL_EFFECT_KILL_CREDIT2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 135 SPELL_EFFECT_CALL_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 136 SPELL_EFFECT_HEAL_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 137 SPELL_EFFECT_ENERGIZE_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 138 SPELL_EFFECT_LEAP_BACK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 139 SPELL_EFFECT_CLEAR_QUEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 140 SPELL_EFFECT_FORCE_CAST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 141 SPELL_EFFECT_FORCE_CAST_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 142 SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 143 SPELL_EFFECT_APPLY_AREA_AURA_OWNER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 144 SPELL_EFFECT_KNOCK_BACK_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 145 SPELL_EFFECT_PULL_TOWARDS_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 146 SPELL_EFFECT_ACTIVATE_RUNE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 147 SPELL_EFFECT_QUEST_FAIL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 148 SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 149 SPELL_EFFECT_CHARGE_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 150 SPELL_EFFECT_QUEST_START
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 151 SPELL_EFFECT_TRIGGER_SPELL_2
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 152 SPELL_EFFECT_SUMMON_RAF_FRIEND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 153 SPELL_EFFECT_CREATE_TAMED_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 154 SPELL_EFFECT_DISCOVER_TAXI
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 155 SPELL_EFFECT_TITAN_GRIP
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 156 SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 157 SPELL_EFFECT_CREATE_ITEM_2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 158 SPELL_EFFECT_MILLING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 159 SPELL_EFFECT_ALLOW_RENAME_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 160 SPELL_EFFECT_FORCE_CAST_2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 161 SPELL_EFFECT_TALENT_SPEC_COUNT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 162 SPELL_EFFECT_TALENT_SPEC_SELECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 163 SPELL_EFFECT_163
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 164 SPELL_EFFECT_REMOVE_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 165 SPELL_EFFECT_165
    {EFFECT_IMPLICIT_TARGET_CASTER,   TARGET_OBJECT_TYPE_UNIT}, // 166 SPELL_EFFECT_GIVE_CURRENCY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 167 SPELL_EFFECT_167
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 168 SPELL_EFFECT_168
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_ITEM}, // 169 SPELL_EFFECT_DESTROY_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 170 SPELL_EFFECT_170
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 171 SPELL_EFFECT_SUMMON_PERSONAL_GAMEOBJECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 172 SPELL_EFFECT_RESURRECT_WITH_AURA
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 173 SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 174 SPELL_EFFECT_APPLY_AURA_2
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 175 SPELL_EFFECT_175
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 176 SPELL_EFFECT_176
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 177 SPELL_EFFECT_177
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 178 SPELL_EFFECT_178
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 179 SPELL_EFFECT_CREATE_AREATRIGGER
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 180 SPELL_EFFECT_180
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 181 SPELL_EFFECT_181
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 182 SPELL_EFFECT_182
};

SpellInfo::SpellInfo(SpellEntry const* spellEntry, SpellEffectEntry const** effects)
{
    Id = spellEntry->ID;
    Attributes = spellEntry->Attributes;
    AttributesEx = spellEntry->AttributesEx;
    AttributesEx2 = spellEntry->AttributesEx2;
    AttributesEx3 = spellEntry->AttributesEx3;
    AttributesEx4 = spellEntry->AttributesEx4;
    AttributesEx5 = spellEntry->AttributesEx5;
    AttributesEx6 = spellEntry->AttributesEx6;
    AttributesEx7 = spellEntry->AttributesEx7;
    AttributesEx8 = spellEntry->AttributesEx8;
    AttributesEx9 = spellEntry->AttributesEx9;
    AttributesEx10 = spellEntry->AttributesEx10;
    AttributesCu = 0;
    CastTimeEntry = spellEntry->CastingTimeIndex ? sSpellCastTimesStore.LookupEntry(spellEntry->CastingTimeIndex) : nullptr;
    DurationEntry = spellEntry->DurationIndex ? sSpellDurationStore.LookupEntry(spellEntry->DurationIndex) : nullptr;
    PowerType = spellEntry->PowerType;
    RangeEntry = spellEntry->RangeIndex ? sSpellRangeStore.LookupEntry(spellEntry->RangeIndex) : nullptr;
    Speed = spellEntry->Speed;

    for (uint8 i = 0; i < 2; ++i)
        SpellVisual[i] = spellEntry->SpellVisualID[i];

    SpellIconID = spellEntry->SpellIconID;
    ActiveIconID = spellEntry->ActiveIconID;
    SpellName = spellEntry->Name;
    Rank = spellEntry->NameSubtext;
    SchoolMask = spellEntry->SchoolMask;
    RuneCostID = spellEntry->RuneCostID;
    SpellDifficultyId = spellEntry->Difficulty;
    BonusCoefficient = spellEntry->BonusCoefficient;
    SpellScalingId = spellEntry->ScalingID;
    SpellAuraOptionsId = spellEntry->AuraOptionsID;
    SpellAuraRestrictionsId = spellEntry->AuraRestrictionsID;
    SpellCastingRequirementsId = spellEntry->CastingRequirementsID;
    SpellCategoriesId = spellEntry->CategoriesID;
    SpellClassOptionsId = spellEntry->ClassOptionsID;
    SpellCooldownsId = spellEntry->CooldownsID;
    SpellEquippedItemsId = spellEntry->EquippedItemsID;
    SpellInterruptsId = spellEntry->InterruptsID;
    SpellLevelsId = spellEntry->LevelsID;
    SpellPowerId = spellEntry->PowerDisplayID;
    SpellReagentsId = spellEntry->ReagentsID;
    SpellShapeshiftId = spellEntry->ShapeshiftID;
    SpellTargetRestrictionsId = spellEntry->TargetRestrictionsID;
    SpellTotemsId = spellEntry->TotemsID;
    ResearchProjectId = spellEntry->RequiredProjectID;

    // SpellDifficultyEntry
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        Effects[i] = SpellEffectInfo(spellEntry, this, i, effects[i]);

    // SpellScalingEntry
    SpellScalingEntry const* _scaling = GetSpellScaling();
    Scaling.CastTimeMin = _scaling ? _scaling->CastTimeMin : 0;
    Scaling.CastTimeMax = _scaling ? _scaling->CastTimeMax : 0;
    Scaling.CastTimeMaxLevel = _scaling ? _scaling->CastTimeMaxLevel : 0;
    Scaling.Class = _scaling ? _scaling->Class : 0;
    Scaling.NerfFactor = _scaling ? _scaling->NerfFactor : 0.f;
    Scaling.NerfMaxLevel = _scaling ? _scaling->NerfMaxLevel : 0;

    // SpellAuraOptionsEntry
    SpellAuraOptionsEntry const* _options = GetSpellAuraOptions();
    ProcFlags = _options ? _options->ProcTypeMask : 0;
    ProcChance = _options ? _options->ProcChance : 0;
    ProcCharges = _options ? _options->ProcCharges : 0;
    StackAmount = _options ? _options->CumulativeAura : 0;

    // SpellAuraRestrictionsEntry
    SpellAuraRestrictionsEntry const* _aura = GetSpellAuraRestrictions();
    CasterAuraState = _aura ? _aura->CasterAuraState : 0;
    TargetAuraState = _aura ? _aura->TargetAuraState : 0;
    CasterAuraStateNot = _aura ? _aura->ExcludeCasterAuraState : 0;
    TargetAuraStateNot = _aura ? _aura->ExcludeTargetAuraState : 0;
    CasterAuraSpell = _aura ? _aura->CasterAuraSpell : 0;
    TargetAuraSpell = _aura ? _aura->TargetAuraSpell : 0;
    ExcludeCasterAuraSpell = _aura ? _aura->ExcludeCasterAuraSpell : 0;
    ExcludeTargetAuraSpell = _aura ? _aura->ExcludeTargetAuraSpell : 0;

    // SpellCastingRequirementsEntry
    SpellCastingRequirementsEntry const* _castreq = GetSpellCastingRequirements();
    RequiresSpellFocus = _castreq ? _castreq->RequiresSpellFocus : 0;
    FacingCasterFlags = _castreq ? SpellFacingCasterFlags(_castreq->FacingCasterFlags) : SpellFacingCasterFlags::None;
    MinFactionId = _castreq ? _castreq->MinFactionID : 0;
    MinReputation = _castreq ? _castreq->MinReputation : 0;
    AreaGroupId = _castreq ? _castreq->RequiredAreasID : -1;

    // SpellCategoriesEntry
    SpellCategoriesEntry const* _categorie = GetSpellCategories();
    CategoryEntry = _categorie ? sSpellCategoryStore.LookupEntry(_categorie->Category) : nullptr;
    Dispel = _categorie ? _categorie->DispelType : 0;
    Mechanic = _categorie ? _categorie->Mechanic : 0;
    StartRecoveryCategory = _categorie ? _categorie->StartRecoveryCategory : 0;
    DmgClass = _categorie ? _categorie->DefenseType : 0;
    PreventionType = _categorie ? _categorie->PreventionType : 0;

    // SpellClassOptionsEntry
    SpellClassOptionsEntry const* _class = GetSpellClassOptions();
    SpellFamilyName = _class ? _class->SpellClassSet : 0;
    SpellFamilyFlags = _class ? _class->SpellFamilyMask : flag96(0);

    // SpellCooldownsEntry
    SpellCooldownsEntry const* _cooldowns = GetSpellCooldowns();
    RecoveryTime = _cooldowns ? _cooldowns->RecoveryTime : 0;
    CategoryRecoveryTime = _cooldowns ? _cooldowns->CategoryRecoveryTime : 0;
    StartRecoveryTime = _cooldowns ? _cooldowns->StartRecoveryTime : 0;

    // SpellEquippedItemsEntry
    SpellEquippedItemsEntry const* _equipped = GetSpellEquippedItems();
    EquippedItemClass = _equipped ? _equipped->EquippedItemClass : -1;
    EquippedItemSubClassMask = _equipped ?_equipped->EquippedItemSubclass : -1;
    EquippedItemInventoryTypeMask = _equipped ? _equipped->EquippedItemInvTypes : -1;

    // SpellInterruptsEntry
    SpellInterruptsEntry const* _interrupt = GetSpellInterrupts();

    AuraInterruptFlags = _interrupt ? SpellAuraInterruptFlags(_interrupt->AuraInterruptFlags[0]) : SpellAuraInterruptFlags::None;
    AuraInterruptFlags2 = _interrupt? SpellAuraInterruptFlags2(_interrupt->AuraInterruptFlags[1]) : SpellAuraInterruptFlags2::None;
    ChannelInterruptFlags = _interrupt ? SpellAuraInterruptFlags(_interrupt->ChannelInterruptFlags[0] ) : SpellAuraInterruptFlags::None;
    ChannelInterruptFlags2 =  _interrupt ? SpellAuraInterruptFlags2(_interrupt->ChannelInterruptFlags[1]) : SpellAuraInterruptFlags2::None;
    InterruptFlags = _interrupt ? SpellInterruptFlags(_interrupt->InterruptFlags) : SpellInterruptFlags::None;

    // SpellLevelsEntry
    SpellLevelsEntry const* _levels = GetSpellLevels();
    MaxLevel = _levels ? _levels->MaxLevel : 0;
    BaseLevel = _levels ? _levels->BaseLevel : 0;
    SpellLevel = _levels ? _levels->SpellLevel : 0;

    // SpellPowerEntry
    SpellPowerEntry const* _power = GetSpellPower();
    ManaCost = _power ? _power->ManaCost : 0;
    ManaCostPerlevel = _power ? _power->ManaCostPerLevel : 0;
    ManaCostPercentage = _power ? _power->PowerCostPct : 0;
    ManaCostPercentage2 = _power ? _power->PowerCostPct2 : 0.f;
    ManaPerSecond = _power ? _power->ManaPerSecond : 0;

    // SpellReagentsEntry
    SpellReagentsEntry const* _reagents = GetSpellReagents();
    for (uint8 i = 0; i < MAX_SPELL_REAGENTS; ++i)
        Reagent[i] = _reagents ? _reagents->Reagents[i] : 0;
    for (uint8 i = 0; i < MAX_SPELL_REAGENTS; ++i)
        ReagentCount[i] = _reagents ? _reagents->ReagentCount[i] : 0;

    // SpellShapeshiftEntry
    if (SpellShapeshiftEntry const* _shapeshift = GetSpellShapeshift())
    {
        Stances = MAKE_PAIR64(_shapeshift->ShapeshiftMask[0], _shapeshift->ShapeshiftMask[1]);
        StancesNot = MAKE_PAIR64(_shapeshift->ShapeshiftExclude[0], _shapeshift->ShapeshiftExclude[1]);
    }
    else
    {
        Stances = UI64LIT(0);
        StancesNot = UI64LIT(0);
    }

    // SpellTargetRestrictionsEntry
    SpellTargetRestrictionsEntry const* _target = GetSpellTargetRestrictions();
    ConeAngle = _target ? _target->ConeAngle : 0.f;
    Targets = _target ? _target->Targets : 0;
    TargetCreatureType = _target ? _target->TargetCreatureType : 0;
    MaxAffectedTargets = _target ? _target->MaxTargets : 0;
    MaxTargetLevel = _target ? _target->MaxTargetLevel : 0;

    // SpellTotemsEntry
    SpellTotemsEntry const* _totem = GetSpellTotems();
    for (uint8 i = 0; i < 2; ++i)
        TotemCategory[i] = _totem ? _totem->RequiredTotemCategoryID[i] : 0;
    for (uint8 i = 0; i < 2; ++i)
        Totem[i] = _totem ? _totem->Totem[i] : 0;

    ChainEntry = nullptr;
    ExplicitTargetMask = 0;

    _spellSpecific = SPELL_SPECIFIC_NORMAL;
    _auraState = AURA_STATE_NONE;

    _allowedMechanicMask = 0;
}

SpellInfo::~SpellInfo()
{
    _UnloadImplicitTargetConditionLists();
}

uint32 SpellInfo::GetCategory() const
{
    return CategoryEntry ? CategoryEntry->ID : 0;
}

bool SpellInfo::HasEffect(SpellEffects effect) const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect(effect))
            return true;
    return false;
}

bool SpellInfo::HasAura(AuraType aura) const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsAura(aura))
            return true;
    return false;
}

bool SpellInfo::HasAreaAuraEffect() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsAreaAuraEffect())
            return true;
    return false;
}

bool SpellInfo::HasOnlyDamageEffects() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].IsEffect())
        {
            switch (Effects[i].Effect)
            {
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
                case SPELL_EFFECT_HEALTH_LEECH:
                    continue;
                default:
                    return false;
            }
        }
    }

    return true;
}

bool SpellInfo::HasTargetType(::Targets target) const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (!Effects[i].IsEffect())
            continue;

        if (Effects[i].TargetA.GetTarget() == target || Effects[i].TargetB.GetTarget() == target)
            return true;
    }

    return false;
}

bool SpellInfo::CanBeInterrupted(Unit const* interruptTarget, bool ignoreImmunity /*= false*/) const
{
    return HasAttribute(SPELL_ATTR7_NO_UI_NOT_INTERRUPTIBLE)
        || HasChannelInterruptFlag(SpellAuraInterruptFlags::Damage | SpellAuraInterruptFlags::EnteringCombat)
        || (interruptTarget->IsPlayer() && InterruptFlags.HasFlag(SpellInterruptFlags::DamageCancelsPlayerOnly))
        || ((!(interruptTarget->GetMechanicImmunityMask() & (1 << MECHANIC_INTERRUPT)) || ignoreImmunity)
        && PreventionType & SPELL_PREVENTION_TYPE_SILENCE);
}

bool SpellInfo::HasAnyAuraInterruptFlag() const
{
    return AuraInterruptFlags != SpellAuraInterruptFlags::None || AuraInterruptFlags2 != SpellAuraInterruptFlags2::None;
}

bool SpellInfo::IsExplicitDiscovery() const
{
    return ((Effects[0].Effect == SPELL_EFFECT_CREATE_RANDOM_ITEM
        || Effects[0].Effect == SPELL_EFFECT_CREATE_ITEM_2)
        && Effects[1].Effect == SPELL_EFFECT_SCRIPT_EFFECT)
        || Id == 64323;
}

bool SpellInfo::IsLootCrafting() const
{
    return (Effects[0].Effect == SPELL_EFFECT_CREATE_RANDOM_ITEM ||
        // different random cards from Inscription (121==Virtuoso Inking Set category) r without explicit item
        (Effects[0].Effect == SPELL_EFFECT_CREATE_ITEM_2 &&
        ((TotemCategory[0] != 0 || (Totem[0] != 0 && SpellIconID == 1)) || Effects[0].ItemType == 0)));
}

bool SpellInfo::IsQuestTame() const
{
    return Effects[0].Effect == SPELL_EFFECT_THREAT && Effects[1].Effect == SPELL_EFFECT_APPLY_AURA && Effects[1].ApplyAuraName == SPELL_AURA_DUMMY;
}

bool SpellInfo::IsProfessionOrRiding() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsProfessionOrRidingSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsProfession() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsProfessionSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsPrimaryProfession() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsPrimaryProfessionSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsPrimaryProfessionFirstRank() const
{
    return IsPrimaryProfession() && GetRank() == 1;
}

bool SpellInfo::IsAbilityLearnedWithProfession() const
{
    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(Id);

    for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
    {
        SkillLineAbilityEntry const* pAbility = _spell_idx->second;
        if (!pAbility || pAbility->AcquireMethod != SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE)
            continue;

        if (pAbility->MinSkillLineRank > 0)
            return true;
    }

    return false;
}

bool SpellInfo::IsAbilityOfSkillType(uint32 skillType) const
{
    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(Id);

    for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
        if (_spell_idx->second->SkillLine == uint32(skillType))
            return true;

    return false;
}

bool SpellInfo::IsAffectingArea() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && (Effects[i].IsTargetingArea() || Effects[i].IsEffect(SPELL_EFFECT_PERSISTENT_AREA_AURA) || Effects[i].IsAreaAuraEffect()))
            return true;
    return false;
}

// checks if spell targets are selected from area, doesn't include spell effects in check (like area wide auras for example)
bool SpellInfo::IsTargetingArea() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && Effects[i].IsTargetingArea())
            return true;
    return false;
}

bool SpellInfo::NeedsExplicitUnitTarget() const
{
    return (GetExplicitTargetMask() & TARGET_FLAG_UNIT_MASK) != 0;
}

bool SpellInfo::NeedsToBeTriggeredByCaster(SpellInfo const* triggeringSpell) const
{
    if (NeedsExplicitUnitTarget())
        return true;

    /*
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].IsEffect())
        {
            if (Effects[i].TargetA.GetSelectionCategory() == TARGET_SELECT_CATEGORY_CHANNEL
                || Effects[i].TargetB.GetSelectionCategory() == TARGET_SELECT_CATEGORY_CHANNEL)
                return true;
        }
    }
    */

    if (triggeringSpell->IsChanneled())
    {
        uint32 mask = 0;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER && Effects[i].TargetA.GetTarget() != TARGET_DEST_CASTER
                && Effects[i].TargetB.GetTarget() != TARGET_UNIT_CASTER && Effects[i].TargetB.GetTarget() != TARGET_DEST_CASTER)
            {
                mask |= Effects[i].GetProvidedTargetMask();
            }
        }

        if (mask & TARGET_FLAG_UNIT_MASK)
            return true;
    }

    return false;
}

bool SpellInfo::IsSelfCast() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].Effect && Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER)
            return false;
    return true;
}

bool SpellInfo::IsPassive() const
{
    return HasAttribute(SPELL_ATTR0_PASSIVE);
}

bool SpellInfo::IsAutocastable() const
{
    if (IsPassive())
        return false;
    if (HasAttribute(SPELL_ATTR1_NO_AUTOCAST_AI))
        return false;
    return true;
}

bool SpellInfo::IsStackableWithRanks() const
{
    if (IsPassive())
        return false;
    if (PowerType != POWER_MANA && PowerType != POWER_HEALTH)
        return false;
    if (IsProfessionOrRiding())
        return false;

    if (IsAbilityLearnedWithProfession())
        return false;

    // All stance spells. if any better way, change it.
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_PALADIN:
                // Paladin aura Spell
                if (Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID)
                    return false;
                break;
            case SPELLFAMILY_DRUID:
                // Druid form Spell
                if (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA &&
                    Effects[i].ApplyAuraName == SPELL_AURA_MOD_SHAPESHIFT)
                    return false;
                break;
        }
    }
    return true;
}

bool SpellInfo::IsPassiveStackableWithRanks() const
{
    return IsPassive() && !HasEffect(SPELL_EFFECT_APPLY_AURA);
}

bool SpellInfo::IsMultiSlotAura() const
{
    if (IsPassive() || HasAura(SPELL_AURA_CONTROL_VEHICLE))
        return true;

    switch (Id)
    {
        case 55849: // Power Spark
        case 40075: // Fel Flak Fire
        case 44413: // Incanter's Absorption
        case 79010: // Point of Vulnerability
        case 79920: // Shared Health
            return true;
        default:
            return false;
    }
}

bool SpellInfo::IsStackableOnOneSlotWithDifferentCasters() const
{
    /// TODO: Re-verify meaning of SPELL_ATTR3_STACK_FOR_DIFF_CASTERS and update conditions here
    return StackAmount > 1 && !IsChanneled() && !HasAttribute(SPELL_ATTR3_DOT_STACKING_RULE);
}

bool SpellInfo::IsCooldownStartedOnEvent() const
{
    if (HasAttribute(SPELL_ATTR0_COOLDOWN_ON_EVENT))
        return true;

    return CategoryEntry && CategoryEntry->GetFlags().HasFlag(SpellCategoryFlags::CooldownEventOnLeaveCombat);
}

bool SpellInfo::IsDeathPersistent() const
{
    return HasAttribute(SPELL_ATTR3_ALLOW_AURA_WHILE_DEAD);
}

bool SpellInfo::IsRequiringDeadTarget() const
{
    return HasAttribute(SPELL_ATTR3_ONLY_ON_GHOSTS);
}

bool SpellInfo::IsAllowingDeadTarget() const
{
    if (HasAttribute(SPELL_ATTR2_ALLOW_DEAD_TARGET) || Targets & (TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_UNIT_DEAD))
        return true;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].TargetA.GetTarget() == TARGET_CORPSE_SRC_AREA_RAID)
            return true;

    return false;
}

bool SpellInfo::IsGroupBuff() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (Effects[i].TargetA.GetCheckType())
        {
            case TARGET_CHECK_PARTY:
            case TARGET_CHECK_RAID:
            case TARGET_CHECK_RAID_CLASS:
                return true;
            default:
                break;
        }
    }

    return false;
}

bool SpellInfo::CanBeUsedInCombat() const
{
    return !HasAttribute(SPELL_ATTR0_NOT_IN_COMBAT_ONLY_PEACEFUL);
}

bool SpellInfo::IsPositive() const
{
    return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE);
}

bool SpellInfo::IsPositiveEffect(uint8 effIndex) const
{
    switch (effIndex)
    {
        default:
        case 0:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF0);
        case 1:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF1);
        case 2:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF2);
    }
}

bool SpellInfo::IsChanneled() const
{
    return HasAttribute(SpellAttr1(SPELL_ATTR1_IS_CHANNELLED | SPELL_ATTR1_IS_SELF_CHANNELLED));
}

bool SpellInfo::IsMoveAllowedChannel() const
{
    return IsChanneled() && !ChannelInterruptFlags.HasFlag(SpellAuraInterruptFlags::Moving | SpellAuraInterruptFlags::Turning);
}

bool SpellInfo::NeedsComboPoints() const
{
    return HasAttribute(SpellAttr1(SPELL_ATTR1_FINISHING_MOVE_DAMAGE | SPELL_ATTR1_FINISHING_MOVE_DURATION));
}

bool SpellInfo::IsNextMeleeSwingSpell() const
{
    return HasAttribute(SpellAttr0(SPELL_ATTR0_ON_NEXT_SWING_NO_DAMAGE | SPELL_ATTR0_ON_NEXT_SWING));
}

bool SpellInfo::IsRangedWeaponSpell() const
{
    return (EquippedItemClass == ITEM_CLASS_WEAPON) && (EquippedItemSubClassMask & ITEM_SUBCLASS_MASK_WEAPON_RANGED);
}

bool SpellInfo::IsAutoRepeatRangedSpell() const
{
    return HasAttribute(SPELL_ATTR2_AUTO_REPEAT);
}

bool SpellInfo::CausesInitialThreat() const
{
    return !HasAttribute(SPELL_ATTR1_NO_THREAT) && !HasAttribute(SPELL_ATTR2_NO_INITIAL_THREAT) && !HasAttribute(SPELL_ATTR4_NO_HARMFUL_THREAT);
}

WeaponAttackType SpellInfo::GetAttackType() const
{
    WeaponAttackType result;
    switch (DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            if (HasAttribute(SPELL_ATTR3_REQUIRES_OFF_HAND_WEAPON))
                result = OFF_ATTACK;
            else
                result = BASE_ATTACK;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            result = IsRangedWeaponSpell() ? RANGED_ATTACK : BASE_ATTACK;
            break;
        default:
            // Wands
            if (IsAutoRepeatRangedSpell())
                result = RANGED_ATTACK;
            else
                result = BASE_ATTACK;
            break;
    }

    return result;
}

bool SpellInfo::IsAffected(uint32 familyName, flag96 const& familyFlags) const
{
    if (!familyName)
        return true;

    if (familyName != SpellFamilyName)
        return false;

    if (familyFlags && !(familyFlags & SpellFamilyFlags))
        return false;

    return true;
}

bool SpellInfo::IsAffectedBySpellMods() const
{
    return !HasAttribute(SPELL_ATTR3_IGNORE_CASTER_MODIFIERS);
}

bool SpellInfo::IsAffectedBySpellMod(SpellModifier const* mod) const
{
    if (!IsAffectedBySpellMods())
        return false;

    SpellInfo const* affectSpell = sSpellMgr->GetSpellInfo(mod->spellId);
    if (!affectSpell)
        return false;

    // TEMP: dont use IsAffected - !familyName and !familyFlags are not valid options for spell mods
    // TODO: investigate if the !familyName and !familyFlags conditions are even valid for all other (nonmod) uses of SpellInfo::IsAffected
    return affectSpell->SpellFamilyName == SpellFamilyName && mod->mask & SpellFamilyFlags;
}

bool SpellInfo::CanPierceImmuneAura(SpellInfo const* auraSpellInfo) const
{
    // aura can't be pierced
    if (!auraSpellInfo || auraSpellInfo->HasAttribute(SPELL_ATTR0_NO_IMMUNITIES))
        return false;

    // these spells pierce all available spells (Resurrection Sickness for example)
    if (HasAttribute(SPELL_ATTR0_NO_IMMUNITIES))
        return true;

    // these spells (Cyclone for example) can pierce all...
    if (HasAttribute(SPELL_ATTR1_IMMUNITY_TO_HOSTILE_AND_FRIENDLY_EFFECTS) || HasAttribute(SPELL_ATTR2_NO_SCHOOL_IMMUNITIES))
    {
        // ...but not these (Divine shield, Ice block, Cyclone and Banish for example)
        if (auraSpellInfo->Mechanic != MECHANIC_IMMUNE_SHIELD &&
               auraSpellInfo->Mechanic != MECHANIC_INVULNERABILITY &&
               (auraSpellInfo->Mechanic != MECHANIC_BANISH || (IsRankOf(auraSpellInfo) && auraSpellInfo->Dispel != DISPEL_NONE))) // Banish shouldn't be immune to itself, but Cyclone should
            return true;
    }

    // Dispels other auras on immunity, check if this spell makes the unit immune to aura
    if (HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT) && CanSpellProvideImmunityAgainstAura(auraSpellInfo))
        return true;

    return false;
}

bool SpellInfo::CanDispelAura(SpellInfo const* auraSpellInfo) const
{
    // These auras (like Divine Shield) can't be dispelled
    if (auraSpellInfo->HasAttribute(SPELL_ATTR0_NO_IMMUNITIES))
        return false;

    // These spells (like Mass Dispel) can dispel all auras
    if (HasAttribute(SPELL_ATTR0_NO_IMMUNITIES))
        return true;

    // These auras (Cyclone for example) are not dispelable
    if ((auraSpellInfo->HasAttribute(SPELL_ATTR1_IMMUNITY_TO_HOSTILE_AND_FRIENDLY_EFFECTS) && auraSpellInfo->Mechanic != MECHANIC_NONE)
        || auraSpellInfo->HasAttribute(SPELL_ATTR2_NO_SCHOOL_IMMUNITIES))
        return false;

    return true;
}

bool SpellInfo::IsSingleTarget() const
{
    // all other single target spells have if it has AttributesEx5
    if (HasAttribute(SPELL_ATTR5_LIMIT_N))
        return true;

    return false;
}

bool SpellInfo::IsAuraExclusiveBySpecificWith(SpellInfo const* spellInfo) const
{
    SpellSpecificType spellSpec1 = GetSpellSpecific();
    SpellSpecificType spellSpec2 = spellInfo->GetSpellSpecific();
    switch (spellSpec1)
    {
        case SPELL_SPECIFIC_WARLOCK_ARMOR:
        case SPELL_SPECIFIC_MAGE_ARMOR:
        case SPELL_SPECIFIC_ELEMENTAL_SHIELD:
        case SPELL_SPECIFIC_MAGE_POLYMORPH:
        case SPELL_SPECIFIC_PRESENCE:
        case SPELL_SPECIFIC_CHARM:
        case SPELL_SPECIFIC_SCROLL:
        case SPELL_SPECIFIC_WARRIOR_ENRAGE:
        case SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE:
        case SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT:
            return spellSpec1 == spellSpec2;
        case SPELL_SPECIFIC_FOOD:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_FOOD_AND_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        default:
            return false;
    }
}

bool SpellInfo::IsAuraExclusiveBySpecificPerCasterWith(SpellInfo const* spellInfo) const
{
    SpellSpecificType spellSpec = GetSpellSpecific();
    switch (spellSpec)
    {
        case SPELL_SPECIFIC_SEAL:
        case SPELL_SPECIFIC_HAND:
        case SPELL_SPECIFIC_AURA:
        case SPELL_SPECIFIC_STING:
        case SPELL_SPECIFIC_CURSE:
        case SPELL_SPECIFIC_BANE:
        case SPELL_SPECIFIC_ASPECT:
        case SPELL_SPECIFIC_WARLOCK_CORRUPTION:
            return spellSpec == spellInfo->GetSpellSpecific();
        default:
            return false;
    }
}

SpellCastResult SpellInfo::CheckShapeshift(uint32 form) const
{
    // talents that learn spells can have stance requirements that need ignore
    // (this requirement only for client-side stance show in talent description)
    if (sDBCManager.GetTalentSpellCost(Id) > 0 &&
        (Effects[0].Effect == SPELL_EFFECT_LEARN_SPELL || Effects[1].Effect == SPELL_EFFECT_LEARN_SPELL || Effects[2].Effect == SPELL_EFFECT_LEARN_SPELL))
        return SPELL_CAST_OK;

    uint64 stanceMask = (form ? UI64LIT(1) << (form - 1) : 0);

    if (stanceMask & StancesNot)                 // can explicitly not be cast in this stance
        return SPELL_FAILED_NOT_SHAPESHIFT;

    if (stanceMask & Stances)                    // can explicitly be cast in this stance
        return SPELL_CAST_OK;

    bool actAsShifted = false;
    SpellShapeshiftFormEntry const* shapeInfo = nullptr;
    if (form > 0)
    {
        shapeInfo = sSpellShapeshiftFormStore.LookupEntry(form);
        if (!shapeInfo)
        {
            TC_LOG_ERROR("spells", "GetErrorAtShapeshiftedCast: unknown shapeshift %u", form);
            return SPELL_CAST_OK;
        }
        actAsShifted = !shapeInfo->GetFlags().HasFlag(SpellShapeshiftFormFlags::Stance);
    }

    if (actAsShifted)
    {
        if (HasAttribute(SPELL_ATTR0_NOT_SHAPESHIFTED) || (shapeInfo && shapeInfo->GetFlags().HasFlag(SpellShapeshiftFormFlags::CanOnlyCastShapeshiftSpells))) // not while shapeshifted
            return SPELL_FAILED_NOT_SHAPESHIFT;
        else if (Stances != 0)                   // needs other shapeshift
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }
    else
    {
        // needs shapeshift
        if (!HasAttribute(SPELL_ATTR2_ALLOW_WHILE_NOT_SHAPESHIFTED_CASTER_FORM) && Stances != 0)
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    // Check if stance disables cast of not-stance spells
    // Example: cannot cast any other spells in zombie or ghoul form
    /// @todo Find a way to disable use of these spells clientside
    if (shapeInfo && shapeInfo->Flags & 0x400)
    {
        if (!(stanceMask & Stances))
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckLocation(uint32 map_id, uint32 zone_id, uint32 area_id, Player const* player) const
{
    // normal case
    if (AreaGroupId > 0)
    {
        bool found = false;
        AreaGroupEntry const* groupEntry = sAreaGroupStore.LookupEntry(AreaGroupId);
        while (groupEntry)
        {
            for (uint8 i = 0; i < MAX_GROUP_AREA_IDS; ++i)
                if (DBCManager::IsInArea(area_id, groupEntry->AreaID[i]))
                    found = true;

            if (found || !groupEntry->NextAreaID)
                break;
            // Try search in next group
            groupEntry = sAreaGroupStore.LookupEntry(groupEntry->NextAreaID);
        }

        if (!found)
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // continent limitation (virtual continent)
    if (HasAttribute(SPELL_ATTR4_ONLY_FLYING_AREAS))
    {
        EnumFlag<AreaMountFlags> mountFlags = AreaMountFlags::None;
        if (player && player->HasAuraType(SPELL_AURA_MOUNT_RESTRICTIONS))
        {
            for (AuraEffect const* auraEffect : player->GetAuraEffectsByType(SPELL_AURA_MOUNT_RESTRICTIONS))
                mountFlags |= AreaMountFlags(auraEffect->GetMiscValue());
        }
        else if (AreaTableEntry const* areaTable = sAreaTableStore.LookupEntry(area_id))
            mountFlags = areaTable->GetMountFlags();

        if (!(mountFlags.HasFlag(AreaMountFlags::AllowFlyingMounts)))
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // raid instance limitation
    if (HasAttribute(SPELL_ATTR6_NOT_IN_RAID_INSTANCES))
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
        if (!mapEntry || mapEntry->IsRaid())
            return SPELL_FAILED_NOT_IN_RAID_INSTANCE;
    }

    if (HasAttribute(SPELL_ATTR8_REMOVE_OUTSIDE_DUNGEONS_AND_RAIDS))
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
        if (!mapEntry || !mapEntry->IsDungeon())
            return SPELL_FAILED_TARGET_NOT_IN_INSTANCE;
    }

    if (HasAttribute(SPELL_ATTR8_NOT_IN_BATTLEGROUND))
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
        if (!mapEntry || mapEntry->IsBattleground())
            return SPELL_FAILED_NOT_IN_BATTLEGROUND;
    }

    // DB base check (if non empty then must fit at least single for allow)
    SpellAreaMapBounds saBounds = sSpellMgr->GetSpellAreaMapBounds(Id);
    if (saBounds.first != saBounds.second)
    {
        for (SpellAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
        {
            if (itr->second.IsFitToRequirements(player, zone_id, area_id))
                return SPELL_CAST_OK;
        }
        return SPELL_FAILED_INCORRECT_AREA;
    }

    // bg spell checks
    switch (Id)
    {
        case 23333:                                         // Horde Flag
        case 23335:                                         // Alliance Flag
            return (map_id == 489 || map_id == 726) && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 34976:                                         // Netherstorm Flag
            return map_id == 566 && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 2584:                                          // Waiting to Resurrect
        case 22011:                                         // Spirit Heal Channel
        case 22012:                                         // Spirit Heal
        case 24171:                                         // Resurrection Impact Visual
        case 42792:                                         // Recently Dropped Flag
        case 43681:                                         // Inactive
        case 44535:                                         // Spirit Heal (mana)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return zone_id == AREA_WINTERGRASP || (mapEntry->IsBattleground() && player && player->InBattleground()) ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 44521:                                         // Preparation
        {
            if (!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if (!mapEntry->IsBattleground())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* bg = player->GetBattleground();
            return bg && bg->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32724:                                         // Gold Team (Alliance)
        case 32725:                                         // Green Team (Alliance)
        case 35774:                                         // Gold Team (Horde)
        case 35775:                                         // Green Team (Horde)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return mapEntry->IsBattleArena() && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32727:                                         // Arena Preparation
        {
            if (!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if (!mapEntry->IsBattleArena())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* bg = player->GetBattleground();
            return bg && bg->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
    }

    // aura limitations
    if (player)
    {
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!Effects[i].IsAura())
                continue;

            switch (Effects[i].ApplyAuraName)
            {
                case SPELL_AURA_MOD_SHAPESHIFT:
                {
                    if (SpellShapeshiftFormEntry const* spellShapeshiftForm = sSpellShapeshiftFormStore.LookupEntry(Effects[i].MiscValue))
                        if (uint32 mountType = spellShapeshiftForm->MountTypeID)
                            if (!player->GetMountCapability(mountType))
                                return SPELL_FAILED_NOT_HERE;
                    break;
                }
                case SPELL_AURA_MOUNTED:
                {
                    if (Effects[i].MiscValueB && !player->GetMountCapability(Effects[i].MiscValueB))
                        return SPELL_FAILED_NOT_HERE;
                    break;
                }
                break;
            }
        }
    }
    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckTarget(WorldObject const* caster, WorldObject const* target, bool implicit /*= true*/) const
{
    if (HasAttribute(SPELL_ATTR1_EXCLUDE_CASTER) && caster == target)
        return SPELL_FAILED_BAD_TARGETS;

    // check visibility - ignore stealth for implicit (area) targets
    if (!HasAttribute(SPELL_ATTR6_IGNORE_PHASE_SHIFT) && !caster->CanSeeOrDetect(target, implicit))
        return SPELL_FAILED_BAD_TARGETS;

    Unit const* unitTarget = target->ToUnit();

    /*
    * @todo: 4.x - old hold due to an upcoming tempsummon rework
    if (HasAttribute(SPELL_ATTR8_ONLY_TARGET_IF_SAME_CREATOR))
        if (caster != target && caster->GetGUID() != target->GetOwnerGUID())
            return SPELL_FAILED_BAD_TARGETS;
    */

    // creature/player specific target checks
    if (unitTarget)
    {
        // spells cannot be cast if target has a pet in combat either
        if (HasAttribute(SPELL_ATTR1_ONLY_PEACEFUL_TARGETS) && (unitTarget->IsInCombat() || unitTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PET_IN_COMBAT)))
            return SPELL_FAILED_TARGET_AFFECTING_COMBAT;

        // only spells with SPELL_ATTR3_ONLY_TARGET_GHOSTS can target ghosts
        if (HasAttribute(SPELL_ATTR3_ONLY_ON_GHOSTS) != unitTarget->HasAuraType(SPELL_AURA_GHOST))
        {
            if (HasAttribute(SPELL_ATTR3_ONLY_ON_GHOSTS))
                return SPELL_FAILED_TARGET_NOT_GHOST;
            else
                return SPELL_FAILED_BAD_TARGETS;
        }

        if (caster != unitTarget)
        {
            if (caster->GetTypeId() == TYPEID_PLAYER)
            {
                // Do not allow these spells to target creatures not tapped by us (Banish, Polymorph, many quest spells)
                if (HasAttribute(SPELL_ATTR2_CANNOT_CAST_ON_TAPPED))
                    if (Creature const* targetCreature = unitTarget->ToCreature())
                        if (targetCreature->hasLootRecipient() && !targetCreature->isTappedBy(caster->ToPlayer()))
                            return SPELL_FAILED_CANT_CAST_ON_TAPPED;

                if (HasAttribute(SPELL_ATTR0_CU_PICKPOCKET))
                {
                    Creature const* targetCreature = unitTarget->ToCreature();
                    if (!targetCreature)
                        return SPELL_FAILED_BAD_TARGETS;

                    if (!LootTemplates_Pickpocketing.HaveLootFor(targetCreature->GetCreatureTemplate()->pickpocketLootId))
                        return SPELL_FAILED_TARGET_NO_POCKETS;
                }

                // Not allow disarm unarmed player
                if (Mechanic == MECHANIC_DISARM)
                {
                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        Player const* player = unitTarget->ToPlayer();
                        if (!player->GetWeaponForAttack(BASE_ATTACK) || !player->IsUseEquipedWeapon(true))
                            return SPELL_FAILED_TARGET_NO_WEAPONS;
                    }
                    else if (!unitTarget->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID))
                        return SPELL_FAILED_TARGET_NO_WEAPONS;
                }
            }
        }

        if (HasAttribute(SPELL_ATTR8_ONLY_TARGET_OWN_SUMMONS))
            if (!unitTarget->IsSummon() || unitTarget->ToTempSummon()->GetSummonerGUID() != caster->GetGUID())
                return SPELL_FAILED_BAD_TARGETS;
    }
    // corpse specific target checks
    else if (Corpse const* corpseTarget = target->ToCorpse())
    {
        // cannot target bare bones
        if (corpseTarget->GetType() == CORPSE_BONES)
            return SPELL_FAILED_BAD_TARGETS;
        // we have to use owner for some checks (aura preventing resurrection for example)
        if (Player* owner = ObjectAccessor::FindPlayer(corpseTarget->GetOwnerGUID()))
            unitTarget = owner;
        // we're not interested in corpses without owner
        else
            return SPELL_FAILED_BAD_TARGETS;
    }
    // other types of objects - always valid
    else return SPELL_CAST_OK;

    // corpseOwner and unit specific target checks
    if (!unitTarget->IsPlayer())
    {
        if (HasAttribute(SPELL_ATTR3_ONLY_ON_PLAYER))
            return SPELL_FAILED_TARGET_NOT_PLAYER;

        if (HasAttribute(SPELL_ATTR5_NOT_ON_PLAYER_CONTROLLED_NPC) && unitTarget->IsControlledByPlayer())
            return SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED;
    }
    else if (HasAttribute(SPELL_ATTR5_NOT_ON_PLAYER))
        return SPELL_FAILED_TARGET_IS_PLAYER;

    if (!IsAllowingDeadTarget() && !unitTarget->IsAlive())
        return SPELL_FAILED_TARGETS_DEAD;

    // check this flag only for implicit targets (chain and area), allow to explicitly target units for spells like Shield of Righteousness
    if (implicit && HasAttribute(SPELL_ATTR6_DO_NOT_CHAIN_TO_CROWD_CONTROLLED_TARGETS) && !unitTarget->CanFreeMove())
        return SPELL_FAILED_BAD_TARGETS;

    // Do not allow pet or guardian targets if the spell is a raid buff or may not target pets at all
    if (HasAttribute(SPELL_ATTR5_NOT_ON_PLAYER_CONTROLLED_NPC) && unitTarget->IsCreature() && unitTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
        return SPELL_FAILED_BAD_TARGETS;

    // checked in Unit::IsValidAttack/AssistTarget, shouldn't be checked for ENTRY targets
    //if (!HasAttribute(SPELL_ATTR6_CAN_TARGET_UNTARGETABLE) && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
    //    return SPELL_FAILED_BAD_TARGETS;

    if (!CheckTargetCreatureType(unitTarget))
    {
        if (target->GetTypeId() == TYPEID_PLAYER)
            return SPELL_FAILED_TARGET_IS_PLAYER;
        else
            return SPELL_FAILED_BAD_TARGETS;
    }

    // check GM mode and GM invisibility - only for player casts (npc casts are controlled by AI) and negative spells
    if (unitTarget != caster && (caster->GetAffectingPlayer() || !IsPositive()) && unitTarget->GetTypeId() == TYPEID_PLAYER)
    {
        if (!unitTarget->ToPlayer()->IsVisible())
            return SPELL_FAILED_BM_OR_INVISGOD;

        if (unitTarget->ToPlayer()->IsGameMaster())
            return SPELL_FAILED_BM_OR_INVISGOD;
    }

    // not allow casting on flying player
    if (unitTarget->HasUnitState(UNIT_STATE_IN_FLIGHT) && !HasAttribute(SPELL_ATTR0_CU_ALLOW_INFLIGHT_TARGET))
        return SPELL_FAILED_BAD_TARGETS;

    /* TARGET_UNIT_MASTER gets blocked here for passengers, because the whole idea of this check is to
    not allow passengers to be implicitly hit by spells, however this target type should be an exception,
    if this is left it kills spells that award kill credit from vehicle to master (few spells),
    the use of these 2 covers passenger target check, logically, if vehicle cast this to master it should always hit
    him, because it would be it's passenger, there's no such case where this gets to fail legitimacy, this problem
    cannot be solved from within the check in other way since target type cannot be called for the spell currently
    Spell examples: [ID - 52864 Devour Water, ID - 52862 Devour Wind, ID - 49370 Wyrmrest Defender: Destabilize Azure Dragonshrine Effect] */
    if (Unit const* unitCaster = caster->ToUnit())
    {
        if (!unitCaster->IsVehicle() && !(unitCaster->GetCharmerOrOwner() == target))
        {
            if (TargetAuraState && !unitTarget->HasAuraState(AuraStateType(TargetAuraState), this, unitCaster))
                return SPELL_FAILED_TARGET_AURASTATE;

            if (TargetAuraStateNot && unitTarget->HasAuraState(AuraStateType(TargetAuraStateNot), this, unitCaster))
                return SPELL_FAILED_TARGET_AURASTATE;
        }
    }

    if (TargetAuraSpell && !unitTarget->HasAura(sSpellMgr->GetSpellIdForDifficulty(TargetAuraSpell, caster)))
        return SPELL_FAILED_TARGET_AURASTATE;

    if (ExcludeTargetAuraSpell && unitTarget->HasAura(sSpellMgr->GetSpellIdForDifficulty(ExcludeTargetAuraSpell, caster)))
        return SPELL_FAILED_TARGET_AURASTATE;

    if (unitTarget->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        if (HasEffect(SPELL_EFFECT_SELF_RESURRECT) || HasEffect(SPELL_EFFECT_RESURRECT) || HasEffect(SPELL_EFFECT_RESURRECT_NEW))
            return SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED;

    if (!unitTarget->CheckPrivateObjectOwnerVisibility(caster))
        return SPELL_FAILED_BAD_TARGETS;

    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckExplicitTarget(WorldObject const* caster, WorldObject const* target, Item const* itemTarget) const
{
    uint32 neededTargets = GetExplicitTargetMask();
    if (!target)
    {
        if (neededTargets & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT_MASK | TARGET_FLAG_CORPSE_MASK))
            if (!(neededTargets & TARGET_FLAG_GAMEOBJECT_ITEM) || !itemTarget)
                return SPELL_FAILED_BAD_TARGETS;
        return SPELL_CAST_OK;
    }

    if (Unit const* unitTarget = target->ToUnit())
    {
        if (neededTargets & (TARGET_FLAG_UNIT_ENEMY | TARGET_FLAG_UNIT_ALLY | TARGET_FLAG_UNIT_RAID | TARGET_FLAG_UNIT_PARTY | TARGET_FLAG_UNIT_MINIPET | TARGET_FLAG_UNIT_PASSENGER))
        {
            Unit const* unitCaster = caster->ToUnit();
            if (neededTargets & TARGET_FLAG_UNIT_ENEMY)
                if (caster->IsValidAttackTarget(unitTarget, this))
                    return SPELL_CAST_OK;
            if ((neededTargets & TARGET_FLAG_UNIT_ALLY)
                || ((neededTargets & TARGET_FLAG_UNIT_PARTY) && unitCaster && unitCaster->IsInPartyWith(unitTarget))
                || ((neededTargets & TARGET_FLAG_UNIT_RAID) && unitCaster && unitCaster->IsInRaidWith(unitTarget)))
                if (caster->IsValidAssistTarget(unitTarget, this))
                    return SPELL_CAST_OK;
            if ((neededTargets & TARGET_FLAG_UNIT_MINIPET) && unitCaster)
                if (unitTarget->GetGUID() == unitCaster->GetCritterGUID())
                    return SPELL_CAST_OK;
            if ((neededTargets & TARGET_FLAG_UNIT_PASSENGER) && unitCaster)
                if (unitTarget->IsOnVehicle(unitCaster))
                    return SPELL_CAST_OK;
            return SPELL_FAILED_BAD_TARGETS;
        }
    }
    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckVehicle(Unit const* caster) const
{
    // All creatures should be able to cast as passengers freely, restriction and attribute are only for players
    if (caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Vehicle* vehicle = caster->GetVehicle();
    if (vehicle)
    {
        uint16 checkMask = 0;
        for (uint8 effIndex = EFFECT_0; effIndex < MAX_SPELL_EFFECTS; ++effIndex)
        {
            if (Effects[effIndex].ApplyAuraName == SPELL_AURA_MOD_SHAPESHIFT)
            {
                SpellShapeshiftFormEntry const* shapeShiftFromEntry = sSpellShapeshiftFormStore.LookupEntry(Effects[effIndex].MiscValue);
                if (shapeShiftFromEntry && (shapeShiftFromEntry->Flags & 1) == 0)  // unk flag
                    checkMask |= VEHICLE_SEAT_FLAG_UNCONTROLLED;
                break;
            }
        }

        if (HasAura(SPELL_AURA_MOUNTED))
            checkMask |= VEHICLE_SEAT_FLAG_CAN_CAST_MOUNT_SPELL;

        if (!checkMask)
            checkMask = VEHICLE_SEAT_FLAG_CAN_ATTACK;

        VehicleSeatEntry const* vehicleSeat = vehicle->GetSeatForPassenger(caster);
        if (!vehicleSeat)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        if (!HasAttribute(SPELL_ATTR6_ALLOW_WHILE_RIDING_VEHICLE) && !HasAttribute(SPELL_ATTR0_ALLOW_WHILE_MOUNTED)
            && (vehicleSeat->Flags & checkMask) != checkMask)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        // Can only summon uncontrolled minions/guardians when on controlled vehicle
        if (vehicleSeat->Flags & (VEHICLE_SEAT_FLAG_CAN_CONTROL | VEHICLE_SEAT_FLAG_UNK2))
        {
            for (uint32 i = EFFECT_0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (Effects[i].Effect != SPELL_EFFECT_SUMMON)
                    continue;

                SummonPropertiesEntry const* props = sSummonPropertiesStore.LookupEntry(Effects[i].MiscValueB);
                if (props && props->Control != SUMMON_CATEGORY_WILD)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            }
        }
    }

    return SPELL_CAST_OK;
}

bool SpellInfo::CheckTargetCreatureType(Unit const* target) const
{
    // Curse of Doom & Exorcism: not find another way to fix spell target check :/
    if (SpellFamilyName == SPELLFAMILY_WARLOCK && GetCategory() == 1179)
    {
        // not allow cast at player
        if (target->GetTypeId() == TYPEID_PLAYER)
            return false;
        else
            return true;
    }

    // if target is magnet (i.e Grounding Totem) the check is skipped
    if (target->IsMagnet())
        return true;

    uint32 creatureType = target->GetCreatureTypeMask();
    return !TargetCreatureType || !creatureType || (creatureType & TargetCreatureType);
}

SpellSchoolMask SpellInfo::GetSchoolMask() const
{
    return SpellSchoolMask(SchoolMask);
}

uint32 SpellInfo::GetAllEffectsMechanicMask() const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && Effects[i].Mechanic)
            mask |= 1 << Effects[i].Mechanic;
    return mask;
}

uint32 SpellInfo::GetEffectMechanicMask(uint8 effIndex) const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    if (Effects[effIndex].IsEffect() && Effects[effIndex].Mechanic)
        mask |= 1 << Effects[effIndex].Mechanic;
    return mask;
}

uint32 SpellInfo::GetSpellMechanicMaskByEffectMask(uint32 effectMask) const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if ((effectMask & (1 << i)) && Effects[i].Mechanic)
            mask |= 1 << Effects[i].Mechanic;
    return mask;
}

Mechanics SpellInfo::GetEffectMechanic(uint8 effIndex) const
{
    if (Effects[effIndex].IsEffect() && Effects[effIndex].Mechanic)
        return Mechanics(Effects[effIndex].Mechanic);
    if (Mechanic)
        return Mechanics(Mechanic);
    return MECHANIC_NONE;
}

bool SpellInfo::HasAnyEffectMechanic() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].Mechanic)
            return true;
    return false;
}

uint32 SpellInfo::GetDispelMask() const
{
    return GetDispelMask(DispelType(Dispel));
}

uint32 SpellInfo::GetDispelMask(DispelType type)
{
    // If dispel all
    if (type == DISPEL_ALL)
        return DISPEL_ALL_MASK;
    else
        return uint32(1 << type);
}

uint32 SpellInfo::GetExplicitTargetMask() const
{
    return ExplicitTargetMask;
}

AuraStateType SpellInfo::GetAuraState() const
{
    return _auraState;
}

void SpellInfo::_LoadAuraState()
{
    _auraState = [this]() -> AuraStateType
    {
        // Seals
        if (GetSpellSpecific() == SPELL_SPECIFIC_SEAL)
            return AURA_STATE_JUDGEMENT;

        // Conflagrate aura state on Immolate and Shadowflame
        if (SpellFamilyName == SPELLFAMILY_WARLOCK &&
            // Immolate
            ((SpellFamilyFlags[0] & 4) ||
                // Shadowflame
            (SpellFamilyFlags[2] & 2)))
            return AURA_STATE_CONFLAGRATE;

        // Faerie Fire (druid versions)
        if (SpellFamilyName == SPELLFAMILY_DRUID && SpellFamilyFlags[0] & 0x400)
            return AURA_STATE_FAERIE_FIRE;

        // Sting (hunter's pet ability)
        if (GetCategory() == 1133)
            return AURA_STATE_FAERIE_FIRE;

        // Victorious
        if (SpellFamilyName == SPELLFAMILY_WARRIOR &&  SpellFamilyFlags[1] & 0x00040000)
            return AURA_STATE_WARRIOR_VICTORY_RUSH;

        // Swiftmend state on Regrowth & Rejuvenation
        if (SpellFamilyName == SPELLFAMILY_DRUID && SpellFamilyFlags[0] & 0x50)
            return AURA_STATE_SWIFTMEND;

        // Deadly poison aura state
        if (SpellFamilyName == SPELLFAMILY_ROGUE && SpellFamilyFlags[0] & 0x10000)
            return AURA_STATE_DEADLY_POISON;

        // Enrage aura state
        if (Dispel == DISPEL_ENRAGE)
            return AURA_STATE_ENRAGE;

        // Bleeding aura state
        if (GetAllEffectsMechanicMask() & 1 << MECHANIC_BLEED)
            return AURA_STATE_BLEEDING;

        if (GetSchoolMask() & SPELL_SCHOOL_MASK_FROST)
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (Effects[i].IsAura() && (Effects[i].ApplyAuraName == SPELL_AURA_MOD_STUN
                    || Effects[i].ApplyAuraName == SPELL_AURA_MOD_ROOT))
                    return AURA_STATE_FROZEN;

        switch (Id)
        {
            case 71465: // Divine Surge
            case 50241: // Evasive Charges
                return AURA_STATE_UNKNOWN22;
            case 9991:  // Touch of Zanzil
            case 35325: // Glowing Blood
            case 35328: // Lambent Blood
            case 35329: // Vibrant Blood
            case 35331: // Black Blood
            case 49163: // Perpetual Instability
                return AURA_STATE_FAERIE_FIRE;
            default:
                break;
        }

        return AURA_STATE_NONE;
    }();
}

SpellSpecificType SpellInfo::GetSpellSpecific() const
{
    return _spellSpecific;
}

void SpellInfo::_LoadSpellSpecific()
{
    _spellSpecific = [this]()->SpellSpecificType
    {
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                // Food / Drinks (mostly)
                if (HasAuraInterruptFlag(SpellAuraInterruptFlags::Standing))
                {
                    bool food = false;
                    bool drink = false;
                    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    {
                        if (!Effects[i].IsAura())
                            continue;
                        switch (Effects[i].ApplyAuraName)
                        {
                            // Food
                            case SPELL_AURA_MOD_REGEN:
                            case SPELL_AURA_OBS_MOD_HEALTH:
                                food = true;
                                break;
                                // Drink
                            case SPELL_AURA_MOD_POWER_REGEN:
                            case SPELL_AURA_OBS_MOD_POWER:
                                drink = true;
                                break;
                            default:
                                break;
                        }
                    }

                    if (food && drink)
                        return SPELL_SPECIFIC_FOOD_AND_DRINK;
                    else if (food)
                        return SPELL_SPECIFIC_FOOD;
                    else if (drink)
                        return SPELL_SPECIFIC_DRINK;
                }
                // scrolls effects
                else
                {
                    SpellInfo const* firstRankSpellInfo = GetFirstRankSpell();
                    switch (firstRankSpellInfo->Id)
                    {
                        case 8118: // Strength
                        case 8099: // Stamina
                        case 8112: // Spirit
                        case 8096: // Intellect
                        case 8115: // Agility
                        case 8091: // Armor
                            return SPELL_SPECIFIC_SCROLL;
                        case 12880: // Enrage (Enrage)
                        case 57518: // Enrage (Wrecking Crew)
                            return SPELL_SPECIFIC_WARRIOR_ENRAGE;
                    }
                }
                break;
            }
            case SPELLFAMILY_MAGE:
            {
                // family flags 18(Molten), 25(Frost/Ice), 28(Mage)
                if (SpellFamilyFlags[0] & 0x12040000)
                    return SPELL_SPECIFIC_MAGE_ARMOR;

                // Arcane brillance and Arcane intelect (normal check fails because of flags difference)
                if (SpellFamilyFlags[0] & 0x400)
                    return SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE;

                if ((SpellFamilyFlags[0] & 0x1000000) && Effects[0].ApplyAuraName == SPELL_AURA_MOD_CONFUSE)
                    return SPELL_SPECIFIC_MAGE_POLYMORPH;

                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                if (Id == 12292) // Death Wish
                    return SPELL_SPECIFIC_WARRIOR_ENRAGE;

                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Warlock (Bane of Doom | Bane of Agony | Bane of Havoc)
                if (Id == 603 || Id == 980 || Id == 80240)
                    return SPELL_SPECIFIC_BANE;

                // only warlock curses have this
                if (Dispel == DISPEL_CURSE)
                    return SPELL_SPECIFIC_CURSE;

                // Warlock (Demon Armor | Demon Skin | Fel Armor)
                if (SpellFamilyFlags[1] & 0x20000020 || SpellFamilyFlags[2] & 0x00000010)
                    return SPELL_SPECIFIC_WARLOCK_ARMOR;

                //seed of corruption and corruption
                if (SpellFamilyFlags[1] & 0x10 || SpellFamilyFlags[0] & 0x2)
                    return SPELL_SPECIFIC_WARLOCK_CORRUPTION;
                break;
            }
            case SPELLFAMILY_PRIEST:
            {
                // Divine Spirit and Prayer of Spirit
                if (SpellFamilyFlags[0] & 0x20)
                    return SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT;

                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // only hunter stings have this
                if (Dispel == DISPEL_POISON)
                    return SPELL_SPECIFIC_STING;

                // only hunter aspects have this (but not all aspects in hunter family)
                if (SpellFamilyFlags.HasFlag(0x00380000, 0x00440000, 0x00001010))
                    return SPELL_SPECIFIC_ASPECT;

                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Collection of all the seal family flags. No other paladin spell has any of those.
                if (SpellFamilyFlags[1] & 0xA2000800)
                    return SPELL_SPECIFIC_SEAL;

                if (SpellFamilyFlags[0] & 0x00002190)
                    return SPELL_SPECIFIC_HAND;

                // only paladin auras have this (for palaldin class family)
                if (SpellFamilyFlags[2] & 0x00000020)
                    return SPELL_SPECIFIC_AURA;

                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                // family flags 10 (Lightning), 42 (Earth), 37 (Water), proc shield from T2 8 pieces bonus
                if (SpellFamilyFlags[1] & 0x420
                    || SpellFamilyFlags[0] & 0x00000400
                    || Id == 23552)
                    return SPELL_SPECIFIC_ELEMENTAL_SHIELD;

                break;
            }
            case SPELLFAMILY_DEATHKNIGHT:
                if (Id == 48266 || Id == 48263 || Id == 48265)
                    return SPELL_SPECIFIC_PRESENCE;
                break;
        }

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA)
            {
                switch (Effects[i].ApplyAuraName)
                {
                    case SPELL_AURA_MOD_CHARM:
                    case SPELL_AURA_MOD_POSSESS:
                    case SPELL_AURA_AOE_CHARM:
                        return SPELL_SPECIFIC_CHARM;
                    case SPELL_AURA_TRACK_CREATURES:
                        /// @workaround For non-stacking tracking spells (We need generic solution)
                        if (Id == 30645) // Gas Cloud Tracking
                            return SPELL_SPECIFIC_NORMAL;
                        [[fallthrough]];
                    case SPELL_AURA_TRACK_RESOURCES:
                    case SPELL_AURA_TRACK_STEALTHED:
                        return SPELL_SPECIFIC_TRACKER;
                }
            }
        }

        return SPELL_SPECIFIC_NORMAL;
    }();
}

void SpellInfo::_LoadSpellDiminishInfo()
{
    auto diminishingGroupCompute = [this](bool triggered) -> DiminishingGroup
    {
        if (IsPositive())
            return DIMINISHING_NONE;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
                return DIMINISHING_TAUNT;
        }

        // Explicit Diminishing Groups
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                // Pet charge effects (Infernal Awakening, Demon Charge)
                if (SpellVisual[0] == 2816 && SpellIconID == 15)
                    return DIMINISHING_CONTROLLED_STUN;
                // Frost Tomb
                else if (Id == 48400)
                    return DIMINISHING_NONE;
                // Gnaw
                else if (Id == 47481)
                    return DIMINISHING_CONTROLLED_STUN;
                // ToC Icehowl Arctic Breath
                else if (SpellVisual[0] == 14153)
                    return DIMINISHING_NONE;
                // Black Plague
                else if (Id == 64155)
                    return DIMINISHING_NONE;
                // Screams of the Dead (King Ymiron)
                else if (Id == 51750)
                    return DIMINISHING_NONE;
                break;
            }
            // Event spells
            case SPELLFAMILY_UNK1:
                return DIMINISHING_NONE;
            case SPELLFAMILY_MAGE:
            {
                // Frostbite
                if (SpellFamilyFlags[1] & 0x80000000)
                    return DIMINISHING_ROOT;
                // Shattered Barrier
                else if (SpellVisual[0] == 12297)
                    return DIMINISHING_ROOT;
                // Deep Freeze
                else if (SpellIconID == 2939 && SpellVisual[0] == 9963)
                    return DIMINISHING_CONTROLLED_STUN;
                // Frost Nova / Freeze (Water Elemental)
                else if (SpellIconID == 193)
                    return DIMINISHING_CONTROLLED_ROOT;
                // Dragon's Breath
                else if (SpellFamilyFlags[0] & 0x800000)
                    return DIMINISHING_DRAGONS_BREATH;
                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                // Hamstring - limit duration to 10s in PvP
                if (SpellFamilyFlags[0] & 0x2)
                    return DIMINISHING_LIMITONLY;
                // Charge Stun (own diminishing)
                else if (SpellFamilyFlags[0] & 0x01000000)
                    return DIMINISHING_CHARGE;
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Curses/etc
                if ((SpellFamilyFlags[0] & 0x80000000) || (SpellFamilyFlags[1] & 0x200))
                    return DIMINISHING_LIMITONLY;
                // Seduction
                else if (SpellFamilyFlags[1] & 0x10000000)
                    return DIMINISHING_FEAR;
                // Sin and Punishment (Priest spell, don't ask)
                else if (SpellIconID == 1869)
                    return DIMINISHING_NONE;
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                // Pounce - there are two spells which share the same family: the stun effect and the bleed spell. We don't want the bleed to be affected by DR
                if ((SpellFamilyFlags[0] & 0x20000) && Id != 9007)
                    return DIMINISHING_OPENING_STUN;
                // Cyclone
                else if (SpellFamilyFlags[1] & 0x20)
                    return DIMINISHING_CYCLONE;
                // Entangling Roots
                // Nature's Grasp
                else if (SpellFamilyFlags[0] & 0x00000200)
                    return DIMINISHING_CONTROLLED_ROOT;
                // Faerie Fire
                else if (SpellFamilyFlags[0] & 0x400)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                // Gouge
                if (SpellFamilyFlags[0] & 0x8)
                    return DIMINISHING_DISORIENT;
                // Blind
                else if (SpellFamilyFlags[0] & 0x1000000)
                    return DIMINISHING_FEAR;
                // Cheap Shot
                else if (SpellFamilyFlags[0] & 0x400)
                    return DIMINISHING_OPENING_STUN;
                // Crippling poison - Limit to 10 seconds in PvP (No SpellFamilyFlags)
                else if (SpellIconID == 163)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Hunter's Mark
                if ((SpellFamilyFlags[0] & 0x400) && SpellIconID == 538)
                    return DIMINISHING_LIMITONLY;
                // Scatter Shot (own diminishing)
                else if ((SpellFamilyFlags[0] & 0x40000) && SpellIconID == 132)
                    return DIMINISHING_SCATTER_SHOT;
                // Entrapment (own diminishing)
                else if (SpellVisual[0] == 7484 && SpellIconID == 20)
                    return DIMINISHING_ENTRAPMENT;
                // Wyvern Sting mechanic is MECHANIC_SLEEP but the diminishing is DIMINISHING_DISORIENT
                else if ((SpellFamilyFlags[1] & 0x1000) && SpellIconID == 1721)
                    return DIMINISHING_DISORIENT;
                // Freezing Arrow
                else if (SpellFamilyFlags[0] & 0x8)
                    return DIMINISHING_DISORIENT;
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Judgement of Justice - limit duration to 10s in PvP
                if (SpellFamilyFlags[0] & 0x100000)
                    return DIMINISHING_LIMITONLY;
                // Turn Evil
                else if ((SpellFamilyFlags[1] & 0x804000) && SpellIconID == 309)
                    return DIMINISHING_FEAR;
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                // Storm, Earth and Fire - Earthgrab
                if (SpellFamilyFlags[2] & 0x4000)
                    return DIMINISHING_NONE;
                break;
            }
            case SPELLFAMILY_DEATHKNIGHT:
            {
                // Hungering Cold (no flags)
                if (SpellIconID == 2797)
                    return DIMINISHING_DISORIENT;
                // Mark of Blood
                else if ((SpellFamilyFlags[0] & 0x10000000) && SpellIconID == 2285)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            default:
                break;
        }

        // Lastly - Set diminishing depending on mechanic
        uint32 mechanic = GetAllEffectsMechanicMask();
        if (mechanic & (1 << MECHANIC_CHARM))
            return DIMINISHING_MIND_CONTROL;
        if (mechanic & (1 << MECHANIC_SILENCE))
            return DIMINISHING_SILENCE;
        if (mechanic & (1 << MECHANIC_SLEEP))
            return DIMINISHING_SLEEP;
        if (mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
            return DIMINISHING_DISORIENT;
        // Mechanic Knockout, except Blast Wave
        if (mechanic & (1 << MECHANIC_KNOCKOUT) && SpellIconID != 292)
            return DIMINISHING_DISORIENT;
        if (mechanic & (1 << MECHANIC_DISARM))
            return DIMINISHING_DISARM;
        if (mechanic & (1 << MECHANIC_FEAR))
            return DIMINISHING_FEAR;
        if (mechanic & (1 << MECHANIC_STUN))
            return triggered ? DIMINISHING_STUN : DIMINISHING_CONTROLLED_STUN;
        if (mechanic & (1 << MECHANIC_BANISH))
            return DIMINISHING_BANISH;
        if (mechanic & (1 << MECHANIC_ROOT))
            return triggered ? DIMINISHING_ROOT : DIMINISHING_CONTROLLED_ROOT;
        if (mechanic & (1 << MECHANIC_HORROR))
            return DIMINISHING_HORROR;

        return DIMINISHING_NONE;
    };

    auto diminishingTypeCompute = [](DiminishingGroup group) -> DiminishingReturnsType
    {
        switch (group)
        {
            case DIMINISHING_TAUNT:
            case DIMINISHING_CONTROLLED_STUN:
            case DIMINISHING_STUN:
            case DIMINISHING_OPENING_STUN:
            case DIMINISHING_CYCLONE:
            case DIMINISHING_CHARGE:
                return DRTYPE_ALL;
            case DIMINISHING_LIMITONLY:
            case DIMINISHING_NONE:
                return DRTYPE_NONE;
            default:
                return DRTYPE_PLAYER;
        }
    };

    auto diminishingMaxLevelCompute = [](DiminishingGroup group) -> DiminishingLevels
    {
        switch (group)
        {
            case DIMINISHING_TAUNT:
                return DIMINISHING_LEVEL_TAUNT_IMMUNE;
            default:
                return DIMINISHING_LEVEL_IMMUNE;
        }
    };

    auto diminishingLimitDurationCompute = [this](DiminishingGroup group) -> int32
    {
        auto isGroupDurationLimited = [group]() -> bool
        {
            switch (group)
            {
                case DIMINISHING_BANISH:
                case DIMINISHING_CONTROLLED_STUN:
                case DIMINISHING_CONTROLLED_ROOT:
                case DIMINISHING_CYCLONE:
                case DIMINISHING_DISORIENT:
                case DIMINISHING_ENTRAPMENT:
                case DIMINISHING_FEAR:
                case DIMINISHING_HORROR:
                case DIMINISHING_MIND_CONTROL:
                case DIMINISHING_OPENING_STUN:
                case DIMINISHING_ROOT:
                case DIMINISHING_STUN:
                case DIMINISHING_SLEEP:
                case DIMINISHING_LIMITONLY:
                    return true;
                default:
                    return false;
            }
        };

        if (!isGroupDurationLimited())
            return 0;

        // Explicit diminishing duration
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_DRUID:
            {
                // Faerie Fire - limit to 40 seconds in PvP (3.1)
                if (SpellFamilyFlags[0] & 0x400)
                    return 40 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Wyvern Sting
                if (SpellFamilyFlags[1] & 0x1000)
                    return 6 * IN_MILLISECONDS;
                // Hunter's Mark
                if (SpellFamilyFlags[0] & 0x400)
                    return 30 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Repentance - limit to 6 seconds in PvP
                if (SpellFamilyFlags[0] & 0x4)
                    return 6 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Banish - limit to 6 seconds in PvP
                if (SpellFamilyFlags[1] & 0x8000000)
                    return 6 * IN_MILLISECONDS;
                // Curse of Tongues - limit to 12 seconds in PvP
                else if (SpellFamilyFlags[2] & 0x800)
                    return 12 * IN_MILLISECONDS;
                // Curse of Elements - limit to 120 seconds in PvP
                else if (SpellFamilyFlags[1] & 0x200)
                    return 120 * IN_MILLISECONDS;
                break;
            }
            default:
                break;
        }

        return 8 * IN_MILLISECONDS;
    };

    SpellDiminishInfo triggeredInfo, normalInfo;
    triggeredInfo.DiminishGroup = diminishingGroupCompute(true);
    triggeredInfo.DiminishReturnType = diminishingTypeCompute(triggeredInfo.DiminishGroup);
    triggeredInfo.DiminishMaxLevel = diminishingMaxLevelCompute(triggeredInfo.DiminishGroup);
    triggeredInfo.DiminishDurationLimit = diminishingLimitDurationCompute(triggeredInfo.DiminishGroup);

    normalInfo.DiminishGroup = diminishingGroupCompute(false);
    normalInfo.DiminishReturnType = diminishingTypeCompute(normalInfo.DiminishGroup);
    normalInfo.DiminishMaxLevel = diminishingMaxLevelCompute(normalInfo.DiminishGroup);
    normalInfo.DiminishDurationLimit = diminishingLimitDurationCompute(normalInfo.DiminishGroup);

    _diminishInfoTriggered = triggeredInfo;
    _diminishInfoNonTriggered = normalInfo;
}

DiminishingGroup SpellInfo::GetDiminishingReturnsGroupForSpell(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishGroup : _diminishInfoNonTriggered.DiminishGroup;
}

DiminishingReturnsType SpellInfo::GetDiminishingReturnsGroupType(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishReturnType : _diminishInfoNonTriggered.DiminishReturnType;
}

DiminishingLevels SpellInfo::GetDiminishingReturnsMaxLevel(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishMaxLevel : _diminishInfoNonTriggered.DiminishMaxLevel;
}

int32 SpellInfo::GetDiminishingReturnsLimitDuration(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishDurationLimit : _diminishInfoNonTriggered.DiminishDurationLimit;
}

void SpellInfo::_LoadImmunityInfo()
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        uint32 schoolImmunityMask = 0;
        uint32 applyHarmfulAuraImmunityMask = 0;
        uint32 mechanicImmunityMask = 0;
        uint32 dispelImmunity = 0;
        uint32 damageImmunityMask = 0;

        if (!Effects[i].IsEffect())
            continue;

        int32 miscVal = Effects[i].MiscValue;
        int32 amount = Effects[i].CalcValue();

        ImmunityInfo& immuneInfo = _immunityInfo[i];

        switch (Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_MECHANIC_IMMUNITY_MASK:
            {
                switch (miscVal)
                {
                    case 96:   // Free Friend, Uncontrollable Frenzy, Warlord's Presence
                    {
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        break;
                    }
                    case 1615: // Incite Rage, Wolf Spirit, Overload, Lightning Tendrils
                    {
                        switch (Id)
                        {
                            case 43292: // Incite Rage
                            case 49172: // Wolf Spirit
                                mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                                [[fallthrough]];
                            case 61869: // Overload
                            case 63481:
                            case 61887: // Lightning Tendrils
                            case 63486:
                                mechanicImmunityMask |= (1 << MECHANIC_INTERRUPT) | (1 << MECHANIC_SILENCE);

                                immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK);
                                immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK_DEST);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    case 679:  // Mind Control, Avenging Fury
                    {
                        if (Id == 57742) // Avenging Fury
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 1557: // Startling Roar, Warlord Roar, Break Bonds, Stormshield
                    {
                        if (Id == 64187) // Stormshield
                        {
                            mechanicImmunityMask |= (1 << MECHANIC_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                        }
                        else
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 1614: // Fixate
                    case 1694: // Fixated, Lightning Tendrils
                    {
                        immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_ATTACK_ME);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_TAUNT);
                        break;
                    }
                    case 1630: // Fervor, Berserk
                    {
                        if (Id == 64112) // Berserk
                        {
                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_ATTACK_ME);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_TAUNT);
                        }
                        else
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 477:  // Bladestorm
                    case 1733: // Bladestorm, Killing Spree
                    {
                        if (!amount)
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK);
                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK_DEST);

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 878: // Whirlwind, Fog of Corruption, Determination
                    {
                        if (Id == 66092) // Determination
                        {
                            mechanicImmunityMask |= (1 << MECHANIC_SNARE) | (1 << MECHANIC_STUN)
                                | (1 << MECHANIC_DISORIENTED) | (1 << MECHANIC_FREEZE);

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (immuneInfo.AuraTypeImmune.empty())
                {
                    if (miscVal & (1 << 10))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                    if (miscVal & (1 << 1))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_TRANSFORM);

                    // These flag can be recognized wrong:
                    if (miscVal & (1 << 6))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                    if (miscVal & (1 << 0))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                    if (miscVal & (1 << 2))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                    if (miscVal & (1 << 9))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                    if (miscVal & (1 << 7))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DISARM);
                }
                break;
            }
            case SPELL_AURA_MECHANIC_IMMUNITY:
            {
                switch (Id)
                {
                    case 42292: // PvP trinket
                    case 59752: // Every Man for Himself
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED);
                        break;
                    case 34471: // The Beast Within
                    case 19574: // Bestial Wrath
                    case 53490: // Bullheaded
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
                        break;
                    case 54508: // Demonic Empowerment
                        mechanicImmunityMask |= (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT) | (1 << MECHANIC_STUN);
                        break;
                    default:
                        if (miscVal < 1)
                            continue;

                        mechanicImmunityMask |= 1 << miscVal;
                        break;
                }
                break;
            }
            case SPELL_AURA_EFFECT_IMMUNITY:
            {
                immuneInfo.SpellEffectImmune.insert(static_cast<SpellEffects>(miscVal));
                break;
            }
            case SPELL_AURA_STATE_IMMUNITY:
            {
                immuneInfo.AuraTypeImmune.insert(static_cast<AuraType>(miscVal));
                break;
            }
            case SPELL_AURA_SCHOOL_IMMUNITY:
            {
                schoolImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL:
            {
                applyHarmfulAuraImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_DAMAGE_IMMUNITY:
            {
                damageImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_DISPEL_IMMUNITY:
            {
                dispelImmunity = uint32(miscVal);
                break;
            }
            default:
                break;
        }

        immuneInfo.SchoolImmuneMask = schoolImmunityMask;
        immuneInfo.ApplyHarmfulAuraImmuneMask = applyHarmfulAuraImmunityMask;
        immuneInfo.MechanicImmuneMask = mechanicImmunityMask;
        immuneInfo.DispelImmune = dispelImmunity;
        immuneInfo.DamageSchoolMask = damageImmunityMask;

        immuneInfo.AuraTypeImmune.shrink_to_fit();
        immuneInfo.SpellEffectImmune.shrink_to_fit();

        _allowedMechanicMask |= immuneInfo.MechanicImmuneMask;
    }

    if (HasAttribute(SPELL_ATTR5_ALLOW_WHILE_STUNNED))
    {
        switch (Id)
        {
            case 22812: // Barkskin
            case 47585: // Dispersion
                _allowedMechanicMask |=
                    (1 << MECHANIC_STUN) |
                    (1 << MECHANIC_FREEZE) |
                    (1 << MECHANIC_KNOCKOUT) |
                    (1 << MECHANIC_SLEEP);
                break;
            case 49039: // Lichborne, don't allow normal stuns
                break;
            default:
                _allowedMechanicMask |= (1 << MECHANIC_STUN);
                break;
        }
    }

    if (HasAttribute(SPELL_ATTR5_ALLOW_WHILE_CONFUSED))
        _allowedMechanicMask |= (1 << MECHANIC_DISORIENTED);

    if (HasAttribute(SPELL_ATTR5_ALLOW_WHILE_FLEEING))
    {
        switch (Id)
        {
            case 22812: // Barkskin
            case 47585: // Dispersion
                _allowedMechanicMask |= (1 << MECHANIC_FEAR) | (1 << MECHANIC_HORROR);
                break;
            default:
                _allowedMechanicMask |= (1 << MECHANIC_FEAR);
                break;
        }
    }
}

void SpellInfo::ApplyAllSpellImmunitiesTo(Unit* target, uint8 effIndex, bool apply) const
{
    ImmunityInfo const& immuneInfo = _immunityInfo[effIndex];

    if (uint32 schoolImmunity = immuneInfo.SchoolImmuneMask)
    {
        target->ApplySpellImmune(Id, IMMUNITY_SCHOOL, schoolImmunity, apply);

        if (apply && HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT))
        {
            target->RemoveAppliedAuras([this, schoolImmunity](AuraApplication const* aurApp) -> bool
            {
                SpellInfo const* auraSpellInfo = aurApp->GetBase()->GetSpellInfo();
                return ((auraSpellInfo->GetSchoolMask() & schoolImmunity) != 0 && // Check for school mask
                    CanDispelAura(auraSpellInfo) &&
                    (IsPositive() != aurApp->IsPositive()) &&                     // Check spell vs aura possitivity
                    !auraSpellInfo->IsPassive() &&                                // Don't remove passive auras
                    auraSpellInfo->Id != Id);                                     // Don't remove self
            });
        }
        if (apply && schoolImmunity & SPELL_SCHOOL_MASK_NORMAL)
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::InvulnerabilityBuff);
    }

    if (uint32 mechanicImmunity = immuneInfo.MechanicImmuneMask)
    {
        for (uint32 i = MECHANIC_CHARM; i < MAX_MECHANIC; ++i)
            if (mechanicImmunity & (1 << i))
                target->ApplySpellImmune(Id, IMMUNITY_MECHANIC, i, apply);

        if (HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT))
        {
            if (apply)
                target->RemoveAurasWithMechanic(mechanicImmunity, AuraRemoveFlags::ByDefault, Id);
            else
            {
                std::vector<Aura*> aurasToUpdateTargets;
                target->RemoveAppliedAuras([mechanicImmunity, &aurasToUpdateTargets](AuraApplication const* aurApp)
                {
                    Aura* aura = aurApp->GetBase();
                    if (aura->GetSpellInfo()->GetAllEffectsMechanicMask() & mechanicImmunity)
                        aurasToUpdateTargets.push_back(aura);

                    // only update targets, don't remove anything
                    return false;
                });

                for (Aura* aura : aurasToUpdateTargets)
                    aura->UpdateTargetMap(aura->GetCaster());
            }
        }
    }

    if (uint32 dispelImmunity = immuneInfo.DispelImmune)
    {
        target->ApplySpellImmune(Id, IMMUNITY_DISPEL, dispelImmunity, apply);

        if (apply && HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT))
        {
            target->RemoveAppliedAuras([dispelImmunity](AuraApplication const* aurApp) -> bool
            {
                SpellInfo const* spellInfo = aurApp->GetBase()->GetSpellInfo();
                if (spellInfo->Dispel == dispelImmunity)
                    return true;

                return false;
            });
        }
    }

    if (uint32 damageImmunity = immuneInfo.DamageSchoolMask)
    {
        target->ApplySpellImmune(Id, IMMUNITY_DAMAGE, damageImmunity, apply);

        if (apply && damageImmunity & SPELL_SCHOOL_MASK_NORMAL)
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::InvulnerabilityBuff);
    }

    for (AuraType auraType : immuneInfo.AuraTypeImmune)
    {
        target->ApplySpellImmune(Id, IMMUNITY_STATE, auraType, apply);
        if (apply && HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT))
            target->RemoveAurasByType(auraType, [](AuraApplication const* aurApp) -> bool
            {
                // if the aura has SPELL_ATTR0_NO_IMMUNITIES, then it cannot be removed by immunity
                return !aurApp->GetBase()->GetSpellInfo()->HasAttribute(SPELL_ATTR0_NO_IMMUNITIES);
            });
    }

    for (SpellEffects effectType : immuneInfo.SpellEffectImmune)
        target->ApplySpellImmune(Id, IMMUNITY_EFFECT, effectType, apply);
}

bool SpellInfo::CanSpellProvideImmunityAgainstAura(SpellInfo const* auraSpellInfo) const
{
    if (!auraSpellInfo)
        return false;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        ImmunityInfo const* immuneInfo = _immunityInfo + i;

        if (!auraSpellInfo->HasAttribute(SPELL_ATTR1_IMMUNITY_TO_HOSTILE_AND_FRIENDLY_EFFECTS) && !auraSpellInfo->HasAttribute(SPELL_ATTR2_NO_SCHOOL_IMMUNITIES))
        {
            if (uint32 schoolImmunity = immuneInfo->SchoolImmuneMask)
                if ((auraSpellInfo->SchoolMask & schoolImmunity) != 0)
                    return true;
        }

        if (uint32 mechanicImmunity = immuneInfo->MechanicImmuneMask)
            if ((mechanicImmunity & (1 << auraSpellInfo->Mechanic)) != 0)
                return true;

        if (uint32 dispelImmunity = immuneInfo->DispelImmune)
            if (auraSpellInfo->Dispel == dispelImmunity)
                return true;

        bool immuneToAllEffects = true;
        for (uint8 effIndex = 0; effIndex < MAX_SPELL_EFFECTS; ++effIndex)
        {
            uint32 effectName = auraSpellInfo->Effects[effIndex].Effect;
            if (!effectName)
                continue;

            auto spellImmuneItr = immuneInfo->SpellEffectImmune.find(static_cast<SpellEffects>(effectName));
            if (spellImmuneItr == immuneInfo->SpellEffectImmune.cend())
            {
                immuneToAllEffects = false;
                break;
            }

            if (uint32 mechanic = auraSpellInfo->Effects[effIndex].Mechanic)
            {
                if (!(immuneInfo->MechanicImmuneMask & (1 << mechanic)))
                {
                    immuneToAllEffects = false;
                    break;
                }
            }

            if (!auraSpellInfo->HasAttribute(SPELL_ATTR3_ALWAYS_HIT))
            {
                if (uint32 auraName = auraSpellInfo->Effects[effIndex].ApplyAuraName)
                {
                    bool isImmuneToAuraEffectApply = false;
                    auto auraImmuneItr = immuneInfo->AuraTypeImmune.find(static_cast<AuraType>(auraName));
                    if (auraImmuneItr != immuneInfo->AuraTypeImmune.cend())
                        isImmuneToAuraEffectApply = true;

                    if (!isImmuneToAuraEffectApply && !auraSpellInfo->IsPositiveEffect(effIndex) && !auraSpellInfo->HasAttribute(SPELL_ATTR2_NO_SCHOOL_IMMUNITIES))
                    {
                        if (uint32 applyHarmfulAuraImmunityMask = immuneInfo->ApplyHarmfulAuraImmuneMask)
                            if ((auraSpellInfo->GetSchoolMask() & applyHarmfulAuraImmunityMask) != 0)
                                isImmuneToAuraEffectApply = true;
                    }

                    if (!isImmuneToAuraEffectApply)
                    {
                        immuneToAllEffects = false;
                        break;
                    }
                }
            }
        }

        if (immuneToAllEffects)
            return true;
    }

    return false;
}

// based on client Spell_C::CancelsAuraEffect
bool SpellInfo::SpellCancelsAuraEffect(SpellInfo const* auraSpellInfo, uint8 auraEffIndex) const
{
    if (!HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT))
        return false;

    if (auraSpellInfo->HasAttribute(SPELL_ATTR0_NO_IMMUNITIES))
        return false;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect != SPELL_EFFECT_APPLY_AURA && Effects[i].Effect != SPELL_EFFECT_APPLY_AURA_2)
            continue;

        uint32 const miscValue = static_cast<uint32>(Effects[i].MiscValue);
        switch (Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_STATE_IMMUNITY:
                if (miscValue != auraSpellInfo->Effects[auraEffIndex].ApplyAuraName)
                    continue;
                break;
            case SPELL_AURA_SCHOOL_IMMUNITY:
            case SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL:
                if (auraSpellInfo->HasAttribute(SPELL_ATTR2_NO_SCHOOL_IMMUNITIES) || !(auraSpellInfo->SchoolMask & miscValue))
                    continue;
                break;
            case SPELL_AURA_DISPEL_IMMUNITY:
                if (miscValue != auraSpellInfo->Dispel)
                    continue;
                break;
            case SPELL_AURA_MECHANIC_IMMUNITY:
                if (miscValue != auraSpellInfo->Mechanic)
                {
                    if (miscValue != auraSpellInfo->Effects[auraEffIndex].Mechanic)
                        continue;
                }
                break;
            default:
                continue;
        }

        return true;
    }

    return false;
}

uint32 SpellInfo::GetAllowedMechanicMask() const
{
    return _allowedMechanicMask;
}

uint32 SpellInfo::GetMechanicImmunityMask(Unit* caster) const
{
    uint32 casterMechanicImmunityMask = caster->GetMechanicImmunityMask();
    uint32 mechanicImmunityMask = 0;

    if (CanBeInterrupted(caster, true))
    {
        if (casterMechanicImmunityMask & (1 << MECHANIC_INTERRUPT))
            mechanicImmunityMask |= (1 << MECHANIC_INTERRUPT);

        if (casterMechanicImmunityMask & (1 << MECHANIC_SILENCE))
            mechanicImmunityMask |= (1 << MECHANIC_SILENCE);
    }

    return mechanicImmunityMask;
}

float SpellInfo::CalculateScaledCoefficient(Unit const* caster, float coefficient) const
{
    if (coefficient == 0.f || !caster || SpellScalingId == 0)
        return coefficient;

    return coefficient *= GetSpellScalingMultiplier(caster);
}

float SpellInfo::GetMinRange(bool positive) const
{
    if (!RangeEntry)
        return 0.0f;
    if (positive)
        return RangeEntry->RangeMin[1];
    return RangeEntry->RangeMin[0];
}

float SpellInfo::GetMaxRange(bool positive, WorldObject* caster, Spell* spell) const
{
    if (!RangeEntry)
        return 0.0f;
    float range;
    if (positive)
        range = RangeEntry->RangeMax[1];
    else
        range = RangeEntry->RangeMax[0];
    if (caster)
        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(Id, SpellModOp::Range, range, spell);
    return range;
}

int32 SpellInfo::GetDuration() const
{
    if (!DurationEntry)
        return IsPassive() ? -1 : 0;
    return (DurationEntry->Duration == -1) ? -1 : abs(DurationEntry->Duration);
}

int32 SpellInfo::GetMaxDuration() const
{
    if (!DurationEntry)
        return IsPassive() ? -1 : 0;
    return (DurationEntry->MaxDuration == -1) ? -1 : abs(DurationEntry->MaxDuration);
}

float SpellInfo::CalcPeriodicHasteMod(Unit const* caster) const
{
    float hasteMod = 1.f;
    if (HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC))
        hasteMod = caster->GetFloatValue(UNIT_MOD_CAST_HASTE);
    else if (HasAttribute(SPELL_ATTR8_MELEE_HASTE_AFFECTS_PERIODIC) && caster->IsPlayer())
        hasteMod = caster->GetFloatValue(PLAYER_FIELD_MOD_HASTE);

    return hasteMod;
}

int32 SpellInfo::CalcDuration(WorldObject const* caster /*= nullptr*/) const
{
    if (!DurationEntry)
        return IsPassive() ? -1 : 0;

    int32 duration = GetMaxDuration();
    if (duration == -1)
        return -1;

    Unit const* unitCaster = caster ? caster->ToUnit() : nullptr;
    if (!unitCaster)
        return std::min(GetMaxDuration(), DurationEntry->Duration);

    uint32 level = unitCaster->getLevel();
    if (MaxLevel > 0 && level > MaxLevel)
        level = MaxLevel;
    if (BaseLevel > 0)
        level -= BaseLevel;

    duration = DurationEntry->Duration + DurationEntry->DurationPerLevel * level;
    duration = std::min(GetMaxDuration(), duration);

    if (duration == -1)
        return -1;

    // Increase duration based on combo points
    if (HasAttribute(SPELL_ATTR1_FINISHING_MOVE_DURATION))
    {
        if (uint8 comboPoints = unitCaster->IsMovedByClient() ? unitCaster->GetGameClientMovingMe()->GetBasePlayer()->GetComboPoints() : 0)
        {
            if (GetDuration() != GetMaxDuration() && GetDuration() != -1)
                duration += int32((GetMaxDuration() - GetDuration()) * comboPoints / 5);
        }
    }

    if (Player const* modOwner = unitCaster->GetSpellModOwner())
        modOwner->ApplySpellMod(Id, SpellModOp::Duration, duration);

    if (HasAttribute(SPELL_ATTR8_HASTE_AFFECTS_DURATION) || HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC))
    {
        SpellEffIndex periodicEffectIndex = EFFECT_0;
        bool hasPeriodicEffect = [&]()
        {
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (Effects[i].IsEffect() && Effects[i].AuraPeriod)
                {
                    periodicEffectIndex = SpellEffIndex(i);
                    return true;
                }

            return false;
        }();

        if (hasPeriodicEffect && !HasAttribute(SPELL_ATTR3_IGNORE_CASTER_MODIFIERS) && (HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC) || HasAttribute(SPELL_ATTR8_MELEE_HASTE_AFFECTS_PERIODIC)))
        {
            float hasteMod = CalcPeriodicHasteMod(unitCaster);
            if (hasteMod > 0.001f)
            {
                if (HasAttribute(SPELL_ATTR8_HASTE_AFFECTS_DURATION))
                    return int32(duration * hasteMod);

                int32 period = CalcPeriod(caster, periodicEffectIndex);
                if (period > 0)
                    duration = int32(period * (duration / (float)period));
            }
        }
    }

    return duration;
}

int32 SpellInfo::CalcPeriod(WorldObject const* caster, SpellEffIndex effIndex, Optional<int32> periodOverride /*= {}*/) const
{
    int32 period = 0;
    if (periodOverride.has_value() && *periodOverride > 0)
        period = *periodOverride;
    else
    {
        if (Effects[effIndex].IsEffect() && Effects[effIndex].AuraPeriod != 0)
            period = Effects[effIndex].AuraPeriod;
        else
            period = 5200;
    }

    Unit const* unitCaster = caster ? caster->ToUnit() : nullptr;
    if (!unitCaster)
        return period;

    if (Player const* modOwner = unitCaster->GetSpellModOwner())
        modOwner->ApplySpellMod(Id, SpellModOp::Period, period);

    if (!HasAttribute(SPELL_ATTR3_IGNORE_CASTER_MODIFIERS) && (HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC) || HasAttribute(SPELL_ATTR8_MELEE_HASTE_AFFECTS_PERIODIC)))
    {
        float hasteMod = CalcPeriodicHasteMod(unitCaster);
        if (hasteMod > 0.001f)
            period = ((float)period * hasteMod);
    }

    return period;
}

uint32 SpellInfo::CalcCastTime(uint8 level, Spell* spell /*= nullptr*/) const
{
    int32 castTime = 0;
    if (!level && spell && spell->GetCaster()->IsUnit())
        level = spell->GetCaster()->ToUnit()->getLevel();

    // not all spells have cast time index and this is all is passive abilities
    if (level && Scaling.CastTimeMax > 0)
    {
        castTime = Scaling.CastTimeMax;
        if (Scaling.CastTimeMaxLevel > level)
            castTime = Scaling.CastTimeMin + int32(level - 1) * (Scaling.CastTimeMax - Scaling.CastTimeMin) / (Scaling.CastTimeMaxLevel - 1);
    }
    else if (CastTimeEntry)
        castTime = CastTimeEntry->Base;

    if (!castTime)
        return 0;

    if (spell)
        spell->GetCaster()->ModSpellCastTime(this, castTime, spell);

    return (castTime > 0) ? uint32(castTime) : 0;
}

uint32 SpellInfo::GetMaxTicks() const
{
    uint32 totalTicks = 0;
    int32 DotDuration = GetDuration();

    for (uint8 x = 0; x < MAX_SPELL_EFFECTS; x++)
    {
        if (Effects[x].Effect == SPELL_EFFECT_APPLY_AURA || Effects[x].Effect == SPELL_EFFECT_APPLY_AURA_2)
        {
            switch (Effects[x].ApplyAuraName)
            {
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_OBS_MOD_HEALTH:
                case SPELL_AURA_OBS_MOD_POWER:
                case SPELL_AURA_48:
                case SPELL_AURA_POWER_BURN:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_PERIODIC_ENERGIZE:
                case SPELL_AURA_PERIODIC_DUMMY:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                    // skip infinite periodics
                    if (Effects[x].AuraPeriod > 0 && DotDuration > 0)
                    {
                        totalTicks = static_cast<uint32>(DotDuration) / Effects[x].AuraPeriod;
                        if (HasAttribute(SPELL_ATTR5_EXTRA_INITIAL_PERIOD))
                            ++totalTicks;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return totalTicks;
}

uint32 SpellInfo::GetRecoveryTime() const
{
    return RecoveryTime > CategoryRecoveryTime ? RecoveryTime : CategoryRecoveryTime;
}

int32 SpellInfo::CalcPowerCost(WorldObject const* caster, SpellSchoolMask schoolMask, Spell* spell) const
{
    // gameobject casts don't use power
    Unit const* unitCaster = caster->ToUnit();
    if (!unitCaster)
        return 0;

    // Spell drain all exist power on cast (Only paladin lay of Hands)
    if (HasAttribute(SPELL_ATTR1_USE_ALL_MANA))
    {
        // If power type - health drain all
        if (PowerType == POWER_HEALTH)
            return unitCaster->GetHealth();
        // Else drain all power
        if (PowerType < MAX_POWERS)
            return unitCaster->GetPower(Powers(PowerType));

        TC_LOG_ERROR("spells", "SpellInfo::CalcPowerCost: Unknown power type '%d' in spell %d", PowerType, Id);
        return 0;
    }

    bool invertSign = false;
    int32 powerCost = 0;

    if (SpellPowerId)
    {
        int32 manaCost = ManaCost;
        powerCost = manaCost;
        float powerCostPct = ManaCostPercentage2 != 0.f ? ManaCostPercentage2 : (float)ManaCostPercentage;

        if (manaCost < 0 || powerCostPct < 0.f)
        {
            manaCost = std::abs(manaCost);
            powerCostPct = std::fabs(powerCostPct);
            invertSign = true;
            powerCost = manaCost;
        }

        // Base calculation
        if (ManaCostPercentage)
        {
            int32 ressource = 0;
            switch (PowerType)
            {
                // health as power used
                case POWER_HEALTH:
                    ressource = unitCaster->GetCreateHealth();
                    break;
                case POWER_MANA:
                    ressource = unitCaster->GetCreateMana();
                    break;
                case POWER_RAGE:
                case POWER_FOCUS:
                case POWER_ENERGY:
                    ressource = unitCaster->GetMaxPower(Powers(PowerType));
                    break;
                case POWER_RUNE:
                case POWER_RUNIC_POWER:
                    TC_LOG_DEBUG("spells", "CalculateManaCost: Not implemented yet!");
                    break;
                default:
                    TC_LOG_ERROR("spells", "CalculateManaCost: Unknown power type '%d' in spell %d", PowerType, Id);
                    return 0;
            }

            manaCost += int32((ressource * powerCostPct) / 100);
            powerCost = manaCost;
        }

        // Scaling
        if (SpellScalingId)
            powerCost = int32(ceilf(GetSpellScalingMultiplier(unitCaster, true) * (double)powerCost));
        else
        {
            uint32 manaCostPerLevel = invertSign ? -int32(ManaCostPerlevel) : ManaCostPerlevel;
            powerCost = manaCostPerLevel * SpellLevel + manaCost;
        }
    }

    // Flat mod from caster auras by spell school and power type
    Unit::AuraEffectList const& auras = unitCaster->GetAuraEffectsByType(SPELL_AURA_MOD_POWER_COST_SCHOOL);
    for (Unit::AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
    {
        if (!((*i)->GetMiscValue() & schoolMask))
            continue;
        if (!((*i)->GetMiscValueB() & (1 << PowerType)))
            continue;
        powerCost += (*i)->GetAmount();
    }

    // Shiv - costs 20 + weaponSpeed*10 energy (apply only to non-triggered spell with energy cost)
    if (HasAttribute(SPELL_ATTR4_WEAPON_SPEED_COST_SCALING))
    {
        uint32 speed = 0;
        if (SpellShapeshiftFormEntry const* ss = sSpellShapeshiftFormStore.LookupEntry(unitCaster->GetShapeshiftForm()))
            speed = ss->CombatRoundTime;
        else
        {
            WeaponAttackType slot = BASE_ATTACK;
            if (!HasAttribute(SPELL_ATTR3_REQUIRES_MAIN_HAND_WEAPON) && HasAttribute(SPELL_ATTR3_REQUIRES_OFF_HAND_WEAPON))
                slot = OFF_ATTACK;

            speed = unitCaster->GetAttackTime(slot);
        }

        powerCost += speed / 100;
    }

    // Apply cost mod by spell
    if (Player* modOwner = caster->GetSpellModOwner())
        modOwner->ApplySpellMod(Id, SpellModOp::PowerCost0, powerCost, spell);

    if (!unitCaster->IsControlledByPlayer())
    {
        if (HasAttribute(SPELL_ATTR0_SCALES_WITH_CREATURE_LEVEL))
        {
            GtNPCManaCostScalerEntry const* spellScaler = sGtNPCManaCostScalerStore.LookupEntry(SpellLevel - 1);
            GtNPCManaCostScalerEntry const* casterScaler = sGtNPCManaCostScalerStore.LookupEntry(unitCaster->getLevel() - 1);
            if (spellScaler && spellScaler->ratio > 0.f && casterScaler && casterScaler->ratio > 0.f)
                powerCost *= casterScaler->ratio / spellScaler->ratio;
        }
    }

    if (invertSign)
        powerCost = -powerCost;

    // PCT mod from user auras by spell school and power type
    Unit::AuraEffectList const& aurasPct = unitCaster->GetAuraEffectsByType(SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT);
    for (Unit::AuraEffectList::const_iterator i = aurasPct.begin(); i != aurasPct.end(); ++i)
    {
        if (!((*i)->GetMiscValue() & schoolMask))
            continue;
        if (!((*i)->GetMiscValueB() & (1 << PowerType)))
            continue;
        powerCost += CalculatePct(powerCost, (*i)->GetAmount());
    }
    if (powerCost < 0)
        powerCost = 0;
    return powerCost;
}

float SpellInfo::GetSpellScalingMultiplier(WorldObject const* caster, bool isPowerCostRelated /*= false*/) const
{
    if (!caster || !caster->IsUnit() || SpellScalingId == 0)
        return 1.f;

    float multiplier = 1.f;
    float scalingMultiplier = 1.f;

    uint8 casterLevel = caster->ToUnit()->getLevel();

    if (casterLevel < Scaling.CastTimeMaxLevel && Scaling.CastTimeMax)
    {
        int32 castTime = Scaling.CastTimeMin + ((casterLevel - 1) * (Scaling.CastTimeMax - Scaling.CastTimeMin)) / (Scaling.CastTimeMaxLevel - 1);
        multiplier = castTime / (float)Scaling.CastTimeMax;
        scalingMultiplier = castTime / (float)Scaling.CastTimeMax;
    }

    if (!isPowerCostRelated)
    {
        if (casterLevel < Scaling.NerfMaxLevel)
            scalingMultiplier = ((((1.f - Scaling.NerfFactor) * (casterLevel - 1)) / (Scaling.NerfMaxLevel - 1)) + Scaling.NerfFactor) * multiplier;
    }

    return scalingMultiplier;
}

bool SpellInfo::IsRanked() const
{
    return ChainEntry != nullptr;
}

uint8 SpellInfo::GetRank() const
{
    if (!ChainEntry)
        return 1;
    return ChainEntry->rank;
}

SpellInfo const* SpellInfo::GetFirstRankSpell() const
{
    if (!ChainEntry)
        return this;
    return ChainEntry->first;
}
SpellInfo const* SpellInfo::GetLastRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->last;
}
SpellInfo const* SpellInfo::GetNextRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->next;
}
SpellInfo const* SpellInfo::GetPrevRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->prev;
}

SpellInfo const* SpellInfo::GetAuraRankForLevel(uint8 level) const
{
    // ignore passive spells
    if (IsPassive())
        return this;

    // Client ignores spell with these attributes (sub_53D9D0)
    if (HasAttribute(SPELL_ATTR0_AURA_IS_DEBUFF) || HasAttribute(SPELL_ATTR2_ALLOW_LOW_LEVEL_BUFF))
        return this;

    bool needRankSelection = false;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (IsPositiveEffect(i) &&
            (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AURA_2 ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_PARTY ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID) &&
            !Effects[i].Scaling.Coefficient)
        {
            needRankSelection = true;
            break;
        }
    }

    // not required
    if (!needRankSelection)
        return this;

    for (SpellInfo const* nextSpellInfo = this; nextSpellInfo != nullptr; nextSpellInfo = nextSpellInfo->GetPrevRankSpell())
    {
        // if found appropriate level
        if (uint32(level + 10) >= nextSpellInfo->SpellLevel)
            return nextSpellInfo;

        // one rank less then
    }

    // not found
    return nullptr;
}

bool SpellInfo::IsRankOf(SpellInfo const* spellInfo) const
{
    return GetFirstRankSpell() == spellInfo->GetFirstRankSpell();
}

bool SpellInfo::IsDifferentRankOf(SpellInfo const* spellInfo) const
{
    if (Id == spellInfo->Id)
        return false;
    return IsRankOf(spellInfo);
}

bool SpellInfo::IsHighRankOf(SpellInfo const* spellInfo) const
{
    if (ChainEntry && spellInfo->ChainEntry)
        if (ChainEntry->first == spellInfo->ChainEntry->first)
            if (ChainEntry->rank > spellInfo->ChainEntry->rank)
                return true;

    return false;
}

void SpellInfo::_InitializeExplicitTargetMask()
{
    bool srcSet = false;
    bool dstSet = false;
    uint32 targetMask = Targets;
    // prepare target mask using effect target entries
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (!Effects[i].IsEffect())
            continue;

        targetMask |= Effects[i].TargetA.GetExplicitTargetMask(srcSet, dstSet);
        targetMask |= Effects[i].TargetB.GetExplicitTargetMask(srcSet, dstSet);

        // add explicit target flags based on spell effects which have EFFECT_IMPLICIT_TARGET_EXPLICIT and no valid target provided
        if (Effects[i].GetImplicitTargetType() != EFFECT_IMPLICIT_TARGET_EXPLICIT)
            continue;

        // extend explicit target mask only if valid targets for effect could not be provided by target types
        uint32 effectTargetMask = Effects[i].GetMissingTargetMask(srcSet, dstSet, targetMask);

        // don't add explicit object/dest flags when spell has no max range
        if (GetMaxRange(true) == 0.0f && GetMaxRange(false) == 0.0f)
            effectTargetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_DEST_LOCATION);

        targetMask |= effectTargetMask;
    }

    ExplicitTargetMask = targetMask;
}

inline bool _isPositiveTarget(SpellInfo const* spellInfo, uint8 effIndex)
{
    if (!spellInfo->Effects[effIndex].IsEffect())
        return true;

    return (spellInfo->Effects[effIndex].TargetA.GetCheckType() != TARGET_CHECK_ENEMY &&
        spellInfo->Effects[effIndex].TargetB.GetCheckType() != TARGET_CHECK_ENEMY);
}

bool _isPositiveEffectImpl(SpellInfo const* spellInfo, uint8 effIndex, std::unordered_set<std::pair<uint32, uint8>>& visited)
{
    if (!spellInfo->Effects[effIndex].IsEffect())
        return true;

    // attribute may be already set in DB
    if (!spellInfo->IsPositiveEffect(effIndex))
        return false;

    // passive auras like talents are all positive
    if (spellInfo->IsPassive())
        return true;

    // not found a single positive spell with this attribute
    if (spellInfo->HasAttribute(SPELL_ATTR0_AURA_IS_DEBUFF))
        return false;

    if (spellInfo->HasAttribute(SPELL_ATTR4_AURA_IS_BUFF))
        return true;

    visited.insert({ spellInfo->Id, effIndex });

    //We need scaling level info for some auras that compute bp 0 or positive but should be debuffs
    float bpScalePerLevel = spellInfo->Effects[effIndex].RealPointsPerLevel;
    int32 bp = spellInfo->Effects[effIndex].CalcValue();
    switch (spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch (spellInfo->Id)
            {
                case 40268: // Spiritual Vengeance, Teron Gorefiend, Black Temple
                case 61987: // Avenging Wrath Marker
                case 61988: // Divine Shield exclude aura
                case 64412: // Phase Punch, Algalon the Observer, Ulduar
                case 72410: // Rune of Blood, Saurfang, Icecrown Citadel
                case 71204: // Touch of Insignificance, Lady Deathwhisper, Icecrown Citadel
                    return false;
                case 24732: // Bat Costume
                case 30877: // Tag Murloc
                case 61716: // Rabbit Costume
                case 61734: // Noblegarden Bunny
                case 62344: // Fists of Stone
                case 50344: // Dream Funnel
                case 61819: // Manabonked! (item)
                case 61834: // Manabonked! (minigob)
                case 73523: // Rigor Mortis
                    return true;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_ROGUE:
            switch (spellInfo->Id)
            {
                // Envenom must be considered as a positive effect even though it deals damage
                case 32645: // Envenom
                    return true;
                case 40251: // Shadow of Death, Teron Gorefiend, Black Temple
                    return false;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_MAGE:
            // Arcane Missiles
            if (spellInfo->SpellFamilyFlags[0] == 0x00000800)
                return false;
            break;
        case SPELLFAMILY_WARRIOR:
            // Slam, Execute
            if ((spellInfo->SpellFamilyFlags[0] & 0x20200000) != 0)
                return false;
            break;
        default:
            break;
    }

    switch (spellInfo->Mechanic)
    {
        case MECHANIC_IMMUNE_SHIELD:
            return true;
        default:
            break;
    }

    // Special case: effects which determine positivity of whole spell
    if (spellInfo->HasAttribute(SPELL_ATTR1_AURA_UNIQUE)) // @todo: rename to and handle SPELL_ATTR1_AURA_UNIQUE
    {
        // check for targets, there seems to be an assortment of dummy triggering spells that should be negative
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            if (!_isPositiveTarget(spellInfo, i))
                return false;
    }

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (spellInfo->Effects[i].Effect)
        {
            case SPELL_EFFECT_HEAL:
            case SPELL_EFFECT_LEARN_SPELL:
            case SPELL_EFFECT_SKILL_STEP:
            case SPELL_EFFECT_HEAL_PCT:
                return true;
            case SPELL_EFFECT_INSTAKILL:
                if (i != effIndex && // for spells like 38044: instakill effect is negative but auras on target must count as buff
                    spellInfo->Effects[i].TargetA.GetTarget() == spellInfo->Effects[effIndex].TargetA.GetTarget() &&
                    spellInfo->Effects[i].TargetB.GetTarget() == spellInfo->Effects[effIndex].TargetB.GetTarget())
                    return false;
                break;
            default:
                break;
        }

        if (spellInfo->Effects[i].IsAura())
        {
            switch (spellInfo->Effects[i].ApplyAuraName)
            {
                case SPELL_AURA_MOD_STEALTH:
                case SPELL_AURA_MOD_UNATTACKABLE:
                    return true;
                case SPELL_AURA_SCHOOL_HEAL_ABSORB:
                case SPELL_AURA_EMPATHY:
                case SPELL_AURA_MOD_DAMAGE_FROM_CASTER:
                case SPELL_AURA_PREVENTS_FLEEING:
                    return false;
                default:
                    break;
            }
        }
    }

    switch (spellInfo->Effects[effIndex].Effect)
    {
        case SPELL_EFFECT_WEAPON_DAMAGE:
        case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
        case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
        case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
        case SPELL_EFFECT_SCHOOL_DAMAGE:
        case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
        case SPELL_EFFECT_HEALTH_LEECH:
        case SPELL_EFFECT_INSTAKILL:
        case SPELL_EFFECT_POWER_DRAIN:
        case SPELL_EFFECT_STEAL_BENEFICIAL_BUFF:
        case SPELL_EFFECT_INTERRUPT_CAST:
        case SPELL_EFFECT_PICKPOCKET:
        case SPELL_EFFECT_GAMEOBJECT_DAMAGE:
        case SPELL_EFFECT_DURABILITY_DAMAGE:
        case SPELL_EFFECT_DURABILITY_DAMAGE_PCT:
        case SPELL_EFFECT_APPLY_AREA_AURA_ENEMY:
        case SPELL_EFFECT_TAMECREATURE:
        case SPELL_EFFECT_DISTRACT:
            return false;
        case SPELL_EFFECT_ENERGIZE:
        case SPELL_EFFECT_ENERGIZE_PCT:
        case SPELL_EFFECT_HEAL_PCT:
        case SPELL_EFFECT_HEAL_MAX_HEALTH:
        case SPELL_EFFECT_HEAL_MECHANICAL:
            return true;
        case SPELL_EFFECT_KNOCK_BACK:
        case SPELL_EFFECT_CHARGE:
        case SPELL_EFFECT_PERSISTENT_AREA_AURA:
        case SPELL_EFFECT_ATTACK_ME:
        case SPELL_EFFECT_POWER_BURN:
            // check targets
            if (!_isPositiveTarget(spellInfo, effIndex))
                return false;
            break;
        case SPELL_EFFECT_DISPEL:
            // non-positive dispel
            switch (spellInfo->Effects[effIndex].MiscValue)
            {
                case DISPEL_STEALTH:
                case DISPEL_INVISIBILITY:
                case DISPEL_ENRAGE:
                    return false;
                default:
                    break;
            }

            // also check targets
            if (!_isPositiveTarget(spellInfo, effIndex))
                return false;
            break;
        case SPELL_EFFECT_DISPEL_MECHANIC:
            if (!_isPositiveTarget(spellInfo, effIndex))
            {
                // non-positive mechanic dispel on negative target
                switch (spellInfo->Effects[effIndex].MiscValue)
                {
                    case MECHANIC_BANDAGE:
                    case MECHANIC_SHIELD:
                    case MECHANIC_MOUNT:
                    case MECHANIC_INVULNERABILITY:
                        return false;
                    default:
                        break;
                }
            }
            break;
        case SPELL_EFFECT_THREAT:
        case SPELL_EFFECT_MODIFY_THREAT_PERCENT:
            // check targets AND basepoints
            if (!_isPositiveTarget(spellInfo, effIndex) && bp > 0)
                return false;
            break;
        default:
            break;
    }

    if (spellInfo->Effects[effIndex].IsAura())
    {
        // non-positive aura use
        switch (spellInfo->Effects[effIndex].ApplyAuraName)
        {
            case SPELL_AURA_MOD_STAT:                   // dependent from basepoint sign (negative -> negative)
            case SPELL_AURA_MOD_SKILL:
            case SPELL_AURA_MOD_DODGE_PERCENT:
            case SPELL_AURA_MOD_HEALING_DONE:
            case SPELL_AURA_MOD_DAMAGE_DONE_CREATURE:
            case SPELL_AURA_OBS_MOD_HEALTH:
            case SPELL_AURA_OBS_MOD_POWER:
            case SPELL_AURA_MOD_CRIT_PCT:
            case SPELL_AURA_MOD_HIT_CHANCE:
            case SPELL_AURA_MOD_SPELL_HIT_CHANCE:
            case SPELL_AURA_MOD_SPELL_CRIT_CHANCE:
            case SPELL_AURA_MOD_RANGED_HASTE:
            case SPELL_AURA_MOD_MELEE_RANGED_HASTE:
            case SPELL_AURA_MOD_MELEE_RANGED_HASTE_2:
            case SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK:
            case SPELL_AURA_HASTE_SPELLS:
            case SPELL_AURA_HASTE_RANGED:
            case SPELL_AURA_MOD_DETECT_RANGE:
            case SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT:
            case SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE:
            case SPELL_AURA_MOD_INCREASE_SWIM_SPEED:
            case SPELL_AURA_MOD_PERCENT_STAT:
            case SPELL_AURA_MOD_INCREASE_HEALTH:
            case SPELL_AURA_MOD_SPEED_ALWAYS:
                if (bp < 0 || bpScalePerLevel < 0) //TODO: What if both are 0? Should it be a buff or debuff?
                    return false;
                break;
            case SPELL_AURA_MOD_ATTACKSPEED:            // some buffs have negative bp, check both target and bp
            case SPELL_AURA_MOD_MELEE_HASTE:
            case SPELL_AURA_MOD_DAMAGE_DONE:
            case SPELL_AURA_MOD_RESISTANCE:
            case SPELL_AURA_MOD_RESISTANCE_PCT:
            case SPELL_AURA_MOD_RATING:
            case SPELL_AURA_MOD_ATTACK_POWER:
            case SPELL_AURA_MOD_RANGED_ATTACK_POWER:
            case SPELL_AURA_MOD_DAMAGE_PERCENT_DONE:
            case SPELL_AURA_MOD_SPEED_SLOW_ALL:
            case SPELL_AURA_MELEE_SLOW:
            case SPELL_AURA_MOD_ATTACK_POWER_PCT:
            case SPELL_AURA_MOD_HEALING_DONE_PERCENT:
            case SPELL_AURA_MOD_HEALING_PCT:
            case SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE:
                if (!_isPositiveTarget(spellInfo, effIndex) && bp < 0)
                    return false;
                break;
            case SPELL_AURA_MOD_DAMAGE_TAKEN:           // dependent from basepoint sign (positive -> negative)
            case SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN:
            case SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT:
            case SPELL_AURA_MOD_POWER_COST_SCHOOL:
            case SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT:
            case SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT:
            case SPELL_AURA_BYPASS_ARMOR_FOR_CASTER:
                if (bp > 0)
                    return false;
                break;
            case SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN:   // check targets and basepoints (ex Recklessness)
                if (!_isPositiveTarget(spellInfo, effIndex) && bp > 0)
                    return false;
                break;
            case SPELL_AURA_MOD_HEALTH_REGEN_PERCENT:   // check targets and basepoints (target enemy and negative bp -> negative)
                if (!_isPositiveTarget(spellInfo, effIndex) && bp < 0)
                    return false;
                break;
            case SPELL_AURA_ADD_TARGET_TRIGGER:
                return true;
            case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                if (!_isPositiveTarget(spellInfo, effIndex))
                {
                    if (SpellInfo const* spellTriggeredProto = sSpellMgr->GetSpellInfo(spellInfo->Effects[effIndex].TriggerSpell))
                    {
                        // negative targets of main spell return early
                        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            // already seen this
                            if (visited.count({ spellTriggeredProto->Id, i }) > 0)
                                continue;

                            if (!spellTriggeredProto->Effects[i].Effect)
                                continue;

                            // if non-positive trigger cast targeted to positive target this main cast is non-positive
                            // this will place this spell auras as debuffs
                            if (_isPositiveTarget(spellTriggeredProto, i) && !_isPositiveEffectImpl(spellTriggeredProto, i, visited))
                                return false;
                        }
                    }
                }
                break;
            case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
            case SPELL_AURA_MOD_STUN:
            case SPELL_AURA_TRANSFORM:
            case SPELL_AURA_MOD_DECREASE_SPEED:
            case SPELL_AURA_MOD_FEAR:
            case SPELL_AURA_MOD_TAUNT:
                // special auras: they may have non negative target but still need to be marked as debuff
                // checked again after all effects (SpellInfo::_InitializeSpellPositivity)
            case SPELL_AURA_MOD_PACIFY:
            case SPELL_AURA_MOD_PACIFY_SILENCE:
            case SPELL_AURA_MOD_DISARM:
            case SPELL_AURA_MOD_DISARM_OFFHAND:
            case SPELL_AURA_MOD_DISARM_RANGED:
            case SPELL_AURA_MOD_CHARM:
            case SPELL_AURA_AOE_CHARM:
            case SPELL_AURA_MOD_POSSESS:
            case SPELL_AURA_MOD_LANGUAGE:
            case SPELL_AURA_DAMAGE_SHIELD:
            case SPELL_AURA_PROC_TRIGGER_SPELL:
            case SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE:
            case SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE:
            case SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE:
            case SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE:
            case SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE:
            case SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE:
            case SPELL_AURA_DUMMY:
            case SPELL_AURA_PERIODIC_DUMMY:
            case SPELL_AURA_MOD_HEALING:
            case SPELL_AURA_MOD_WEAPON_CRIT_PERCENT:
            case SPELL_AURA_POWER_BURN:
            case SPELL_AURA_MOD_COOLDOWN:
            case SPELL_AURA_MOD_INCREASE_SPEED:
            case SPELL_AURA_MOD_PARRY_PERCENT:
            case SPELL_AURA_SET_VEHICLE_ID:
            case SPELL_AURA_PERIODIC_ENERGIZE:
            case SPELL_AURA_EFFECT_IMMUNITY:
            case SPELL_AURA_OVERRIDE_CLASS_SCRIPTS:
            case SPELL_AURA_MOD_SHAPESHIFT:
            case SPELL_AURA_MOD_THREAT:
            case SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE:
                // check target for positive and negative spells
                if (!_isPositiveTarget(spellInfo, effIndex))
                    return false;
                break;
            case SPELL_AURA_MOD_CONFUSE:
            case SPELL_AURA_CHANNEL_DEATH_ITEM:
            case SPELL_AURA_MOD_ROOT:
            case SPELL_AURA_MOD_SILENCE:
            case SPELL_AURA_MOD_DETAUNT:
            case SPELL_AURA_GHOST:
            case SPELL_AURA_PERIODIC_LEECH:
            case SPELL_AURA_PERIODIC_MANA_LEECH:
            case SPELL_AURA_MOD_STALKED:
            case SPELL_AURA_PREVENT_RESURRECTION:
            case SPELL_AURA_PERIODIC_DAMAGE:
            case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
            case SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS:
            case SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS:
            case SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE:
                return false;
            case SPELL_AURA_MECHANIC_IMMUNITY:
            {
                // non-positive immunities
                switch (spellInfo->Effects[effIndex].MiscValue)
                {
                    case MECHANIC_BANDAGE:
                    case MECHANIC_SHIELD:
                    case MECHANIC_MOUNT:
                    case MECHANIC_INVULNERABILITY:
                        return false;
                    default:
                        break;
                }
                break;
            }
            case SPELL_AURA_ADD_FLAT_MODIFIER:          // mods
            case SPELL_AURA_ADD_PCT_MODIFIER:
            {
                switch (SpellModOp(spellInfo->Effects[effIndex].MiscValue))
                {
                    case SpellModOp::ChangeCastTime:             // dependent from basepoint sign (positive -> negative)
                    case SpellModOp::Period:
                    case SpellModOp::PowerCostOnMiss:
                    case SpellModOp::StartCooldown:
                        if (bp > 0)
                            return false;
                        break;
                    case SpellModOp::Cooldown:
                    case SpellModOp::PowerCost0:
                        if (!spellInfo->IsPositive() && bp > 0) // dependent on prev effects too (ex Arcane Power)
                            return false;
                        break;
                    case SpellModOp::PointsIndex0:                  // always positive
                    case SpellModOp::PointsIndex1:
                    case SpellModOp::PointsIndex2:
                    case SpellModOp::Points:
                    case SpellModOp::Hate:
                    case SpellModOp::ChainAmplitude:
                    case SpellModOp::Amplitude:
                        return true;
                    case SpellModOp::Duration:
                    case SpellModOp::CritChance:
                    case SpellModOp::HealingAndDamage:
                    case SpellModOp::ChainTargets:
                        if (!spellInfo->IsPositive() && bp < 0) // dependent on prev effects too
                            return false;
                        break;
                    default:                                // dependent from basepoint sign (negative -> negative)
                        if (bp < 0)
                            return false;
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
    // negative spell if triggered spell is negative
    if (!spellInfo->Effects[effIndex].ApplyAuraName && spellInfo->Effects[effIndex].TriggerSpell)
    {
        if (SpellInfo const* spellTriggeredProto = sSpellMgr->GetSpellInfo(spellInfo->Effects[effIndex].TriggerSpell))
        {
            // spells with at least one negative effect are considered negative
            // some self-applied spells have negative effects but in self casting case negative check ignored.
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                // already seen this
                if (visited.count({ spellTriggeredProto->Id, i }) > 0)
                    continue;

                if (!spellTriggeredProto->Effects[i].IsEffect())
                    continue;

                if (!_isPositiveEffectImpl(spellTriggeredProto, i, visited))
                    return false;
            }
        }
    }

    // ok, positive
    return true;
}

void SpellInfo::_InitializeSpellPositivity()
{
    std::unordered_set<std::pair<uint32 /*spellId*/, uint8 /*effIndex*/>> visited;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (!_isPositiveEffectImpl(this, i, visited))
            AttributesCu |= (SPELL_ATTR0_CU_NEGATIVE_EFF0 << i);

    // additional checks after effects marked
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (!Effects[i].IsEffect() || !IsPositiveEffect(i))
            continue;

        switch (Effects[i].ApplyAuraName)
        {
            // has other non positive effect?
            // then it should be marked negative if has same target as negative effect (ex 8510, 8511, 8893, 10267)
            case SPELL_AURA_DUMMY:
            case SPELL_AURA_MOD_STUN:
            case SPELL_AURA_MOD_FEAR:
            case SPELL_AURA_MOD_TAUNT:
            case SPELL_AURA_TRANSFORM:
            case SPELL_AURA_MOD_ATTACKSPEED:
            case SPELL_AURA_MOD_DECREASE_SPEED:
            {
                for (uint8 j = i + 1; j < MAX_SPELL_EFFECTS; ++j)
                    if (!IsPositiveEffect(j) && Effects[i].TargetA.GetTarget() == Effects[j].TargetA.GetTarget() && Effects[i].TargetB.GetTarget() == Effects[j].TargetB.GetTarget())
                        AttributesCu |= (SPELL_ATTR0_CU_NEGATIVE_EFF0 << i);

                break;
            }
            default:
                break;
        }
    }
}

SpellTargetRestrictionsEntry const* SpellInfo::GetSpellTargetRestrictions() const
{
    return SpellTargetRestrictionsId ? sSpellTargetRestrictionsStore.LookupEntry(SpellTargetRestrictionsId) : nullptr;
}

SpellEquippedItemsEntry const* SpellInfo::GetSpellEquippedItems() const
{
    return SpellEquippedItemsId ? sSpellEquippedItemsStore.LookupEntry(SpellEquippedItemsId) : nullptr;
}

SpellInterruptsEntry const* SpellInfo::GetSpellInterrupts() const
{
    return SpellInterruptsId ? sSpellInterruptsStore.LookupEntry(SpellInterruptsId) : nullptr;
}

SpellLevelsEntry const* SpellInfo::GetSpellLevels() const
{
    return SpellLevelsId ? sSpellLevelsStore.LookupEntry(SpellLevelsId) : nullptr;
}

SpellPowerEntry const* SpellInfo::GetSpellPower() const
{
    return SpellPowerId ? sSpellPowerStore.LookupEntry(SpellPowerId) : nullptr;
}

SpellReagentsEntry const* SpellInfo::GetSpellReagents() const
{
    return SpellReagentsId ? sSpellReagentsStore.LookupEntry(SpellReagentsId) : nullptr;
}

SpellScalingEntry const* SpellInfo::GetSpellScaling() const
{
    return SpellScalingId ? sSpellScalingStore.LookupEntry(SpellScalingId) : nullptr;
}

SpellShapeshiftEntry const* SpellInfo::GetSpellShapeshift() const
{
    return SpellShapeshiftId ? sSpellShapeshiftStore.LookupEntry(SpellShapeshiftId) : nullptr;
}

SpellTotemsEntry const* SpellInfo::GetSpellTotems() const
{
    return SpellTotemsId ? sSpellTotemsStore.LookupEntry(SpellTotemsId) : nullptr;
}

SpellAuraOptionsEntry const* SpellInfo::GetSpellAuraOptions() const
{
    return SpellAuraOptionsId ? sSpellAuraOptionsStore.LookupEntry(SpellAuraOptionsId) : nullptr;
}

SpellAuraRestrictionsEntry const* SpellInfo::GetSpellAuraRestrictions() const
{
    return SpellAuraRestrictionsId ? sSpellAuraRestrictionsStore.LookupEntry(SpellAuraRestrictionsId) : nullptr;
}

SpellCastingRequirementsEntry const* SpellInfo::GetSpellCastingRequirements() const
{
    return SpellCastingRequirementsId ? sSpellCastingRequirementsStore.LookupEntry(SpellCastingRequirementsId) : nullptr;
}

SpellCategoriesEntry const* SpellInfo::GetSpellCategories() const
{
    return SpellCategoriesId ? sSpellCategoriesStore.LookupEntry(SpellCategoriesId) : nullptr;
}

SpellClassOptionsEntry const* SpellInfo::GetSpellClassOptions() const
{
    return SpellClassOptionsId ? sSpellClassOptionsStore.LookupEntry(SpellClassOptionsId) : nullptr;
}

SpellCooldownsEntry const* SpellInfo::GetSpellCooldowns() const
{
    return SpellCooldownsId ? sSpellCooldownsStore.LookupEntry(SpellCooldownsId) : nullptr;
}

void SpellInfo::_UnloadImplicitTargetConditionLists()
{
    // find the same instances of ConditionList and delete them.
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        ConditionContainer* cur = Effects[i].ImplicitTargetConditions;
        if (!cur)
            continue;
        for (uint8 j = i; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (Effects[j].ImplicitTargetConditions == cur)
                Effects[j].ImplicitTargetConditions = nullptr;
        }
        delete cur;
    }
}
