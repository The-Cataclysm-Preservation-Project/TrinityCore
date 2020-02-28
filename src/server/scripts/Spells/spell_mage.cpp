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

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

#include "ScriptMgr.h"
#include "GridNotifiers.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"

enum MageSpells
{
    SPELL_MAGE_ARCANE_POTENCY_RANK_1             = 31571,
    SPELL_MAGE_ARCANE_POTENCY_RANK_2             = 31572,
    SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_1     = 57529,
    SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_2     = 57531,
    SPELL_MAGE_ARCANE_BLAST                      = 30451,
    SPELL_MAGE_ARCANE_MISSILES                   = 5143,
    SPELL_MAGE_ARCANE_MISSILES_DAMAGE            = 7268,
    SPELL_MAGE_ARCANE_MISSILES_AURASTATE         = 79808,
    SPELL_MAGE_BLAZING_SPEED                     = 31643,
    SPELL_MAGE_BRAIN_FREEZE_R1                   = 44546,
    SPELL_MAGE_BURNOUT                           = 29077,
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_COMBUSTION_DAMAGE                 = 83853,
    SPELL_MAGE_DEEP_FREEZE_DAMAGE                = 71757,
    SPELL_MAGE_EARLY_FROST_R1                    = 83049,
    SPELL_MAGE_EARLY_FROST_R2                    = 83050,
    SPELL_MAGE_EARLY_FROST_TRIGGERED_R1          = 83162,
    SPELL_MAGE_EARLY_FROST_TRIGGERED_R2          = 83239,
    SPELL_MAGE_EARLY_FROST_VISUAL                = 94315,
    SPELL_MAGE_FIREBALL                          = 133,
    SPELL_MAGE_FIRE_BLAST                        = 2136,
    SPELL_MAGE_FOCUS_MAGIC_PROC                  = 54648,
    SPELL_MAGE_FROST_NOVA                        = 122,
    SPELL_MAGE_FROST_WARDING_R1                  = 11189,
    SPELL_MAGE_FROST_WARDING_TRIGGERED           = 57776,
    SPELL_MAGE_FLAME_ORB_DUMMY                   = 82731,
    SPELL_MAGE_FLAME_ORB_SUMMON                  = 84765,
    SPELL_MAGE_FLAME_ORB_AOE                     = 82734,
    SPELL_MAGE_FLAME_ORB_BEAM_DUMMY              = 86719,
    SPELL_MAGE_FLAME_ORB_DAMAGE                  = 82739,
    SPELL_MAGE_FLAME_ORB_SELF_SNARE              = 82736,
    SPELL_MAGE_FROSTFIRE_BOLT                    = 44614,
    SPELL_MAGE_FROSTFIRE_ORB_DUMMY               = 92283,
    SPELL_MAGE_FROSTFIRE_ORB_SUMMON              = 84714,
    SPELL_MAGE_FROSTFIRE_ORB_AOE                 = 84718,
    SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R1           = 95969,
    SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R2           = 84721,
    SPELL_MAGE_FROSTFIRE_ORB_RANK_R2             = 84727,
    SPELL_MAGE_FROSTFIRE_BOLT_CHILL_EFFECT       = 44614,
    SPELL_MAGE_HOT_STREAK                        = 44445,
    SPELL_MAGE_HOT_STREAK_TRIGGERED              = 48108,
    SPELL_MAGE_IMPROVED_HOT_STREAK_R1            = 44446,
    SPELL_MAGE_IMPROVED_HOT_STREAK_R2            = 44448,
    SPELL_MAGE_IMPROVED_POLYMORPH_RANK_1         = 11210,
    SPELL_MAGE_IMPROVED_POLYMORPH_STUN_RANK_1    = 83046,
    SPELL_MAGE_IMPROVED_POLYMORPH_MARKER         = 87515,
    SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED    = 44413,
    SPELL_MAGE_INCANTERS_ABSORBTION_KNOCKBACK    = 86261,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE       = 29077,
    SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FIRE       = 88092,
    SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_ARCANE     = 88091,
    SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FROST      = 58832,
    SPELL_MAGE_PERMAFROST                        = 91394,
    SPELL_MAGE_PYROBLAST                         = 11366,
    SPELL_MAGE_PYROMANIAC_TRIGGERED              = 83582,
    SPELL_MAGE_SCORCH                            = 2948,
    SPELL_MAGE_SLOW                              = 31589,
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820,
    SPELL_MAGE_GLYPH_OF_ETERNAL_WATER            = 70937,
    SPELL_MAGE_SHATTERED_BARRIER                 = 55080,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT  = 70908,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY  = 70907,
    SPELL_MAGE_GLYPH_OF_BLAST_WAVE               = 62126,

    SPELL_MAGE_FLAMESTRIKE                       = 2120,

    SPELL_MAGE_CHILLED_R1                        = 12484,
    SPELL_MAGE_CHILLED_R2                        = 12485,

    SPELL_MAGE_CONE_OF_COLD_AURA_R1              = 11190,
    SPELL_MAGE_CONE_OF_COLD_AURA_R2              = 12489,
    SPELL_MAGE_CONE_OF_COLD_TRIGGER_R1           = 83301,
    SPELL_MAGE_CONE_OF_COLD_TRIGGER_R2           = 83302,

    SPELL_MAGE_SHATTERED_BARRIER_R1              = 44745,
    SPELL_MAGE_SHATTERED_BARRIER_R2              = 54787,
    SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R1       = 55080,
    SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R2       = 83073,

    SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED       = 83098,

    SPELL_MAGE_RING_OF_FROST_SUMMON              = 82676,
    SPELL_MAGE_RING_OF_FROST_FREEZE              = 82691,
    SPELL_MAGE_RING_OF_FROST_DUMMY               = 91264,

    SPELL_MAGE_FINGERS_OF_FROST                  = 44544,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT             = 80354,
};

enum MageIcons
{
    ICON_MAGE_SHATTER                            = 976,
    ICON_MAGE_IMPROVED_FLAMESTRIKE               = 37,
    ICON_MAGE_IMPROVED_FREEZE                    = 94,
    ICON_MAGE_INCANTER_S_ABSORPTION              = 2941,
    ICON_MAGE_IMPROVED_MANA_GEM                  = 1036,
    ICON_MAGE_EARLY_FROST_SKILL                  = 189,
    ICON_MAGE_EARLY_FROST                        = 2114,
    ICON_MAGE_GLYPH_OF_MIRROR_IMAGE              = 331,
    ICON_MAGE_LIVING_BOMB                        = 3000,
    ICON_MAGE_GLYPH_OF_FROSTFIRE                 = 2946

};

enum MiscSpells
{
    SPELL_HUNTER_INSANITY                        = 95809,
    SPELL_PRIEST_SHADOW_WORD_DEATH               = 32409,
    SPELL_SHAMAN_EXHAUSTION                      = 57723,
    SPELL_SHAMAN_SATED                           = 57724
};

// -31571 - Arcane Potency
class spell_mage_arcane_potency : public AuraScript
{
    PrepareAuraScript(spell_mage_arcane_potency);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MAGE_ARCANE_POTENCY_RANK_1,
                SPELL_MAGE_ARCANE_POTENCY_RANK_2,
                SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_1,
                SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_2
            });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        uint32 spellId = 0;

        if (GetSpellInfo()->Id == SPELL_MAGE_ARCANE_POTENCY_RANK_1)
            spellId = SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_1;
        else if (GetSpellInfo()->Id == SPELL_MAGE_ARCANE_POTENCY_RANK_2)
            spellId = SPELL_MAGE_ARCANE_POTENCY_TRIGGER_RANK_2;
        if (!spellId)
            return;

        GetTarget()->CastSpell(GetTarget(), spellId, true, nullptr, aurEff);

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_arcane_potency::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

enum MageSpellIcons
{
    SPELL_ICON_MAGE_SHATTERED_BARRIER = 2945
};

// 11113 - Blast Wave
class spell_mage_blast_wave : public SpellScriptLoader
{
    public:
        spell_mage_blast_wave() : SpellScriptLoader("spell_mage_blast_wave") { }

        class spell_mage_blast_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blast_wave_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_FLAMESTRIKE });
            }

            void CountTargets(std::list<WorldObject*>& targetList)
            {
                _targetCount = targetList.size();
            }

            void HandleImprovedFlamestrike()
            {
                if (_targetCount >= 2)
                    if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_FLAMESTRIKE, EFFECT_0))
                        if (roll_chance_i(aurEff->GetAmount()))
                        {
                            float x, y, z;
                            WorldLocation const* loc = GetExplTargetDest();
                            if (!loc)
                                return;

                            loc->GetPosition(x, y, z);
                            GetCaster()->CastSpell(x, y, z, SPELL_MAGE_FLAMESTRIKE, true);
                        }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_blast_wave_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                AfterCast += SpellCastFn(spell_mage_blast_wave_SpellScript::HandleImprovedFlamestrike);
            }

            uint32 _targetCount = 0;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_blast_wave_SpellScript();
        }
};

// -31641 - Blazing Speed
class spell_mage_blazing_speed : public SpellScriptLoader
{
    public:
        spell_mage_blazing_speed() : SpellScriptLoader("spell_mage_blazing_speed") { }

        class spell_mage_blazing_speed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_blazing_speed_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_BLAZING_SPEED });
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_BLAZING_SPEED, true, nullptr, aurEff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_blazing_speed_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_blazing_speed_AuraScript();
        }
};

// 42208 - Blizzard
/// Updated 4.3.4
class spell_mage_blizzard : public SpellScriptLoader
{
   public:
       spell_mage_blizzard() : SpellScriptLoader("spell_mage_blizzard") { }

       class spell_mage_blizzard_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_blizzard_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/) override
           {
               return ValidateSpellInfo(
                {
                    SPELL_MAGE_CHILLED_R1,
                    SPELL_MAGE_CHILLED_R2
                });
           }

           void AddChillEffect(SpellEffIndex /*effIndex*/)
           {
               Unit* caster = GetCaster();
               if (Unit* unitTarget = GetHitUnit())
               {
                   if (caster->IsScriptOverriden(GetSpellInfo(), 836))
                       caster->CastSpell(unitTarget, SPELL_MAGE_CHILLED_R1, true);
                   else if (caster->IsScriptOverriden(GetSpellInfo(), 988))
                       caster->CastSpell(unitTarget, SPELL_MAGE_CHILLED_R2, true);
               }
           }

           void Register() override
           {
               OnEffectHitTarget += SpellEffectFn(spell_mage_blizzard_SpellScript::AddChillEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
           }
       };

       SpellScript* GetSpellScript() const override
       {
           return new spell_mage_blizzard_SpellScript();
       }
};

// 11958 - Cold Snap
class spell_mage_cold_snap : public SpellScriptLoader
{
    public:
        spell_mage_cold_snap() : SpellScriptLoader("spell_mage_cold_snap") { }

        class spell_mage_cold_snap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cold_snap_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->GetSpellHistory()->ResetCooldowns([](SpellHistory::CooldownStorageType::iterator itr) -> bool
                {
                    SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
                    return spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && (spellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_FROST) &&
                        spellInfo->Id != SPELL_MAGE_COLD_SNAP && spellInfo->GetRecoveryTime() > 0;
                }, true);
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_mage_cold_snap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_cold_snap_SpellScript();
        }
};

// 120 - Cone of Cold
/// Updated 4.3.4
class spell_mage_cone_of_cold : public SpellScriptLoader
{
    public:
        spell_mage_cone_of_cold() : SpellScriptLoader("spell_mage_cone_of_cold") { }

        class spell_mage_cone_of_cold_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cone_of_cold_SpellScript);

            void HandleConeOfColdScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    if (caster->HasAura(SPELL_MAGE_CONE_OF_COLD_AURA_R1)) // Improved Cone of Cold Rank 1
                        unitTarget->CastSpell(unitTarget, SPELL_MAGE_CONE_OF_COLD_TRIGGER_R1, true);
                    else if (caster->HasAura(SPELL_MAGE_CONE_OF_COLD_AURA_R2)) // Improved Cone of Cold Rank 2
                        unitTarget->CastSpell(unitTarget, SPELL_MAGE_CONE_OF_COLD_TRIGGER_R2, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_cone_of_cold_SpellScript::HandleConeOfColdScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_cone_of_cold_SpellScript();
        }
};

// 42955 Conjure Refreshment
/// Updated 4.3.4
struct ConjureRefreshmentData
{
    uint32 minLevel;
    uint32 maxLevel;
    uint32 spellId;
};

ConjureRefreshmentData const _conjureData[] =
{
    { 33, 43, 92739 },
    { 44, 53, 92799 },
    { 54, 63, 92802 },
    { 64, 73, 92805 },
    { 74, 79, 74625 },
    { 80, 84, 92822 },
    { 85, 85, 92727 }
};
uint8 const MAX_CONJURE_REFRESHMENT_SPELLS = std::extent<decltype(_conjureData)>::value;

// 42955 - Conjure Refreshment
class spell_mage_conjure_refreshment : public SpellScriptLoader
{
    public:
        spell_mage_conjure_refreshment() : SpellScriptLoader("spell_mage_conjure_refreshment") { }

        class spell_mage_conjure_refreshment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_conjure_refreshment_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
                    if (!ValidateSpellInfo({ _conjureData[i].spellId }))
                        return false;
                return true;
            }

            bool Load() override
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint8 level = GetHitUnit()->getLevel();
                for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
                {
                    ConjureRefreshmentData const& spellData = _conjureData[i];
                    if (level < spellData.minLevel || level > spellData.maxLevel)
                        continue;
                    GetHitUnit()->CastSpell(GetHitUnit(), spellData.spellId);
                    break;
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_conjure_refreshment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_conjure_refreshment_SpellScript();
        }
};

// -31661 - Dragon's Breath
class spell_mage_dragon_breath : public AuraScript
{
    PrepareAuraScript(spell_mage_dragon_breath);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // Dont proc with Living Bomb explosion
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (spellInfo && spellInfo->SpellIconID == ICON_MAGE_LIVING_BOMB && spellInfo->SpellFamilyName == SPELLFAMILY_MAGE)
            return false;
        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_dragon_breath::CheckProc);
    }
};

// 543  - Fire War
class spell_mage_fire_frost_ward : public SpellScriptLoader
{
    public:
        spell_mage_fire_frost_ward() : SpellScriptLoader("spell_mage_fire_frost_ward") { }

        class spell_mage_fire_frost_ward_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_fire_frost_ward_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_FROST_WARDING_TRIGGERED, SPELL_MAGE_FROST_WARDING_R1 });
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                canBeRecalculated = false;
                if (Unit* caster = GetCaster())
                {
                    // +80.68% from sp bonus
                    float bonus = 0.8068f;

                    bonus *= caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask());

                    amount += int32(bonus);
                }
            }

            void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_FROST_WARDING_R1, EFFECT_0))
                {
                    int32 chance = talentAurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue(); // SPELL_EFFECT_DUMMY with NO_TARGET

                    if (roll_chance_i(chance))
                    {
                        int32 bp = dmgInfo.GetDamage();
                        dmgInfo.AbsorbDamage(bp);
                        target->CastCustomSpell(target, SPELL_MAGE_FROST_WARDING_TRIGGERED, &bp, nullptr, nullptr, true, nullptr, aurEff);
                        absorbAmount = 0;
                        PreventDefaultAction();
                    }
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_fire_frost_ward_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_fire_frost_ward_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_fire_frost_ward_AuraScript();
        }
};

// 54646 - Focus Magic
class spell_mage_focus_magic : public SpellScriptLoader
{
    public:
        spell_mage_focus_magic() : SpellScriptLoader("spell_mage_focus_magic") { }

        class spell_mage_focus_magic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_focus_magic_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_FOCUS_MAGIC_PROC });
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                _procTarget = GetCaster();
                return _procTarget && _procTarget->IsAlive();
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(_procTarget, SPELL_MAGE_FOCUS_MAGIC_PROC, true, nullptr, aurEff);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_focus_magic_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_focus_magic_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            }

            Unit* _procTarget = nullptr;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_focus_magic_AuraScript();
        }
};

// 116 - Frostbolt
/// Updated 4.3.4
class spell_mage_frostbolt : public SpellScript
{
    PrepareSpellScript(spell_mage_frostbolt);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                 SPELL_MAGE_EARLY_FROST_R1,
                 SPELL_MAGE_EARLY_FROST_R2,
                 SPELL_MAGE_EARLY_FROST_TRIGGERED_R1,
                 SPELL_MAGE_EARLY_FROST_TRIGGERED_R2,
                 SPELL_MAGE_EARLY_FROST_VISUAL
            });
    }

    bool Load() override
    {
        _earlyFrostActive = false;
        _earlyFrostSpellId = 0;

        Unit* caster = GetCaster();
        if (!caster)
            return false;

        // Check Early Frost state
        if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_MAGE, ICON_MAGE_EARLY_FROST_SKILL, 0))
        {
            // Check if the cooldown effect is inactive
            if (!caster->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_MAGE, ICON_MAGE_EARLY_FROST, EFFECT_0))
            {
                _earlyFrostActive = true;
                if (aurEff->GetId() == SPELL_MAGE_EARLY_FROST_R1)
                    _earlyFrostSpellId = SPELL_MAGE_EARLY_FROST_TRIGGERED_R1;
                else if (aurEff->GetId() == SPELL_MAGE_EARLY_FROST_R2)
                    _earlyFrostSpellId = SPELL_MAGE_EARLY_FROST_TRIGGERED_R2;
            }
        }

        return true;
    }

    void HandleEarlyFrost()
    {
        if (Unit* caster = GetCaster())
        {
            if (_earlyFrostActive)
            {
                caster->CastSpell(caster, _earlyFrostSpellId, true);
                caster->RemoveAurasDueToSpell(SPELL_MAGE_EARLY_FROST_VISUAL);
            }
        }
    }

    void RecalculateDamage(SpellEffIndex /*effIndex*/)
    {
        if (GetHitUnit() && GetHitUnit()->HasAuraState(AURA_STATE_FROZEN, GetSpellInfo(), GetCaster()))
        {
            if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_SHATTER, EFFECT_1))
            {
                int32 damage = GetHitDamage();
                AddPct(damage, aurEff->GetAmount());
                SetHitDamage(damage);
            }
        }
    }

    void Register() override
    {
        OnSpellStart += SpellCastFn(spell_mage_frostbolt::HandleEarlyFrost);
        OnEffectHitTarget += SpellEffectFn(spell_mage_frostbolt::RecalculateDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    bool _earlyFrostActive;
    uint32 _earlyFrostSpellId;
};

// 56372 - Glyph of Ice Block
class spell_mage_glyph_of_ice_block : public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_ice_block() : SpellScriptLoader("spell_mage_glyph_of_ice_block") { }

        class spell_mage_glyph_of_ice_block_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_ice_block_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_FROST_NOVA });
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                return GetTarget()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                // Remove Frost Nova cooldown
                GetTarget()->ToPlayer()->GetSpellHistory()->ResetCooldown(SPELL_MAGE_FROST_NOVA, true);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_glyph_of_ice_block_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_glyph_of_ice_block_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_glyph_of_ice_block_AuraScript();
        }
};

// 56374 - Glyph of Icy Veins
class spell_mage_glyph_of_icy_veins : public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_icy_veins() : SpellScriptLoader("spell_mage_glyph_of_icy_veins") { }

        class spell_mage_glyph_of_icy_veins_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_icy_veins_AuraScript);

            void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();

                GetTarget()->RemoveAurasByType(SPELL_AURA_HASTE_SPELLS, ObjectGuid::Empty, 0, true, false);
                GetTarget()->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_glyph_of_icy_veins_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_glyph_of_icy_veins_AuraScript();
        }
};

// 56375 - Glyph of Polymorph
class spell_mage_glyph_of_polymorph : public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_polymorph() : SpellScriptLoader("spell_mage_glyph_of_polymorph") { }

        class spell_mage_glyph_of_polymorph_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_polymorph_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_PRIEST_SHADOW_WORD_DEATH });
            }

            void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* target = eventInfo.GetProcTarget();

                target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE, ObjectGuid::Empty, target->GetAura(SPELL_PRIEST_SHADOW_WORD_DEATH)); // SW:D shall not be removed.
                target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_glyph_of_polymorph_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_glyph_of_polymorph_AuraScript();
        }
};

// 44457 - Living Bomb
class spell_mage_living_bomb : public AuraScript
{
    PrepareAuraScript(spell_mage_living_bomb);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return ValidateSpellInfo({ uint32(spellInfo->Effects[EFFECT_1].CalcValue()) });
    }

    void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
        if (removeMode != AURA_REMOVE_BY_ENEMY_SPELL && removeMode != AURA_REMOVE_BY_EXPIRE)
            return;

        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget(), uint32(aurEff->GetAmount()), true, nullptr, aurEff);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 11426 - Ice Barrier
/// Updated 4.3.4
class spell_mage_ice_barrier : public SpellScriptLoader
{
   public:
       spell_mage_ice_barrier() : SpellScriptLoader("spell_mage_ice_barrier") { }

       class spell_mage_ice_barrier_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_ice_barrier_AuraScript);

           void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
           {
               canBeRecalculated = false;
               if (Unit* caster = GetCaster())
                   amount += int32(0.87f * caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask()));
           }

           void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
           {
               if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                   return;

               if (GetTarget()->HasAura(SPELL_MAGE_SHATTERED_BARRIER_R1))
                   GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R1, true);
               else if (GetTarget()->HasAura(SPELL_MAGE_SHATTERED_BARRIER_R2))
                   GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R2, true);
           }

           void Register() override
           {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_ice_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ice_barrier_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ice_barrier_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
           }
       };

       AuraScript* GetAuraScript() const override
       {
           return new spell_mage_ice_barrier_AuraScript();
       }
};

// -11119 - Ignite
class spell_mage_ignite : public AuraScript
{
    PrepareAuraScript(spell_mage_ignite);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MAGE_IGNITE,
                SPELL_MAGE_FLAME_ORB_DAMAGE,
                SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R1,
                SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R2
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo() || !eventInfo.GetProcTarget())
            return false;

        switch (eventInfo.GetSpellInfo()->Id)
        {
            case SPELL_MAGE_FLAME_ORB_DAMAGE:
            case SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R1:
            case SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R2:
                return false;
        }

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* target = GetTarget();
        DamageInfo* damage = eventInfo.GetDamageInfo();

        int32 bp = CalculatePct(damage->GetDamage(), aurEff->GetAmount()) * 0.5f;

        if (AuraEffect const* ignite = eventInfo.GetProcTarget()->GetAuraEffect(SPELL_MAGE_IGNITE, EFFECT_0, target->GetGUID()))
            if (!ignite->GetTickNumber())
                bp += ignite->GetAmount();

        target->CastCustomSpell(SPELL_MAGE_IGNITE, SPELLVALUE_BASE_POINT0, bp, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_ignite::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_ignite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};
// 543 - Mage Ward
/// Updated 4.3.4
class spell_mage_mage_ward : public SpellScriptLoader
{
   public:
       spell_mage_mage_ward() : SpellScriptLoader("spell_mage_mage_ward") { }

       class spell_mage_mage_ward_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_mage_ward_AuraScript);

           void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
           {
               canBeRecalculated = false;
               if (Unit* caster = GetCaster())
               {
                   // 80,7% of the spellpower as bonus
                   float bonus = 0.807f * caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask());
                   amount += int32(bonus);
               }
           }

           void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
           {
               if (AuraEffect const* aurEff = GetTarget()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, ICON_MAGE_INCANTER_S_ABSORPTION, EFFECT_0))
               {
                   int32 bp = CalculatePct(absorbAmount, aurEff->GetAmount());
                   GetTarget()->CastCustomSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, nullptr, nullptr, true);
               }
           }

            void Register() override
           {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_mage_ward_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_mage_ward_AuraScript::HandleAbsorb, EFFECT_0);
           }
       };

        AuraScript* GetAuraScript() const override
       {
           return new spell_mage_mage_ward_AuraScript();
       }
};

// 1463 - Mana Shield
/// Updated 4.3.4
class spell_mage_mana_shield : public SpellScriptLoader
{
    public:
       spell_mage_mana_shield() : SpellScriptLoader("spell_mage_mana_shield") { }

       class spell_mage_mana_shield_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_mana_shield_AuraScript);

           bool Validate(SpellInfo const* /*spellInfo*/) override
           {
               return ValidateSpellInfo(
               {
                   SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED,
                   SPELL_MAGE_INCANTERS_ABSORBTION_KNOCKBACK
               });
           }

           void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
           {
               canBeRecalculated = false;
               if (Unit* caster = GetCaster())
               {
                   // 87% of the spellpower as bonus
                   float bonus = 0.807f * caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask());
                   amount += int32(bonus);
               }
           }

           void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
           {
               if (AuraEffect const* aurEff = GetTarget()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, ICON_MAGE_INCANTER_S_ABSORPTION, EFFECT_0))
               {
                   int32 bp = CalculatePct(absorbAmount, aurEff->GetAmount());
                   GetTarget()->CastCustomSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, nullptr, nullptr, true);
               }
           }

           void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
           {
               if (GetTarget()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, ICON_MAGE_INCANTER_S_ABSORPTION, EFFECT_0))
                   if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                       GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_KNOCKBACK, true);
           }

           void Register() override
           {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_mana_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MANA_SHIELD);
                AfterEffectManaShield += AuraEffectManaShieldFn(spell_mage_mana_shield_AuraScript::HandleAbsorb, EFFECT_0);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_mana_shield_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MANA_SHIELD, AURA_EFFECT_HANDLE_REAL);
           }
       };

        AuraScript* GetAuraScript() const override
       {
           return new spell_mage_mana_shield_AuraScript();
       }
};

// -29074 - Master of Elements
class spell_mage_master_of_elements : public AuraScript
{
    PrepareAuraScript(spell_mage_master_of_elements);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo()->GetSpellInfo() != nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        SpellInfo const* spell = eventInfo.GetSpellInfo();
        int32 mana = int32(spell->CalcPowerCost(GetTarget(), spell->GetSchoolMask()));
        mana = CalculatePct(mana, aurEff->GetAmount());

        if (mana)
            GetTarget()->CastCustomSpell(SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_master_of_elements::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_master_of_elements::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -86181 - Nether Vortex
class spell_mage_nether_vortex : public AuraScript
{
    PrepareAuraScript(spell_mage_nether_vortex);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_SLOW });
    }

    bool DoCheck(ProcEventInfo& eventInfo)
    {
        if (Aura* aura = eventInfo.GetProcTarget()->GetAura(SPELL_MAGE_SLOW))
            if (aura->GetCasterGUID() != GetTarget()->GetGUID())
                return false;

        return true;
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_SLOW, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_nether_vortex::DoCheck);
        OnEffectProc += AuraEffectProcFn(spell_mage_nether_vortex::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -11175 - Permafrost
class spell_mage_permafrost : public SpellScriptLoader
{
    public:
        spell_mage_permafrost() : SpellScriptLoader("spell_mage_permafrost") { }

        class spell_mage_permafrost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_permafrost_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_PERMAFROST });
            }

            bool DoCheck(ProcEventInfo& eventInfo)
            {
                return GetTarget()->GetGuardianPet() && eventInfo.GetDamageInfo()->GetDamage();
            }

            void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                int32 heal = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
                GetTarget()->CastCustomSpell(SPELL_MAGE_PERMAFROST, SPELLVALUE_BASE_POINT0, heal, (Unit*)nullptr, true, nullptr, aurEff);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_permafrost_AuraScript::DoCheck);
                OnEffectProc += AuraEffectProcFn(spell_mage_permafrost_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_permafrost_AuraScript();
        }
};

// 118 - Polymorph
class spell_mage_polymorph : public SpellScriptLoader
{
    public:
        spell_mage_polymorph() : SpellScriptLoader("spell_mage_polymorph") { }

        class spell_mage_polymorph_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_polymorph_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_MAGE_IMPROVED_POLYMORPH_RANK_1,
                    SPELL_MAGE_IMPROVED_POLYMORPH_STUN_RANK_1,
                    SPELL_MAGE_IMPROVED_POLYMORPH_MARKER
                });
            }

            bool Load() override
            {
                _caster = nullptr;
                return true;
            }

            bool DoCheck(ProcEventInfo& eventInfo)
            {
                _caster = GetCaster();
                return _caster && eventInfo.GetDamageInfo();
            }

            void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                // Improved Polymorph
                if (AuraEffect const* improvedPolymorph = _caster->GetAuraEffectOfRankedSpell(SPELL_MAGE_IMPROVED_POLYMORPH_RANK_1, EFFECT_0))
                {
                    if (_caster->HasAura(SPELL_MAGE_IMPROVED_POLYMORPH_MARKER))
                        return;

                    GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_MAGE_IMPROVED_POLYMORPH_STUN_RANK_1, improvedPolymorph->GetSpellInfo()->GetRank()), true, nullptr, aurEff);
                    _caster->CastSpell(_caster, SPELL_MAGE_IMPROVED_POLYMORPH_MARKER, true, nullptr, aurEff);
                }
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_polymorph_AuraScript::DoCheck);
                OnEffectProc += AuraEffectProcFn(spell_mage_polymorph_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_MOD_CONFUSE);
            }

        private:
            Unit* _caster = nullptr;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_polymorph_AuraScript();
        }
};

enum SilvermoonPolymorph
{
    NPC_AUROSALIA       = 18744
};

/// @todo move out of here and rename - not a mage spell
// 32826 - Polymorph (Visual)
class spell_mage_polymorph_cast_visual : public SpellScriptLoader
{
    public:
        spell_mage_polymorph_cast_visual() : SpellScriptLoader("spell_mage_polymorph_visual") { }

        class spell_mage_polymorph_cast_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_polymorph_cast_visual_SpellScript);

            static const uint32 PolymorhForms[6];

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                // check if spell ids exist in dbc
                return ValidateSpellInfo(PolymorhForms);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetCaster()->FindNearestCreature(NPC_AUROSALIA, 30.0f))
                    if (target->GetTypeId() == TYPEID_UNIT)
                        target->CastSpell(target, PolymorhForms[urand(0, 5)], true);
            }

            void Register() override
            {
                // add dummy effect spell handler to Polymorph visual
                OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_cast_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_polymorph_cast_visual_SpellScript();
        }
};

uint32 const spell_mage_polymorph_cast_visual::spell_mage_polymorph_cast_visual_SpellScript::PolymorhForms[6] =
{
    SPELL_MAGE_SQUIRREL_FORM,
    SPELL_MAGE_GIRAFFE_FORM,
    SPELL_MAGE_SERPENT_FORM,
    SPELL_MAGE_DRAGONHAWK_FORM,
    SPELL_MAGE_WORGEN_FORM,
    SPELL_MAGE_SHEEP_FORM
};

// 5405  - Replenish Mana (Mana Gem)
/// Updated 4.3.4
class spell_mage_replenish_mana : public SpellScriptLoader
{
   public:
       spell_mage_replenish_mana() : SpellScriptLoader("spell_mage_replenish_mana") { }

       class spell_mage_replenish_mana_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_replenish_mana_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/) override
           {
               return ValidateSpellInfo({ SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED });
           }

           void HandleImprovedManaGem()
           {
               if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_MANA_GEM, EFFECT_0))
               {
                   int32 bp = CalculatePct(GetCaster()->GetMaxPower(POWER_MANA), aurEff->GetAmount());
                   GetCaster()->CastCustomSpell(GetCaster(), SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED, &bp, &bp, nullptr, true);
               }
           }

           void Register() override
           {
               AfterCast += SpellCastFn(spell_mage_replenish_mana_SpellScript::HandleImprovedManaGem);
           }
       };

       SpellScript* GetSpellScript() const override
       {
           return new spell_mage_replenish_mana_SpellScript();
       }
};

// 82676 - Ring of Frost
/// Updated 4.3.4
class spell_mage_ring_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost() : SpellScriptLoader("spell_mage_ring_of_frost") { }

        class spell_mage_ring_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_MAGE_RING_OF_FROST_SUMMON,
                    SPELL_MAGE_RING_OF_FROST_FREEZE,
                    SPELL_MAGE_RING_OF_FROST_DUMMY
                });
            }

            bool Load() override
            {
                ringOfFrost = nullptr;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (ringOfFrost)
                    if (GetMaxDuration() - (int32)ringOfFrost->GetTimer() >= sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY)->GetDuration())
                        GetTarget()->CastSpell(ringOfFrost->GetPositionX(), ringOfFrost->GetPositionY(), ringOfFrost->GetPositionZ(), SPELL_MAGE_RING_OF_FROST_FREEZE, true);
            }

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                std::list<Creature*> MinionList;
                GetTarget()->GetAllMinionsByEntry(MinionList, GetSpellInfo()->Effects[EFFECT_0].MiscValue);

                // Get the last summoned RoF, save it and despawn older ones
                for (std::list<Creature*>::iterator itr = MinionList.begin(); itr != MinionList.end(); itr++)
                {
                    TempSummon* summon = (*itr)->ToTempSummon();

                    if (ringOfFrost && summon)
                    {
                        if (summon->GetTimer() > ringOfFrost->GetTimer())
                        {
                            ringOfFrost->DespawnOrUnsummon();
                            ringOfFrost = summon;
                        }
                        else
                            summon->DespawnOrUnsummon();
                    }
                    else if (summon)
                        ringOfFrost = summon;
                }
            }

            TempSummon* ringOfFrost;

            void Register() override
            {
                 OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                 OnEffectApply += AuraEffectApplyFn(spell_mage_ring_of_frost_AuraScript::Apply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_ring_of_frost_AuraScript();
        }
};

// 82691 - Ring of Frost (freeze efect)
/// Updated 4.3.4
class spell_mage_ring_of_frost_freeze : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost_freeze() : SpellScriptLoader("spell_mage_ring_of_frost_freeze") { }

        class spell_mage_ring_of_frost_freeze_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_ring_of_frost_freeze_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_RING_OF_FROST_SUMMON, SPELL_MAGE_RING_OF_FROST_FREEZE });
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                float outRadius = sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON)->Effects[EFFECT_0].CalcRadius();
                float inRadius  = 4.7f;

                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if (Unit* unit = (*itr)->ToUnit())
                        if (unit->HasAura(SPELL_MAGE_RING_OF_FROST_DUMMY) || unit->HasAura(SPELL_MAGE_RING_OF_FROST_FREEZE) || unit->GetExactDist(GetExplTargetDest()) > outRadius || unit->GetExactDist(GetExplTargetDest()) < inRadius)
                            targets.erase(itr--);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_ring_of_frost_freeze_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_ring_of_frost_freeze_SpellScript();
        }

        class spell_mage_ring_of_frost_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_freeze_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_MAGE_RING_OF_FROST_DUMMY });
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    if (GetCaster())
                        GetCaster()->CastSpell(GetTarget(), SPELL_MAGE_RING_OF_FROST_DUMMY, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ring_of_frost_freeze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_ring_of_frost_freeze_AuraScript();
        }
};

// 80353 - Time Warp
class spell_mage_time_warp : public SpellScriptLoader
{
    public:
        spell_mage_time_warp() : SpellScriptLoader("spell_mage_time_warp") { }

        class spell_mage_time_warp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_time_warp_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_MAGE_TEMPORAL_DISPLACEMENT,
                    SPELL_HUNTER_INSANITY,
                    SPELL_SHAMAN_EXHAUSTION,
                    SPELL_SHAMAN_SATED
                });
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SPELL_MAGE_TEMPORAL_DISPLACEMENT, true);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_ALL, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_mage_time_warp_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_time_warp_SpellScript();
        }
};

// 33395 Water Elemental's Freeze
/// Updated 4.3.4
class spell_mage_water_elemental_freeze : public SpellScriptLoader
{
   public:
       spell_mage_water_elemental_freeze() : SpellScriptLoader("spell_mage_water_elemental_freeze") { }

       class spell_mage_water_elemental_freeze_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_water_elemental_freeze_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/) override
           {
               return ValidateSpellInfo({ SPELL_MAGE_FINGERS_OF_FROST });
           }

           void CountTargets(std::list<WorldObject*>& targetList)
           {
               _didHit = !targetList.empty();
           }

           void HandleImprovedFreeze()
           {
               if (!_didHit)
                   return;

               Unit* owner = GetCaster()->GetOwner();
               if (!owner)
                   return;

               if (AuraEffect* aurEff = owner->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_FREEZE, EFFECT_0))
               {
                   if (roll_chance_i(aurEff->GetAmount()))
                       owner->CastCustomSpell(SPELL_MAGE_FINGERS_OF_FROST, SPELLVALUE_AURA_STACK, 2, owner, true);
               }
           }

           void Register() override
           {
               OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_water_elemental_freeze_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
               AfterCast += SpellCastFn(spell_mage_water_elemental_freeze_SpellScript::HandleImprovedFreeze);
           }

       private:
           bool _didHit;
       };

       SpellScript* GetSpellScript() const override
       {
           return new spell_mage_water_elemental_freeze_SpellScript();
       }
};

// 79683 Arcane Missiles!
class spell_mage_arcane_missiles_trigger : public SpellScriptLoader
{
public:
    spell_mage_arcane_missiles_trigger() : SpellScriptLoader("spell_mage_arcane_missiles_trigger") { }

    class spell_mage_arcane_missiles_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_arcane_missiles_trigger_AuraScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_MAGE_ARCANE_MISSILES_AURASTATE });
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_ARCANE_MISSILES_AURASTATE, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetCaster()->RemoveAura(SPELL_MAGE_ARCANE_MISSILES_AURASTATE);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_mage_arcane_missiles_trigger_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            AfterEffectApply += AuraEffectApplyFn(spell_mage_arcane_missiles_trigger_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_missiles_trigger_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_arcane_missiles_trigger_AuraScript();
    }
};

// 79684 Offensive State (DND)
class spell_mage_offensive_state_dnd : public AuraScript
{
    PrepareAuraScript(spell_mage_offensive_state_dnd);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MAGE_ARCANE_MISSILES,
                SPELL_MAGE_ARCANE_MISSILES_DAMAGE,
                SPELL_MAGE_ARCANE_BLAST,
                SPELL_MAGE_HOT_STREAK,
                SPELL_MAGE_BRAIN_FREEZE_R1
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetTarget()->ToPlayer();
        if (!player)
            return false;

        // Don't proc when caster does not know Arcane Missiles
        if (!player->HasSpell(SPELL_MAGE_ARCANE_MISSILES))
            return false;

        // Hot Streak will no longer allow Arcane Missiles to proc
        if (player->HasAura(SPELL_MAGE_HOT_STREAK))
            return false;

        // Brain Freeze will no longer allow Arcane Missiles to proc
        if (player->GetAuraOfRankedSpell(SPELL_MAGE_BRAIN_FREEZE_R1))
            return false;

        // Don't proc Arcane Missiles from triggered Missiles
        if (eventInfo.GetSpellInfo()->Id == SPELL_MAGE_ARCANE_MISSILES_DAMAGE)
            return false;

        return roll_chance_i(40);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_offensive_state_dnd::CheckProc);
    }
};

class spell_mage_early_frost : public SpellScriptLoader
{
public:
    spell_mage_early_frost() : SpellScriptLoader("spell_mage_early_frost") { }

    class spell_mage_early_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_early_frost_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_MAGE_EARLY_FROST_VISUAL });
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_MAGE_EARLY_FROST_VISUAL , true, nullptr, aurEff);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_early_frost_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_early_frost_AuraScript();
    }
};

// 82731 - Flame Orb
class spell_mage_flame_orb: public SpellScriptLoader
{
    public:
        spell_mage_flame_orb() : SpellScriptLoader("spell_mage_flame_orb") { }

        class spell_mage_flame_orb_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_flame_orb_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_MAGE_FLAME_ORB_DUMMY,
                        SPELL_MAGE_FROSTFIRE_ORB_DUMMY,
                        SPELL_MAGE_FLAME_ORB_SUMMON,
                        SPELL_MAGE_FROSTFIRE_ORB_SUMMON
                    });
            }

            bool Load() override
            {
                dummySpellId = GetSpellInfo()->Id;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    switch (dummySpellId)
                    {
                        case SPELL_MAGE_FLAME_ORB_DUMMY:
                            caster->CastSpell(caster, SPELL_MAGE_FLAME_ORB_SUMMON, true);
                            break;
                        case SPELL_MAGE_FROSTFIRE_ORB_DUMMY:
                            caster->CastSpell(caster, SPELL_MAGE_FROSTFIRE_ORB_SUMMON, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_flame_orb_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        private:
            uint32 dummySpellId;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_flame_orb_SpellScript();
        }
};

// 82734 - Flame Orb dummy AOE
class spell_mage_flame_orb_aoe_dummy: public SpellScriptLoader
{
    public:
        spell_mage_flame_orb_aoe_dummy() : SpellScriptLoader("spell_mage_flame_orb_aoe_dummy") { }

        class spell_mage_flame_orb_aoe_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_flame_orb_aoe_dummy_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_MAGE_FLAME_ORB_AOE,
                        SPELL_MAGE_FROSTFIRE_ORB_AOE,
                        SPELL_MAGE_FLAME_ORB_BEAM_DUMMY,
                        SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R1,
                        SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R2,
                        SPELL_MAGE_FLAME_ORB_DAMAGE,
                        SPELL_MAGE_FLAME_ORB_SELF_SNARE,
                        SPELL_MAGE_FROSTFIRE_ORB_RANK_R2,
                    });
            }

            bool Load() override
            {
                dummySpellId = GetSpellInfo()->Id;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster(), true));
                targets.resize(1);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (TempSummon* caster = GetCaster()->ToTempSummon())
                    if (Unit* summoner = caster->GetSummoner())
                        if (Unit* target = GetHitUnit())
                        {
                            switch (dummySpellId)
                            {
                                case SPELL_MAGE_FLAME_ORB_AOE:
                                    caster->CastSpell(caster, SPELL_MAGE_FLAME_ORB_SELF_SNARE, true);
                                    caster->CastSpell(target, SPELL_MAGE_FLAME_ORB_BEAM_DUMMY, true);
                                    summoner->CastSpell(target, SPELL_MAGE_FLAME_ORB_DAMAGE, true);
                                    break;
                                case SPELL_MAGE_FROSTFIRE_ORB_AOE:
                                    caster->CastSpell(caster, SPELL_MAGE_FLAME_ORB_SELF_SNARE, true);
                                    caster->CastSpell(target, SPELL_MAGE_FLAME_ORB_BEAM_DUMMY, true);

                                    if (summoner->HasAura(SPELL_MAGE_FROSTFIRE_ORB_RANK_R2))
                                        summoner->CastSpell(target, SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R2, true);
                                    else
                                        summoner->CastSpell(target, SPELL_MAGE_FROSTFIRE_ORB_DAMAGE_R1, true);
                                    break;
                                default:
                                    break;
                            }
                        }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_flame_orb_aoe_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_mage_flame_orb_aoe_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        private:
            uint32 dummySpellId;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_flame_orb_aoe_dummy_SpellScript();
        }
};

// 44445 - Hot Streak
class spell_mage_hot_streak : public SpellScriptLoader
{
    public:
        spell_mage_hot_streak() : SpellScriptLoader("spell_mage_hot_streak") { }

        class spell_mage_hot_streak_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_hot_streak_AuraScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_MAGE_HOT_STREAK_TRIGGERED,
                        SPELL_MAGE_IMPROVED_HOT_STREAK_R1,
                        SPELL_MAGE_IMPROVED_HOT_STREAK_R2,
                        SPELL_MAGE_FIREBALL,
                        SPELL_MAGE_PYROBLAST,
                        SPELL_MAGE_FIRE_BLAST,
                        SPELL_MAGE_FROSTFIRE_BOLT,
                        SPELL_MAGE_SCORCH
                    });
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                if (Unit* caster = GetCaster())
                    if (roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                        && !caster->HasAura(SPELL_MAGE_IMPROVED_HOT_STREAK_R1)
                        && !caster->HasAura(SPELL_MAGE_IMPROVED_HOT_STREAK_R2))
                        switch (eventInfo.GetSpellInfo()->Id)
                        {
                            case SPELL_MAGE_FIREBALL:
                            case SPELL_MAGE_PYROBLAST:
                            case SPELL_MAGE_FIRE_BLAST:
                            case SPELL_MAGE_FROSTFIRE_BOLT:
                            case SPELL_MAGE_SCORCH:
                                return true;
                            default:
                                return false;
                        }

                return false;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_HOT_STREAK_TRIGGERED, true, nullptr, aurEff);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_hot_streak_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_hot_streak_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_hot_streak_AuraScript();
        }
};

// -44446 - Improved Hot Streak
class spell_mage_improved_hot_streak : public SpellScriptLoader
{
    public:
        spell_mage_improved_hot_streak() : SpellScriptLoader("spell_mage_improved_hot_streak") { }

        class spell_mage_improved_hot_streak_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_improved_hot_streak_AuraScript);

            uint8 criticalStrikesCounter;

            bool Load() override
            {
                criticalStrikesCounter = 0;
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_MAGE_HOT_STREAK_TRIGGERED,
                        SPELL_MAGE_FIREBALL,
                        SPELL_MAGE_PYROBLAST,
                        SPELL_MAGE_FIRE_BLAST,
                        SPELL_MAGE_FROSTFIRE_BOLT,
                        SPELL_MAGE_SCORCH
                    });
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                switch (eventInfo.GetSpellInfo()->Id)
                {
                    case SPELL_MAGE_FIREBALL:
                    case SPELL_MAGE_PYROBLAST:
                    case SPELL_MAGE_FIRE_BLAST:
                    case SPELL_MAGE_FROSTFIRE_BOLT:
                    case SPELL_MAGE_SCORCH:
                        if (eventInfo.GetDamageInfo()->GetHitMask() & PROC_HIT_CRITICAL)
                            criticalStrikesCounter++;
                        else
                            criticalStrikesCounter = 0;

                        if (criticalStrikesCounter == 2)
                        {
                            criticalStrikesCounter = 0;
                            return roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                        }
                }
                return false;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_HOT_STREAK_TRIGGERED, true, nullptr, aurEff);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_improved_hot_streak_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_improved_hot_streak_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_improved_hot_streak_AuraScript();
        }
};

// 55342 - Mirror Image
class spell_mage_mirror_image : public SpellScript
{
    PrepareSpellScript(spell_mage_mirror_image);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FIRE,
                SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_ARCANE,
                SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FROST
            });
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetHitPlayer();
        if (!player)
            return;

        uint32 spellId = SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FROST;
        if (player->GetDummyAuraEffect(SPELLFAMILY_MAGE, ICON_MAGE_GLYPH_OF_MIRROR_IMAGE, EFFECT_0))
        {
            if (player->GetPrimaryTalentTree(player->GetActiveSpec()) == TALENT_TREE_MAGE_FIRE)
                spellId = SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_FIRE;
            else if (player->GetPrimaryTalentTree(player->GetActiveSpec()) == TALENT_TREE_MAGE_ARCANE)
                spellId = SPELL_MAGE_MIRROR_IMAGE_TRIGGERED_ARCANE;
        }

        player->CastSpell(player, spellId, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_mirror_image::HandleDummyEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

class spell_mage_mirror_image_AurasScript : public AuraScript
{
    PrepareAuraScript(spell_mage_mirror_image_AurasScript);

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        if (aurEff->GetTickNumber() == 1)
            GetTarget()->CastSpell(GetTarget(), GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_mirror_image_AurasScript::HandleEffectPeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class SummonerCheck
{
    public:
        SummonerCheck(Unit* _summoner) : summoner(_summoner)  { }

        bool operator()(WorldObject* object)
        {
            if (Unit* unit = object->ToUnit())
                if (TempSummon* summon = unit->ToTempSummon())
                    return (summon->GetSummoner() && summon->GetSummoner() != summoner);

            return false;
        }

    private:
        Unit* summoner;
};

class spell_mage_initialize_images : public SpellScript
{
    PrepareSpellScript(spell_mage_initialize_images);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

        targets.remove_if(SummonerCheck(GetCaster()));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_initialize_images::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_mage_deep_freeze : public SpellScript
{
    PrepareSpellScript(spell_mage_deep_freeze);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_DEEP_FREEZE_DAMAGE });
    }

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        if (!target)
            return;

        if (target->GetTypeId() == TYPEID_UNIT)
            if (target->IsImmunedToSpell(GetSpellInfo(), caster))
                caster->CastSpell(target, SPELL_MAGE_DEEP_FREEZE_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_mage_deep_freeze::HandleDamage, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class spell_mage_fingers_of_frost_charges : public AuraScript
{
    PrepareAuraScript(spell_mage_fingers_of_frost_charges);

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetAura()->ModStackAmount(-1);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_fingers_of_frost_charges::HandleProc, EFFECT_0, SPELL_AURA_ABILITY_IGNORE_AURASTATE);
    }
};

// -34293 - Pyromaniac
class spell_mage_pyromaniac : public AuraScript
{
    PrepareAuraScript(spell_mage_pyromaniac);

    bool Load() override
    {
        _buffActive = false;
        return true;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_PYROMANIAC_TRIGGERED });
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 1 * IN_MILLISECONDS;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        _dotTargetGuids.insert(eventInfo.GetProcTarget()->GetGUID());

        // Pyomaniac is active already so there is no need to iterate through potential targets
        if (_buffActive)
            return;

        Unit* target = GetTarget();
        CleanDotTargets(target);

        if (_dotTargetGuids.size() >= 3)
        {
            target->CastCustomSpell(SPELL_MAGE_PYROMANIAC_TRIGGERED, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), target, true, nullptr, aurEff);
            _buffActive = true;
        }
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        // Buff is not active right now so there is no need to check targets
        if (!_buffActive)
            return;

        Unit* target = GetTarget();
        CleanDotTargets(target);

        if (_dotTargetGuids.size() < 3)
        {
            target->RemoveAurasDueToSpell(SPELL_MAGE_PYROMANIAC_TRIGGERED);
            _buffActive = false;
        }
    }

    void Register() override
    {
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_mage_pyromaniac::CalcPeriodic, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_mage_pyromaniac::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_pyromaniac::HandlePeriodic, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    GuidSet _dotTargetGuids;
    bool _buffActive;

    void CleanDotTargets(Unit* caster)
    {
        GuidSet guids = _dotTargetGuids;
        for (ObjectGuid guid : guids)
            if (!IsValidDotTarget(caster, guid))
                _dotTargetGuids.erase(guid);
    }

    bool IsValidDotTarget(Unit* caster, ObjectGuid guid) const
    {
        Unit* target = ObjectAccessor::GetUnit(*caster, guid);
        if (!target)
            return false;

        std::list<AuraEffect*> dotAuraEffects = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
        if (dotAuraEffects.empty())
            return false;

        for (AuraEffect const* effect : dotAuraEffects)
            if (effect->GetCasterGUID() == caster->GetGUID() && effect->GetSpellInfo()->SpellFamilyFlags[2] & 0x00000008)
                return true;

        return true;
    }
};

// -11103 - Impact
class spell_mage_impact : public AuraScript
{
    PrepareAuraScript(spell_mage_impact);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_FIRE_BLAST });
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        GetTarget()->GetSpellHistory()->ResetCooldown(SPELL_MAGE_FIRE_BLAST, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_impact::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 12355 - Impact
class spell_mage_impact_triggered : public SpellScript
{
    PrepareSpellScript(spell_mage_impact_triggered);

    void RegisterFireDots(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        Unit* launchTarget = GetHitUnit();
        if (!target || !caster || target != launchTarget)
            return;

        std::list<AuraEffect*> dotAuraEffects = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
        if (dotAuraEffects.empty())
            return;

        for (AuraEffect const* effect : dotAuraEffects)
            if (effect->GetCasterGUID() == caster->GetGUID() && effect->GetSpellInfo()->SpellFamilyFlags[2] & 0x00000008)
                _fireDotEffects.push_back(effect->GetBase());
    }

    void SpeadFireDots(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (GetExplTargetUnit() == GetHitUnit())
            return;

        for (Aura* aura : _fireDotEffects)
        {
            if (Aura* addAura = caster->AddAura(aura->GetSpellInfo()->Id, GetHitUnit()))
            {
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; i++)
                    if (AuraEffect* originalEffect = aura->GetEffect(i))
                        if (AuraEffect* effect = addAura->GetEffect(i))
                            effect->SetAmount(originalEffect->GetAmount());

                addAura->SetDuration(aura->GetDuration());
            }
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_mage_impact_triggered::RegisterFireDots, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        OnEffectHitTarget += SpellEffectFn(spell_mage_impact_triggered::SpeadFireDots, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
private:
    std::vector<Aura*> _fireDotEffects;
};

// 11129 - Combustion
class spell_mage_combustion : public SpellScript
{
    PrepareSpellScript(spell_mage_combustion);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_COMBUSTION_DAMAGE });
    }

    void HandleDotEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!target || !caster)
            return;

        std::list<AuraEffect*> dotAuraEffects = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
        if (dotAuraEffects.empty())
            return;

        int32 totalAmount = 0;
        for (AuraEffect const* effect : dotAuraEffects)
            if (effect->GetCasterGUID() == caster->GetGUID() && effect->GetSpellInfo()->SpellFamilyFlags[2] & 0x00000008)
                totalAmount += effect->GetAmount();

        if (totalAmount)
            caster->CastCustomSpell(SPELL_MAGE_COMBUSTION_DAMAGE, SPELLVALUE_BASE_POINT0, totalAmount, target, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_combustion::HandleDotEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }

private:
    std::vector<Aura*> _fireDotEffects;
};

// 44614 - Frostfire Bolt
class spell_mage_frostfire_bolt : public SpellScript
{
    PrepareSpellScript(spell_mage_frostfire_bolt);

    void HandleGlyphSlow(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Aura* aura = GetHitAura())
        {
            if (AuraEffect* effect = aura->GetEffect(effIndex))
            {
                if (caster->GetDummyAuraEffect(SPELLFAMILY_MAGE, ICON_MAGE_GLYPH_OF_FROSTFIRE, EFFECT_2))
                    effect->SetAmount(0);
                else
                    effect->SetAmount(GetSpellInfo()->Effects[effIndex].CalcValue());
            }
        }
    }

    void HandleGlyphDot(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            if (!caster->GetDummyAuraEffect(SPELLFAMILY_MAGE, ICON_MAGE_GLYPH_OF_FROSTFIRE, EFFECT_2))
                if (Aura* aura = GetHitAura())
                    if (AuraEffect* effect = aura->GetEffect(effIndex))
                        effect->SetAmount(0);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_frostfire_bolt::HandleGlyphSlow, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        OnEffectHitTarget += SpellEffectFn(spell_mage_frostfire_bolt::HandleGlyphDot, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
    }
};

void AddSC_mage_spell_scripts()
{
    RegisterAuraScript(spell_mage_arcane_potency);
    new spell_mage_blast_wave();
    new spell_mage_blazing_speed();
    new spell_mage_blizzard();
    new spell_mage_cold_snap();
    RegisterSpellScript(spell_mage_combustion);
    new spell_mage_cone_of_cold();
    new spell_mage_conjure_refreshment();
    RegisterSpellScript(spell_mage_deep_freeze);
    RegisterAuraScript(spell_mage_dragon_breath);
    new spell_mage_early_frost();
    new spell_mage_fire_frost_ward();
    new spell_mage_flame_orb();
    new spell_mage_flame_orb_aoe_dummy();
    new spell_mage_focus_magic();
    RegisterAuraScript(spell_mage_fingers_of_frost_charges);
    RegisterSpellScript(spell_mage_frostbolt);
    RegisterSpellScript(spell_mage_frostfire_bolt);
    new spell_mage_hot_streak();
    new spell_mage_ice_barrier();
    RegisterAuraScript(spell_mage_ignite);
    RegisterAuraScript(spell_mage_impact);
    RegisterSpellScript(spell_mage_impact_triggered);
    new spell_mage_improved_hot_streak();
    RegisterSpellScript(spell_mage_initialize_images);
    new spell_mage_glyph_of_ice_block();
    new spell_mage_glyph_of_icy_veins();
    new spell_mage_glyph_of_polymorph();
    RegisterAuraScript(spell_mage_living_bomb);
    new spell_mage_mage_ward();
    new spell_mage_mana_shield();
    RegisterAuraScript(spell_mage_master_of_elements);
    RegisterSpellAndAuraScriptPair(spell_mage_mirror_image, spell_mage_mirror_image_AurasScript);
    RegisterAuraScript(spell_mage_nether_vortex);
    RegisterAuraScript(spell_mage_offensive_state_dnd);
    new spell_mage_permafrost();
    new spell_mage_polymorph();
    new spell_mage_polymorph_cast_visual();
    RegisterAuraScript(spell_mage_pyromaniac);
    new spell_mage_replenish_mana();
    new spell_mage_ring_of_frost();
    new spell_mage_ring_of_frost_freeze();
    new spell_mage_time_warp();
    new spell_mage_water_elemental_freeze();
    new spell_mage_arcane_missiles_trigger();
}
