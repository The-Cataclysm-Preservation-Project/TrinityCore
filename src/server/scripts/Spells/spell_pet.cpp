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
    SPELL_FERAL_SPIRIT_PET_SCALING_01  = 35674, // Serverside spell
    SPELL_FERAL_SPIRIT_PET_SCALING_02  = 35675, // Serverside spell
    SPELL_FERAL_SPIRIT_PET_SCALING_03  = 35676, // Serverside spell
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
    SPELL_ICON_ID_GLYPH_OF_FERAL_SPIRIT = 3081
};

class spell_warl_pet_scaling_01 : public AuraScript
{
    void CalculateStaminaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 healthBonus = summoner->GetMaxHealth() * 0.3f;
        amount = healthBonus;
    }

    void CalculateAttackPowerAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 fire = summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
        int32 shadow = summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);

        amount = std::max(fire, shadow) * 0.15f;
    }

    void CalculateDamageDoneAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 fire = summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
        int32 shadow = summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);

        amount = std::max(fire, shadow) * 0.57f;
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetMaxPower(POWER_MANA) * 0.3f;
    }

    void CalculateArmorAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetArmor();
    }

    void CalculateFireResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetResistance(SPELL_SCHOOL_MASK_FIRE) * 0.4f;
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;
        amount = uint32(summoner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;
        amount = uint32(summoner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_scaling_04::CalculateResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
    }
};

class spell_warl_pet_scaling_05 : public AuraScript
{
    void CalculateMeleeHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        float hitChance = 0.0f;
        hitChance += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
        if (summoner->IsPlayer())
            hitChance += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_MELEE);

        amount += int32(hitChance);
    }

    void CalculateSpellHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        float hitChance = 0.0f;
        hitChance += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        if (summoner->IsPlayer())
            hitChance += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_SPELL);
        amount += int32(hitChance);
    }

    void CalculateExpertiseBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        float expertise = 0.0f;
        expertise += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
        if (summoner->IsPlayer())
            expertise += summoner->ToPlayer()->GetRatingBonusValue(CR_EXPERTISE);
        amount += int32(expertise);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount = uint32(summoner->GetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE));
    }

    void CalculateMeleeHasteAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        if (float meleeHaste = (1.0f - summoner->m_modAttackSpeedPct[BASE_ATTACK]) * 100.0f)
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
    void CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

            // Crit from Intellect
        amount += summoner->ToPlayer()->GetSpellCritFromIntellect();
            // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            // Increase crit from SPELL_AURA_MOD_CRIT_PCT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
            // Increase crit spell from spell crit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_CRIT_SPELL);
    }

    void CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

       // Crit from Agility
        amount += summoner->ToPlayer()->GetMeleeCritFromAgility();
       // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
       // Increase crit from SPELL_AURA_MOD_CRIT_PCT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
       // Increase crit melee from melee crit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_CRIT_MELEE);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_warl_pet_passive::CalculateAmountCritSpell, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_warl_pet_passive::CalculateAmountCritMelee, EFFECT_1, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
    }
};

class spell_sha_pet_scaling_04 : public AuraScript
{
    void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
        // Increase hit melee from meele hit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_MELEE);
    }

    void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        // Increase hit spell from spell hit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_SPELL);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_sha_pet_scaling_04::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_sha_pet_scaling_04::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetMaxHealth() * 0.3f;
    }

    void CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.22f;
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        // Pets deal 12.87% of owner's ranged attack power as spell bonus damage
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = uint32(summoner->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.1287f);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 resistanceSchool = GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue;
        amount = uint32(summoner->GetResistance(SpellSchoolMask(resistanceSchool)) * 0.4);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

         amount = uint32(summoner->GetResistance(SpellSchoolMask(GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue)) * 0.4);
    }

    void CalculateArmorAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = summoner->GetArmor() * 0.7f;

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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_MELEE);
    }

    void CalculateSpellHitChanceBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_SPELL);
    }

    void CalculateExpertiseBonusAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        float expertise = 0.0f;
        expertise += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
        expertise += summoner->ToPlayer()->GetRatingBonusValue(CR_EXPERTISE);
        amount += int32(expertise);
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

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount = uint32(summoner->GetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE));
    }

    void CalculateMeleeHasteAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        if (float meleeHaste = (1.0f - summoner->m_modAttackSpeedPct[BASE_ATTACK]) * 100.0f)
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
    void CalculateAmountCritSpell(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        // Crit from Intellect
        amount += summoner->ToPlayer()->GetSpellCritFromIntellect();
        // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
        // Increase crit from SPELL_AURA_MOD_CRIT_PCT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
        // Increase crit spell from spell crit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_CRIT_SPELL);
    }

    void CalculateAmountCritMelee(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        // Crit from Agility
        amount += summoner->ToPlayer()->GetMeleeCritFromAgility();
        // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
        // Increase crit from SPELL_AURA_MOD_CRIT_PCT
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
        // Increase crit melee from melee crit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_CRIT_MELEE);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_hun_pet_passive_crit::CalculateAmountCritSpell, EFFECT_1, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_hun_pet_passive_crit::CalculateAmountCritMelee, EFFECT_0, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
    }
};

class spell_dk_avoidance_passive : public AuraScript
{
    void CalculateAvoidanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -90;
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_avoidance_passive::CalculateAvoidanceAmount, EFFECT_0, SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE);
    }
};

class spell_dk_pet_scaling_01 : public AuraScript
{
    void CalculateStaminaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 percentage = 45;

        // Glyph of Raise Dead
        if (AuraEffect const* aurEff = summoner->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, DEATH_KNIGHT_ICON_ID_GLYPH_OF_RAISE_DEAD, EFFECT_0))
            percentage += aurEff->GetAmount();

        amount = int32(CalculatePct(summoner->GetStat(STAT_STAMINA), percentage));
    }

    void CalculateStrengthAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        int32 percentage = 70;

        // Glyph of Raise Dead
        if (AuraEffect const* aurEff = summoner->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, DEATH_KNIGHT_ICON_ID_GLYPH_OF_RAISE_DEAD, EFFECT_0))
            percentage += aurEff->GetAmount();

        amount = int32(CalculatePct(summoner->GetStat(STAT_STAMINA), percentage));
    }

    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount += summoner->GetTotalAttackPowerValue(BASE_ATTACK) * 0.11f;
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
    void CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount += int32((1 - summoner->m_modAttackSpeedPct[BASE_ATTACK]) * 100);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_02::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_dk_pet_scaling_03 : public AuraScript
{
    void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
        // Increase hit melee from meele hit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_MELEE);
    }

    void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

            // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
            // Increase hit spell from spell hit ratings
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_HIT_SPELL);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_03::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_03::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
    }
};

class spell_dk_pet_scaling_05 : public AuraScript
{
    void CalculateAmountCritPct(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount += summoner->ToPlayer()->GetMeleeCritFromAgility();
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE);
        amount += summoner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
        amount += summoner->ToPlayer()->GetRatingBonusValue(CR_CRIT_MELEE);
    }

    void CalculateAmountResistance(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner || !summoner->IsPlayer())
            return;

        amount += summoner->GetInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_05::CalculateAmountCritPct, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT);
        DoEffectCalcAmount.Register(&spell_dk_pet_scaling_05::CalculateAmountResistance, EFFECT_2, SPELL_AURA_MOD_TARGET_RESISTANCE);
    }
};

class spell_dk_rune_weapon_scaling_02 : public AuraScript
{
    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount += summoner->CalculateDamage(BASE_ATTACK, true, true);
    }

    void CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
        amount += int32((1 - summoner->m_modAttackSpeedPct[BASE_ATTACK]) * 100);
    }

    void Register() override
    {
        DoEffectCalcAmount.Register(&spell_dk_rune_weapon_scaling_02::CalculateDamageDoneAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
        DoEffectCalcAmount.Register(&spell_dk_rune_weapon_scaling_02::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
    }
};

class spell_mage_water_elemental_scaling_01 : public AuraScript
{
    void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = int32(summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST) * 0.4f);
    }

    void CalculateAttackPowerAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        amount = int32(summoner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST));
    }

    void CalculateHealthAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        float stamina = CalculatePct(summoner->GetStat(STAT_STAMINA), 30);
        float staminaBonus = stamina * 7.5f;
        float maxHealthBonus = summoner->CountPctFromMaxHealth(50);

        amount = int32(staminaBonus + maxHealthBonus);
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
    void CalculateManaAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* summoner = GetUnitOwner()->GetCreator();
        if (!summoner)
            return;

        float intellect = CalculatePct(summoner->GetStat(STAT_INTELLECT), 30);
        float intellectBonus = intellect * 5.0f;
        float maxManaBonus = summoner->CountPctFromMaxPower(POWER_MANA, 50);

        amount = int32(intellectBonus + maxManaBonus);
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

    RegisterSpellScript(spell_sha_pet_scaling_04);

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
