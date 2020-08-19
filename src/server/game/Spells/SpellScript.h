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

#ifndef __SPELL_SCRIPT_H
#define __SPELL_SCRIPT_H

#include "ObjectGuid.h"
#include "Optional.h"
#include "SharedDefines.h"
#include "SpellAuraDefines.h"
#include "Util.h"
#include "advstd.h"

#include <memory>
#include <functional>
#include <stack>

class Aura;
class AuraApplication;
class AuraEffect;
class Creature;
class Corpse;
class DamageInfo;
class DispelInfo;
class DynamicObject;
class GameObject;
class Item;
class ModuleReference;
class Player;
class ProcEventInfo;
class Spell;
class SpellInfo;
class SpellScript;
class Unit;
class WorldLocation;
class WorldObject;
struct SpellDestination;
struct SpellModifier;
struct SpellValue;

#define SPELL_EFFECT_ANY (uint16)-1
#define SPELL_AURA_ANY (uint16)-1

enum SpellScriptState
{
    SPELL_SCRIPT_STATE_NONE = 0,
    SPELL_SCRIPT_STATE_REGISTRATION,
    SPELL_SCRIPT_STATE_LOADING,
    SPELL_SCRIPT_STATE_UNLOADING
};
#define SPELL_SCRIPT_STATE_END SPELL_SCRIPT_STATE_UNLOADING + 1


template <typename R, typename... Args>
struct TC_GAME_API HookHandler
{
        template <typename T>
        using HookType = R(T::*)(Args...);

        template <typename Base, typename Derived = Base, typename = std::enable_if_t<std::is_base_of_v<Base, Derived>>>
        HookHandler(Derived* owner, HookType<Base> hookFunction) {
            _handler = [=](Args&&... args) mutable -> R {
                return (owner->*hookFunction)(std::forward<Args>(args)...);
            };
        }

        R Call(Args... args)
        {
            return _handler(std::forward<Args>(args)...);
        }

    private:
        std::function<R(Args...)> _handler;
};

// helper class from which SpellScript and AuraScript derive, use these classes instead
class TC_GAME_API _SpellScript
{
    // internal use classes & functions
    // DO NOT OVERRIDE THESE IN SCRIPTS
    protected:
        virtual bool _Validate(SpellInfo const* entry);

    public:
        _SpellScript() : m_currentScriptState(SPELL_SCRIPT_STATE_NONE), m_scriptName(nullptr), m_scriptSpellId(0) {}
        virtual ~_SpellScript() { }
        void _Register();
        void _Unload();
        void _Init(std::string const* scriptname, uint32 spellId);
        std::string const* _GetScriptName() const;

    protected:
        template <typename T>
        struct TC_GAME_API EffectIndexHook : public T
        {
                template <typename U>
                using HookType = typename T::template HookType<U>;

                template <typename... Args>
                EffectIndexHook(uint8 effIndex, Args&&... args) : T(std::forward<Args&&>(args)...)
                {
                    // effect index must be in range <0;2>, allow use of special effindexes
                    ASSERT(_effIndex == EFFECT_ALL || _effIndex == EFFECT_FIRST_FOUND || _effIndex < MAX_SPELL_EFFECTS);
                    _effIndex = effIndex;
                }

                virtual ~EffectIndexHook() { }

                uint8 GetAffectedEffectsMask(SpellInfo const* spellInfo)
                {
                    uint8 mask = 0;
                    if ((_effIndex == EFFECT_ALL) || (_effIndex == EFFECT_FIRST_FOUND))
                    {
                        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if ((_effIndex == EFFECT_FIRST_FOUND) && mask)
                                return mask;
                            if (CheckEffect(spellInfo, i))
                                mask |= (uint8)1 << i;
                        }
                    }
                    else
                    {
                        if (CheckEffect(spellInfo, _effIndex))
                            mask |= (uint8)1 << _effIndex;
                    }
                    return mask;
                }

                bool IsEffectAffected(SpellInfo const* spellInfo, uint8 effIndex)
                {
                    return (GetAffectedEffectsMask(spellInfo) & (1 << effIndex)) != 0;
                }

                virtual bool CheckEffect(SpellInfo const* spellInfo, uint8 effIndex) = 0;

                std::string EffIndexToString() const
                {
                    switch (_effIndex)
                    {
                        case EFFECT_ALL:
                            return "EFFECT_ALL";
                        case EFFECT_FIRST_FOUND:
                            return "EFFECT_FIRST_FOUND";
                        case EFFECT_0:
                            return "EFFECT_0";
                        case EFFECT_1:
                            return "EFFECT_1";
                        case EFFECT_2:
                            return "EFFECT_2";
                    }
                    return "Invalid Value";
                }


            protected:
                uint8 _effIndex;
        };

        class TC_GAME_API EffectNameHook
        {
            public:
                EffectNameHook(uint16 _effName) { effName = _effName; }
                bool Check(SpellInfo const* spellInfo, uint8 effIndex);
                std::string ToString();
            private:
                uint16 effName;
        };

        class TC_GAME_API EffectAuraNameHook
        {
            public:
                EffectAuraNameHook(uint16 _effAurName) { effAurName = _effAurName; }
                bool Check(SpellInfo const* spellInfo, uint8 effIndex);
                std::string ToString();
            private:
                uint16 effAurName;
        };

        uint8 m_currentScriptState;
        std::string const* m_scriptName;
        uint32 m_scriptSpellId;

    private:

#ifdef TRINITY_API_USE_DYNAMIC_LINKING

        // Strong reference to keep the binary code loaded
        std::shared_ptr<ModuleReference> m_moduleReference;

#endif // #ifndef TRINITY_API_USE_DYNAMIC_LINKING

    public:
        //
        // SpellScript/AuraScript interface base
        // these functions are safe to override, see notes below for usage instructions
        //
        // Function in which handler functions are registered, must be implemented in script
        virtual void Register() = 0;
        // Function called on server startup, if returns false script won't be used in core
        // use for: dbc/template data presence/correctness checks
        virtual bool Validate(SpellInfo const* /*spellInfo*/) { return true; }
        // Function called when script is created, if returns false script will be unloaded afterwards
        // use for: initializing local script variables (DO NOT USE CONSTRUCTOR FOR THIS PURPOSE!)
        virtual bool Load() { return true; }
        // Function called when script is destroyed
        // use for: deallocating memory allocated by script
        virtual void Unload() { }
        // Helpers
        static bool ValidateSpellInfo(std::initializer_list<uint32> spellIds)
        {
            return _ValidateSpellInfo(spellIds.begin(), spellIds.end());
        }

        template <class T>
        static bool ValidateSpellInfo(T const& spellIds)
        {
            return _ValidateSpellInfo(std::begin(spellIds), std::end(spellIds));
        }

    private:
        static bool _ValidateSpellInfo(uint32 const* begin, uint32 const* end);
};

// SpellScript interface - enum used for runtime checks of script function calls
enum SpellScriptHookType
{
    SPELL_SCRIPT_HOOK_EFFECT_LAUNCH = SPELL_SCRIPT_STATE_END,
    SPELL_SCRIPT_HOOK_EFFECT_LAUNCH_TARGET,
    SPELL_SCRIPT_HOOK_EFFECT_HIT,
    SPELL_SCRIPT_HOOK_EFFECT_HIT_TARGET,
    SPELL_SCRIPT_HOOK_EFFECT_SUCCESSFUL_DISPEL,
    SPELL_SCRIPT_HOOK_BEFORE_HIT,
    SPELL_SCRIPT_HOOK_HIT,
    SPELL_SCRIPT_HOOK_AFTER_HIT,
    SPELL_SCRIPT_HOOK_OBJECT_AREA_TARGET_SELECT,
    SPELL_SCRIPT_HOOK_OBJECT_TARGET_SELECT,
    SPELL_SCRIPT_HOOK_DESTINATION_TARGET_SELECT,
    SPELL_SCRIPT_HOOK_CHECK_CAST,
    SPELL_SCRIPT_HOOK_BEFORE_CAST,
    SPELL_SCRIPT_HOOK_ON_CAST,
    SPELL_SCRIPT_HOOK_AFTER_CAST,
    SPELL_SCRIPT_HOOK_ON_SPELL_START
};

#define HOOK_SPELL_HIT_START SPELL_SCRIPT_HOOK_EFFECT_HIT
#define HOOK_SPELL_HIT_END SPELL_SCRIPT_HOOK_AFTER_HIT + 1
#define HOOK_SPELL_START SPELL_SCRIPT_HOOK_EFFECT
#define HOOK_SPELL_END SPELL_SCRIPT_HOOK_CHECK_CAST + 1
#define HOOK_SPELL_COUNT HOOK_SPELL_END - HOOK_SPELL_START

class TC_GAME_API SpellScript : public _SpellScript
{
    // internal use classes & functions
    // DO NOT OVERRIDE THESE IN SCRIPTS
    public:

        template <typename EffectHandler>
        class TC_GAME_API NamedEffectHook : public  _SpellScript::EffectNameHook, public _SpellScript::EffectIndexHook<EffectHandler>
        {
            using BaseHook = _SpellScript::EffectIndexHook<EffectHandler>;

            public:
                template <typename Base, typename Derived = Base>
                NamedEffectHook(Derived* owner, typename EffectHandler::template HookType<Base> hookFunction, uint8 _effIndex, uint16 _effName)
                    : _SpellScript::EffectNameHook(_effName), BaseHook(_effIndex, owner, hookFunction)
                {

                }

                std::string ToString()
                {
                    return "Index: " + BaseHook::EffIndexToString() + " Name: " + _SpellScript::EffectNameHook::ToString();
                }

                bool CheckEffect(SpellInfo const* spellInfo, uint8 effIndex) override
                {
                    return _SpellScript::EffectNameHook::Check(spellInfo, effIndex);
                }
        };
        
        using CheckCastHook   = HookHandler<SpellCastResult>;
        using EffectIndexHook = HookHandler<void, SpellEffIndex>;
        using HitHook         = HookHandler<void>;
        using CastHook        = HookHandler<void>;

        template <typename EffectHandler>
        class TC_GAME_API EffectTargetHook : public _SpellScript::EffectIndexHook<EffectHandler>
        {
            public:
                template <typename Base, typename Derived = Base>
                EffectTargetHook(Derived* owner, typename EffectHandler::template HookType<Base> hookFunction, uint8 _effectIndex, uint16 targetType, bool area, bool dest)
                    : _SpellScript::EffectIndexHook<EffectHandler>(_effectIndex, owner, hookFunction), _targetType(targetType), _area(area), _dest(dest)
                {
                }

                bool CheckEffect(SpellInfo const* spellEntry, uint8 effIndex) override
                {
                    if (!_targetType)
                        return false;

                    if (spellEntry->Effects[effIndex].TargetA.GetTarget() != _targetType &&
                        spellEntry->Effects[effIndex].TargetB.GetTarget() != _targetType)
                        return false;

                    SpellImplicitTargetInfo targetInfo(_targetType);
                    switch (targetInfo.GetSelectionCategory())
                    {
                        case TARGET_SELECT_CATEGORY_CHANNEL: // SINGLE
                            return !_area;
                        case TARGET_SELECT_CATEGORY_NEARBY: // BOTH
                            return true;
                        case TARGET_SELECT_CATEGORY_CONE: // AREA
                        case TARGET_SELECT_CATEGORY_AREA: // AREA
                            return _area;
                        case TARGET_SELECT_CATEGORY_DEFAULT:
                            switch (targetInfo.GetObjectType())
                            {
                                case TARGET_OBJECT_TYPE_SRC: // EMPTY
                                    return false;
                                case TARGET_OBJECT_TYPE_DEST: // DEST
                                    return _dest;
                                default:
                                    switch (targetInfo.GetReferenceType())
                                    {
                                        case TARGET_REFERENCE_TYPE_CASTER: // SINGLE
                                            return !_area;
                                        case TARGET_REFERENCE_TYPE_TARGET: // BOTH
                                            return true;
                                        default:
                                            break;
                                    }
                                    break;
                            }
                            break;
                        default:
                            break;
                    }

                    return false;
                }

                std::string ToString()
                {
                    std::ostringstream oss;
                    oss << "Index: " << EffIndexToString() << " Target: " << targetType;
                    return oss.str();
                }

                uint16 GetTarget() const { return targetType; }

            private:
                uint16 _targetType;
                bool _area;
                bool _dest;
        };

        using ObjectAreaTargetSelectHandler = HookHandler<void, std::list<WorldObject*>&>;
        struct TC_GAME_API ObjectAreaTargetSelectHook : public EffectTargetHook<ObjectAreaTargetSelectHandler>
        {
            template <typename T>
            using HookType = typename ObjectAreaTargetSelectHandler::template HookType<T>;

            template <typename Base, typename Derived = Base>
            ObjectAreaTargetSelectHook(Derived* owner, HookType<Base> hookFunction, uint8 _effIndex, uint16 _targetType)
                : EffectTargetHook<ObjectAreaTargetSelectHandler>(owner, hookFunction, _effIndex, _targetType, true, false)
            {
            }
        };

        using ObjectTargetSelectHandler = HookHandler<void, WorldObject*&>;
        struct TC_GAME_API ObjectTargetSelectHook : public EffectTargetHook<ObjectTargetSelectHandler>
        {
            template <typename T>
            using HookType = typename ObjectTargetSelectHandler::template HookType<T>;

            template <typename Base, typename Derived = Base>
            ObjectTargetSelectHook(Derived* owner, HookType<Base> hookFunction, uint8 _effIndex, uint16 _targetType)
                : EffectTargetHook<ObjectTargetSelectHandler>(owner, hookFunction, _effIndex, _targetType, false, false)
            {
            }
        };

        using DestinationTargetSelectHandler = HookHandler<void, SpellDestination&>;
        struct TC_GAME_API DestinationTargetSelectHook : public EffectTargetHook<DestinationTargetSelectHandler>
        {
            template <typename T>
            using HookType = typename DestinationTargetSelectHandler::template HookType<T>;

            template <typename Base, typename Derived = Base>
            DestinationTargetSelectHook(Derived* owner, HookType<Base> hookFunction, uint8 _effIndex, uint16 _targetType)
                : EffectTargetHook<DestinationTargetSelectHandler>(owner, hookFunction, _effIndex, _targetType, false, true)
            {
            }
        };

    public:
        bool _Validate(SpellInfo const* entry) override;
        bool _Load(Spell* spell);
        void _InitHit();
        bool _IsEffectPrevented(SpellEffIndex effIndex) { return (m_hitPreventEffectMask & (1 << effIndex)) != 0; }
        bool _IsDefaultEffectPrevented(SpellEffIndex effIndex) { return (m_hitPreventDefaultEffectMask & (1 << effIndex)) != 0; }
        void _PrepareScriptCall(SpellScriptHookType hookType);
        void _FinishScriptCall();
        bool IsInCheckCastHook() const;
        bool IsInTargetHook() const;
        bool IsInModifiableHook() const;
        bool IsInHitPhase() const;
        bool IsInEffectHook() const;
    private:
        Spell* m_spell;
        uint8 m_hitPreventEffectMask;
        uint8 m_hitPreventDefaultEffectMask;

    public: // SpellScript interface - Cast hooks

        // OnSpellStart.Register(this, &class::function);
        // Function is: void function();
        HookList<CastHook> OnSpellStart;

        // BeforeCast.Register(this, &class::function);
        // Function is: void function();
        HookList<CastHook> BeforeCast;

        // OnCast.Register(this, &class::function);
        // Function is: void function();
        HookList<CastHook> OnCast;

        // AfterCast.Register(this, &class::function);
        // Function is: void function();
        HookList<CastHook> AfterCast;

        // OnCheckCast.Register(this, &class::function);
        // Function is: SpellCastResult function()
        HookList<CheckCastHook> OnCheckCast;

    public: // SpellScript interface - Effect hooks

        // OnEffect****.Register(this, &class::function, EffectIndexSpecifier, EffectNameSpecifier);
        // Function is: void function(SpellEffIndex effIndex);
        HookList<NamedEffectHook<EffectIndexHook>> OnEffectLaunch;
        HookList<NamedEffectHook<EffectIndexHook>> OnEffectLaunchTarget;
        HookList<NamedEffectHook<EffectIndexHook>> OnEffectHit;
        HookList<NamedEffectHook<EffectIndexHook>> OnEffectHitTarget;
        HookList<NamedEffectHook<EffectIndexHook>> OnEffectSuccessfulDispel;

    public: // SpellScript interface - Hit hooks

        // BeforeHit.Register(this, class::function);
        // Function is: void function()
        HookList<HitHook> BeforeHit;

        // OnHit.Register(this, &class::function);
        // Function is: void function()
        HookList<HitHook> OnHit;

        // AfterHit.Register(this, &class::function);
        // Function is: void function()
        HookList<HitHook> AfterHit;

    public: // SpellScript interface - Target selection hooks

        // OnObjectAreaTargetSelect.Register(this, &class::function, EffectIndexSpecifier, TargetsNameSpecifier);
        // Function is: void function(std::list<WorldObject*>& targets)
        HookList<ObjectAreaTargetSelectHook> OnObjectAreaTargetSelect;

        // OnObjectTargetSelect.Register(this, &class::function, EffectIndexSpecifier, TargetsNameSpecifier);
        // Function is: void function(WorldObject*& target)
        HookList<ObjectTargetSelectHook> OnObjectTargetSelect;

        // OnDestinationTargetSelect.Register(this, &class::function, EffectIndexSpecifier, TargetsNameSpecifier);
        // Function is: void function(SpellDestination& target)
        HookList<DestinationTargetSelectHook> OnDestinationTargetSelect;

        // hooks are executed in following order, at specified event of spell:
        // 1. BeforeCast - executed when spell preparation is finished (when cast bar becomes full) before cast is handled
        // 2. OnCheckCast - allows to override result of CheckCast function
        // 3a. OnObjectAreaTargetSelect - executed just before adding selected targets to final target list (for area targets)
        // 3b. OnObjectTargetSelect - executed just before adding selected target to final target list (for single unit targets)
        // 3c. OnDestinationTargetSelect - executed just before adding selected target to final target list (for destination targets)
        // 4. OnCast - executed just before spell is launched (creates missile) or executed
        // 5. AfterCast - executed after spell missile is launched and immediate spell actions are done
        // 6. OnEffectLaunch - executed just before specified effect handler call - when spell missile is launched
        // 7. OnEffectLaunchTarget - executed just before specified effect handler call - when spell missile is launched - called for each target from spell target map
        // 8. OnEffectHit - executed just before specified effect handler call - when spell missile hits dest
        // 9. BeforeHit - executed just before spell hits a target - called for each target from spell target map
        // 10. OnEffectHitTarget - executed just before specified effect handler call - called for each target from spell target map
        // 11. OnHit - executed just before spell deals damage and procs auras - when spell hits target - called for each target from spell target map
        // 12. AfterHit - executed just after spell finishes all it's jobs for target - called for each target from spell target map

        // this hook is only executed after a successful dispel of any aura
        // OnEffectSuccessfulDispel - executed just after effect successfully dispelled aura(s)

        //
        // methods allowing interaction with Spell object
        //
        // methods useable during all spell handling phases
        Unit* GetCaster() const;
        Unit* GetOriginalCaster() const;
        SpellInfo const* GetSpellInfo() const;
        SpellValue const* GetSpellValue() const;

        // methods useable after spell is prepared
        // accessors to the explicit targets of the spell
        // explicit target - target selected by caster (player, game client, or script - DoCast(explicitTarget, ...), required for spell to be cast
        // examples:
        // -shadowstep - explicit target is the unit you want to go behind of
        // -chain heal - explicit target is the unit to be healed first
        // -holy nova/arcane explosion - explicit target = nullptr because target you are selecting doesn't affect how spell targets are selected
        // you can determine if spell requires explicit targets by dbc columns:
        // - Targets - mask of explicit target types
        // - ImplicitTargetXX set to TARGET_XXX_TARGET_YYY, _TARGET_ here means that explicit target is used by the effect, so spell needs one too

        // returns: WorldLocation which was selected as a spell destination or nullptr
        WorldLocation const* GetExplTargetDest() const;

        void SetExplTargetDest(WorldLocation& loc);

        // returns: WorldObject which was selected as an explicit spell target or nullptr if there's no target
        WorldObject* GetExplTargetWorldObject() const;

        // returns: Unit which was selected as an explicit spell target or nullptr if there's no target
        Unit* GetExplTargetUnit() const;

        // returns: GameObject which was selected as an explicit spell target or nullptr if there's no target
        GameObject* GetExplTargetGObj() const;

        // returns: Item which was selected as an explicit spell target or nullptr if there's no target
        Item* GetExplTargetItem() const;

        // methods useable only during spell hit on target, or during spell launch on target:
        // returns: target of current effect if it was Unit otherwise nullptr
        Unit* GetHitUnit() const;
        // returns: target of current effect if it was Creature otherwise nullptr
        Creature* GetHitCreature() const;
        // returns: target of current effect if it was Player otherwise nullptr
        Player* GetHitPlayer() const;
        // returns: target of current effect if it was Item otherwise nullptr
        Item* GetHitItem() const;
        // returns: target of current effect if it was GameObject otherwise nullptr
        GameObject* GetHitGObj() const;
        // returns: target of current effect if it was Corpse otherwise nullptr
        Corpse* GetHitCorpse() const;
        // returns: destination of current effect
        WorldLocation* GetHitDest() const;
        // setter/getter for for damage done by spell to target of spell hit
        // returns damage calculated before hit, and real dmg done after hit
        int32 GetHitDamage() const;
        void SetHitDamage(int32 damage);
        void PreventHitDamage() { SetHitDamage(0); }
        // setter/getter for for heal done by spell to target of spell hit
        // returns healing calculated before hit, and real dmg done after hit
        int32 GetHitHeal() const;
        void SetHitHeal(int32 heal);
        void PreventHitHeal() { SetHitHeal(0); }
        Spell* GetSpell() const { return m_spell; }
        // returns current spell hit target aura
        Aura* GetHitAura() const;
        // prevents applying aura on current spell hit target
        void PreventHitAura();

        // prevents effect execution on current spell hit target
        // including other effect/hit scripts
        // will not work on aura/damage/heal
        // will not work if effects were already handled
        void PreventHitEffect(SpellEffIndex effIndex);

        // prevents default effect execution on current spell hit target
        // will not work on aura/damage/heal effects
        // will not work if effects were already handled
        void PreventHitDefaultEffect(SpellEffIndex effIndex);

        // method avalible only in EffectHandler method
        int32 GetEffectValue() const;
        void SetEffectValue(int32 value);

        // returns: cast item if present.
        Item* GetCastItem() const;

        // Creates item. Calls Spell::DoCreateItem method.
        void CreateItem(uint32 effIndex, uint32 itemId);

        // Returns SpellInfo from the spell that triggered the current one
        SpellInfo const* GetTriggeringSpell() const;

        // finishes spellcast prematurely with selected error message
        void FinishCast(SpellCastResult result, uint32* param1 = nullptr, uint32* param2 = nullptr);

        void SetCustomCastResultMessage(SpellCustomErrors result);
};

// AuraScript interface - enum used for runtime checks of script function calls
enum AuraScriptHookType
{
    AURA_SCRIPT_HOOK_EFFECT_APPLY = SPELL_SCRIPT_STATE_END,
    AURA_SCRIPT_HOOK_EFFECT_AFTER_APPLY,
    AURA_SCRIPT_HOOK_EFFECT_REMOVE,
    AURA_SCRIPT_HOOK_EFFECT_AFTER_REMOVE,
    AURA_SCRIPT_HOOK_EFFECT_PERIODIC,
    AURA_SCRIPT_HOOK_EFFECT_UPDATE_PERIODIC,
    AURA_SCRIPT_HOOK_EFFECT_CALC_AMOUNT,
    AURA_SCRIPT_HOOK_EFFECT_CALC_PERIODIC,
    AURA_SCRIPT_HOOK_EFFECT_CALC_SPELLMOD,
    AURA_SCRIPT_HOOK_EFFECT_ABSORB,
    AURA_SCRIPT_HOOK_EFFECT_AFTER_ABSORB,
    AURA_SCRIPT_HOOK_EFFECT_MANASHIELD,
    AURA_SCRIPT_HOOK_EFFECT_AFTER_MANASHIELD,
    AURA_SCRIPT_HOOK_EFFECT_SPLIT,
    AURA_SCRIPT_HOOK_CHECK_AREA_TARGET,
    AURA_SCRIPT_HOOK_DISPEL,
    AURA_SCRIPT_HOOK_AFTER_DISPEL,
    // Spell Proc Hooks
    AURA_SCRIPT_HOOK_CHECK_PROC,
    AURA_SCRIPT_HOOK_CHECK_EFFECT_PROC,
    AURA_SCRIPT_HOOK_PREPARE_PROC,
    AURA_SCRIPT_HOOK_PROC,
    AURA_SCRIPT_HOOK_EFFECT_PROC,
    AURA_SCRIPT_HOOK_EFFECT_AFTER_PROC,
    AURA_SCRIPT_HOOK_AFTER_PROC,
    /*AURA_SCRIPT_HOOK_APPLY,
    AURA_SCRIPT_HOOK_REMOVE, */
};

/*
#define HOOK_AURA_EFFECT_START HOOK_AURA_EFFECT_APPLY
#define HOOK_AURA_EFFECT_END HOOK_AURA_EFFECT_CALC_SPELLMOD + 1
#define HOOK_AURA_EFFECT_COUNT HOOK_AURA_EFFECT_END - HOOK_AURA_EFFECT_START
*/
class TC_GAME_API AuraScript : public _SpellScript
{
    // internal use classes & functions
    // DO NOT OVERRIDE THESE IN SCRIPTS
    public:

        template <typename EffectHandler>
        using AuraHook = _SpellScript::EffectIndexHook<EffectHandler>;

        template <typename EffectHandler>
        struct TC_GAME_API AuraNameHook : public  _SpellScript::EffectAuraNameHook, public AuraHook<EffectHandler>
        {
            template <typename T>
            using HookType = typename EffectHandler::template HookType<T>;

            template <typename Base, typename Derived = Base>
            AuraNameHook(Derived* owner, HookType<Base> hookFunction, uint8 effIndex, uint16 auraName)
                : _SpellScript::EffectAuraNameHook(auraName), AuraHook<EffectHandler>(effIndex, owner, hookFunction)
            {

            }

            std::string ToString()
            {
                return "Index: " + EffIndexToString() + " AuraName: " + _SpellScript::EffectAuraNameHook::ToString();
            }

            bool CheckEffect(SpellInfo const* spellInfo, uint8 effIndex) override
            {
                return _SpellScript::EffectAuraNameHook::Check(spellInfo, effIndex);
            }
        };

        using AuraCheckAreaTargetHook      = HookHandler<bool, Unit*>;
        using AuraDispelHook               = HookHandler<void, DispelInfo*>;
        using AuraEffectPeriodicHook       = AuraNameHook<HookHandler<void, AuraEffect const*>>;
        using AuraEffectUpdatePeriodicHook = AuraNameHook<HookHandler<void, AuraEffect*>>;
        using AuraEffectCalcAmountHook     = AuraNameHook<HookHandler<void, AuraEffect const*, int32&, bool&>>;
        using AuraEffectCalcPeriodicHook   = AuraNameHook<HookHandler<void, AuraEffect const*, bool&, int32&>>;
        using AuraEffectCalcSpellModHook   = AuraNameHook<HookHandler<void, AuraEffect const*, SpellModifier*&>>;

        struct TC_GAME_API AuraEffectApplyHook : AuraNameHook<HookHandler<void, AuraEffect const*, AuraEffectHandleModes>>
        {
            using BaseHook = AuraNameHook<HookHandler<void, AuraEffect const*, AuraEffectHandleModes>>;

            template <typename T>
            using HookType = typename HookHandler<void, AuraEffect const*, AuraEffectHandleModes>::template HookType<T>;

            template <typename Base, typename Derived = Base>
            AuraEffectApplyHook(Derived * owner, HookType<Base> hookFunction, uint8 effIndex, uint16 auraName)
                : BaseHook(owner, hookFunction, effIndex, auraName)
            {

            }

            template <typename Base, typename Derived = Base>
            AuraEffectApplyHook(Derived* owner, HookType<Base> hookFunction, uint8 effIndex, uint16 auraName, AuraEffectHandleModes mode)
                : BaseHook(owner, hookFunction, effIndex, auraName), _mode(mode)
            {

            }

            void Call(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!_mode.has_value() || _mode.value() & mode)
                    return BaseHook::Call(aurEff, mode);
            }

        private:
            Optional<AuraEffectHandleModes> _mode;
        };
        
        struct TC_GAME_API AuraEffectAbsorbHook : AuraNameHook<HookHandler<void, AuraEffect*, DamageInfo&, uint32&>>
        {
            using BaseHook = AuraNameHook<HookHandler<void, AuraEffect*, DamageInfo&, uint32&>>;

            template <typename T>
            using HookType = typename HookHandler<void, AuraEffect*, DamageInfo&, uint32&>::template HookType<T>;

            template <typename Base, typename Derived = Base>
            AuraEffectAbsorbHook(Derived* owner, HookType<Base> hookFunction, uint8 effIndex)
                : BaseHook(owner, hookFunction, effIndex, SPELL_AURA_SCHOOL_ABSORB)
            {
            }
        };

        using AuraEffectManaShieldHook = AuraEffectAbsorbHook;

        struct TC_GAME_API AuraEffectSplitHook : AuraNameHook<HookHandler<void, AuraEffect*, DamageInfo&, uint32&>>
        {
            using BaseHook = AuraNameHook<HookHandler<void, AuraEffect*, DamageInfo&, uint32&>>;

            template <typename T>
            using HookType = typename HookHandler<void, AuraEffect*, DamageInfo&, uint32&>::template HookType<T>;

            template <typename Base, typename Derived = Base>
            AuraEffectSplitHook(Derived* owner, HookType<Base> hookFunction, uint8 effIndex)
                : BaseHook(owner, hookFunction, effIndex, SPELL_AURA_SPLIT_DAMAGE_PCT)
            {
            }
        };

        using AuraCheckProcHook            = HookHandler<bool, ProcEventInfo&>;
        using AuraEffectCheckProcHook      = AuraNameHook<HookHandler<bool, AuraEffect const*, ProcEventInfo&>>;
        using AuraProcHook                 = HookHandler<void, ProcEventInfo&>;
        using AuraEffectProcHook           = AuraNameHook<HookHandler<void, AuraEffect const*, ProcEventInfo&>>;

    public:
        AuraScript() : _SpellScript(), m_aura(nullptr), m_auraApplication(nullptr), m_defaultActionPrevented(false)
        { }
        bool _Validate(SpellInfo const* entry) override;
        bool _Load(Aura* aura);
        void _PrepareScriptCall(AuraScriptHookType hookType, AuraApplication const* aurApp = nullptr);
        void _FinishScriptCall();
        bool _IsDefaultActionPrevented();
    private:
        Aura* m_aura;
        AuraApplication const* m_auraApplication;
        bool m_defaultActionPrevented;

        class TC_GAME_API ScriptStateStore
        {
        public:
            AuraApplication const* _auraApplication;
            uint8 _currentScriptState;
            bool _defaultActionPrevented;
            ScriptStateStore(uint8 currentScriptState, AuraApplication const* auraApplication, bool defaultActionPrevented)
                : _auraApplication(auraApplication), _currentScriptState(currentScriptState), _defaultActionPrevented(defaultActionPrevented)
            { }
        };
        typedef std::stack<ScriptStateStore> ScriptStateStack;
        ScriptStateStack m_scriptStates;

    public:
        //
        // AuraScript interface
        // hooks to which you can attach your functions
        //

        // executed when area aura checks if it can be applied on target
        // example: OnEffectApply.Register(this, &class::function);
        // where function is: bool function (Unit* target);
        HookList<AuraCheckAreaTargetHook> DoCheckAreaTarget;

        // executed when aura is dispelled by a unit
        // example: OnDispel.Register(this, &class::function);
        // where function is: void function (DispelInfo* dispelInfo);
        HookList<AuraDispelHook> OnDispel;
        // executed after aura is dispelled by a unit
        // example: AfterDispel.Register(this, &class::function);
        // where function is: void function (DispelInfo* dispelInfo);
        HookList<AuraDispelHook> AfterDispel;

        // executed when aura effect is applied with specified mode to target
        // should be used when when effect handler preventing/replacing is needed, do not use this hook for triggering spellcasts/removing auras etc - may be unsafe
        // example: OnEffectApply.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, AuraEffectHandleModes mode);
        HookList<AuraEffectApplyHook> OnEffectApply;
        // executed after aura effect is applied with specified mode to target
        // example: AfterEffectApply.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, AuraEffectHandleModes mode);
        HookList<AuraEffectApplyHook> AfterEffectApply;

        // executed after aura effect is removed with specified mode from target
        // should be used when when effect handler preventing/replacing is needed, do not use this hook for triggering spellcasts/removing auras etc - may be unsafe
        // example: OnEffectRemove.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // example: OnEffectRemove.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier, AuraEffectHandleModes);
        // where function is: void function (AuraEffect const* aurEff, AuraEffectHandleModes mode);
        HookList<AuraEffectApplyHook> OnEffectRemove;
        // executed when aura effect is removed with specified mode from target
        // example: AfterEffectRemove.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, AuraEffectHandleModes mode);
        HookList<AuraEffectApplyHook> AfterEffectRemove;

        // executed when periodic aura effect ticks on target
        // example: OnEffectPeriodic.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff);
        HookList<AuraEffectPeriodicHook> OnEffectPeriodic;

        // executed when periodic aura effect is updated
        // example: OnEffectUpdatePeriodic.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect* aurEff);
        HookList<AuraEffectUpdatePeriodicHook> OnEffectUpdatePeriodic;

        // executed when aura effect calculates amount
        // example: DoEffectCalcAmount.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect* aurEff, int32& amount, bool& canBeRecalculated);
        HookList<AuraEffectCalcAmountHook> DoEffectCalcAmount;

        // executed when aura effect calculates periodic data
        // example: DoEffectCalcPeriodic.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, bool& isPeriodic, int32& amplitude);
        HookList<AuraEffectCalcPeriodicHook> DoEffectCalcPeriodic;

        // executed when aura effect calculates spellmod
        // example: DoEffectCalcSpellMod.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, SpellModifier*& spellMod);
        HookList<AuraEffectCalcSpellModHook> DoEffectCalcSpellMod;

        // executed when absorb aura effect is going to reduce damage
        // example: OnEffectAbsorb.Register(this, &class::function, EffectIndexSpecifier);
        // where function is: void function (AuraEffect const* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount);
        HookList<AuraEffectAbsorbHook> OnEffectAbsorb;

        // executed after absorb aura effect reduced damage to target - absorbAmount is real amount absorbed by aura
        // example: AfterEffectAbsorb.Register(this, &class::function, EffectIndexSpecifier);
        // where function is: void function (AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount);
        HookList<AuraEffectAbsorbHook> AfterEffectAbsorb;

        // executed when mana shield aura effect is going to reduce damage
        // example: OnEffectManaShield.Register(this, &class::function, EffectIndexSpecifier);
        // where function is: void function (AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount);
        HookList<AuraEffectManaShieldHook> OnEffectManaShield;

        // executed after mana shield aura effect reduced damage to target - absorbAmount is real amount absorbed by aura
        // example: AfterEffectManaShield.Register(this, &class::function, EffectIndexSpecifier);
        // where function is: void function (AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount);
        HookList<AuraEffectManaShieldHook> AfterEffectManaShield;

        // executed when the caster of some spell with split dmg aura gets damaged through it
        // example: OnEffectSplit.Register(this, &class::function, EffectIndexSpecifier);
        // where function is: void function (AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& splitAmount);
        HookList<AuraEffectSplitHook> OnEffectSplit;

        // executed when aura checks if it can proc
        // example: DoCheckProc.Register(this, &class::function);
        // where function is: bool function (ProcEventInfo& eventInfo);
        HookList<AuraCheckProcHook> DoCheckProc;

        // executed when aura effect checks if it can proc the aura
        // example: DoCheckEffectProc.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is bool function (AuraEffect const* aurEff, ProcEventInfo& eventInfo);
        HookList<AuraEffectCheckProcHook> DoCheckEffectProc;

        // executed before aura procs (possibility to prevent charge drop/cooldown)
        // example: DoPrepareProc.Register(this, &class::function);
        // where function is: void function (ProcEventInfo& eventInfo);
        HookList<AuraProcHook> DoPrepareProc;
        // executed when aura procs
        // example: OnProc.Register(this, &class::function);
        // where function is: void function (ProcEventInfo& eventInfo);
        HookList<AuraProcHook> OnProc;
        // executed after aura proced
        // example: AfterProc.Register(this, &class::function);
        // where function is: void function (ProcEventInfo& eventInfo);
        HookList<AuraProcHook> AfterProc;

        // executed when aura effect procs
        // example: OnEffectProc.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, ProcEventInfo& procInfo);
        HookList<AuraEffectProcHook> OnEffectProc;
        // executed after aura effect proced
        // example: AfterEffectProc.Register(this, &class::function, EffectIndexSpecifier, EffectAuraNameSpecifier);
        // where function is: void function (AuraEffect const* aurEff, ProcEventInfo& procInfo);
        HookList<AuraEffectProcHook> AfterEffectProc;

        // AuraScript interface - hook/effect execution manipulators

        // prevents default action of a hook from being executed (works only while called in a hook which default action can be prevented)
        void PreventDefaultAction();

        // AuraScript interface - functions which are redirecting to Aura class

        // returns proto of the spell
        SpellInfo const* GetSpellInfo() const;
        // returns spellid of the spell
        uint32 GetId() const;

        // returns guid of object which cast the aura (m_originalCaster of the Spell class)
        ObjectGuid GetCasterGUID() const;
        // returns unit which cast the aura or nullptr if not avalible (caster logged out for example)
        Unit* GetCaster() const;
        // returns object on which aura was cast, target for non-area auras, area aura source for area auras
        WorldObject* GetOwner() const;
        // returns owner if it's unit or unit derived object, nullptr otherwise (only for persistent area auras nullptr is returned)
        Unit* GetUnitOwner() const;
        // returns owner if it's dynobj, nullptr otherwise
        DynamicObject* GetDynobjOwner() const;

        // removes aura with remove mode (see AuraRemoveFlags enum)
        void Remove(AuraRemoveFlags removeMode = AuraRemoveFlags::ByDefault);
        // returns aura object of script
        Aura* GetAura() const;

        // returns type of the aura, may be dynobj owned aura or unit owned aura
        AuraObjectType GetType() const;

        // aura duration manipulation - when duration goes to 0 aura is removed
        int32 GetDuration() const;
        void SetDuration(int32 duration, bool withMods = false);
        // sets duration to maxduration
        void RefreshDuration();
        time_t GetApplyTime() const;
        int32 GetMaxDuration() const;
        void SetMaxDuration(int32 duration);
        int32 CalcMaxDuration() const;
        // expired - duration just went to 0
        bool IsExpired() const;
        // permament - has infinite duration
        bool IsPermanent() const;

        // charges manipulation - 0 - not charged aura
        uint8 GetCharges() const;
        void SetCharges(uint8 charges);
        uint8 CalcMaxCharges() const;
        bool ModCharges(int8 num, AuraRemoveFlags removeMode = AuraRemoveFlags::ByDefault);
        // returns true if last charge dropped
        bool DropCharge(AuraRemoveFlags removeMode = AuraRemoveFlags::ByDefault);

        // stack amount manipulation
        uint8 GetStackAmount() const;
        void SetStackAmount(uint8 num);
        bool ModStackAmount(int32 num, AuraRemoveFlags removeMode = AuraRemoveFlags::ByDefault);

        // passive - "working in background", not saved, not removed by immunities, not seen by player
        bool IsPassive() const;
        // death persistent - not removed on death
        bool IsDeathPersistent() const;

        // check if aura has effect of given effindex
        bool HasEffect(uint8 effIndex) const;
        // returns aura effect of given effect index or nullptr
        AuraEffect* GetEffect(uint8 effIndex) const;

        // check if aura has effect of given aura type
        bool HasEffectType(AuraType type) const;

        // AuraScript interface - functions which are redirecting to AuraApplication class
        // Do not call these in hooks in which AuraApplication is not avalible, otherwise result will differ from expected (the functions will return nullptr)

        // returns currently processed target of an aura
        // Return value does not need to be nullptr-checked, the only situation this will (always)
        // return nullptr is when the call happens in an unsupported hook, in other cases, it is always valid
        Unit* GetTarget() const;
        // returns AuraApplication object of currently processed target
        AuraApplication const* GetTargetApplication() const;
};

//
// definitions:
//
// EffectIndexSpecifier - specifies conditions for effects
// EFFECT_0 - first effect matches
// EFFECT_1 - second effect matches
// EFFECT_2 - third effect matches
// EFFECT_FIRST_FOUND - first effect matching other conditions matches
// EFFECT_ALL - all effects of spell match
//
// EffectNameSpecifier - specifies conditions for spell effect names
// SPELL_EFFECT_ANY - any effect but not 0 matches condition
// SPELL_EFFECT_XXX - one of values of enum SpellEffects - effect with equal name matches
//

#endif // __SPELL_SCRIPT_H
