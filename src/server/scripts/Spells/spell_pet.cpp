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
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Pet.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"

enum HunterPetCalculate
{
    SPELL_TAMED_PET_PASSIVE_09          = 34667,
    SPELL_TAMED_PET_PASSIVE_10          = 34675,
    SPELL_HUNTER_PET_SCALING_01         = 34902,
    SPELL_HUNTER_PET_SCALING_02         = 34903,
    SPELL_HUNTER_PET_SCALING_03         = 34904,
    SPELL_HUNTER_PET_SCALING_04         = 61017,
    SPELL_HUNTER_PET_SCALING_05         = 89446,

    SPELL_HUNTER_PET_CUNNING_MARKER     = 87884,
    SPELL_HUNTER_PET_FEROCITY_MARKER    = 87887,
    SPELL_HUNTER_PET_TENACITY_MARKER    = 87891,
};

enum WarlockPetCalculate
{
    SPELL_PET_PASSIVE_CRIT             = 35695,
    SPELL_PET_PASSIVE_DAMAGE_TAKEN     = 35697,
    SPELL_WARLOCK_PET_SCALING_01       = 34947,
    SPELL_WARLOCK_PET_SCALING_02       = 34956,
    SPELL_WARLOCK_PET_SCALING_03       = 34957,
    SPELL_WARLOCK_PET_SCALING_04       = 34958,
    SPELL_WARLOCK_PET_SCALING_05       = 61013,
    SPELL_WARLOCK_GLYPH_OF_VOIDWALKER  = 56247,
};

enum DKPetCalculate
{
    SPELL_DEATH_KNIGHT_RUNE_WEAPON_02       = 51906,
    SPELL_DEATH_KNIGHT_PET_SCALING_01       = 54566,
    SPELL_DEATH_KNIGHT_PET_SCALING_02       = 51996,
    SPELL_DEATH_KNIGHT_PET_SCALING_03       = 61697,
    SPELL_DEATH_KNIGHT_PET_SCALING_05       = 110474,
    SPELL_NIGHT_OF_THE_DEAD                 = 55620,
    SPELL_DEATH_KNIGHT_GLYPH_OF_GHOUL       = 58686,
    DEATH_KNIGHT_ICON_ID_GLYPH_OF_RAISE_DEAD = 221,
};

enum ShamanPetCalculate
{
    SPELL_FERAL_SPIRIT_PET_UNK_01      = 35674,
    SPELL_FERAL_SPIRIT_PET_UNK_02      = 35675,
    SPELL_FERAL_SPIRIT_PET_UNK_03      = 35676,
    SPELL_FERAL_SPIRIT_PET_SCALING_04  = 61783,
};

enum MiscPetCalculate
{
    SPELL_MAGE_PET_PASSIVE_ELEMENTAL   = 44559,
    SPELL_PET_HEALTH_SCALING           = 61679,
};

enum WarlockSpellIconId
{
    SPELL_ICON_ID_GLYPH_OF_VOID_WALKER  = 217,
};

class spell_warl_pet_scaling_01 : public AuraScript
{
    void CalculateStaminaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float stamina = owner->GetStat(STAT_STAMINA) * 0.6496f;

                float staminaBonus = 0.0f;
                switch (pet->GetEntry())
                {
                    case ENTRY_IMP:
                        staminaBonus = stamina * 8.4f;
                        break;
                    case ENTRY_FELGUARD:
                    case ENTRY_VOIDWALKER:
                        staminaBonus = stamina * 11.0f;
                        break;
                    case ENTRY_SUCCUBUS:
                        staminaBonus = stamina * 9.1f;
                        break;
                    case ENTRY_FELHUNTER:
                        staminaBonus = stamina * 9.5f;
                        break;
                    default:
                        break;
                }

                float glyphBonus = 0.0f;
                // Glyph of Voidwalker
                if (pet->GetEntry() == ENTRY_VOIDWALKER)
                    if (AuraEffect* glyphEffect = owner->GetDummyAuraEffect(SPELLFAMILY_WARLOCK, SPELL_ICON_ID_GLYPH_OF_VOID_WALKER, EFFECT_0))
                        glyphBonus = CalculatePct(pet->GetCreateHealth() + staminaBonus, glyphEffect->GetAmount());

                amount = int32(staminaBonus + glyphBonus);
            }
        }
    }

    void CalculateAttackPowerAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
                int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);

                amount = std::max(fire, shadow);
            }
        }
    }

    void CalculateDamageDoneAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
                int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);

                amount = std::max(fire, shadow) * 0.5f;
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_01::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_MAX_HEALTH);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_01::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_01::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_warl_pet_scaling_02 : public AuraScript
{
    void CalculateIntellectAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float intellect = owner->GetStat(STAT_INTELLECT);
                float manaBonus = 0.0f;
                switch (pet->GetEntry())
                {
                    case ENTRY_IMP:
                        manaBonus = uint32(intellect * 4.9f);
                        break;
                    case ENTRY_FELGUARD:
                    case ENTRY_VOIDWALKER:
                    case ENTRY_SUCCUBUS:
                    case ENTRY_FELHUNTER:
                        manaBonus = uint32(intellect * 11.5f);
                        break;
                    default:
                        break;
                }

                amount = int32(manaBonus);
            }
        }
    }

    void CalculateArmorAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float ownerBonus = 0.0f;
                ownerBonus = CalculatePct(owner->GetArmor(), 35);
                amount = ownerBonus;
            }
        }
    }

    void CalculateFireResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float ownerBonus = 0.0f;
                ownerBonus = CalculatePct(owner->GetResistance(SPELL_SCHOOL_MASK_FIRE), 40);
                amount = ownerBonus;
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_02::CalculateIntellectAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_ENERGY);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_02::CalculateArmorAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_02::CalculateFireResistanceAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
    }
};

class spell_warl_pet_scaling_03 : public AuraScript
{
    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        // Formular: owner resistance of targeted school * 0.4
        canBeRecalculated = true;
        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;

        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_03::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);
    }
};

class spell_warl_pet_scaling_04 : public AuraScript
{
    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        // Formular: owner resistance of targeted school * 0.4
        canBeRecalculated = true;
        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;

        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
    }

    void CalculatePowerRegen(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            // For others recalculate it from:
            float regen = 0.0f;
            // Increase regen from new max power
            regen += pet->GetMaxPower(POWER_MANA) * 0.02;

            amount += int32(regen);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_04::CalculateResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_04::CalculatePowerRegen, EFFECT_1, SPELL_AURA_MOD_POWER_REGEN);
    }
};

class spell_warl_pet_scaling_05 : public AuraScript
{
    void CalculateMeleeHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float hitChance = 0.0f;
                hitChance += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                hitChance += owner->GetRatingBonusValue(CR_HIT_MELEE);
                amount += int32(hitChance);
            }
        }
    }

    void CalculateSpellHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float hitChance = 0.0f;
                hitChance += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                hitChance += owner->GetRatingBonusValue(CR_HIT_SPELL);
                amount += int32(hitChance);
            }
        }
    }

    void CalculateExpertiseBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float expertise = 0.0f;
                expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
                expertise += owner->GetRatingBonusValue(CR_EXPERTISE);
                amount += int32(expertise);
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_05::CalculateMeleeHitChanceBonusAmount, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_05::CalculateSpellHitChanceBonusAmount, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_05::CalculateExpertiseBonusAmount, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
    }
};

class spell_warl_pet_scaling_06 : public AuraScript
{
    void CalculateCritChanceBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE));
    }

    void CalculateMeleeHasteAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                if (float meleeHaste = (1.0f - owner->m_modAttackSpeedPct[BASE_ATTACK]) * 100.0f)
                    amount += int32(meleeHaste);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_06::CalculateMeleeHasteAmount, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT);
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_06::CalculateMeleeHasteAmount, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_warl_pet_passive : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float CritSpell = 0.0f;
            // Crit from Intellect
            CritSpell += owner->GetSpellCritFromIntellect();
            // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            // Increase crit from SPELL_AURA_MOD_CRIT_PCT
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            // Increase crit spell from spell crit ratings
            CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

            amount += CritSpell;
        }
    }

    void CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float CritMelee = 0.0f;
            // Crit from Agility
            CritMelee += owner->GetMeleeCritFromAgility();
            // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
            CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
            // Increase crit from SPELL_AURA_MOD_CRIT_PCT
            CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            // Increase crit melee from melee crit ratings
            CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

            amount += CritMelee;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_passive::CalculateAmountCritSpell, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_passive::CalculateAmountCritMelee, EFFECT_1, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
    }
};

class spell_sha_pet_scaling_04 : public SpellScriptLoader
{
public:
    spell_sha_pet_scaling_04() : SpellScriptLoader("spell_sha_pet_scaling_04") { }

    class spell_sha_pet_scaling_04_AuraScript : public AuraScript
    {
        bool Load() override
        {
            if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = true;
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitMelee = 0.0f;
                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                // Increase hit melee from meele hit ratings
                HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                amount += int32(HitMelee);
            }
        }

        void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = true;
            if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            {
                // For others recalculate it from:
                float HitSpell = 0.0f;
                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                // Increase hit spell from spell hit ratings
                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                amount += int32(HitSpell);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount.Register(&spell_sha_pet_scaling_04_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
            DoEffectCalcAmount.Register(&spell_sha_pet_scaling_04_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_sha_pet_scaling_04_AuraScript();
    }
};

class spell_hun_pet_scaling_01 : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_HUNTER_PET_FEROCITY_MARKER,
                SPELL_HUNTER_PET_TENACITY_MARKER,
                SPELL_HUNTER_PET_CUNNING_MARKER
            });
    }

    void CalculateHealthAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float mod = 0.0f;
                if (pet->HasAura(SPELL_HUNTER_PET_FEROCITY_MARKER))
                    mod = 0.67f;
                else if (pet->HasAura(SPELL_HUNTER_PET_TENACITY_MARKER))
                    mod = 0.78f;
                else if (pet->HasAura(SPELL_HUNTER_PET_CUNNING_MARKER))
                    mod = 0.725f;

                amount = int32(owner->GetHealthBonusFromStamina() * mod);
            }
        }
    }

    void CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = owner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.425f;
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        // Pets deal 12.87% of owner's ranged attack power as spell bonus damage
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.1287f);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_01::CalculateHealthAmount, EFFECT_0, SPELL_AURA_MOD_MAX_HEALTH);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_01::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_01::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_hun_pet_scaling_02 : public AuraScript
{
    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        // Formular: owner resistance of targeted school * 0.4
        canBeRecalculated = true;
        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;

        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_02::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);
    }
};

class spell_hun_pet_scaling_03 : public AuraScript
{
    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        // Formular: owner resistance of targeted school * 0.4
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetResistance(SpellSchoolMask(GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue)) * 0.4);
    }

    void CalculateArmorAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float mod = 0.0f;
                if (pet->HasAura(SPELL_HUNTER_PET_FEROCITY_MARKER))
                    mod = 0.5f;
                else if (pet->HasAura(SPELL_HUNTER_PET_TENACITY_MARKER))
                    mod = 0.6f;
                else if (pet->HasAura(SPELL_HUNTER_PET_CUNNING_MARKER))
                    mod = 0.7f;

                amount = owner->GetArmor() * mod;
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_03::CalculateResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_03::CalculateResistanceAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_03::CalculateArmorAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
    }
};

class spell_hun_pet_scaling_04 : public AuraScript
{
    void CalculateMeleeHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float hitChance = 0.0f;
                hitChance += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                hitChance += owner->GetRatingBonusValue(CR_HIT_MELEE);
                amount += int32(hitChance);
            }
        }
    }

    void CalculateSpellHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float hitChance = 0.0f;
                hitChance += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                hitChance += owner->GetRatingBonusValue(CR_HIT_SPELL);
                amount += int32(hitChance);
            }
        }
    }

    void CalculateExpertiseBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float expertise = 0.0f;
                expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
                expertise += owner->GetRatingBonusValue(CR_EXPERTISE);
                amount += int32(expertise);
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_04::CalculateMeleeHitChanceBonusAmount, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_04::CalculateSpellHitChanceBonusAmount, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_04::CalculateExpertiseBonusAmount, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
    }
};

class spell_hun_pet_scaling_05 : public AuraScript
{
    void CalculateCritChanceBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                amount = uint32(owner->GetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE));
    }

    void CalculateMeleeHasteAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
            if (Player* owner = pet->GetOwner())
                if (float meleeHaste = (1.0f - owner->m_modAttackSpeedPct[BASE_ATTACK]) * 100.0f)
                    amount += int32(meleeHaste);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_05::CalculateMeleeHasteAmount, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT);
        DoEffectCalcAmount.Register(&spell_hun_pet_scaling_05::CalculateMeleeHasteAmount, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_hun_pet_passive_crit : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (!GetCaster() || !GetCaster()->GetOwner())
            return;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float CritSpell = 0.0f;
            // Crit from Intellect
            CritSpell += owner->GetSpellCritFromIntellect();
            // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            // Increase crit from SPELL_AURA_MOD_CRIT_PCT
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            // Increase crit spell from spell crit ratings
            CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

            amount += CritSpell;
        }
    }

    void CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (!GetCaster() || !GetCaster()->GetOwner())
            return;
        if (Player *owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float CritMelee = 0.0f;
            // Crit from Agility
            CritMelee += owner->GetMeleeCritFromAgility();
            // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
            CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
            // Increase crit from SPELL_AURA_MOD_CRIT_PCT
            CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            // Increase crit melee from melee crit ratings
            CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

            amount += CritMelee;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_passive_crit::CalculateAmountCritSpell, EFFECT_1, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_passive_crit::CalculateAmountCritMelee, EFFECT_0, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
    }
};

class spell_dk_avoidance_passive : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAvoidanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Unit* pet = GetUnitOwner())
        {
            if (Unit* owner = pet->GetOwner())
            {
                // Army of the dead ghoul
                if (pet->GetEntry() == ENTRY_ARMY_OF_THE_DEAD_GHOUL)
                    amount = -90;
                // Night of the dead
                else if (Aura* aur = owner->GetAuraOfRankedSpell(SPELL_NIGHT_OF_THE_DEAD))
                    amount = aur->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_avoidance_passive::CalculateAvoidanceAmount, EFFECT_0, SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE);
    }
};

class spell_dk_pet_scaling_01 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateStaminaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            uint8 percentage = 45;

            // Glyph of Raise Dead
            if (AuraEffect const* aurEff = owner->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, DEATH_KNIGHT_ICON_ID_GLYPH_OF_RAISE_DEAD, EFFECT_0))
                percentage += aurEff->GetAmount();

            amount = int32(CalculatePct(owner->GetStat(STAT_STAMINA), percentage));
        }
    }

    void CalculateStrengthAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            uint8 percentage = 70;

            // Glyph of Raise Dead
            if (AuraEffect const* aurEff = owner->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, DEATH_KNIGHT_ICON_ID_GLYPH_OF_RAISE_DEAD, EFFECT_0))
                percentage += aurEff->GetAmount();

            amount = int32(CalculatePct(owner->GetStat(STAT_STAMINA), percentage));
        }
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            float bonusDamage = owner->GetTotalAttackPowerValue(BASE_ATTACK) * 0.11f;
            amount += bonusDamage;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_01::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_01::CalculateStrengthAmount, EFFECT_1, SPELL_AURA_MOD_STAT);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_01::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_dk_pet_scaling_02 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float HasteMelee = 0.0f;
            // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
            HasteMelee += (1 - owner->m_modAttackSpeedPct[BASE_ATTACK]) * 100;

            amount += int32(HasteMelee);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_02::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_dk_pet_scaling_03 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float HitMelee = 0.0f;
            // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
            HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
            // Increase hit melee from meele hit ratings
            HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

            amount += int32(HitMelee);
        }
    }

    void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float HitSpell = 0.0f;
            // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
            HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
            // Increase hit spell from spell hit ratings
            HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

            amount += int32(HitSpell);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_03::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_03::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
    }
};

class spell_dk_pet_scaling_05 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateAmountCritPct(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            float CritSpell = owner->GetMeleeCritFromAgility();
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE);
            CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            CritSpell += owner->GetRatingBonusValue(CR_CRIT_MELEE);
            amount += int32(CritSpell);
        }
    }

    void CalculateAmountResistance(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
            amount += owner->GetInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_05::CalculateAmountCritPct, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_05::CalculateAmountResistance, EFFECT_2, SPELL_AURA_MOD_TARGET_RESISTANCE);
    }
};

class spell_dk_rune_weapon_scaling_02 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Unit* pet = GetUnitOwner())
        {
            Unit* owner = pet->GetOwner();
            if (!owner)
                return;

            amount += owner->CalculateDamage(BASE_ATTACK, true, true);
        }
    }

    void CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (!GetCaster() || !GetCaster()->GetOwner())
            return;
        if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
        {
            // For others recalculate it from:
            float HasteMelee = 0.0f;
            // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
            HasteMelee += (1 - owner->m_modAttackSpeedPct[BASE_ATTACK]) * 100;

            amount += int32(HasteMelee);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_rune_weapon_scaling_02::CalculateDamageDoneAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
        DoEffectCalcAmount.Register(&spell_dk_rune_weapon_scaling_02::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_mage_water_elemental_scaling_01 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float bonusDamage = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST) * 0.4f;
                amount = int32(bonusDamage);
            }
        }
    }

    void CalculateAttackPowerAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float bonusDamage = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST);
                amount = int32(bonusDamage);
            }
        }
    }

    void CalculateHealthAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float stamina = CalculatePct(owner->GetStat(STAT_STAMINA), 30);
                float staminaBonus = stamina * 7.5f;
                float maxHealthBonus = owner->CountPctFromMaxHealth(50);

                amount = int32(staminaBonus + maxHealthBonus);
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_mage_water_elemental_scaling_01::CalculateHealthAmount, EFFECT_0, SPELL_AURA_MOD_MAX_HEALTH);
        DoEffectCalcAmount.Register(&spell_mage_water_elemental_scaling_01::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
        DoEffectCalcAmount.Register(&spell_mage_water_elemental_scaling_01::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_mage_water_elemental_scaling_02 : public AuraScript
{
    bool Load() override
    {
        if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
            return false;
        return true;
    }

    void CalculateManaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Pet* pet = GetUnitOwner()->ToPet())
        {
            if (Player* owner = pet->GetOwner())
            {
                float intellect = CalculatePct(owner->GetStat(STAT_INTELLECT), 30);
                float intellectBonus = intellect * 5.0f;
                float maxManaBonus = owner->CountPctFromMaxPower(POWER_MANA, 50);

                amount = int32(intellectBonus + maxManaBonus);
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_mage_water_elemental_scaling_02::CalculateManaAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_ENERGY);
    }
};

void AddSC_pet_spell_scripts()
{
    RegisterSpellScript(spell_warl_pet_scaling_01);
    RegisterSpellScript(spell_warl_pet_scaling_02);
    RegisterSpellScript(spell_warl_pet_scaling_03);
    RegisterSpellScript(spell_warl_pet_scaling_04);
    RegisterSpellScript(spell_warl_pet_scaling_05);
    RegisterSpellScript(spell_warl_pet_scaling_06);
    RegisterSpellScript(spell_warl_pet_passive);

    RegisterSpellScript(spell_hun_pet_scaling_01);
    RegisterSpellScript(spell_hun_pet_scaling_02);
    RegisterSpellScript(spell_hun_pet_scaling_03);
    RegisterSpellScript(spell_hun_pet_scaling_04);
    RegisterSpellScript(spell_hun_pet_scaling_05);
    RegisterSpellScript(spell_hun_pet_passive_crit);

    RegisterSpellScript(spell_dk_avoidance_passive);
    RegisterSpellScript(spell_dk_pet_scaling_01);
    RegisterSpellScript(spell_dk_pet_scaling_02);
    RegisterSpellScript(spell_dk_pet_scaling_03);
    RegisterSpellScript(spell_dk_pet_scaling_05);

    RegisterSpellScript(spell_dk_rune_weapon_scaling_02);

    RegisterSpellScript(spell_mage_water_elemental_scaling_01);
    RegisterSpellScript(spell_mage_water_elemental_scaling_02);
}
