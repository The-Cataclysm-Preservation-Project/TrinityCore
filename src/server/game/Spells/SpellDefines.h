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

#ifndef TRINITY_SPELLDEFINES_H
#define TRINITY_SPELLDEFINES_H

#include "Define.h"
#include "EnumFlag.h"
#include "ObjectGuid.h"
#include <vector>

class Item;
class AuraEffect;

enum class SpellInterruptFlags : uint32
{
    None                        = 0,
    Movement                    = 0x00000001,
    DamagePushbackPlayerOnly    = 0x00000002,
    Stun                        = 0x00000004, // useless, even spells without it get interrupted
    Combat                      = 0x00000008,
    DamageCancelsPlayerOnly     = 0x00000010,
    MeleeCombat                 = 0x00000020  // NYI
};

DEFINE_ENUM_FLAG(SpellInterruptFlags);

// See SpellAuraInterruptFlags for other values definitions
enum SpellChannelInterruptFlags
{
    CHANNEL_INTERRUPT_FLAG_INTERRUPT    = 0x08,  // interrupt
    CHANNEL_FLAG_DELAY                  = 0x4000
};

enum class SpellAuraInterruptFlags : uint32
{
    None                        = 0,
    HostileActionReceived       = 0x00000001,
    Damage                      = 0x00000002,
    Action                      = 0x00000004,
    Moving                      = 0x00000008,
    Turning                     = 0x00000010,
    Anim                        = 0x00000020,
    Dismount                    = 0x00000040,
    UnderWater                  = 0x00000080, // TODO: disallow casting when swimming (SPELL_FAILED_ONLY_ABOVEWATER)
    AboveWater                  = 0x00000100, // TODO: disallow casting when not swimming (SPELL_FAILED_ONLY_UNDERWATER)
    Sheathing                   = 0x00000200,
    Interacting                 = 0x00000400, // TODO: more than gossip, replace all the feign death removals by aura type
    Looting                     = 0x00000800,
    Attacking                   = 0x00001000,
    ItemUse                     = 0x00002000,
    DamageChannelDuration       = 0x00004000,
    Shapeshifting               = 0x00008000,
    ActionDelayed               = 0x00010000,
    Mount                       = 0x00020000,
    Standing                    = 0x00040000,
    LeaveWorld                  = 0x00080000,
    StealthOrInvis              = 0x00100000,
    InvulnerabilityBuff         = 0x00200000,
    EnterWorld                  = 0x00400000,
    PvPActive                   = 0x00800000,
    NonPeriodicDamage           = 0x01000000,
    LandingOrFlight             = 0x02000000,
    Release                     = 0x04000000,
    DamageCancelsScript         = 0x08000000, // NYI dedicated aura script hook
    EnteringCombat              = 0x10000000,
    Login                       = 0x20000000,
    Summon                      = 0x40000000,
    LeavingCombat               = 0x80000000,

    NOT_VICTIM                  = (HostileActionReceived | Damage | NonPeriodicDamage)
};

DEFINE_ENUM_FLAG(SpellAuraInterruptFlags);

enum class SpellAuraInterruptFlags2 : uint32
{
    None                        = 0,
    Falling                     = 0x00000001, // NYI
    Swimming                    = 0x00000002, // NYI
    NotMoving                   = 0x00000004, // NYI
    Ground                      = 0x00000008, // NYI
    Transform                   = 0x00000010, // NYI
    Jump                        = 0x00000020,
    ChangeSpec                  = 0x00000040, // NYI
    AbandonVehicle              = 0x00000080, // NYI
    StartOfEncounter            = 0x00000100, // NYI
    EndOfEncounter              = 0x00000200, // NYI
    Disconnect                  = 0x00000400, // NYI
    EnteringInstance            = 0x00000800, // NYI
    DuelEnd                     = 0x00001000, // NYI
    LeaveArenaOrBattleground    = 0x00002000, // NYI
    ChangeTalent                = 0x00004000, // NYI
    ChangeGlyph                 = 0x00008000, // NYI
    SeamlessTransfer            = 0x00010000, // NYI
    WarModeLeave                = 0x00020000, // NYI
    TouchingGround              = 0x00040000, // NYI
    ChromieTime                 = 0x00080000, // NYI
    SplineFlightOrFreeFlight    = 0x00100000, // NYI
    ProcOrPeriodicAttacking     = 0x00200000  // NYI
};

DEFINE_ENUM_FLAG(SpellAuraInterruptFlags2);

enum SpellModOp : uint8
{
    SPELLMOD_DAMAGE                     = 0,
    SPELLMOD_DURATION                   = 1,
    SPELLMOD_THREAT                     = 2,
    SPELLMOD_EFFECT1                    = 3,
    SPELLMOD_CHARGES                    = 4,
    SPELLMOD_RANGE                      = 5,
    SPELLMOD_RADIUS                     = 6,
    SPELLMOD_CRITICAL_CHANCE            = 7,
    SPELLMOD_ALL_EFFECTS                = 8,
    SPELLMOD_NOT_LOSE_CASTING_TIME      = 9,
    SPELLMOD_CASTING_TIME               = 10,
    SPELLMOD_COOLDOWN                   = 11,
    SPELLMOD_EFFECT2                    = 12,
    SPELLMOD_IGNORE_ARMOR               = 13,
    SPELLMOD_COST                       = 14,
    SPELLMOD_CRIT_DAMAGE_BONUS          = 15,
    SPELLMOD_RESIST_MISS_CHANCE         = 16,
    SPELLMOD_JUMP_TARGETS               = 17,
    SPELLMOD_CHANCE_OF_SUCCESS          = 18,
    SPELLMOD_ACTIVATION_TIME            = 19,
    SPELLMOD_DAMAGE_MULTIPLIER          = 20,
    SPELLMOD_GLOBAL_COOLDOWN            = 21,
    SPELLMOD_DOT                        = 22,
    SPELLMOD_EFFECT3                    = 23,
    SPELLMOD_BONUS_MULTIPLIER           = 24,
    // spellmod 25
    SPELLMOD_PROC_PER_MINUTE            = 26,
    SPELLMOD_VALUE_MULTIPLIER           = 27,
    SPELLMOD_RESIST_DISPEL_CHANCE       = 28,
    SPELLMOD_CRIT_DAMAGE_BONUS_2        = 29, //one not used spell
    SPELLMOD_SPELL_COST_REFUND_ON_FAIL  = 30,

    MAX_SPELLMOD
};

enum SpellValueMod : uint8
{
    SPELLVALUE_BASE_POINT0,
    SPELLVALUE_BASE_POINT1,
    SPELLVALUE_BASE_POINT2,
    SPELLVALUE_RADIUS_MOD,
    SPELLVALUE_MAX_TARGETS,
    SPELLVALUE_AURA_STACK,
    SPELLVALUE_AURA_DURATION
};

enum SpellFacingFlags
{
    SPELL_FACING_FLAG_INFRONT = 0x1
};

enum TriggerCastFlags : uint32
{
    TRIGGERED_NONE                                  = 0x00000000,   //! Not triggered
    TRIGGERED_IGNORE_GCD                            = 0x00000001,   //! Will ignore GCD
    TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD          = 0x00000002,   //! Will ignore Spell and Category cooldowns
    TRIGGERED_IGNORE_POWER_AND_REAGENT_COST         = 0x00000004,   //! Will ignore power and reagent cost
    TRIGGERED_IGNORE_CAST_ITEM                      = 0x00000008,   //! Will not take away cast item or update related achievement criteria
    TRIGGERED_IGNORE_AURA_SCALING                   = 0x00000010,   //! Will ignore aura scaling
    TRIGGERED_IGNORE_CAST_IN_PROGRESS               = 0x00000020,   //! Will not check if a current cast is in progress
    TRIGGERED_IGNORE_COMBO_POINTS                   = 0x00000040,   //! Will ignore combo point requirement
    TRIGGERED_CAST_DIRECTLY                         = 0x00000080,   //! In Spell::prepare, will be cast directly without setting containers for executed spell
    TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS           = 0x00000100,   //! Will ignore interruptible aura's at cast
    TRIGGERED_IGNORE_SET_FACING                     = 0x00000200,   //! Will not adjust facing to target (if any)
    TRIGGERED_IGNORE_SHAPESHIFT                     = 0x00000400,   //! Will ignore shapeshift checks
    TRIGGERED_IGNORE_CASTER_AURASTATE               = 0x00000800,   //! Will ignore caster aura states including combat requirements and death state
    TRIGGERED_DISALLOW_PROC_EVENTS                  = 0x00001000,   //! Disallows proc events from triggered spell (default)
    TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE   = 0x00002000,   //! Will ignore mounted/on vehicle restrictions
    TRIGGERED_IGNORE_BLOCKED_SPELL_FAMILY           = 0x00004000,   //! Will ignore blocked casts of SPELL_AURA_BLOCK_SPELL_FAMILY
    // reuse                                        = 0x00008000,
    TRIGGERED_IGNORE_CASTER_AURAS                   = 0x00010000,   //! Will ignore caster aura restrictions or requirements
    // reuse                                        = 0x00020000,
    TRIGGERED_DONT_REPORT_CAST_ERROR                = 0x00040000,   //! Will return SPELL_FAILED_DONT_REPORT in CheckCast functions
    TRIGGERED_FULL_MASK                             = 0x0007FFFF,   //! Used when doing CastSpell with triggered == true

    // debug flags (used with .cast triggered commands)
    TRIGGERED_IGNORE_EQUIPPED_ITEM_REQUIREMENT      = 0x00080000,   //! Will ignore equipped item requirements
    TRIGGERED_IGNORE_TARGET_CHECK                   = 0x00100000,   //! Will ignore most target checks (mostly DBC target checks)
    TRIGGERED_FULL_DEBUG_MASK                       = 0xFFFFFFFF
};

struct TC_GAME_API CastSpellExtraArgs
{
    CastSpellExtraArgs() {}
    CastSpellExtraArgs(bool triggered) : TriggerFlags(triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE) {}
    CastSpellExtraArgs(TriggerCastFlags trigger) : TriggerFlags(trigger) {}
    CastSpellExtraArgs(Item* item) : TriggerFlags(TRIGGERED_FULL_MASK), CastItem(item) {}
    CastSpellExtraArgs(AuraEffect const* eff) : TriggerFlags(TRIGGERED_FULL_MASK), TriggeringAura(eff) {}
    CastSpellExtraArgs(ObjectGuid const& origCaster) : TriggerFlags(TRIGGERED_FULL_MASK), OriginalCaster(origCaster) {}
    CastSpellExtraArgs(AuraEffect const* eff, ObjectGuid const& origCaster) : TriggerFlags(TRIGGERED_FULL_MASK), TriggeringAura(eff), OriginalCaster(origCaster) {}
    CastSpellExtraArgs(SpellValueMod mod, int32 val) { SpellValueOverrides.AddMod(mod, val); }

    CastSpellExtraArgs& SetTriggerFlags(TriggerCastFlags flag) { TriggerFlags = flag; return *this; }
    CastSpellExtraArgs& SetCastItem(Item* item) { CastItem = item; return *this; }
    CastSpellExtraArgs& SetTriggeringAura(AuraEffect const* triggeringAura) { TriggeringAura = triggeringAura; return *this; }
    CastSpellExtraArgs& SetOriginalCaster(ObjectGuid const& guid) { OriginalCaster = guid; return *this; }
    CastSpellExtraArgs& AddSpellMod(SpellValueMod mod, int32 val) { SpellValueOverrides.AddMod(mod, val); return *this; }
    CastSpellExtraArgs& AddSpellBP0(int32 val) { SpellValueOverrides.AddBP0(val); return *this; }

    TriggerCastFlags TriggerFlags = TRIGGERED_NONE;
    Item* CastItem = nullptr;
    AuraEffect const* TriggeringAura = nullptr;
    ObjectGuid OriginalCaster = ObjectGuid::Empty;
    struct
    {
        public:
        void AddMod(SpellValueMod mod, int32 val) { data.push_back({ mod, val }); }
        void AddBP0(int32 bp0) { AddMod(SPELLVALUE_BASE_POINT0, bp0); } // because i don't want to type SPELLVALUE_BASE_POINT0 300 times

        private:
        auto begin() const { return data.cbegin(); }
        auto end() const { return data.cend(); }

        std::vector<std::pair<SpellValueMod, int32>> data;

        friend class Unit;
    } SpellValueOverrides;
};

enum class SummonPropertiesParamType : uint8
{
    None            = 0,
    Health          = 1,
    NumUnitsMin     = 2, // Minimum 1
    SeatNumber      = 3,
    RideSpell       = 4,
    CreatureLevel   = 5,
    MaxSummons      = 6, // Totem Slot
    NumUnitsMax     = 7  // Fail if less than 1
};

#endif // TRINITY_SPELLDEFINES_H
