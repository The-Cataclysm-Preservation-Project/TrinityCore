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

#include "SpellAuraEffects.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "CellImpl.h"
#include "Common.h"
#include "DBCStores.h"
#include "GridNotifiersImpl.h"
#include "Item.h"
#include "Log.h"
#include "LootMgr.h"
#include "MiscPackets.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "OutdoorPvPMgr.h"
#include "Pet.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "ThreatManager.h"
#include "Unit.h"
#include "Util.h"
#include "Vehicle.h"
#include "Weather.h"
#include "WorldPacket.h"
#include <G3D/g3dmath.h>
#include <numeric>

class Aura;
//
// EFFECT HANDLER NOTES
//
// in aura handler there should be check for modes:
// AURA_EFFECT_HANDLE_REAL set
// AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK set
// AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK set - aura is recalculated or is just applied/removed - need to redo all things related to m_amount
// AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK - logical or of above conditions
// AURA_EFFECT_HANDLE_STAT - set when stats are reapplied
// such checks will speedup trinity change amount/send for client operations
// because for change amount operation packets will not be send
// aura effect handlers shouldn't contain any AuraEffect or Aura object modifications

pAuraEffectHandler AuraEffectHandler[TOTAL_AURAS]=
{
    &AuraEffect::HandleNULL,                                      //  0 SPELL_AURA_NONE
    &AuraEffect::HandleBindSight,                                 //  1 SPELL_AURA_BIND_SIGHT
    &AuraEffect::HandleModPossess,                                //  2 SPELL_AURA_MOD_POSSESS
    &AuraEffect::HandleNoImmediateEffect,                         //  3 SPELL_AURA_PERIODIC_DAMAGE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraDummy,                                 //  4 SPELL_AURA_DUMMY
    &AuraEffect::HandleModConfuse,                                //  5 SPELL_AURA_MOD_CONFUSE
    &AuraEffect::HandleModCharm,                                  //  6 SPELL_AURA_MOD_CHARM
    &AuraEffect::HandleModFear,                                   //  7 SPELL_AURA_MOD_FEAR
    &AuraEffect::HandleNoImmediateEffect,                         //  8 SPELL_AURA_PERIODIC_HEAL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModAttackSpeed,                            //  9 SPELL_AURA_MOD_ATTACKSPEED
    &AuraEffect::HandleModThreat,                                 // 10 SPELL_AURA_MOD_THREAT
    &AuraEffect::HandleModTaunt,                                  // 11 SPELL_AURA_MOD_TAUNT
    &AuraEffect::HandleAuraModStun,                               // 12 SPELL_AURA_MOD_STUN
    &AuraEffect::HandleModDamageDone,                             // 13 SPELL_AURA_MOD_DAMAGE_DONE
    &AuraEffect::HandleNoImmediateEffect,                         // 14 SPELL_AURA_MOD_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         // 15 SPELL_AURA_DAMAGE_SHIELD    implemented in Unit::DoAttackDamage
    &AuraEffect::HandleModStealth,                                // 16 SPELL_AURA_MOD_STEALTH
    &AuraEffect::HandleModStealthDetect,                          // 17 SPELL_AURA_MOD_DETECT
    &AuraEffect::HandleModInvisibility,                           // 18 SPELL_AURA_MOD_INVISIBILITY
    &AuraEffect::HandleModInvisibilityDetect,                     // 19 SPELL_AURA_MOD_INVISIBILITY_DETECT
    &AuraEffect::HandleNoImmediateEffect,                         // 20 SPELL_AURA_OBS_MOD_HEALTH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         // 21 SPELL_AURA_OBS_MOD_POWER implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraModResistance,                         // 22 SPELL_AURA_MOD_RESISTANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 23 SPELL_AURA_PERIODIC_TRIGGER_SPELL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         // 24 SPELL_AURA_PERIODIC_ENERGIZE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraModPacify,                             // 25 SPELL_AURA_MOD_PACIFY
    &AuraEffect::HandleAuraModRoot,                               // 26 SPELL_AURA_MOD_ROOT
    &AuraEffect::HandleAuraModSilence,                            // 27 SPELL_AURA_MOD_SILENCE
    &AuraEffect::HandleNoImmediateEffect,                         // 28 SPELL_AURA_REFLECT_SPELLS        implement in Unit::SpellHitResult
    &AuraEffect::HandleAuraModStat,                               // 29 SPELL_AURA_MOD_STAT
    &AuraEffect::HandleAuraModSkill,                              // 30 SPELL_AURA_MOD_SKILL
    &AuraEffect::HandleAuraModIncreaseSpeed,                      // 31 SPELL_AURA_MOD_INCREASE_SPEED
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               // 32 SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED
    &AuraEffect::HandleAuraModDecreaseSpeed,                      // 33 SPELL_AURA_MOD_DECREASE_SPEED
    &AuraEffect::HandleAuraModIncreaseHealth,                     // 34 SPELL_AURA_MOD_INCREASE_HEALTH
    &AuraEffect::HandleAuraModIncreaseEnergy,                     // 35 SPELL_AURA_MOD_INCREASE_ENERGY
    &AuraEffect::HandleAuraModShapeshift,                         // 36 SPELL_AURA_MOD_SHAPESHIFT
    &AuraEffect::HandleAuraModEffectImmunity,                     // 37 SPELL_AURA_EFFECT_IMMUNITY
    &AuraEffect::HandleAuraModStateImmunity,                      // 38 SPELL_AURA_STATE_IMMUNITY
    &AuraEffect::HandleAuraModSchoolImmunity,                     // 39 SPELL_AURA_SCHOOL_IMMUNITY
    &AuraEffect::HandleAuraModDmgImmunity,                        // 40 SPELL_AURA_DAMAGE_IMMUNITY
    &AuraEffect::HandleAuraModDispelImmunity,                     // 41 SPELL_AURA_DISPEL_IMMUNITY
    &AuraEffect::HandleNoImmediateEffect,                         // 42 SPELL_AURA_PROC_TRIGGER_SPELL  implemented in AuraEffect::HandleProc
    &AuraEffect::HandleNoImmediateEffect,                         // 43 SPELL_AURA_PROC_TRIGGER_DAMAGE implemented in AuraEffect::HandleProc
    &AuraEffect::HandleAuraTrackCreatures,                        // 44 SPELL_AURA_TRACK_CREATURES
    &AuraEffect::HandleAuraTrackResources,                        // 45 SPELL_AURA_TRACK_RESOURCES
    &AuraEffect::HandleNULL,                                      // 46 SPELL_AURA_46 (used in test spells 54054 and 54058, and spell 48050) (3.0.8a)
    &AuraEffect::HandleAuraModParryPercent,                       // 47 SPELL_AURA_MOD_PARRY_PERCENT
    &AuraEffect::HandleNULL,                                      // 48 SPELL_AURA_48 spell Napalm (area damage spell with additional delayed damage effect)
    &AuraEffect::HandleAuraModDodgePercent,                       // 49 SPELL_AURA_MOD_DODGE_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         // 50 SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT implemented in Unit::SpellCriticalHealingBonus
    &AuraEffect::HandleAuraModBlockPercent,                       // 51 SPELL_AURA_MOD_BLOCK_PERCENT
    &AuraEffect::HandleAuraModWeaponCritPercent,                  // 52 SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         // 53 SPELL_AURA_PERIODIC_LEECH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModHitChance,                              // 54 SPELL_AURA_MOD_HIT_CHANCE
    &AuraEffect::HandleModSpellHitChance,                         // 55 SPELL_AURA_MOD_SPELL_HIT_CHANCE
    &AuraEffect::HandleAuraTransform,                             // 56 SPELL_AURA_TRANSFORM
    &AuraEffect::HandleModSpellCritChance,                        // 57 SPELL_AURA_MOD_SPELL_CRIT_CHANCE
    &AuraEffect::HandleAuraModIncreaseSwimSpeed,                  // 58 SPELL_AURA_MOD_INCREASE_SWIM_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         // 59 SPELL_AURA_MOD_DAMAGE_DONE_CREATURE implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleAuraModPacifyAndSilence,                   // 60 SPELL_AURA_MOD_PACIFY_SILENCE
    &AuraEffect::HandleAuraModScale,                              // 61 SPELL_AURA_MOD_SCALE
    &AuraEffect::HandleNoImmediateEffect,                         // 62 SPELL_AURA_PERIODIC_HEALTH_FUNNEL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleUnused,                                    // 63 unused (4.3.4) old SPELL_AURA_PERIODIC_MANA_FUNNEL
    &AuraEffect::HandleNoImmediateEffect,                         // 64 SPELL_AURA_PERIODIC_MANA_LEECH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModCastingSpeed,                           // 65 SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK
    &AuraEffect::HandleFeignDeath,                                // 66 SPELL_AURA_FEIGN_DEATH
    &AuraEffect::HandleAuraModDisarm,                             // 67 SPELL_AURA_MOD_DISARM
    &AuraEffect::HandleAuraModStalked,                            // 68 SPELL_AURA_MOD_STALKED
    &AuraEffect::HandleNoImmediateEffect,                         // 69 SPELL_AURA_SCHOOL_ABSORB implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleUnused,                                    // 70 SPELL_AURA_EXTRA_ATTACKS clientside
    &AuraEffect::HandleModSpellCritChanceShool,                   // 71 SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL
    &AuraEffect::HandleModPowerCostPCT,                           // 72 SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT
    &AuraEffect::HandleModPowerCost,                              // 73 SPELL_AURA_MOD_POWER_COST_SCHOOL
    &AuraEffect::HandleNoImmediateEffect,                         // 74 SPELL_AURA_REFLECT_SPELLS_SCHOOL  implemented in Unit::SpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         // 75 SPELL_AURA_MOD_LANGUAGE
    &AuraEffect::HandleNoImmediateEffect,                         // 76 SPELL_AURA_FAR_SIGHT
    &AuraEffect::HandleModMechanicImmunity,                       // 77 SPELL_AURA_MECHANIC_IMMUNITY
    &AuraEffect::HandleAuraMounted,                               // 78 SPELL_AURA_MOUNTED
    &AuraEffect::HandleModDamagePercentDone,                      // 79 SPELL_AURA_MOD_DAMAGE_PERCENT_DONE
    &AuraEffect::HandleModPercentStat,                            // 80 SPELL_AURA_MOD_PERCENT_STAT
    &AuraEffect::HandleNoImmediateEffect,                         // 81 SPELL_AURA_SPLIT_DAMAGE_PCT implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleWaterBreathing,                            // 82 SPELL_AURA_WATER_BREATHING
    &AuraEffect::HandleModBaseResistance,                         // 83 SPELL_AURA_MOD_BASE_RESISTANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 84 SPELL_AURA_MOD_REGEN implemented in Player::RegenerateHealth
    &AuraEffect::HandleModPowerRegen,                             // 85 SPELL_AURA_MOD_POWER_REGEN implemented in Player::Regenerate
    &AuraEffect::HandleChannelDeathItem,                          // 86 SPELL_AURA_CHANNEL_DEATH_ITEM
    &AuraEffect::HandleNoImmediateEffect,                         // 87 SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         // 88 SPELL_AURA_MOD_HEALTH_REGEN_PERCENT implemented in Player::RegenerateHealth
    &AuraEffect::HandleNoImmediateEffect,                         // 89 SPELL_AURA_PERIODIC_DAMAGE_PERCENT
    &AuraEffect::HandleUnused,                                    // 90 unused (4.3.4) old SPELL_AURA_MOD_RESIST_CHANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 91 SPELL_AURA_MOD_DETECT_RANGE implemented in Creature::GetAttackDistance
    &AuraEffect::HandlePreventFleeing,                            // 92 SPELL_AURA_PREVENTS_FLEEING
    &AuraEffect::HandleModUnattackable,                           // 93 SPELL_AURA_MOD_UNATTACKABLE
    &AuraEffect::HandleNoImmediateEffect,                         // 94 SPELL_AURA_INTERRUPT_REGEN implemented in Player::Regenerate
    &AuraEffect::HandleAuraGhost,                                 // 95 SPELL_AURA_GHOST
    &AuraEffect::HandleNoImmediateEffect,                         // 96 SPELL_AURA_SPELL_MAGNET implemented in Unit::SelectMagnetTarget
    &AuraEffect::HandleNoImmediateEffect,                         // 97 SPELL_AURA_MANA_SHIELD implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleAuraModSkill,                              // 98 SPELL_AURA_MOD_SKILL_TALENT
    &AuraEffect::HandleAuraModAttackPower,                        // 99 SPELL_AURA_MOD_ATTACK_POWER
    &AuraEffect::HandleUnused,                                    //100 SPELL_AURA_AURAS_VISIBLE obsolete? all player can see all auras now, but still have spells including GM-spell
    &AuraEffect::HandleModResistancePercent,                      //101 SPELL_AURA_MOD_RESISTANCE_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //102 SPELL_AURA_MOD_MELEE_ATTACK_POWER_VERSUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModTotalThreat,                        //103 SPELL_AURA_MOD_TOTAL_THREAT
    &AuraEffect::HandleAuraWaterWalk,                             //104 SPELL_AURA_WATER_WALK
    &AuraEffect::HandleAuraFeatherFall,                           //105 SPELL_AURA_FEATHER_FALL
    &AuraEffect::HandleAuraHover,                                 //106 SPELL_AURA_HOVER
    &AuraEffect::HandleNoImmediateEffect,                         //107 SPELL_AURA_ADD_FLAT_MODIFIER implemented in AuraEffect::CalculateSpellMod()
    &AuraEffect::HandleNoImmediateEffect,                         //108 SPELL_AURA_ADD_PCT_MODIFIER implemented in AuraEffect::CalculateSpellMod()
    &AuraEffect::HandleNoImmediateEffect,                         //109 SPELL_AURA_ADD_TARGET_TRIGGER
    &AuraEffect::HandleModPowerRegenPCT,                          //110 SPELL_AURA_MOD_POWER_REGEN_PERCENT implemented in Player::Regenerate, Creature::Regenerate
    &AuraEffect::HandleNoImmediateEffect,                         //111 SPELL_AURA_ADD_CASTER_HIT_TRIGGER implemented in Unit::SelectMagnetTarget
    &AuraEffect::HandleNoImmediateEffect,                         //112 SPELL_AURA_OVERRIDE_CLASS_SCRIPTS
    &AuraEffect::HandleNoImmediateEffect,                         //113 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //114 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //115 SPELL_AURA_MOD_HEALING                 implemented in Unit::SpellBaseHealingBonusForVictim
    &AuraEffect::HandleNoImmediateEffect,                         //116 SPELL_AURA_MOD_REGEN_DURING_COMBAT
    &AuraEffect::HandleNoImmediateEffect,                         //117 SPELL_AURA_MOD_MECHANIC_RESISTANCE     implemented in Unit::MagicSpellHitResult
    &AuraEffect::HandleModHealingPercent,                         //118 SPELL_AURA_MOD_HEALING_PCT
    &AuraEffect::HandleUnused,                                    //119 unused (4.3.4) old SPELL_AURA_SHARE_PET_TRACKING
    &AuraEffect::HandleAuraUntrackable,                           //120 SPELL_AURA_UNTRACKABLE
    &AuraEffect::HandleAuraEmpathy,                               //121 SPELL_AURA_EMPATHY
    &AuraEffect::HandleModOffhandDamagePercent,                   //122 SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT
    &AuraEffect::HandleModTargetResistance,                       //123 SPELL_AURA_MOD_TARGET_RESISTANCE
    &AuraEffect::HandleAuraModRangedAttackPower,                  //124 SPELL_AURA_MOD_RANGED_ATTACK_POWER
    &AuraEffect::HandleNoImmediateEffect,                         //125 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //126 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //127 SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraFixate,                                //128 SPELL_AURA_FIXATE
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //129 SPELL_AURA_MOD_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               //130 SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS
    &AuraEffect::HandleNoImmediateEffect,                         //131 SPELL_AURA_MOD_RANGED_ATTACK_POWER_VERSUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModIncreaseEnergyPercent,              //132 SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT
    &AuraEffect::HandleAuraModIncreaseHealthPercent,              //133 SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT
    &AuraEffect::HandleAuraModRegenInterrupt,                     //134 SPELL_AURA_MOD_MANA_REGEN_INTERRUPT
    &AuraEffect::HandleModHealingDone,                            //135 SPELL_AURA_MOD_HEALING_DONE
    &AuraEffect::HandleModHealingDonePercent,                     //136 SPELL_AURA_MOD_HEALING_DONE_PERCENT 
    &AuraEffect::HandleModTotalPercentStat,                       //137 SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE
    &AuraEffect::HandleModMeleeSpeedPct,                          //138 SPELL_AURA_MOD_MELEE_HASTE
    &AuraEffect::HandleForceReaction,                             //139 SPELL_AURA_FORCE_REACTION
    &AuraEffect::HandleAuraModRangedHaste,                        //140 SPELL_AURA_MOD_RANGED_HASTE
    &AuraEffect::HandleUnused,                                    //141 SPELL_AURA_141
    &AuraEffect::HandleAuraModBaseResistancePCT,                  //142 SPELL_AURA_MOD_BASE_RESISTANCE_PCT
    &AuraEffect::HandleAuraModResistanceExclusive,                //143 SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE
    &AuraEffect::HandleNoImmediateEffect,                         //144 SPELL_AURA_SAFE_FALL                         implemented in WorldSession::HandleMovementOpcodes
    &AuraEffect::HandleAuraModPetTalentsPoints,                   //145 SPELL_AURA_MOD_PET_TALENT_POINTS
    &AuraEffect::HandleNoImmediateEffect,                         //146 SPELL_AURA_ALLOW_TAME_PET_TYPE
    &AuraEffect::HandleModMechanicImmunityMask,                   //147 SPELL_AURA_MECHANIC_IMMUNITY_MASK
    &AuraEffect::HandleAuraRetainComboPoints,                     //148 SPELL_AURA_RETAIN_COMBO_POINTS
    &AuraEffect::HandleNoImmediateEffect,                         //149 SPELL_AURA_REDUCE_PUSHBACK
    &AuraEffect::HandleShieldBlockValuePercent,                   //150 SPELL_AURA_MOD_SHIELD_BLOCKVALUE_PCT
    &AuraEffect::HandleAuraTrackStealthed,                        //151 SPELL_AURA_TRACK_STEALTHED
    &AuraEffect::HandleNoImmediateEffect,                         //152 SPELL_AURA_MOD_DETECTED_RANGE implemented in Creature::GetAttackDistance
    &AuraEffect::HandleUnused,                                    //153 Unused (4.3.4) old SPELL_AURA_SPLIT_DAMAGE_FLAT
    &AuraEffect::HandleModStealthLevel,                           //154 SPELL_AURA_MOD_STEALTH_LEVEL
    &AuraEffect::HandleNoImmediateEffect,                         //155 SPELL_AURA_MOD_WATER_BREATHING
    &AuraEffect::HandleNoImmediateEffect,                         //156 SPELL_AURA_MOD_REPUTATION_GAIN
    &AuraEffect::HandleNULL,                                      //157 SPELL_AURA_PET_DAMAGE_MULTI
    &AuraEffect::HandleShieldBlockValue,                          //158 SPELL_AURA_MOD_SHIELD_BLOCKVALUE
    &AuraEffect::HandleNoImmediateEffect,                         //159 SPELL_AURA_NO_PVP_CREDIT      only for Honorless Target spell
    &AuraEffect::HandleUnused,                                    //160 Unused (4.3.4) old SPELL_AURA_MOD_AOE_AVOIDANCE
    &AuraEffect::HandleNoImmediateEffect,                         //161 SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT
    &AuraEffect::HandleNoImmediateEffect,                         //162 SPELL_AURA_POWER_BURN implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //163 SPELL_AURA_MOD_CRIT_DAMAGE_BONUS
    &AuraEffect::HandleUnused,                                    //164 unused (3.2.0), only one test spell
    &AuraEffect::HandleNoImmediateEffect,                         //165 SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModAttackPowerPercent,                 //166 SPELL_AURA_MOD_ATTACK_POWER_PCT
    &AuraEffect::HandleAuraModRangedAttackPowerPercent,           //167 SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //168 SPELL_AURA_MOD_DAMAGE_DONE_VERSUS            implemented in Unit::SpellDamageBonus, Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //169 SPELL_AURA_MOD_CRIT_PERCENT_VERSUS           implemented in Unit::DealDamageBySchool, Unit::DoAttackDamage, Unit::SpellCriticalBonus
    &AuraEffect::HandleDetectAmore,                               //170 SPELL_AURA_DETECT_AMORE       used to detect various spells that change visual of units for aura target
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //171 SPELL_AURA_MOD_SPEED_NOT_STACK
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               //172 SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK
    &AuraEffect::HandleUnused,                                    //173 unused (4.3.4) no spells, old SPELL_AURA_ALLOW_CHAMPION_SPELLS  only for Proclaim Champion spell
    &AuraEffect::HandleModSpellDamagePercentFromStat,             //174 SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT  implemented in Unit::SpellBaseDamageBonus
    &AuraEffect::HandleModSpellHealingPercentFromStat,            //175 SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT implemented in Unit::SpellBaseHealingBonus
    &AuraEffect::HandleSpiritOfRedemption,                        //176 SPELL_AURA_SPIRIT_OF_REDEMPTION   only for Spirit of Redemption spell, die at aura end
    &AuraEffect::HandleCharmConvert,                              //177 SPELL_AURA_AOE_CHARM
    &AuraEffect::HandleUnused,                                    //178 old SPELL_AURA_MOD_DEBUFF_RESISTANCE unused 4.3.4
    &AuraEffect::HandleNoImmediateEffect,                         //179 SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE implemented in Unit::SpellCriticalBonus
    &AuraEffect::HandleNoImmediateEffect,                         //180 SPELL_AURA_MOD_FLAT_SPELL_DAMAGE_VERSUS   implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleUnused,                                    //181 unused (4.3.4) old SPELL_AURA_MOD_FLAT_SPELL_CRIT_DAMAGE_VERSUS
    &AuraEffect::HandleAuraModResistenceOfStatPercent,            //182 SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT
    &AuraEffect::HandleNULL,                                      //183 SPELL_AURA_MOD_CRITICAL_THREAT only used in 28746 - miscvalue - spell school
    &AuraEffect::HandleNoImmediateEffect,                         //184 SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE  implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //185 SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //186 SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE  implemented in Unit::MagicSpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         //187 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE  implemented in Unit::GetUnitCriticalChance
    &AuraEffect::HandleNoImmediateEffect,                         //188 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE implemented in Unit::GetUnitCriticalChance
    &AuraEffect::HandleModRating,                                 //189 SPELL_AURA_MOD_RATING
    &AuraEffect::HandleNoImmediateEffect,                         //190 SPELL_AURA_MOD_FACTION_REPUTATION_GAIN     implemented in Player::CalculateReputationGain
    &AuraEffect::HandleAuraModUseNormalSpeed,                     //191 SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED
    &AuraEffect::HandleModMeleeRangedSpeedPct,                    //192 SPELL_AURA_MOD_MELEE_RANGED_HASTE
    &AuraEffect::HandleModCombatSpeedPct,                         //193 SPELL_AURA_MELEE_SLOW (in fact combat (any type attack) speed pct)
    &AuraEffect::HandleNoImmediateEffect,                         //194 SPELL_AURA_MOD_TARGET_ABSORB_SCHOOL implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleNoImmediateEffect,                         //195 SPELL_AURA_MOD_TARGET_ABILITY_ABSORB_SCHOOL implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleNULL,                                      //196 SPELL_AURA_MOD_COOLDOWN - flat mod of spell cooldowns
    &AuraEffect::HandleNoImmediateEffect,                         //197 SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE implemented in Unit::SpellCriticalBonus Unit::GetUnitCriticalChance
    &AuraEffect::HandleUnused,                                    //198 unused (4.3.4) old SPELL_AURA_MOD_ALL_WEAPON_SKILLS
    &AuraEffect::HandleUnused,                                    //199 unused (4.3.4) old SPELL_AURA_MOD_INCREASES_SPELL_PCT_TO_HIT
    &AuraEffect::HandleNoImmediateEffect,                         //200 SPELL_AURA_MOD_XP_PCT implemented in Player::RewardPlayerAndGroupAtKill
    &AuraEffect::HandleAuraAllowFlight,                           //201 SPELL_AURA_FLY                             this aura enable flight mode...
    &AuraEffect::HandleNoImmediateEffect,                         //202 SPELL_AURA_CANNOT_BE_DODGED                implemented in Unit::RollPhysicalOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //203 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_DAMAGE  implemented in Unit::CalculateMeleeDamage and Unit::CalculateSpellDamage
    &AuraEffect::HandleNoImmediateEffect,                         //204 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_DAMAGE implemented in Unit::CalculateMeleeDamage and Unit::CalculateSpellDamage
    &AuraEffect::HandleNULL,                                      //205 SPELL_AURA_MOD_SCHOOL_CRIT_DMG_TAKEN
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //206 SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //207 SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //208 SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //209 SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //210 SPELL_AURA_MOD_VEHICLE_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //211 SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK
    &AuraEffect::HandleUnused,                                    //212 Unused (4.3.4) old SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_STAT_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         //213 SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT implemented in Player::RewardRage
    &AuraEffect::HandleNULL,                                      //214 Tamed Pet Passive
    &AuraEffect::HandleArenaPreparation,                          //215 SPELL_AURA_ARENA_PREPARATION
    &AuraEffect::HandleModCastingSpeed,                           //216 SPELL_AURA_HASTE_SPELLS
    &AuraEffect::HandleModMeleeSpeedPct,                          //217 SPELL_AURA_MOD_MELEE_HASTE_2
    &AuraEffect::HandleAuraModRangedHaste,                        //218 SPELL_AURA_HASTE_RANGED
    &AuraEffect::HandleModManaRegen,                              //219 SPELL_AURA_MOD_MANA_REGEN_FROM_STAT
    &AuraEffect::HandleModRatingFromStat,                         //220 SPELL_AURA_MOD_RATING_FROM_STAT
    &AuraEffect::HandleModDetaunt,                                //221 SPELL_AURA_MOD_DETAUNT
    &AuraEffect::HandleUnused,                                    //222 unused (3.2.0) only for spell 44586 that not used in real spell cast
    &AuraEffect::HandleNoImmediateEffect,                         //223 SPELL_AURA_RAID_PROC_FROM_CHARGE
    &AuraEffect::HandleUnused,                                    //224 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //225 SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE
    &AuraEffect::HandleNoImmediateEffect,                         //226 SPELL_AURA_PERIODIC_DUMMY implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //227 SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //228 SPELL_AURA_DETECT_STEALTH stealth detection
    &AuraEffect::HandleNoImmediateEffect,                         //229 SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE
    &AuraEffect::HandleAuraModIncreaseHealth,                     //230 SPELL_AURA_MOD_INCREASE_HEALTH_2
    &AuraEffect::HandleNoImmediateEffect,                         //231 SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE
    &AuraEffect::HandleNoImmediateEffect,                         //232 SPELL_AURA_MECHANIC_DURATION_MOD           implement in Unit::CalculateSpellDuration
    &AuraEffect::HandleUnused,                                    //233 set model id to the one of the creature with id GetMiscValue() - clientside
    &AuraEffect::HandleNoImmediateEffect,                         //234 SPELL_AURA_MECHANIC_DURATION_MOD_NOT_STACK implement in Unit::CalculateSpellDuration
    &AuraEffect::HandleNoImmediateEffect,                         //235 SPELL_AURA_MOD_DISPEL_RESIST               implement in Unit::MagicSpellHitResult
    &AuraEffect::HandleAuraControlVehicle,                        //236 SPELL_AURA_CONTROL_VEHICLE
    &AuraEffect::HandleModSpellDamagePercentFromAttackPower,      //237 SPELL_AURA_MOD_SPELL_DAMAGE_OF_ATTACK_POWER  implemented in Unit::SpellBaseDamageBonus
    &AuraEffect::HandleModSpellHealingPercentFromAttackPower,     //238 SPELL_AURA_MOD_SPELL_HEALING_OF_ATTACK_POWER implemented in Unit::SpellBaseHealingBonus
    &AuraEffect::HandleAuraModScale,                              //239 SPELL_AURA_MOD_SCALE_2 only in Noggenfogger Elixir (16595) before 2.3.0 aura 61
    &AuraEffect::HandleAuraModExpertise,                          //240 SPELL_AURA_MOD_EXPERTISE
    &AuraEffect::HandleForceMoveForward,                          //241 SPELL_AURA_FORCE_MOVE_FORWARD Forces the caster to move forward
    &AuraEffect::HandleNULL,                                      //242 SPELL_AURA_MOD_SPELL_DAMAGE_FROM_HEALING - 2 test spells: 44183 and 44182
    &AuraEffect::HandleAuraModFaction,                            //243 SPELL_AURA_MOD_FACTION
    &AuraEffect::HandleComprehendLanguage,                        //244 SPELL_AURA_COMPREHEND_LANGUAGE
    &AuraEffect::HandleNoImmediateEffect,                         //245 SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL
    &AuraEffect::HandleNoImmediateEffect,                         //246 SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL_NOT_STACK implemented in Spell::EffectApplyAura
    &AuraEffect::HandleAuraCloneCaster,                           //247 SPELL_AURA_CLONE_CASTER
    &AuraEffect::HandleNoImmediateEffect,                         //248 SPELL_AURA_MOD_COMBAT_RESULT_CHANCE         implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleAuraConvertRune,                           //249 SPELL_AURA_CONVERT_RUNE
    &AuraEffect::HandleAuraModIncreaseHealth,                     //250 SPELL_AURA_MOD_INCREASE_HEALTH_2
    &AuraEffect::HandleNoImmediateEffect,                         //251 SPELL_AURA_MOD_ENEMY_DODGE
    &AuraEffect::HandleModCombatSpeedPct,                         //252 SPELL_AURA_252 Is there any difference between this and SPELL_AURA_MELEE_SLOW ? maybe not stacking mod?
    &AuraEffect::HandleNoImmediateEffect,                         //253 SPELL_AURA_MOD_BLOCK_CRIT_CHANCE  implemented in Unit::isBlockCritical
    &AuraEffect::HandleAuraModDisarm,                             //254 SPELL_AURA_MOD_DISARM_OFFHAND
    &AuraEffect::HandleNoImmediateEffect,                         //255 SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT    implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleNoReagentUseAura,                          //256 SPELL_AURA_NO_REAGENT_USE Use SpellClassMask for spell select
    &AuraEffect::HandleNULL,                                      //257 SPELL_AURA_MOD_TARGET_RESIST_BY_SPELL_CLASS Use SpellClassMask for spell select
    &AuraEffect::HandleNULL,                                      //258 SPELL_AURA_MOD_SPELL_VISUAL
    &AuraEffect::HandleUnused,                                    //259 unused (4.3.4) old SPELL_AURA_MOD_HOT_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //260 SPELL_AURA_SCREEN_EFFECT (miscvalue = id in ScreenEffect.dbc) not required any code
    &AuraEffect::HandlePhase,                                     //261 SPELL_AURA_PHASE
    &AuraEffect::HandleNoImmediateEffect,                         //262 SPELL_AURA_ABILITY_IGNORE_AURASTATE implemented in Spell::CheckCast
    &AuraEffect::HandleAuraDisableCastingExceptAbilities,         //263 SPELL_AURA_DISABLE_CASTING_EXCEPT_ABILITIES implemented in Spell::CheckCast
    &AuraEffect::HandleAuraDisableAttackingExceptAbilities,       //264 SPELL_AURA_DISABLE_ATTACKING_EXCEPT_ABILITIES implemented in Spell::CheckCast, Unit::Attack
    &AuraEffect::HandleUnused,                                    //265 unused (4.3.4)
    &AuraEffect::HandleUnused,                                    //266 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //267 SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL         implemented in Unit::IsImmunedToSpellEffect
    &AuraEffect::HandleUnused,                                    //268 unused (4.3.4) old SPELL_AURA_MOD_ATTACK_POWER_OF_STAT_PERCENT.
    &AuraEffect::HandleNoImmediateEffect,                         //269 SPELL_AURA_MOD_IGNORE_TARGET_RESIST implemented in Unit::CalcAbsorbResist and CalcArmorReducedDamage
    &AuraEffect::HandleUnused,                                    //270 unused (4.3.4) old SPELL_AURA_MOD_ABILITY_IGNORE_TARGET_RESIST
    &AuraEffect::HandleNoImmediateEffect,                         //271 SPELL_AURA_MOD_DAMAGE_FROM_CASTER    implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //272 SPELL_AURA_IGNORE_MELEE_RESET
    &AuraEffect::HandleUnused,                                    //273 clientside
    &AuraEffect::HandleUnused,                                    //274 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //275 SPELL_AURA_MOD_IGNORE_SHAPESHIFT Use SpellClassMask for spell select
    &AuraEffect::HandleNULL,                                      //276 mod damage % mechanic?
    &AuraEffect::HandleUnused,                                    //277 unused (4.3.4) old SPELL_AURA_MOD_MAX_AFFECTED_TARGETS
    &AuraEffect::HandleAuraModDisarm,                             //278 SPELL_AURA_MOD_DISARM_RANGED disarm ranged weapon
    &AuraEffect::HandleNoImmediateEffect,                         //279 SPELL_AURA_INITIALIZE_IMAGES
    &AuraEffect::HandleUnused,                                    //280 unused (4.3.4) old SPELL_AURA_MOD_ARMOR_PENETRATION_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //281 SPELL_AURA_MOD_HONOR_GAIN_PCT implemented in Player::RewardHonor
    &AuraEffect::HandleAuraIncreaseBaseHealthPercent,             //282 SPELL_AURA_MOD_BASE_HEALTH_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //283 SPELL_AURA_MOD_HEALING_RECEIVED       implemented in Unit::SpellHealingBonus
    &AuraEffect::HandleAuraLinked,                                //284 SPELL_AURA_LINKED
    &AuraEffect::HandleAuraModAttackPowerOfArmor,                 //285 SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR  implemented in Player::UpdateAttackPowerAndDamage
    &AuraEffect::HandleNoImmediateEffect,                         //286 SPELL_AURA_ABILITY_PERIODIC_CRIT implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //287 SPELL_AURA_DEFLECT_SPELLS             implemented in Unit::MagicSpellHitResult and Unit::MeleeSpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         //288 SPELL_AURA_IGNORE_HIT_DIRECTION  implemented in Unit::MagicSpellHitResult and Unit::MeleeSpellHitResult Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //289 SPELL_AURA_PREVENT_DURABILITY_LOSS implemented in Player::DurabilityPointsLoss
    &AuraEffect::HandleAuraModCritPct,                            //290 SPELL_AURA_MOD_CRIT_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //291 SPELL_AURA_MOD_XP_QUEST_PCT  implemented in Player::RewardQuest
    &AuraEffect::HandleAuraOpenStable,                            //292 SPELL_AURA_OPEN_STABLE
    &AuraEffect::HandleAuraOverrideSpells,                        //293 SPELL_AURA_OVERRIDE_SPELLS auras which probably add set of abilities to their target based on it's miscvalue
    &AuraEffect::HandleNoImmediateEffect,                         //294 SPELL_AURA_PREVENT_REGENERATE_POWER implemented in Player::Regenerate(Powers power)
    &AuraEffect::HandleUnused,                                    //295 unused (4.3.4)
    &AuraEffect::HandleAuraSetVehicle,                            //296 SPELL_AURA_SET_VEHICLE_ID sets vehicle on target
    &AuraEffect::HandleNULL,                                      //297 Spirit Burst spells
    &AuraEffect::HandleNULL,                                      //298 70569 - Strangulating, maybe prevents talk or cast
    &AuraEffect::HandleUnused,                                    //299 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //300 SPELL_AURA_SHARE_DAMAGE_PCT implemented in Unit::DealDamage
    &AuraEffect::HandleNoImmediateEffect,                         //301 SPELL_AURA_SCHOOL_HEAL_ABSORB implemented in Unit::CalcHealAbsorb
    &AuraEffect::HandleUnused,                                    //302 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //303 SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE implemented in Unit::SpellDamageBonus, Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModFakeInebriation,                    //304 SPELL_AURA_MOD_DRUNK
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //305 SPELL_AURA_MOD_MINIMUM_SPEED
    &AuraEffect::HandleUnused,                                    //306 unused (4.3.4)
    &AuraEffect::HandleUnused,                                    //307 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //308 SPELL_AURA_MOD_CRIT_CHANCE_FOR_CASTER implemented in Unit::GetUnitCriticalChance and Unit::GetUnitSpellCriticalChance
    &AuraEffect::HandleUnused,                                    //309 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //310 SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE implemented in Spell::CalculateDamageDone
    &AuraEffect::HandleNULL,                                      //311 0 spells in 3.3.5
    &AuraEffect::HandleNULL,                                      //312 0 spells in 3.3.5
    &AuraEffect::HandleUnused,                                    //313 unused (4.3.4)
    &AuraEffect::HandlePreventResurrection,                       //314 SPELL_AURA_PREVENT_RESURRECTION todo
    &AuraEffect::HandleNoImmediateEffect,                         //315 SPELL_AURA_UNDERWATER_WALKING todo
    &AuraEffect::HandleNoImmediateEffect,                         //316 unused (4.3.4) old SPELL_AURA_PERIODIC_HASTE
    &AuraEffect::HandleAuraModSpellPowerPct,                      //317 SPELL_AURA_MOD_SPELL_POWER_PCT
    &AuraEffect::HandleMastery,                                   //318 SPELL_AURA_MASTERY
    &AuraEffect::HandleModMeleeSpeedPct,                          //319 SPELL_AURA_MOD_MELEE_HASTE_3
    &AuraEffect::HandleAuraModRangedHaste,                        //320 SPELL_AURA_MOD_RANGED_HASTE_2
    &AuraEffect::HandleAuraModNoActions,                          //321 SPELL_AURA_MOD_NO_ACTIONS
    &AuraEffect::HandleNULL,                                      //322 SPELL_AURA_INTERFERE_TARGETTING
    &AuraEffect::HandleUnused,                                    //323 unused (4.3.4)
    &AuraEffect::HandleNULL,                                      //324 SPELL_AURA_324
    &AuraEffect::HandleUnused,                                    //325 unused (4.3.4)
    &AuraEffect::HandlePhaseGroup,                                //326 SPELL_AURA_PHASE_GROUP
    &AuraEffect::HandleUnused,                                    //327 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //328 SPELL_AURA_PROC_ON_POWER_AMOUNT implemented in AuraEffect::HandleProc
    &AuraEffect::HandleNULL,                                      //329 SPELL_AURA_MOD_RUNE_REGEN_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         //330 SPELL_AURA_CAST_WHILE_WALKING
    &AuraEffect::HandleAuraForceWeather,                          //331 SPELL_AURA_FORCE_WEATHER
    &AuraEffect::HandleNoImmediateEffect,                         //332 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS implemented in Unit::GetCastSpellInfo
    &AuraEffect::HandleNoImmediateEffect,                         //333 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_TRIGGERED implemented in Unit::GetCastSpellInfo
    &AuraEffect::HandleNULL,                                      //334 SPELL_AURA_MOD_BLIND
    &AuraEffect::HandleNULL,                                      //335 SPELL_AURA_335
    &AuraEffect::HandleNULL,                                      //336 SPELL_AURA_MOUNT_RESTRICTIONS implemented in Unit::GetMountCapability
    &AuraEffect::HandleNoImmediateEffect,                         //337 SPELL_AURA_MOD_VENDOR_ITEMS_PRICES
    &AuraEffect::HandleNoImmediateEffect,                         //338 SPELL_AURA_MOD_DURABILITY_LOSS
    &AuraEffect::HandleNoImmediateEffect,                         //339 SPELL_AURA_INCREASE_SKILL_GAIN_CHANCE implemented in SkillGainChance in player.cpp
    &AuraEffect::HandleNoImmediateEffect,                         //340 SPELL_AURA_MOD_RESURRECTED_HEALTH_BY_GUILD_MEMBER implemented in Spell::EffectResurrect, Spell::EffectResurrectNew and Spell::EffectResurrectWithAura
    &AuraEffect::HandleModSpellCategoryCooldown,                  //341 SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN
    &AuraEffect::HandleModMeleeRangedSpeedPct,                    //342 SPELL_AURA_MOD_MELEE_RANGED_HASTE_2
    &AuraEffect::HandleNoImmediateEffect,                         //343 SPELL_AURA_MOD_MELEE_DAMAGE_FROM_CASTER implemented in Unit::MeleeDamageBonusTaken
    &AuraEffect::HandleNoImmediateEffect,                         //344 SPELL_AURA_MOD_AUTOATTACK_DAMAGE implemented in Unit::MeleeDamageBonusDone
    &AuraEffect::HandleNoImmediateEffect,                         //345 SPELL_AURA_BYPASS_ARMOR_FOR_CASTER
    &AuraEffect::HandleEnableAltPower,                            //346 SPELL_AURA_ENABLE_ALT_POWER
    &AuraEffect::HandleNoImmediateEffect,                         //347 SPELL_AURA_MOD_SPELL_COOLDOWN_BY_HASTE  implemented in SpellHistory::StartCooldown
    &AuraEffect::HandleNoImmediateEffect,                         //348 SPELL_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT implemented in WorldSession::HandleLootMoneyOpcode
    &AuraEffect::HandleNoImmediateEffect,                         //349 SPELL_AURA_MOD_CURRENCY_GAIN implemented in Player::ModifyCurrency
    &AuraEffect::HandleNULL,                                      //350 SPELL_AURA_MOD_GATHERING_ITEMS_GAINED_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         //351 SPELL_AURA_MOD_CURRENCY_CATEGORY_GAIN_PCT implemented Loot::AddItem
    &AuraEffect::HandleNULL,                                      //352 SPELL_AURA_352
    &AuraEffect::HandleNULL,                                      //353 SPELL_AURA_MOD_CAMOUFLAGE
    &AuraEffect::HandleNoImmediateEffect,                         //354 SPELL_AURA_MOD_HEALING_FROM_TARGET_HEALTH implemented in Unit::SpellHealingPctDone
    &AuraEffect::HandleUnused,                                    //355 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //356 SPELL_AURA_MOD_DAMAGE_FROM_MANA implemented in Unit::SpellDamagePctDone
    &AuraEffect::HandleNULL,                                      //357 SPELL_AURA_ENABLE_BOSS1_UNIT_FRAME
    &AuraEffect::HandleNULL,                                      //358 SPELL_AURA_358
    &AuraEffect::HandleNULL,                                      //359 SPELL_AURA_359
    &AuraEffect::HandleNoImmediateEffect,                         //360 SPELL_AURA_PROC_TRIGGER_SPELL_COPY implemented in AuraEffect::HandleProc
    &AuraEffect::HandleNoImmediateEffect,                         //361 SPELL_AURA_OVERRIDE_AUTOATTACK_WITH_MELEE_SPELL implemented in Unit::AttackerStateUpdate
    &AuraEffect::HandleUnused,                                    //362 unused (4.3.4)
    &AuraEffect::HandleNULL,                                      //363 SPELL_AURA_MOD_NEXT_SPELL
    &AuraEffect::HandleUnused,                                    //364 unused (4.3.4)
    &AuraEffect::HandleNULL,                                      //365 SPELL_AURA_MAX_FAR_CLIP_PLANE
    &AuraEffect::HandleOverrideSpellPowerByAttackPower,           //366 SPELL_AURA_OVERRIDE_SPELL_POWER_BY_AP_PCT
    &AuraEffect::HandleNULL,                                      //367 SPELL_AURA_OVERRIDE_AUTOATTACK_WITH_RANGED_SPELL
    &AuraEffect::HandleUnused,                                    //368 unused (4.3.4)
    &AuraEffect::HandleNULL,                                      //369 SPELL_AURA_ENABLE_POWER_BAR_TIMER
    &AuraEffect::HandleNULL,                                      //370 SPELL_AURA_SET_FAIR_FAR_CLIP
};

AuraEffect::AuraEffect(Aura* base, uint8 effIndex, int32 const* baseAmount, Unit* caster):
m_base(base), m_spellInfo(base->GetSpellInfo()),
m_baseAmount(baseAmount ? *baseAmount : m_spellInfo->Effects[effIndex].BasePoints),
m_spellmod(nullptr), _periodicTimer(0), _period(0), _ticksDone(0),
m_effIndex(effIndex), m_canBeRecalculated(true), m_isPeriodic(false)
{
    CalculatePeriodic(caster, true, false);

    _amount = CalculateAmount(caster);

    CalculateSpellMod();
}

AuraEffect::~AuraEffect()
{
    delete m_spellmod;
}

template <typename Container>
void AuraEffect::GetTargetList(Container& targetContainer) const
{
    Aura::ApplicationMap const& targetMap = GetBase()->GetApplicationMap();
    // remove all targets which were not added to new list - they no longer deserve area aura
    for (auto appIter = targetMap.begin(); appIter != targetMap.end(); ++appIter)
    {
        if (appIter->second->HasEffect(GetEffIndex()))
            targetContainer.push_back(appIter->second->GetTarget());
    }
}

template <typename Container>
void AuraEffect::GetApplicationList(Container& applicationContainer) const
{
    Aura::ApplicationMap const& targetMap = GetBase()->GetApplicationMap();
    for (auto appIter = targetMap.begin(); appIter != targetMap.end(); ++appIter)
    {
        if (appIter->second->HasEffect(GetEffIndex()))
            applicationContainer.push_back(appIter->second);
    }
}

int32 AuraEffect::CalculateAmount(Unit* caster)
{
    // default amount calculation
    int32 amount = 0;

    if (!m_spellInfo->HasAttribute(SPELL_ATTR8_MASTERY_AFFECTS_POINTS) || G3D::fuzzyEq(m_spellInfo->Effects[m_effIndex].BonusMultiplier, 0.0f))
        amount = m_spellInfo->Effects[m_effIndex].CalcValue(caster, &m_baseAmount, GetBase()->GetOwner()->ToUnit());
    else if (caster && caster->GetTypeId() == TYPEID_PLAYER)
        amount = int32(caster->GetFloatValue(PLAYER_MASTERY) * m_spellInfo->Effects[m_effIndex].BonusMultiplier);

    // check item enchant aura cast
    if (!amount && caster)
        if (ObjectGuid itemGUID = GetBase()->GetCastItemGUID())
            if (Player* playerCaster = caster->ToPlayer())
                if (Item* castItem = playerCaster->GetItemByGuid(itemGUID))
                    if (castItem->GetItemSuffixFactor())
                    {
                        ItemRandomSuffixEntry const* item_rand_suffix = sItemRandomSuffixStore.LookupEntry(abs(castItem->GetItemRandomPropertyId()));
                        if (item_rand_suffix)
                        {
                            for (int k = 0; k < MAX_ITEM_ENCHANTMENT_EFFECTS; k++)
                            {
                                SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(item_rand_suffix->Enchantment[k]);
                                if (pEnchant)
                                {
                                    for (int t = 0; t < MAX_ITEM_ENCHANTMENT_EFFECTS; t++)
                                        if (pEnchant->EffectArg[t] == m_spellInfo->Id)
                                    {
                                        amount = uint32((item_rand_suffix->AllocationPct[k]*castItem->GetItemSuffixFactor()) / 10000);
                                        break;
                                    }
                                }

                                if (amount)
                                    break;
                            }
                        }
                    }

    // custom amount calculations go here
    switch (GetAuraType())
    {
        // crowd control auras
        case SPELL_AURA_MOD_CONFUSE:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_MOD_STUN:
        case SPELL_AURA_MOD_ROOT:
        case SPELL_AURA_TRANSFORM:
            m_canBeRecalculated = false;
            if (!m_spellInfo->ProcFlags)
                break;
            amount = int32(GetBase()->GetUnitOwner()->CountPctFromMaxHealth(10));
            if (caster)
            {
                // Glyphs increasing damage cap
                Unit::AuraEffectList const& overrideClassScripts = caster->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for (Unit::AuraEffectList::const_iterator itr = overrideClassScripts.begin(); itr != overrideClassScripts.end(); ++itr)
                {
                    if ((*itr)->IsAffectingSpell(m_spellInfo))
                    {
                        // Glyph of Frost nova and similar auras
                        if ((*itr)->GetMiscValue() == 7801)
                        {
                            AddPct(amount, (*itr)->GetAmount());
                            break;
                        }
                    }
                }
            }
            break;
        case SPELL_AURA_SCHOOL_ABSORB:
        case SPELL_AURA_MANA_SHIELD:
            m_canBeRecalculated = false;
            break;
        case SPELL_AURA_MOUNTED:
            if (MountCapabilityEntry const* mountCapability = GetBase()->GetUnitOwner()->GetMountCapability(uint32(GetMiscValueB())))
                amount = mountCapability->ID;
            break;
        case SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE:
        {
            if (caster)
            {
                // if Level <= 70 resist = player level
                int32 resist = caster->getLevel();

                if (resist > 70 && resist < 81)
                    resist += (resist - 70) * 5;
                else if (resist > 80)
                    resist += ((resist-70) * 5 + (resist - 80) * 7);

                switch (GetId())
                {
                    case 20043: // Aspect of the Wild
                    case 8185:  // Elemental Resistance
                    case 19891: // Resistance Aura
                    case 79106: // Shadow Protection
                    case 79107: // Shadow Protection
                        amount = resist;
                        break;
                    case 79060: // Mark of the Wild
                    case 79061: // Mark of the Wild
                    case 79062: // Blessing of Kings
                    case 79063: // Blessing of Kings
                    case 90363: // Embrace of the Shale Spider
                        amount = resist / 2;
                        break;
                }
                break;
            }
            break;
        }
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_LEECH:
            if (caster && GetBase()->GetType() == UNIT_AURA_TYPE)
                amount = caster->SpellDamageBonusDone(GetBase()->GetUnitOwner(), GetSpellInfo(), amount, DOT, GetEffIndex(), 1, nullptr, this);
            break;
        case SPELL_AURA_PERIODIC_HEAL:
            if (caster && GetBase()->GetType() == UNIT_AURA_TYPE)
                amount = caster->SpellHealingBonusDone(GetBase()->GetUnitOwner(), GetSpellInfo(), amount, DOT, GetEffIndex(), 1, nullptr, this);
            break;
        default:
            break;
    }

    /*
    * @todo: validate
    if (GetSpellInfo()->HasAttribute(SPELL_ATTR0_CU_ROLLING_PERIODIC))
    {
        Unit::AuraEffectList const& periodicAuras = GetBase()->GetUnitOwner()->GetAuraEffectsByType(GetAuraType());
        amount = std::accumulate(std::begin(periodicAuras), std::end(periodicAuras), amount, [this](int32 val, AuraEffect const* aurEff)
        {
            if (aurEff->GetCasterGUID() == GetCasterGUID() && aurEff->GetId() == GetId() && aurEff->GetEffIndex() == GetEffIndex() && aurEff->GetTotalTicks() > 0)
                val += aurEff->GetAmount() * static_cast<int32>(aurEff->GetRemainingTicks()) / static_cast<int32>(aurEff->GetTotalTicks());
            return val;
        });
    }
    */

    GetBase()->CallScriptEffectCalcAmountHandlers(this, amount, m_canBeRecalculated);
    amount *= GetBase()->GetStackAmount();
    return amount;
}

uint32 AuraEffect::GetTotalTicks() const
{
    uint32 totalTicks = 0;
    if (_period && !GetBase()->IsPermanent())
    {
        totalTicks = static_cast<uint32>((GetBase()->GetMaxDuration() - GetBase()->GetRolledOverDuration()) / _period);
        if (m_spellInfo->HasAttribute(SPELL_ATTR5_EXTRA_INITIAL_PERIOD))
            ++totalTicks;
        if (GetBase()->GetRolledOverDuration() > 0)
            ++totalTicks;
    }

    return totalTicks;
}

void AuraEffect::ResetPeriodic(bool resetPeriodicTimer /*= false*/)
{
    _ticksDone = 0;
    if (resetPeriodicTimer)
    {
        _periodicTimer = 0;
        // Start periodic on next tick or at aura apply
        if (m_spellInfo->HasAttribute(SPELL_ATTR5_EXTRA_INITIAL_PERIOD))
            _periodicTimer = _period;
    }
}

void AuraEffect::CalculatePeriodic(Unit* caster, bool resetPeriodicTimer, bool load)
{
    _period = GetSpellInfo()->Effects[GetEffIndex()].AuraPeriod;

    // prepare periodics
    switch (GetAuraType())
    {
        case SPELL_AURA_OBS_MOD_POWER:
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_OBS_MOD_HEALTH:
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
        case SPELL_AURA_PERIODIC_ENERGIZE:
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
        case SPELL_AURA_PERIODIC_MANA_LEECH:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
        case SPELL_AURA_POWER_BURN:
        case SPELL_AURA_PERIODIC_DUMMY:
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
            m_isPeriodic = true;
            break;
        default:
            break;
    }

    GetBase()->CallScriptEffectCalcPeriodicHandlers(this, m_isPeriodic, _period);

    if (!m_isPeriodic)
    {
        _period = 0;
        return;
    }

    if (_period)
        _period = m_spellInfo->CalcPeriod(caster, SpellEffIndex(GetEffIndex()), _period);

    if (!_period) // prevent infinite loop on Update
        m_isPeriodic = false;

    if (load) // aura loaded from db
    {
        if (_period && !GetBase()->IsPermanent())
        {
            uint32 elapsedTime = GetBase()->GetMaxDuration() - GetBase()->GetDuration();
            _ticksDone = elapsedTime / uint32(_period);
            _periodicTimer = elapsedTime % uint32(_period);
        }

        if (m_spellInfo->HasAttribute(SPELL_ATTR5_EXTRA_INITIAL_PERIOD))
            ++_ticksDone;
    }
    else // aura just created or reapplied
    {
        // reset periodic timer on aura create or reapply
        // we don't reset periodic timers when aura is triggered by proc
        ResetPeriodic(resetPeriodicTimer);
    }
}

void AuraEffect::CalculateSpellMod()
{
    switch (GetAuraType())
    {
        case SPELL_AURA_ADD_FLAT_MODIFIER:
        case SPELL_AURA_ADD_PCT_MODIFIER:
            if (!m_spellmod)
            {
                m_spellmod = new SpellModifier(GetBase());
                m_spellmod->op = SpellModOp(GetMiscValue());

                m_spellmod->type = GetAuraType() == SPELL_AURA_ADD_PCT_MODIFIER ? SPELLMOD_PCT : SPELLMOD_FLAT;
                m_spellmod->spellId = GetId();
                m_spellmod->mask = GetSpellInfo()->Effects[GetEffIndex()].SpellClassMask;
            }
            m_spellmod->value = GetAmount();
            break;
        default:
            break;
    }
    GetBase()->CallScriptEffectCalcSpellModHandlers(this, m_spellmod);
}

void AuraEffect::ChangeAmount(int32 newAmount, bool mark, bool onStackOrReapply)
{
    // Reapply if amount change
    uint8 handleMask = 0;
    if (newAmount != GetAmount())
        handleMask |= AURA_EFFECT_HANDLE_CHANGE_AMOUNT;
    if (onStackOrReapply)
        handleMask |= AURA_EFFECT_HANDLE_REAPPLY;

    if (!handleMask)
        return;

    std::vector<AuraApplication*> effectApplications;
    GetApplicationList(effectApplications);

    for (AuraApplication* aurApp : effectApplications)
    {
        aurApp->GetTarget()->_RegisterAuraEffect(this, false);
        HandleEffect(aurApp, handleMask, false);
    }

    if (handleMask & AURA_EFFECT_HANDLE_CHANGE_AMOUNT)
    {
        if (!mark)
            _amount = newAmount;
        else
            SetAmount(newAmount);
        CalculateSpellMod();
    }

    for (AuraApplication* aurApp : effectApplications)
    {
        if (aurApp->GetRemoveMode().HasAnyFlag())
            continue;

        aurApp->GetTarget()->_RegisterAuraEffect(this, true);
        HandleEffect(aurApp, handleMask, true);
    }

    if (GetSpellInfo()->HasAttribute(SPELL_ATTR8_AURA_POINTS_ON_CLIENT) || Aura::EffectTypeNeedsSendingAmount(GetAuraType()))
        GetBase()->SetNeedClientUpdateForTargets();
}

void AuraEffect::HandleEffect(AuraApplication * aurApp, uint8 mode, bool apply)
{
    // check if call is correct, we really don't want using bitmasks here (with 1 exception)
    ASSERT(mode == AURA_EFFECT_HANDLE_REAL
        || mode == AURA_EFFECT_HANDLE_SEND_FOR_CLIENT
        || mode == AURA_EFFECT_HANDLE_CHANGE_AMOUNT
        || mode == AURA_EFFECT_HANDLE_STAT
        || mode == AURA_EFFECT_HANDLE_SKILL
        || mode == AURA_EFFECT_HANDLE_REAPPLY
        || mode == (AURA_EFFECT_HANDLE_CHANGE_AMOUNT | AURA_EFFECT_HANDLE_REAPPLY));

    // register/unregister effect in lists in case of real AuraEffect apply/remove
    // registration/unregistration is done always before real effect handling (some effect handlers code is depending on this)
    if (mode & AURA_EFFECT_HANDLE_REAL)
        aurApp->GetTarget()->_RegisterAuraEffect(this, apply);

    // real aura apply/remove, handle modifier
    if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK)
        ApplySpellMod(aurApp->GetTarget(), apply);

    // call scripts helping/replacing effect handlers
    bool prevented = false;
    if (apply)
        prevented = GetBase()->CallScriptEffectApplyHandlers(this, aurApp, (AuraEffectHandleModes)mode);
    else
        prevented = GetBase()->CallScriptEffectRemoveHandlers(this, aurApp, (AuraEffectHandleModes)mode);

    // check if script events have removed the aura or if default effect prevention was requested
    if (apply && aurApp->GetRemoveMode().HasAnyFlag())
        return;

    // call default effect handler if it wasn't prevented
    if (!prevented)
        (*this.*AuraEffectHandler[GetAuraType()])(aurApp, mode, apply);

    // check if script events have removed the aura or if default effect prevention was requested
    if (apply && aurApp->GetRemoveMode().HasAnyFlag())
        return;

    // call scripts triggering additional events after apply/remove
    if (apply)
        GetBase()->CallScriptAfterEffectApplyHandlers(this, aurApp, (AuraEffectHandleModes)mode);
    else
        GetBase()->CallScriptAfterEffectRemoveHandlers(this, aurApp, (AuraEffectHandleModes)mode);
}

void AuraEffect::HandleEffect(Unit* target, uint8 mode, bool apply)
{
    AuraApplication* aurApp = GetBase()->GetApplicationOfTarget(target->GetGUID());
    ASSERT(aurApp);
    HandleEffect(aurApp, mode, apply);
}

void AuraEffect::ApplySpellMod(Unit* target, bool apply)
{
    if (!m_spellmod || target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->AddSpellMod(m_spellmod, apply);

    // Auras with charges do not mod amount of passive auras
    if (GetBase()->IsUsingCharges())
        return;

    // reapply some passive spells after add/remove related spellmods
    // Warning: it is a dead loop if 2 auras each other amount-shouldn't happen
    std::bitset<MAX_SPELL_EFFECTS> recalculateEffectMask;
    switch (SpellModOp(GetMiscValue()))
    {
        case SpellModOp::Points:
            recalculateEffectMask.set();
            break;
        case SpellModOp::PointsIndex0:
            recalculateEffectMask.set(EFFECT_0);
            break;
        case SpellModOp::PointsIndex1:
            recalculateEffectMask.set(EFFECT_1);
            break;
        case SpellModOp::PointsIndex2:
            recalculateEffectMask.set(EFFECT_2);
            break;
        default:
            break;
    }

    if (recalculateEffectMask.any())
    {
        ObjectGuid guid = target->GetGUID();
        Unit::AuraApplicationMap& auras = target->GetAppliedAuras();
        for (auto iter = auras.begin(); iter != auras.end(); ++iter)
        {
            Aura* aura = iter->second->GetBase();
            // only passive and permament auras-active auras should have amount set on spellcast and not be affected
            // if aura is cast by others, it will not be affected
            if ((aura->IsPassive() || aura->IsPermanent()) && aura->GetCasterGUID() == guid && aura->GetSpellInfo()->IsAffectedBySpellMod(m_spellmod))
                for (size_t i = 0; i < recalculateEffectMask.size(); ++i)
                    if (recalculateEffectMask[i])
                        if (AuraEffect* aurEff = aura->GetEffect(i))
                            aurEff->RecalculateAmount();
        }
    }
}

void AuraEffect::Update(uint32 diff, Unit* caster)
{
    if (m_isPeriodic && (GetBase()->GetDuration() >=0 || GetBase()->IsPassive() || GetBase()->IsPermanent()))
    {
        uint32 totalTicks = GetTotalTicks();
        _periodicTimer += diff;
        while (_periodicTimer >= _period)
        {
            _periodicTimer -= _period;

            if (!GetBase()->IsPermanent() && (_ticksDone + 1) > totalTicks)
                break;

            ++_ticksDone;

            // update before tick (aura can be removed in TriggerSpell or PeriodicTick calls)
            UpdatePeriodic(caster);

            std::vector<AuraApplication*> effectApplications;
            GetApplicationList(effectApplications);

            // tick on targets of effects
            for (AuraApplication* aurApp : effectApplications)
                PeriodicTick(aurApp, caster);
        }
    }
}

void AuraEffect::UpdatePeriodic(Unit* caster)
{
    switch (GetAuraType())
    {
        case SPELL_AURA_PERIODIC_DUMMY:
            switch (GetSpellInfo()->SpellFamilyName)
            {
                case SPELLFAMILY_GENERIC:
                    switch (GetId())
                    {
                        // Drink
                        case 430:
                        case 431:
                        case 432:
                        case 1133:
                        case 1135:
                        case 1137:
                        case 10250:
                        case 22734:
                        case 27089:
                        case 34291:
                        case 43182:
                        case 43183:
                        case 46755:
                        case 49472: // Drink Coffee
                        case 57073:
                        case 61830:
                        case 69176:
                        case 72623:
                        case 80166:
                        case 80167:
                        case 87958:
                        case 87959:
                        case 92736:
                        case 92797:
                        case 92800:
                        case 92803:
                            if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                                return;
                            // Get SPELL_AURA_MOD_POWER_REGEN aura from spell
                            if (AuraEffect* aurEff = GetBase()->GetEffect(0))
                            {
                                if (aurEff->GetAuraType() != SPELL_AURA_MOD_POWER_REGEN)
                                {
                                    m_isPeriodic = false;
                                    TC_LOG_ERROR("spells", "Aura %d structure has been changed - first aura is no longer SPELL_AURA_MOD_POWER_REGEN", GetId());
                                }
                                else
                                {
                                    // default case - not in arena
                                    if (!caster->ToPlayer()->InArena())
                                    {
                                        aurEff->ChangeAmount(GetAmount());
                                        m_isPeriodic = false;
                                    }
                                    else
                                    {
                                        // **********************************************
                                        // This feature uses only in arenas
                                        // **********************************************
                                        // Here need increase mana regen per tick (6 second rule)
                                        // on 0 tick -   0  (handled in 2 second)
                                        // on 1 tick - 166% (handled in 4 second)
                                        // on 2 tick - 133% (handled in 6 second)

                                        // Apply bonus for 1 - 4 tick
                                        switch (_ticksDone)
                                        {
                                            case 1:   // 0%
                                                aurEff->ChangeAmount(0);
                                                break;
                                            case 2:   // 166%
                                                aurEff->ChangeAmount(GetAmount() * 5 / 3);
                                                break;
                                            case 3:   // 133%
                                                aurEff->ChangeAmount(GetAmount() * 4 / 3);
                                                break;
                                            default:  // 100% - normal regen
                                                aurEff->ChangeAmount(GetAmount());
                                                // No need to update after 4th tick
                                                m_isPeriodic = false;
                                                break;
                                        }
                                    }
                                }
                            }
                            break;
                        case 58549: // Tenacity
                        case 59911: // Tenacity (vehicle)
                           GetBase()->RefreshDuration();
                           break;
                        default:
                            break;
                    }
                    break;
                case SPELLFAMILY_DEATHKNIGHT:
                    // Chains of Ice
                    if (GetSpellInfo()->SpellFamilyFlags[1] & 0x00004000)
                    {
                        // Get 0 effect aura
                        if (AuraEffect* slow = GetBase()->GetEffect(0))
                        {
                            int32 newAmount = slow->GetAmount() + GetAmount();
                            if (newAmount > 0)
                                newAmount = 0;
                            slow->ChangeAmount(newAmount);
                        }
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
           break;
    }
    GetBase()->CallScriptEffectUpdatePeriodicHandlers(this);
}

float AuraEffect::GetCritChanceFor(Unit const* caster, Unit const* target) const
{
    return target->SpellCritChanceTaken(caster, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), GetBase()->GetCritChance(), GetSpellInfo()->GetAttackType(), true);
}

bool AuraEffect::IsAffectingSpell(SpellInfo const* spell) const
{
    if (!spell)
        return false;

    // Check family name and EffectClassMask
    if (!spell->IsAffected(m_spellInfo->SpellFamilyName, m_spellInfo->Effects[m_effIndex].SpellClassMask))
        return false;

    return true;
}

void AuraEffect::SendTickImmune(Unit* target, Unit* caster) const
{
    if (caster)
        caster->SendSpellDamageImmune(target, m_spellInfo->Id);
}

void AuraEffect::PeriodicTick(AuraApplication* aurApp, Unit* caster) const
{
    bool prevented = GetBase()->CallScriptEffectPeriodicHandlers(this, aurApp);
    if (prevented)
        return;

    Unit* target = aurApp->GetTarget();

    switch (GetAuraType())
    {
        case SPELL_AURA_PERIODIC_DUMMY:
            HandlePeriodicDummyAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
            HandlePeriodicTriggerSpellAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
            HandlePeriodicTriggerSpellWithValueAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
            HandlePeriodicDamageAurasTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_LEECH:
            HandlePeriodicHealthLeechAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
            HandlePeriodicHealthFunnelAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_OBS_MOD_HEALTH:
            HandlePeriodicHealAurasTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_MANA_LEECH:
            HandlePeriodicManaLeechAuraTick(target, caster);
            break;
        case SPELL_AURA_OBS_MOD_POWER:
            HandleObsModPowerAuraTick(target, caster);
            break;
        case SPELL_AURA_PERIODIC_ENERGIZE:
            HandlePeriodicEnergizeAuraTick(target, caster);
            break;
        case SPELL_AURA_POWER_BURN:
            HandlePeriodicPowerBurnAuraTick(target, caster);
            break;
        default:
            break;
    }
}

bool AuraEffect::CheckEffectProc(AuraApplication* aurApp, ProcEventInfo& eventInfo) const
{
    bool result = GetBase()->CallScriptCheckEffectProcHandlers(this, aurApp, eventInfo);
    if (!result)
        return false;

    SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
    switch (GetAuraType())
    {
        case SPELL_AURA_MOD_CONFUSE:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_MOD_STUN:
        case SPELL_AURA_MOD_ROOT:
        case SPELL_AURA_TRANSFORM:
        {
            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !damageInfo->GetDamage())
                return false;

            // Spell own damage at apply won't break CC
            if (spellInfo && spellInfo == GetSpellInfo())
            {
                Aura* aura = GetBase();
                // called from spellcast, should not have ticked yet
                if (aura->GetDuration() == aura->GetMaxDuration())
                    return false;
            }
            break;
        }
        case SPELL_AURA_MECHANIC_IMMUNITY:
        case SPELL_AURA_MOD_MECHANIC_RESISTANCE:
            // compare mechanic
            if (!spellInfo || static_cast<int32>(spellInfo->Mechanic) != GetMiscValue())
                return false;
            break;
        case SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK:
            // skip melee hits and instant cast spells
            if (!spellInfo || !spellInfo->CalcCastTime())
                return false;
            break;
        case SPELL_AURA_MOD_DAMAGE_FROM_CASTER:
            // Compare casters
            if (GetCasterGUID() != eventInfo.GetActor()->GetGUID())
                return false;
            break;
        case SPELL_AURA_MOD_POWER_COST_SCHOOL:
        case SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT:
            // Skip melee hits and spells with wrong school or zero cost
            if (!spellInfo || (!spellInfo->ManaCost && !spellInfo->ManaCostPercentage) || // Cost Check
                !(spellInfo->GetSchoolMask() & GetMiscValue())) // School Check
                return false;
            break;
        case SPELL_AURA_REFLECT_SPELLS_SCHOOL:
            // Skip melee hits and spells with wrong school
            if (!spellInfo || !(spellInfo->GetSchoolMask() & GetMiscValue()))
                return false;
            break;
        case SPELL_AURA_PROC_TRIGGER_SPELL:
        case SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE:
        case SPELL_AURA_PROC_ON_POWER_AMOUNT:
        case SPELL_AURA_PROC_TRIGGER_SPELL_COPY:
        {
            // Don't proc extra attacks while already processing extra attack spell
            uint32 triggerSpellId = GetSpellInfo()->Effects[GetEffIndex()].TriggerSpell;
            if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
                if (aurApp->GetTarget()->m_extraAttacks && triggeredSpellInfo->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
                    return false;
            break;
        }
        case SPELL_AURA_MOD_SPELL_CRIT_CHANCE:
            // skip spells that can't crit
            if (!spellInfo || !spellInfo->HasAttribute(SPELL_ATTR0_CU_CAN_CRIT))
                return false;
            break;
        default:
            break;
    }

    return result;
}

void AuraEffect::HandleProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    bool prevented = GetBase()->CallScriptEffectProcHandlers(this, aurApp, eventInfo);
    if (prevented)
        return;

    switch (GetAuraType())
    {
        // CC Auras which use their amount to drop
        // Are there any more auras which need this?
        case SPELL_AURA_MOD_CONFUSE:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_MOD_STUN:
        case SPELL_AURA_MOD_ROOT:
        case SPELL_AURA_TRANSFORM:
            HandleBreakableCCAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_DUMMY:
        case SPELL_AURA_PROC_TRIGGER_SPELL:
            HandleProcTriggerSpellAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE:
            HandleProcTriggerSpellWithValueAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_PROC_TRIGGER_DAMAGE:
            HandleProcTriggerDamageAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_RAID_PROC_FROM_CHARGE:
            HandleRaidProcFromChargeAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE:
            HandleRaidProcFromChargeWithValueAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_PROC_ON_POWER_AMOUNT:
            HandleProcOnPowerAmountAuraProc(aurApp, eventInfo);
            break;
        case SPELL_AURA_PROC_TRIGGER_SPELL_COPY:
            HandleProcTriggerSpellCopyAuraProc(aurApp, eventInfo);
            break;
        default:
            break;
    }

    GetBase()->CallScriptAfterEffectProcHandlers(this, aurApp, eventInfo);
}

void AuraEffect::HandleShapeshiftBoosts(Unit* target, bool apply) const
{
    uint32 spellId = 0;
    uint32 spellId2 = 0;

    switch (GetMiscValue())
    {
        case FORM_CAT:
            spellId = 3025;
            break;
        case FORM_TREE:
            spellId = 34123;
            break;
        case FORM_TRAVEL:
            spellId = 5419;
            break;
        case FORM_AQUA:
            spellId = 5421;
            break;
        case FORM_BEAR:
            spellId = 1178;
            spellId2 = 21178;
            break;
        case FORM_BATTLESTANCE:
            spellId = 21156;
            break;
        case FORM_DEFENSIVESTANCE:
            spellId = 7376;
            break;
        case FORM_BERSERKERSTANCE:
            spellId = 7381;
            break;
        case FORM_MOONKIN:
            spellId = 24905;
            spellId2 = 24907;
            break;
        case FORM_FLIGHT:
            spellId = 33948;
            spellId2 = 34764;
            break;
        case FORM_FLIGHT_EPIC:
            spellId  = 40122;
            spellId2 = 40121;
            break;
        case FORM_METAMORPHOSIS:
            spellId  = 54817;
            spellId2 = 54879;
            break;
        case FORM_SPIRITOFREDEMPTION:
            spellId  = 27792;
            spellId2 = 27795;                               // must be second, this important at aura remove to prevent to early iterator invalidation.
            break;
        case FORM_SHADOW:
            spellId = 49868;
            break;
        case FORM_GHOSTWOLF:
            spellId = 67116;
            break;
        case FORM_GHOUL:
        case FORM_AMBIENT:
        case FORM_STEALTH:
        case FORM_CREATURECAT:
        case FORM_CREATUREBEAR:
            break;
        default:
            break;
    }

    if (apply)
    {
        if (spellId)
            target->CastSpell(target, spellId, this);

        if (spellId2)
            target->CastSpell(target, spellId2, this);

        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            Player* plrTarget = target->ToPlayer();

            PlayerSpellMap const& sp_list = plrTarget->GetSpellMap();
            for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
            {
                if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled)
                    continue;

                if (itr->first == spellId || itr->first == spellId2)
                    continue;

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                if (!spellInfo || !(spellInfo->IsPassive() || spellInfo->HasAttribute(SPELL_ATTR0_DO_NOT_DISPLAY_SPELLBOOK_AURA_ICON_COMBAT_LOG)))
                    continue;

                if (spellInfo->HasAttribute(SPELL_ATTR8_MASTERY_AFFECTS_POINTS) && !plrTarget->IsCurrentSpecMasterySpell(spellInfo))
                    continue;

                if (spellInfo->Stances & (UI64LIT(1) << (GetMiscValue() - 1)))
                    target->CastSpell(target, itr->first, this);
            }

            // Also do it for Glyphs
            for (uint32 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
            {
                if (uint32 glyphId = plrTarget->GetGlyph(plrTarget->GetActiveSpec(), i))
                {
                    if (GlyphPropertiesEntry const* glyph = sGlyphPropertiesStore.LookupEntry(glyphId))
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(glyph->SpellID);
                        if (!spellInfo || !(spellInfo->HasAttribute(SPELL_ATTR0_PASSIVE) || spellInfo->HasAttribute(SPELL_ATTR0_DO_NOT_DISPLAY_SPELLBOOK_AURA_ICON_COMBAT_LOG)))
                            continue;

                        if (spellInfo->Stances & (UI64LIT(1) << (GetMiscValue() - 1)))
                            target->CastSpell(target, glyph->SpellID, this);
                    }
                }
            }

            // Leader of the Pack
            if (plrTarget->HasSpell(17007))
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(24932);
                if (spellInfo && spellInfo->Stances & (UI64LIT(1) << (GetMiscValue() - 1)))
                    target->CastSpell(target, 24932, this);
            }

            // Heart of the Wild
            if (AuraEffect const* heartOfTheWild = target->GetAuraEffectOfRankedSpell(17003, EFFECT_0))
            {
                uint32 heartOfTheWildSpellId = 0;
                int32 heartOfTheWildAmount = 0;

                switch (GetMiscValue())
                {
                    case FORM_CAT:
                        heartOfTheWildSpellId = 24900;
                        heartOfTheWildAmount = heartOfTheWild->GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                        break;
                    case FORM_BEAR:
                        heartOfTheWildSpellId = 24899;
                        heartOfTheWildAmount = heartOfTheWild->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                        break;
                    default:
                        break;
                }

                if (heartOfTheWildSpellId)
                {
                    CastSpellExtraArgs args(this);
                    args.AddSpellMod(SPELLVALUE_BASE_POINT0, heartOfTheWildAmount);
                    target->CastSpell(target, heartOfTheWildSpellId, args);
                }
            }

            switch (GetMiscValue())
            {
                case FORM_CAT:
                    // Savage Roar
                    if (target->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_DRUID, 0, 0x10000000, 0))
                        target->CastSpell(target, 62071, true);
                    // Nurturing Instinct
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT, SPELLFAMILY_DRUID, 2254, EFFECT_0))
                    {
                        uint32 spellId3 = 0;
                        switch (aurEff->GetId())
                        {
                            case 33872:
                                spellId3 = 47179;
                                break;
                            case 33873:
                                spellId3 = 47180;
                                break;
                        }
                        target->CastSpell(target, spellId3, this);
                    }
                    // Master Shapeshifter - Cat
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_HEALING_DONE_PERCENT, SPELLFAMILY_GENERIC, 2851, EFFECT_0))
                    {
                        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
                        args.AddSpellMod(SPELLVALUE_BASE_POINT0, aurEff->GetAmount());
                        target->CastSpell(target, 48420, args);
                    }
                    break;
                case FORM_BEAR:
                    // Master Shapeshifter - Bear
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_HEALING_DONE_PERCENT, SPELLFAMILY_GENERIC, 2851, EFFECT_0))
                    {
                        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
                        args.AddSpellMod(SPELLVALUE_BASE_POINT0, aurEff->GetAmount());
                        target->CastSpell(target, 48418, args);
                    }
                    // Survival of the Fittest
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, SPELLFAMILY_DRUID, 961, EFFECT_0))
                    {
                        CastSpellExtraArgs args(this);
                        args.AddSpellMod(SPELLVALUE_BASE_POINT0, aurEff->GetSpellInfo()->Effects[EFFECT_2].CalcValue());
                        target->CastSpell(target, 62069, args);
                    }
                    break;
                case FORM_MOONKIN:
                    // Master Shapeshifter - Moonkin
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_HEALING_DONE_PERCENT, SPELLFAMILY_GENERIC, 2851, EFFECT_0))
                    {
                        CastSpellExtraArgs args(TRIGGERED_FULL_MASK);
                        args.AddSpellMod(SPELLVALUE_BASE_POINT0, aurEff->GetAmount());
                        target->CastSpell(target, 48421, args);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        if (spellId)
            target->RemoveOwnedAura(spellId, target->GetGUID());
        if (spellId2)
            target->RemoveOwnedAura(spellId2, target->GetGUID());

        Unit::AuraEffectList const& shapeshifts = target->GetAuraEffectsByType(SPELL_AURA_MOD_SHAPESHIFT);
        AuraEffect const* newAura = nullptr;
        // Iterate through all the shapeshift auras that the target has, if there is another aura with SPELL_AURA_MOD_SHAPESHIFT, then this aura is being removed due to that one being applied
        for (AuraEffect const* aurEff : shapeshifts)
        {
            if (aurEff != this)
            {
                newAura = aurEff;
                break;
            }
        }

        Unit::AuraApplicationMap& tAuras = target->GetAppliedAuras();
        for (auto itr = tAuras.begin(); itr != tAuras.end();)
        {
            // Use the new aura to see on what stance the target will be
            uint64 newStance = newAura ? (UI64LIT(1) << (newAura->GetMiscValue() - 1)) : 0;

            // If the stances are not compatible with the spell, remove it
            if (itr->second->GetBase()->IsRemovedOnShapeLost(target) && !(itr->second->GetBase()->GetSpellInfo()->Stances & newStance))
                target->RemoveAura(itr);
            else
                ++itr;
        }
    }
}

bool AuraEffect::CanPeriodicTickCrit(Unit const* caster) const
{
    if (m_spellInfo->HasAttribute(SPELL_ATTR8_PERIODIC_CAN_CRIT))
        return true;

    return caster && caster->HasAuraTypeWithAffectMask(SPELL_AURA_ABILITY_PERIODIC_CRIT, m_spellInfo);
}

/*********************************************************/
/***               AURA EFFECT HANDLERS                ***/
/*********************************************************/

/**************************************/
/***       VISIBILITY & PHASES      ***/
/**************************************/

void AuraEffect::HandleModInvisibilityDetect(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    InvisibilityType type = InvisibilityType(GetMiscValue());

    if (apply)
    {
        target->m_invisibilityDetect.AddFlag(type);
        target->m_invisibilityDetect.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_INVISIBILITY_DETECT))
            target->m_invisibilityDetect.DelFlag(type);

        target->m_invisibilityDetect.AddValue(type, -GetAmount());
    }

    // call functions which may have additional effects after changing state of unit
    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleModInvisibility(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    InvisibilityType type = InvisibilityType(GetMiscValue());

    if (apply)
    {
        // apply glow vision
        if (target->GetTypeId() == TYPEID_PLAYER && type == INVISIBILITY_GENERAL)
            target->SetByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW);

        target->m_invisibility.AddFlag(type);
        target->m_invisibility.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_INVISIBILITY))
        {
            // if not have different invisibility auras.
            // remove glow vision
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW);

            target->m_invisibility.DelFlag(type);
        }
        else
        {
            bool found = false;
            Unit::AuraEffectList const& invisAuras = target->GetAuraEffectsByType(SPELL_AURA_MOD_INVISIBILITY);
            for (Unit::AuraEffectList::const_iterator i = invisAuras.begin(); i != invisAuras.end(); ++i)
            {
                if (GetMiscValue() == (*i)->GetMiscValue())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                // if not have invisibility auras of type INVISIBILITY_GENERAL
                // remove glow vision
                if (target->GetTypeId() == TYPEID_PLAYER && type == INVISIBILITY_GENERAL)
                    target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW);

                target->m_invisibility.DelFlag(type);
            }
        }

        target->m_invisibility.AddValue(type, -GetAmount());
    }

    // call functions which may have additional effects after changing state of unit
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        // drop flag at invisibiliy in bg
        target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::StealthOrInvis);
    }

    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleModStealthDetect(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
    {
        target->m_stealthDetect.AddFlag(type);
        target->m_stealthDetect.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH_DETECT))
            target->m_stealthDetect.DelFlag(type);

        target->m_stealthDetect.AddValue(type, -GetAmount());
    }

    // call functions which may have additional effects after changing state of unit
    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleModStealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
    {
        target->m_stealth.AddFlag(type);
        target->m_stealth.AddValue(type, GetAmount());

        target->SetStandFlags(UNIT_STAND_FLAGS_CREEP);
        if (target->GetTypeId() == TYPEID_PLAYER)
            target->SetByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_STEALTH);
    }
    else
    {
        target->m_stealth.AddValue(type, -GetAmount());

        if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH)) // if last SPELL_AURA_MOD_STEALTH
        {
            target->m_stealth.DelFlag(type);

            target->RemoveStandFlags(UNIT_STAND_FLAGS_CREEP);
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_STEALTH);
        }
    }

    // call functions which may have additional effects after changing state of unit
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        // drop flag at stealth in bg
        target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::StealthOrInvis);
    }

    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleModStealthLevel(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
        target->m_stealth.AddValue(type, GetAmount());
    else
        target->m_stealth.AddValue(type, -GetAmount());

    // call functions which may have additional effects after changing state of unit
    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleDetectAmore(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        target->SetByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, 1 << (GetMiscValue() - 1));
    else
    {
        if (target->HasAuraType(SPELL_AURA_DETECT_AMORE))
        {
            Unit::AuraEffectList const& amoreAuras = target->GetAuraEffectsByType(SPELL_AURA_DETECT_AMORE);
            for (Unit::AuraEffectList::const_iterator i = amoreAuras.begin(); i != amoreAuras.end(); ++i)
            {
                if (GetMiscValue() == (*i)->GetMiscValue())
                    return;
            }
        }

        target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, 1 << (GetMiscValue() - 1));
    }
}

void AuraEffect::HandleSpiritOfRedemption(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    if (apply)
    {
        target->RemoveAllAurasOnDeath();
        target->CastSpell(target, SPELL_UNTRANSFORM_HERO, true);
        target->CastSpell(target, SPELL_SPIRIT_OF_REDEMPTION_IMMUNITY, true);
        target->SetFullHealth();
    }
    else
    {
        target->setDeathState(JUST_DIED);
        target->RemoveAurasDueToSpell(SPELL_SPIRIT_OF_REDEMPTION_IMMUNITY);
    }
}

void AuraEffect::HandleAuraGhost(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
    {
        target->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST);
        target->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
        target->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
    }
    else
    {
        if (target->HasAuraType(SPELL_AURA_GHOST))
            return;

        target->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST);
        target->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
        target->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
    }
}

void AuraEffect::HandlePhase(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        PhasingHandler::AddPhase(target, uint32(GetMiscValueB()), true);
    else
        PhasingHandler::RemovePhase(target, uint32(GetMiscValueB()), true);
}

void AuraEffect::HandlePhaseGroup(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    PhasingHandler::AddPhaseGroup(target, uint32(GetMiscValueB()), true);

    if (apply)
        PhasingHandler::AddPhaseGroup(target, uint32(GetMiscValueB()), true);
    else
        PhasingHandler::RemovePhaseGroup(target, uint32(GetMiscValueB()), true);
}

/**********************/
/***   UNIT MODEL   ***/
/**********************/

void AuraEffect::HandleAuraModShapeshift(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    uint32 modelid = 0;
    Powers PowerType = POWER_MANA;
    ShapeshiftForm form = ShapeshiftForm(GetMiscValue());

    switch (form)
    {
        case FORM_CAT:                                      // 0x01
        case FORM_GHOUL:                                    // 0x07
            PowerType = POWER_ENERGY;
            break;

        case FORM_BEAR:                                     // 0x05

        case FORM_BATTLESTANCE:                             // 0x11
        case FORM_DEFENSIVESTANCE:                          // 0x12
        case FORM_BERSERKERSTANCE:                          // 0x13
            PowerType = POWER_RAGE;
            break;

        case FORM_TREE:                                     // 0x02
        case FORM_TRAVEL:                                   // 0x03
        case FORM_AQUA:                                     // 0x04
        case FORM_AMBIENT:                                  // 0x06

        case FORM_STEVES_GHOUL:                             // 0x09
        case FORM_THARONJA_SKELETON:                        // 0x0A
        case FORM_TEST_OF_STRENGTH:                         // 0x0B
        case FORM_BLB_PLAYER:                               // 0x0C
        case FORM_SHADOW_DANCE:                             // 0x0D
        case FORM_CREATUREBEAR:                             // 0x0E
        case FORM_CREATURECAT:                              // 0x0F
        case FORM_GHOSTWOLF:                                // 0x10

        case FORM_TEST:                                     // 0x14
        case FORM_ZOMBIE:                                   // 0x15
        case FORM_METAMORPHOSIS:                            // 0x16
        case FORM_UNDEAD:                                   // 0x19
        case FORM_MASTER_ANGLER:                            // 0x1A
        case FORM_FLIGHT_EPIC:                              // 0x1B
        case FORM_SHADOW:                                   // 0x1C
        case FORM_FLIGHT:                                   // 0x1D
        case FORM_STEALTH:                                  // 0x1E
        case FORM_MOONKIN:                                  // 0x1F
        case FORM_SPIRITOFREDEMPTION:                       // 0x20
            break;
        default:
            TC_LOG_ERROR("spells", "Auras: Unknown Shapeshift Type: %u", GetMiscValue());
    }

    modelid = target->GetModelForForm(form, GetId());

    if (apply)
    {
        // remove polymorph before changing display id to keep new display id
        switch (form)
        {
            case FORM_CAT:
            case FORM_TREE:
            case FORM_TRAVEL:
            case FORM_AQUA:
            case FORM_BEAR:
            case FORM_FLIGHT_EPIC:
            case FORM_FLIGHT:
            case FORM_MOONKIN:
            {
                // remove movement affects
                target->RemoveAurasByShapeShift();

                // and polymorphic affects
                if (target->IsPolymorphed())
                    target->RemoveAurasDueToSpell(target->getTransForm());
                break;
            }
            default:
               break;
        }

        // remove other shapeshift before applying a new one
        target->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT, ObjectGuid::Empty, GetBase());

        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode().HasAnyFlag())
            return;

        if (PowerType != POWER_MANA)
        {
            int32 oldPower = target->GetPower(PowerType);
            // reset power to default values only at power change
            if (target->GetPowerType() != PowerType)
                target->SetPowerType(PowerType);

            if (form == FORM_CAT || form == FORM_BEAR)
            {
                AuraEffect const* furorEffect = target->GetDummyAuraEffect(SPELLFAMILY_DRUID, 238, 0);
                if (form == FORM_CAT)
                {
                    target->SetPower(POWER_ENERGY, 0);
                    if (furorEffect)
                    {
                        // Allow to retain up to x% of the caster's energy when shape shifting into cat form
                        CastSpellExtraArgs args(this);
                        args.AddSpellMod(SPELLVALUE_BASE_POINT0, std::min<int32>(oldPower, furorEffect->GetAmount()));
                        target->CastSpell(target, 17099, args);
                    }
                }
                else if (furorEffect && roll_chance_i(furorEffect->GetAmount())) // x% chance to gain 10 rage when shape shifting into bear form
                    target->CastSpell(target, 17057, true);
            }
        }
        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode().HasAnyFlag())
            return;

        ShapeshiftForm prevForm = target->GetShapeshiftForm();
        target->SetShapeshiftForm(form);
        // add the shapeshift aura's boosts
        if (prevForm != form)
            HandleShapeshiftBoosts(target, true);

        if (modelid > 0)
        {
            SpellInfo const* transformSpellInfo = sSpellMgr->GetSpellInfo(target->getTransForm());
            if (!transformSpellInfo || !GetSpellInfo()->IsPositive())
                target->SetDisplayId(modelid);
        }

        if (SpellShapeshiftFormEntry const* shapeshift = sSpellShapeshiftFormStore.LookupEntry(form))
            if (!shapeshift->GetFlags().HasFlag(SpellShapeshiftFormFlags::Stance))
                target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::Shapeshifting, GetSpellInfo());
    }
    else
    {
        // reset model id if no other auras present
        // may happen when aura is applied on linked event on aura removal
        if (!target->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT))
        {
            target->SetShapeshiftForm(FORM_NONE);
            if (target->getClass() == CLASS_DRUID)
            {
                // Remove movement impairing effects also when shifting out
                target->RemoveAurasByShapeShift();
            }
        }

        if (modelid > 0)
            target->RestoreDisplayId();

        switch (form)
        {
            // Nordrassil Harness - bonus
            case FORM_BEAR:
            case FORM_CAT:
                if (AuraEffect* dummy = target->GetAuraEffect(37315, 0))
                    target->CastSpell(target, 37316, dummy);
                break;
            // Nordrassil Regalia - bonus
            case FORM_MOONKIN:
                if (AuraEffect* dummy = target->GetAuraEffect(37324, 0))
                    target->CastSpell(target, 37325, dummy);
                break;
            case FORM_BATTLESTANCE:
            case FORM_DEFENSIVESTANCE:
            case FORM_BERSERKERSTANCE:
            {
                int32 Rage_val = 0;
                // Defensive Tactics
                if (form == FORM_DEFENSIVESTANCE)
                {
                    if (AuraEffect const* aurEff = target->IsScriptOverriden(m_spellInfo, 831))
                        Rage_val += aurEff->GetAmount() * 10;
                }
                // Stance mastery + Tactical mastery (both passive, and last have aura only in defense stance, but need apply at any stance switch)
                if (target->GetTypeId() == TYPEID_PLAYER)
                {
                    PlayerSpellMap const& sp_list = target->ToPlayer()->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled)
                            continue;

                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                        if (spellInfo && spellInfo->SpellFamilyName == SPELLFAMILY_WARRIOR && spellInfo->SpellIconID == 139)
                            Rage_val += target->CalculateSpellDamage(target, spellInfo, 0) * 10;
                    }
                }
                if (target->GetPower(POWER_RAGE) > Rage_val)
                    target->SetPower(POWER_RAGE, Rage_val);
                break;
            }
            default:
                break;
        }

        // remove the shapeshift aura's boosts
        HandleShapeshiftBoosts(target, false);
    }

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        target->ToPlayer()->InitDataForForm();
        target->ToPlayer()->UpdateArmorSpecialization();
    }
    else
        target->UpdateDisplayPower();

    if (target->getClass() == CLASS_DRUID)
    {
        // Dash
        if (AuraEffect* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_INCREASE_SPEED, SPELLFAMILY_DRUID, 0, 0, 0x8))
            aurEff->RecalculateAmount();

        // Disarm handling
        // If druid shifts while being disarmed we need to deal with that since forms aren't affected by disarm
        // and also HandleAuraModDisarm is not triggered
        if (!target->CanUseAttackType(BASE_ATTACK))
        {
            if (Item* pItem = target->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                target->ToPlayer()->_ApplyWeaponDamage(EQUIPMENT_SLOT_MAINHAND, pItem->GetTemplate(), apply);
        }
    }

    // stop handling the effect if it was removed by linked event
    if (apply && aurApp->GetRemoveMode().HasAnyFlag())
        return;

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        SpellShapeshiftFormEntry const* shapeInfo = sSpellShapeshiftFormStore.LookupEntry(form);
        ASSERT(shapeInfo);
        // Learn spells for shapeshift form - no need to send action bars or add spells to spellbook
        for (uint8 i = 0; i < MAX_SHAPESHIFT_SPELLS; ++i)
        {
            if (!shapeInfo->PresetSpellID[i])
                continue;
            if (apply)
                target->ToPlayer()->AddTemporarySpell(shapeInfo->PresetSpellID[i]);
            else
                target->ToPlayer()->RemoveTemporarySpell(shapeInfo->PresetSpellID[i]);
        }
    }
}

void AuraEffect::HandleAuraTransform(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        // update active transform spell only when transform not set or not overwriting negative by positive case
        SpellInfo const* transformSpellInfo = sSpellMgr->GetSpellInfo(target->getTransForm());
        if (!transformSpellInfo || !GetSpellInfo()->IsPositive() || transformSpellInfo->IsPositive())
        {
            target->setTransForm(GetId());
            // special case (spell specific functionality)
            if (GetMiscValue() == 0)
            {
                switch (GetId())
                {
                    // Orb of Deception
                    case 16739:
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return;

                        switch (target->getRace())
                        {
                            // Blood Elf
                            case RACE_BLOODELF:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 17830 : 17829);
                                break;
                            // Orc
                            case RACE_ORC:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10140 : 10139);
                                break;
                            // Troll
                            case RACE_TROLL:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10134 : 10135);
                                break;
                            // Tauren
                            case RACE_TAUREN:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10147 : 10136);
                                break;
                            // Undead
                            case RACE_UNDEAD_PLAYER:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10145 : 10146);
                                break;
                            // Draenei
                            case RACE_DRAENEI:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 17828 : 17827);
                                break;
                            // Dwarf
                            case RACE_DWARF:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10142 : 10141);
                                break;
                            // Gnome
                            case RACE_GNOME:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10149 : 10148);
                                break;
                            // Human
                            case RACE_HUMAN:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10138 : 10137);
                                break;
                            // Night Elf
                            case RACE_NIGHTELF:
                                target->SetDisplayId(target->getGender() == GENDER_FEMALE ? 10144 : 10143);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    // Murloc costume
                    case 42365:
                        target->SetDisplayId(21723);
                        break;
                    // Dread Corsair
                    case 50517:
                    // Corsair Costume
                    case 51926:
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return;

                        switch (target->getRace())
                        {
                            // Blood Elf
                            case RACE_BLOODELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25032 : 25043);
                                break;
                            // Orc
                            case RACE_ORC:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25039 : 25050);
                                break;
                            // Troll
                            case RACE_TROLL:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25041 : 25052);
                                break;
                            // Tauren
                            case RACE_TAUREN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25040 : 25051);
                                break;
                            // Undead
                            case RACE_UNDEAD_PLAYER:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25042 : 25053);
                                break;
                            // Draenei
                            case RACE_DRAENEI:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25033 : 25044);
                                break;
                            // Dwarf
                            case RACE_DWARF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25034 : 25045);
                                break;
                            // Gnome
                            case RACE_GNOME:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25035 : 25046);
                                break;
                            // Human
                            case RACE_HUMAN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25037 : 25048);
                                break;
                            // Night Elf
                            case RACE_NIGHTELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25038 : 25049);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    // Pygmy Oil
                    case 53806:
                        target->SetDisplayId(22512);
                        break;
                    // Honor the Dead
                    case 65386:
                    case 65495:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 29203 : 29204);
                        break;
                    // Darkspear Pride
                    case 75532:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 31737 : 31738);
                        break;
                    // Gnomeregan Pride
                    case 75531:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 31654 : 31655);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(GetMiscValue());
                if (!ci)
                {
                    target->SetDisplayId(16358);              // pig pink ^_^
                    TC_LOG_ERROR("spells", "Auras: unknown creature id = %d (only need its modelid) From Spell Aura Transform in Spell ID = %d", GetMiscValue(), GetId());
                }
                else
                {
                    uint32 model_id = 0;

                    // choose a model, based on trigger flag
                    if (uint32 modelid = ObjectMgr::ChooseDisplayId(ci)->CreatureDisplayID)
                        model_id = modelid;

                    // Polymorph (sheep)
                    if (GetSpellInfo()->SpellFamilyName == SPELLFAMILY_MAGE && GetSpellInfo()->SpellIconID == 82 && GetSpellInfo()->SpellVisual[0] == 12978)
                    {
                        if (Unit* caster = GetCaster())
                        {
                            if (caster->HasAura(52648))         // Glyph of the Penguin
                                model_id = 26452;
                            else if (caster->HasAura(57927))	// Glyph of the Monkey
                                model_id = 21362;
                        }
                    }

                    target->SetDisplayId(model_id);

                    // Dragonmaw Illusion (set mount model also)
                    if (GetId() == 42016 && target->GetMountID() && !target->GetAuraEffectsByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED).empty())
                        target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 16314);
                }
            }
        }

        // polymorph case
        if ((mode & AURA_EFFECT_HANDLE_REAL) && target->GetTypeId() == TYPEID_PLAYER && target->IsPolymorphed())
        {
            // for players, start regeneration after 1s (in polymorph fast regeneration case)
            // only if caster is Player (after patch 2.4.2)
            //dismount polymorphed target (after patch 2.4.2)
            if (target->IsMounted())
                target->RemoveAurasByType(SPELL_AURA_MOUNTED);
        }
    }
    else
    {
        // HandleEffect(this, AURA_EFFECT_HANDLE_SEND_FOR_CLIENT, true) will reapply it if need
        if (target->getTransForm() == GetId())
            target->setTransForm(0);

        target->RestoreDisplayId();

        // Dragonmaw Illusion (restore mount model)
        if (GetId() == 42016 && target->GetMountID() == 16314)
        {
            if (!target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).empty())
            {
                uint32 cr_id = target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetMiscValue();
                if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(cr_id))
                {
                    CreatureModel model = *ObjectMgr::ChooseDisplayId(ci);
                    sObjectMgr->GetCreatureModelRandomGender(&model, ci);

                    target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, model.CreatureDisplayID);
                }
            }
        }
    }
}

void AuraEffect::HandleAuraModScale(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    aurApp->GetTarget()->RecalculateObjectScale();
}

void AuraEffect::HandleAuraCloneCaster(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        Unit* caster = GetCaster();
        if (!caster || caster == target)
            return;

        // What must be cloned? at least display and scale
        target->SetDisplayId(caster->GetDisplayId());
        //target->SetObjectScale(caster->GetObjectScale()); // we need retail info about how scaling is handled (aura maybe?)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    }
    else
    {
        target->SetDisplayId(target->GetNativeDisplayId());
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    }
}

/************************/
/***      FIGHT       ***/
/************************/

void AuraEffect::HandleFeignDeath(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        /*
        WorldPacket data(SMSG_FEIGN_DEATH_RESISTED, 0);
        target->SendMessageToSet(&data, true);
        */

        UnitList targets;
        Trinity::AnyUnfriendlyUnitInObjectRangeCheck u_check(target, target, target->GetMap()->GetVisibilityRange());
        Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(target, targets, u_check);
        Cell::VisitAllObjects(target, searcher, target->GetMap()->GetVisibilityRange());
        for (UnitList::iterator iter = targets.begin(); iter != targets.end(); ++iter)
        {
            if (!(*iter)->HasUnitState(UNIT_STATE_CASTING))
                continue;

            for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; i++)
            {
                if ((*iter)->GetCurrentSpell(i)
                && (*iter)->GetCurrentSpell(i)->m_targets.GetUnitTargetGUID() == target->GetGUID())
                {
                    (*iter)->InterruptSpell(CurrentSpellTypes(i), false);
                }
            }
        }
		
        for (auto& pair : target->GetThreatManager().GetThreatenedByMeList())
          pair.second->ScaleThreat(0.0f);

        if (target->GetMap()->IsDungeon()) // feign death does not remove combat in dungeons
        {
            target->AttackStop();
            if (Player* targetPlayer = target->ToPlayer())
                targetPlayer->SendAttackSwingCancelAttack();
        }
        else
            target->CombatStop(false, false);

        // prevent interrupt message
        if (GetCasterGUID() == target->GetGUID() && target->GetCurrentSpell(CURRENT_GENERIC_SPELL))
            target->FinishSpell(CURRENT_GENERIC_SPELL, false);
        target->InterruptNonMeleeSpells(true);

        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode().HasAnyFlag())
            return;

        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);            // blizz like 2.0.x
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);    // blizz like 2.0.x
        target->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);         // blizz like 2.0.x
        target->AddUnitState(UNIT_STATE_DIED);

        if (Creature* creature = target->ToCreature())
            creature->SetReactState(REACT_PASSIVE);
    }
    else
    {
        /*
        WorldPacket data(SMSG_FEIGN_DEATH_RESISTED, 0);
        target->SendMessageToSet(&data, true);
        */

        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);         // blizz like 2.0.x
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH); // blizz like 2.0.x
        target->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);      // blizz like 2.0.x
        target->ClearUnitState(UNIT_STATE_DIED);

        if (Creature* creature = target->ToCreature())
            creature->InitializeReactState();
    }
}

void AuraEffect::HandleModUnattackable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
    if (!apply && target->HasAuraType(SPELL_AURA_MOD_UNATTACKABLE))
        return;

    target->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2, apply);
    target->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE, apply);
}

void AuraEffect::HandleAuraModDisarm(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    //Prevent handling aura twice
    AuraType type = GetAuraType();
    if (apply ? target->GetAuraEffectsByType(type).size() > 1 : target->HasAuraType(type))
        return;

    // Prevent handling aura twice
    if (apply ? target->GetAuraEffectsByType(type).size() > 1 : target->HasAuraType(type))
        return;

    uint32 field, flag, slot;
    WeaponAttackType attType;
    switch (type)
    {
        case SPELL_AURA_MOD_DISARM:
            field = UNIT_FIELD_FLAGS;
            flag = UNIT_FLAG_DISARMED;
            slot = EQUIPMENT_SLOT_MAINHAND;
            attType = BASE_ATTACK;
            break;
        case SPELL_AURA_MOD_DISARM_OFFHAND:
            field = UNIT_FIELD_FLAGS_2;
            flag = UNIT_FLAG2_DISARM_OFFHAND;
            slot = EQUIPMENT_SLOT_OFFHAND;
            attType = OFF_ATTACK;
            break;
        case SPELL_AURA_MOD_DISARM_RANGED:
            field = UNIT_FIELD_FLAGS_2;
            flag = UNIT_FLAG2_DISARM_RANGED;
            slot = EQUIPMENT_SLOT_RANGED;
            attType = RANGED_ATTACK;
            break;
        default:
            return;
    }

    // set/remove flag before weapon bonuses so it's properly reflected in CanUseAttackType
    if (apply)
        target->SetFlag(field, flag);
    else
        target->RemoveFlag(field, flag);

    // Handle damage modification, shapeshifted druids are not affected
    if (target->GetTypeId() == TYPEID_PLAYER && !target->IsInFeralForm())
    {
        Player* player = target->ToPlayer();
        if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        {
            WeaponAttackType const attackType = Player::GetAttackBySlot(slot);

            player->ApplyItemDependentAuras(item, !apply);
            if (attackType != MAX_ATTACK)
            {
                player->_ApplyWeaponDamage(slot, item->GetTemplate(), !apply);
                if (!apply) // apply case already handled on item dependent aura removal (if any)
                    player->UpdateWeaponDependentAuras(attackType);
            }
        }
    }

    if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->GetCurrentEquipmentId())
        target->UpdateDamagePhysical(attType);
}

void AuraEffect::HandleAuraModSilence(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

        // call functions which may have additional effects after chainging state of unit
        // Stop cast only spells vs PreventionType == SPELL_PREVENTION_TYPE_SILENCE
        for (uint32 i = CURRENT_MELEE_SPELL; i < CURRENT_MAX_SPELL; ++i)
            if (Spell* spell = target->GetCurrentSpell(CurrentSpellTypes(i)))
                if (spell->m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_SILENCE)
                    // Stop spells on prepare or casting state
                    target->InterruptSpell(CurrentSpellTypes(i), false);
    }
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_SILENCE) || target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
    }
}

void AuraEffect::HandleAuraModPacify(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_PACIFY) || target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
    }
}

void AuraEffect::HandleAuraModPacifyAndSilence(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // Vengeance of the Blue Flight (@todo REMOVE THIS!)
    /// @workaround
    if (m_spellInfo->Id == 45839)
    {
        if (apply)
            target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        else
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }
    if (!(apply))
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;
    }
    HandleAuraModPacify(aurApp, mode, apply);
    HandleAuraModSilence(aurApp, mode, apply);
}

void AuraEffect::HandleAuraDisableCastingExceptAbilities(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->IsPlayer())
    {
        if (apply)
            target->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_DISABLE_CASTING_EXCEPT_ABILITIES);
        else
        {
            // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
            if (target->HasAuraType(SPELL_AURA_DISABLE_CASTING_EXCEPT_ABILITIES))
                return;
            target->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_DISABLE_CASTING_EXCEPT_ABILITIES);
        }
    }
}

void AuraEffect::HandleAuraDisableAttackingExceptAbilities(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->IsPlayer())
    {
        if (apply)
        {
            target->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_DISABLE_ATTACKING_EXCEPT_ABILITIES);
            target->AttackStop();
        }
        else
        {
            // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
            if (target->HasAuraType(SPELL_AURA_DISABLE_ATTACKING_EXCEPT_ABILITIES))
                return;
            target->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_DISABLE_ATTACKING_EXCEPT_ABILITIES);
        }
    }
}

void AuraEffect::HandleAuraModNoActions(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_NO_ACTIONS);

        // call functions which may have additional effects after chainging state of unit
        // Stop cast only spells vs PreventionType & SPELL_PREVENTION_TYPE_SILENCE
        for (uint32 i = CURRENT_MELEE_SPELL; i < CURRENT_MAX_SPELL; ++i)
            if (Spell* spell = target->GetCurrentSpell(CurrentSpellTypes(i)))
                if (spell->m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_NO_ACTIONS)
                    // Stop spells on prepare or casting state
                    target->InterruptSpell(CurrentSpellTypes(i), false);
    }
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit
        if (target->HasAuraType(SPELL_AURA_MOD_NO_ACTIONS))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_NO_ACTIONS);
    }
}

/****************************/
/***      TRACKING        ***/
/****************************/

void AuraEffect::HandleAuraTrackCreatures(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        target->SetFlag(PLAYER_TRACK_CREATURES, uint32(1) << (GetMiscValue() - 1));
    else
        target->RemoveFlag(PLAYER_TRACK_CREATURES, uint32(1) << (GetMiscValue() - 1));
}

void AuraEffect::HandleAuraTrackResources(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        target->SetFlag(PLAYER_TRACK_RESOURCES, uint32(1) << (GetMiscValue() - 1));
    else
        target->RemoveFlag(PLAYER_TRACK_RESOURCES, uint32(1) << (GetMiscValue() - 1));
}

void AuraEffect::HandleAuraTrackStealthed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!(apply))
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }
    target->ApplyModByteFlag(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_FLAGS, PLAYER_FIELD_BYTE_TRACK_STEALTHED, apply);
}

void AuraEffect::HandleAuraModStalked(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // used by spells: Hunter's Mark, Mind Vision, Syndicate Tracker (MURP) DND
    if (apply)
        target->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TRACK_UNIT);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (!target->HasAuraType(GetAuraType()))
            target->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TRACK_UNIT);
    }

    // call functions which may have additional effects after changing state of unit
    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraUntrackable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetByteFlag(UNIT_FIELD_BYTES_1, UNIT_BYTES_1_OFFSET_VIS_FLAG, UNIT_STAND_FLAGS_UNTRACKABLE);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveByteFlag(UNIT_FIELD_BYTES_1, UNIT_BYTES_1_OFFSET_VIS_FLAG, UNIT_STAND_FLAGS_UNTRACKABLE);
    }
}

/****************************/
/***  SKILLS & TALENTS    ***/
/****************************/

void AuraEffect::HandleAuraModPetTalentsPoints(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate pet talent points
    if (Pet* pet = target->ToPlayer()->GetPet())
        pet->InitTalentForLevel();
}

void AuraEffect::HandleAuraModSkill(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_SKILL)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    uint32 prot = GetMiscValue();
    int32 points = GetAmount();

    if (prot == SKILL_DEFENSE)
        return;

    target->ModifySkillBonus(prot, (apply ? points : -points), GetAuraType() == SPELL_AURA_MOD_SKILL_TALENT);
}

/****************************/
/***       MOVEMENT       ***/
/****************************/

void AuraEffect::HandleAuraMounted(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        if (mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK)
        {
            uint32 creatureEntry = GetMiscValue();
            uint32 displayId = 0;
            uint32 vehicleId = 0;

            // Festive Holiday Mount
            if (target->HasAura(62061))
            {
                if (GetBase()->HasEffectType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                    creatureEntry = 24906;
                else
                    creatureEntry = 15665;
            }

            if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creatureEntry))
            {
                vehicleId = creatureInfo->VehicleId;

                CreatureModel model = *ObjectMgr::ChooseDisplayId(creatureInfo);
                sObjectMgr->GetCreatureModelRandomGender(&model, creatureInfo);
                displayId = model.CreatureDisplayID;

                //some spell has one aura of mount and one of vehicle
                for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if (GetSpellInfo()->Effects[i].Effect == SPELL_EFFECT_SUMMON
                        && GetSpellInfo()->Effects[i].MiscValue == GetMiscValue())
                        displayId = 0;
            }

            target->Mount(displayId, vehicleId, creatureEntry);
        }

        // cast speed aura
        if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK)
            if (MountCapabilityEntry const* mountCapability = sMountCapabilityStore.LookupEntry(GetAmount()))
                target->CastSpell(target, mountCapability->ModSpellAuraID, true);
    }
    else
    {
        if (mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK)
            target->Dismount();

        //some mounts like Headless Horseman's Mount or broom stick are skill based spell
        // need to remove ALL arura related to mounts, this will stop client crash with broom stick
        // and never endless flying after using Headless Horseman's Mount
        if (mode & AURA_EFFECT_HANDLE_REAL)
            target->RemoveAurasByType(SPELL_AURA_MOUNTED);

        if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK)
            // remove speed aura
            if (MountCapabilityEntry const* mountCapability = sMountCapabilityStore.LookupEntry(GetAmount()))
                target->RemoveAurasDueToSpell(mountCapability->ModSpellAuraID, target->GetGUID());
    }
}

void AuraEffect::HandleAuraAllowFlight(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()) || target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
            return;
    }

    target->SetCanFly(apply);

    if (!apply && target->GetTypeId() == TYPEID_UNIT && !target->IsGravityDisabled())
        target->GetMotionMaster()->MoveFall();
}

void AuraEffect::HandleAuraWaterWalk(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetWaterWalking(apply);
}

void AuraEffect::HandleAuraFeatherFall(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetFeatherFall(apply);

    // start fall from current height
    if (!apply && target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->SetFallInformation(0, target->GetPositionZ());
}

void AuraEffect::HandleAuraHover(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetHover(apply);    //! Sets movementflags
}

void AuraEffect::HandleWaterBreathing(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // update timers in client
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateMirrorTimers();
}

void AuraEffect::HandleForceMoveForward(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FORCE_MOVEMENT);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FORCE_MOVEMENT);
    }
}

/****************************/
/***        THREAT        ***/
/****************************/

void AuraEffect::HandleModThreat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    aurApp->GetTarget()->GetThreatManager().UpdateMySpellSchoolModifiers();
}

void AuraEffect::HandleAuraModTotalThreat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->IsAlive() || target->GetTypeId() != TYPEID_PLAYER)
        return;

    Unit* caster = GetCaster();
    if (caster && caster->IsAlive())
        caster->GetThreatManager().UpdateMyTempModifiers();
}

void AuraEffect::HandleModTaunt(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->IsAlive() || !target->CanHaveThreatList())
        return;

    target->GetThreatManager().TauntUpdate();
}

void AuraEffect::HandleModDetaunt(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* caster = GetCaster();
    Unit* target = aurApp->GetTarget();

    if (!caster || !caster->IsAlive() || !target->IsAlive() || !caster->CanHaveThreatList())
        return;

    caster->GetThreatManager().TauntUpdate();
}

void AuraEffect::HandleAuraFixate(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target->IsAlive())
        return;

    Unit* caster = GetCaster();
    if (!caster || caster->GetTypeId() != TYPEID_UNIT || !caster->IsAlive())
        return;

    if (apply)
    {
        caster->EngageWithTarget(target);
        caster->GetThreatManager().FixateTarget(target);
    }
    else
        caster->GetThreatManager().ClearFixate();
}

/*****************************/
/***        CONTROL        ***/
/*****************************/

void AuraEffect::HandleModConfuse(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->SetControlled(apply, UNIT_STATE_CONFUSED);
    if (apply)
        target->GetThreatManager().EvaluateSuppressed();
}

void AuraEffect::HandleModFear(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->SetControlled(apply, UNIT_STATE_FLEEING);
}

void AuraEffect::HandleAuraModStun(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->SetControlled(apply, UNIT_STATE_STUNNED);
    if (apply)
        target->GetThreatManager().EvaluateSuppressed();
}

void AuraEffect::HandleAuraModRoot(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->SetControlled(apply, UNIT_STATE_ROOT);
}

void AuraEffect::HandlePreventFleeing(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    // Since patch 3.0.2 this mechanic no longer affects fear effects. It will ONLY prevent humanoids from fleeing due to low health.
    if (!apply || target->HasAuraType(SPELL_AURA_MOD_FEAR))
        return;
    /// TODO: find a way to cancel fleeing for assistance.
    /// Currently this will only stop creatures fleeing due to low health that could not find nearby allies to flee towards.
    target->SetControlled(false, UNIT_STATE_FLEEING);
}

/***************************/
/***        CHARM        ***/
/***************************/

void AuraEffect::HandleModPossess(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    // no support for posession AI yet
    if (caster && caster->GetTypeId() == TYPEID_UNIT)
    {
        HandleModCharm(aurApp, mode, apply);
        return;
    }

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_POSSESS, aurApp);
    else
        target->RemoveCharmedBy(caster);
}

void AuraEffect::HandleModCharm(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_CHARM, aurApp);
    else
        target->RemoveCharmedBy(caster);
}

void AuraEffect::HandleCharmConvert(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_CONVERT, aurApp);
    else
        target->RemoveCharmedBy(caster);
}

/**
 * Such auras are applied from a caster(=player) to a vehicle.
 * This has been verified using spell #49256
 */
void AuraEffect::HandleAuraControlVehicle(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target->IsVehicle())
        return;

    Unit* caster = GetCaster();
    if (!caster || caster == target)
        return;

    if (apply)
    {
        // Currently spells that have base points  0 and DieSides 0 = "0/0" exception are pushed to -1,
        // however the idea of 0/0 is to ingore flag VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT and -1 checks for it,
        // so this break such spells or most of them.
        // Current formula about m_amount: effect base points + dieside - 1
        // TO DO: Reasearch more about 0/0 and fix it.
        caster->_EnterVehicle(target->GetVehicleKit(), _amount - 1, aurApp);
    }
    else
    {
        // Remove pending passengers before exiting vehicle - might cause an Uninstall
        target->GetVehicleKit()->RemovePendingEventsForPassenger(caster);

        if (GetId() == 53111) // Devour Humanoid
        {
            Unit::Kill(target, caster);
            if (caster->GetTypeId() == TYPEID_UNIT)
                caster->ToCreature()->DespawnOrUnsummon();
        }

        bool seatChange = (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT)                             // Seat change on the same direct vehicle
            || target->HasAuraTypeWithCaster(SPELL_AURA_CONTROL_VEHICLE, caster->GetGUID());    // Seat change to a proxy vehicle (for example turret mounted on a siege engine)

        if (!seatChange)
            caster->_ExitVehicle();
        else
            target->GetVehicleKit()->RemovePassenger(caster);  // Only remove passenger from vehicle without launching exit movement or despawning the vehicle

        // some SPELL_AURA_CONTROL_VEHICLE auras have a dummy effect on the player - remove them
        caster->RemoveAurasDueToSpell(GetId());
    }
}

/*********************************************************/
/***                  MODIFY SPEED                     ***/
/*********************************************************/
void AuraEffect::HandleAuraModIncreaseSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_WALK);
    target->UpdateSpeed(MOVE_RUN);
}

void AuraEffect::HandleAuraModIncreaseMountedSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    HandleAuraModIncreaseSpeed(aurApp, mode, apply);
}

void AuraEffect::HandleAuraModIncreaseFlightSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK)
        target->UpdateSpeed(MOVE_FLIGHT);

    //! Update ability to fly
    if (GetAuraType() == SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK && (apply || (!target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !target->HasAuraType(SPELL_AURA_FLY))))
        {
            if (Player* player = target->ToPlayer())
                player->SetCanTransitionBetweenSwimAndFly(apply);

            target->SetCanFly(apply);

            if (!apply && target->GetTypeId() == TYPEID_UNIT && !target->IsGravityDisabled())
                target->GetMotionMaster()->MoveFall();
        }

        //! Someone should clean up these hacks and remove it from this function. It doesn't even belong here.
        if (mode & AURA_EFFECT_HANDLE_REAL)
        {
            //Players on flying mounts must be immune to polymorph
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);

            // Dragonmaw Illusion (overwrite mount model, mounted aura already applied)
            if (apply && target->HasAuraEffect(42016, 0) && target->GetMountID())
                target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 16314);
        }
    }
    if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK)
        target->UpdateSpeed(MOVE_FLIGHT);
}

void AuraEffect::HandleAuraModIncreaseSwimSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_SWIM);
}

void AuraEffect::HandleAuraModDecreaseSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_RUN);
    target->UpdateSpeed(MOVE_SWIM);
    target->UpdateSpeed(MOVE_FLIGHT);
    target->UpdateSpeed(MOVE_RUN_BACK);
    target->UpdateSpeed(MOVE_SWIM_BACK);
    target->UpdateSpeed(MOVE_FLIGHT_BACK);
}

void AuraEffect::HandleAuraModUseNormalSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_RUN);
    target->UpdateSpeed(MOVE_SWIM);
    target->UpdateSpeed(MOVE_FLIGHT);
}

/*********************************************************/
/***                     IMMUNITY                      ***/
/*********************************************************/

void AuraEffect::HandleModMechanicImmunityMask(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);
}

void AuraEffect::HandleModMechanicImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);
}

void AuraEffect::HandleAuraModEffectImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);

    // when removing flag aura, handle flag drop
    // TODO: this should be handled in aura script for flag spells using AfterEffectRemove hook
    Player* player = target->ToPlayer();
    if (!apply && player && GetSpellInfo()->HasAuraInterruptFlag(SpellAuraInterruptFlags::StealthOrInvis))
    {
        if (player->InBattleground())
        {
            if (Battleground* bg = player->GetBattleground())
                bg->EventPlayerDroppedFlag(player);
        }
        else
            sOutdoorPvPMgr->HandleDropFlag(player, GetSpellInfo()->Id);
    }
}

void AuraEffect::HandleAuraModStateImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);
}

void AuraEffect::HandleAuraModSchoolImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);

    if (GetSpellInfo()->Mechanic == MECHANIC_BANISH)
    {
        if (apply)
            target->AddUnitState(UNIT_STATE_ISOLATED);
        else
        {
            bool banishFound = false;
            Unit::AuraEffectList const& banishAuras = target->GetAuraEffectsByType(GetAuraType());
            for (AuraEffect const* aurEff : banishAuras)
            {
                if (aurEff->GetSpellInfo()->Mechanic == MECHANIC_BANISH)
                {
                    banishFound = true;
                    break;
                }
            }

            if (!banishFound)
                target->ClearUnitState(UNIT_STATE_ISOLATED);
        }
    }

    // TODO: should be changed to a proc script on flag spell (they have "Taken positive" proc flags in db2)
    {
        if (apply && GetMiscValue() == SPELL_SCHOOL_MASK_NORMAL)
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::StealthOrInvis);

        // remove all flag auras (they are positive, but they must be removed when you are immune)
        if (GetSpellInfo()->HasAttribute(SPELL_ATTR1_IMMUNITY_PURGES_EFFECT)
            && GetSpellInfo()->HasAttribute(SPELL_ATTR2_FAIL_ON_ALL_TARGETS_IMMUNE))
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::StealthOrInvis);
    }

    if (apply)
    {
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE);
        target->GetThreatManager().EvaluateSuppressed();
    }
    else
    { 
        // do not remove unit flag if there are more than this auraEffect of that kind on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE);
    }
}

void AuraEffect::HandleAuraModDmgImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);

    if (apply)
        target->GetThreatManager().EvaluateSuppressed();
}

void AuraEffect::HandleAuraModDispelImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    m_spellInfo->ApplyAllSpellImmunitiesTo(target, GetEffIndex(), apply);
}

/*********************************************************/
/***                  MODIFY STATS                     ***/
/*********************************************************/

/********************************/
/***        RESISTANCE        ***/
/********************************/

void AuraEffect::HandleAuraModResistanceExclusive(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (uint8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; ++x)
    {
        if (GetMiscValue() & (1 << x))
        {
            int32 amount = target->GetMaxPositiveAuraModifierByMiscMask(SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE, 1 << x, this);
            if (amount < GetAmount())
            {
                float value = float(GetAmount() - amount);
                target->HandleStatFlatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), BASE_VALUE, value, apply);
                if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                    target->UpdateResistanceBuffModsMod(SpellSchools(x));
            }
        }
    }
}

void AuraEffect::HandleAuraModResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (uint8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; ++x)
    {
        if (GetMiscValue() & (1 << x))
        {
            target->HandleStatFlatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), TOTAL_VALUE, float(GetAmount()), apply);
            if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                target->UpdateResistanceBuffModsMod(SpellSchools(x));
        }
    }
}

void AuraEffect::HandleAuraModBaseResistancePCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        //pets only have base armor
        if (target->IsPet() && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
        {
            if (apply)
                target->ApplyStatPctModifier(UNIT_MOD_ARMOR, BASE_PCT, float(GetAmount()));
            else
            {
                float amount = target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_BASE_RESISTANCE_PCT, SPELL_SCHOOL_MASK_NORMAL);
                target->SetStatPctModifier(UNIT_MOD_ARMOR, BASE_PCT, amount);
            }
        }
    }
    else
    {
        for (uint8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; ++x)
        {
            if (GetMiscValue() & (1 << x))
            {
                if (apply)
                    target->ApplyStatPctModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), BASE_PCT, float(GetAmount()));
                else
                {
                    float amount = target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_BASE_RESISTANCE_PCT, 1 << x);
                    target->SetStatPctModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), BASE_PCT, amount);
                }
            }
        }
    }
}

void AuraEffect::HandleModResistancePercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (uint8 i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
    {
        if (GetMiscValue() & (1 << i))
        {
            float amount = target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_RESISTANCE_PCT, 1 << i);
            if (target->GetPctModifierValue(UnitMods(UNIT_MOD_RESISTANCE_START + i), TOTAL_PCT) == amount)
                continue;

            target->SetStatPctModifier(UnitMods(UNIT_MOD_RESISTANCE_START + i), TOTAL_PCT, amount);
            if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                target->UpdateResistanceBuffModsMod(SpellSchools(i));
        }
    }
}

void AuraEffect::HandleModBaseResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        //pets only have base armor
        if (target->IsPet() && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
            target->HandleStatFlatModifier(UNIT_MOD_ARMOR, TOTAL_VALUE, float(GetAmount()), apply);
    }
    else
    {
        for (uint8 i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
            if (GetMiscValue() & (1 << i))
                target->HandleStatFlatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + i), TOTAL_VALUE, float(GetAmount()), apply);
    }
}

void AuraEffect::HandleModTargetResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // applied to damage as HandleNoImmediateEffect in Unit::CalcAbsorbResist and Unit::CalcArmorReducedDamage

    // show armor penetration
    if (target->GetTypeId() == TYPEID_PLAYER && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
        target->ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, GetAmount(), apply);

    // show as spell penetration only full spell penetration bonuses (all resistances except armor and holy
    if (target->GetTypeId() == TYPEID_PLAYER && (GetMiscValue() & SPELL_SCHOOL_MASK_SPELL) == SPELL_SCHOOL_MASK_SPELL)
        target->ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, GetAmount(), apply);
}

/********************************/
/***           STAT           ***/
/********************************/

void AuraEffect::HandleAuraModStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    if (GetMiscValue() < -2 || GetMiscValue() > 4)
    {
        TC_LOG_ERROR("spells", "WARNING: Spell %u effect %u has an unsupported misc value (%i) for SPELL_AURA_MOD_STAT ", GetId(), GetEffIndex(), GetMiscValue());
        return;
    }

    Unit* target = aurApp->GetTarget();
    int32 spellGroupVal = target->GetHighestExclusiveSameEffectSpellGroupValue(this, SPELL_AURA_MOD_STAT, true, GetMiscValue());
    if (abs(spellGroupVal) >= abs(GetAmount()))
        return;

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        // -1 or -2 is all stats (misc < -2 checked in function beginning)
        if (GetMiscValue() < 0 || GetMiscValue() == i)
        {
            if (spellGroupVal)
            {
                target->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(spellGroupVal), !apply);
                if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                    target->UpdateStatBuffMod(Stats(i));
            }

            target->HandleStatFlatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(GetAmount()), apply);
            if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                target->UpdateStatBuffMod(Stats(i));
        }
    }
}

void AuraEffect::HandleModPercentStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (GetMiscValue() < -1 || GetMiscValue() > 4)
    {
        TC_LOG_ERROR("spells", "WARNING: Misc Value for SPELL_AURA_MOD_PERCENT_STAT not valid");
        return;
    }

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        if (GetMiscValue() == i || GetMiscValue() == -1)
        {
            if (apply)
                target->ApplyStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), BASE_PCT, float(GetAmount()));
            else
            {
                float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_PERCENT_STAT, [i](AuraEffect const* aurEff) -> bool
                {
                    if (aurEff->GetMiscValue() == i || aurEff->GetMiscValue() == -1)
                        return true;
                    return false;
                });
                target->SetStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), BASE_PCT, amount);
            }
        }
    }
}

void AuraEffect::HandleModSpellDamagePercentFromStat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Magic damage modifiers implemented in Unit::SpellDamageBonus
    // This information for client side use only
    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellHealingPercentFromStat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellDamagePercentFromAttackPower(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Magic damage modifiers implemented in Unit::SpellDamageBonus
    // This information for client side use only
    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellHealingPercentFromAttackPower(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModHealingDone(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;
    // implemented in Unit::SpellHealingBonus
    // this information is for client side only
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModTotalPercentStat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    if (GetMiscValue() < -1 || GetMiscValue() > 4)
    {
        TC_LOG_ERROR("spells", "WARNING: Misc Value for SPELL_AURA_MOD_PERCENT_STAT not valid");
        return;
    }

    Unit* target = aurApp->GetTarget();

    // save current health state
    float healthPct = target->GetHealthPct();
    bool zeroHealth = !target->IsAlive();

    // players in corpse state may mean two different states:
    /// 1. player just died but did not release (in this case health == 0)
    /// 2. player is corpse running (ie ghost) (in this case health == 1)
    if (target->getDeathState() == CORPSE)
        zeroHealth = (target->GetHealth() == 0);

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        if ((GetMiscValueB() & 1 << i) || !GetMiscValueB()) // affect the same stats
        {
            float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, [i](AuraEffect const* aurEff) -> bool
            {
                if ((aurEff->GetMiscValueB() & 1 << i) || aurEff->GetMiscValueB() == -1)
                    return true;
                return false;
            });

            if (target->GetPctModifierValue(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT) == amount)
                continue;

            target->SetStatPctModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, amount);
            if (target->GetTypeId() == TYPEID_PLAYER || target->IsPet())
                target->UpdateStatBuffMod(Stats(i));
        }
    }

    // recalculate current HP/MP after applying aura modifications (only for spells with SPELL_ATTR0_IS_ABILITY 0x00000010 flag)
    // this check is total bullshit i think
    if (((GetMiscValueB() & 1 << STAT_STAMINA) || GetMiscValueB() == - 1) && m_spellInfo->HasAttribute(SPELL_ATTR0_IS_ABILITY))
        target->SetHealth(std::max<uint32>(CalculatePct(target->GetMaxHealth(), healthPct), (zeroHealth ? 0 : 1)));
}

void AuraEffect::HandleAuraModResistenceOfStatPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (GetMiscValue() != SPELL_SCHOOL_MASK_NORMAL)
    {
        // support required adding replace UpdateArmor by loop by UpdateResistence at intellect update
        // and include in UpdateResistence same code as in UpdateArmor for aura mod apply.
        TC_LOG_ERROR("spells", "Aura SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT(182) does not work for non-armor type resistances!");
        return;
    }

    // Recalculate Armor
    target->UpdateArmor();
}

void AuraEffect::HandleAuraModExpertise(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateExpertise(BASE_ATTACK);
    target->ToPlayer()->UpdateExpertise(OFF_ATTACK);
}

/********************************/
/***      HEAL & ENERGIZE     ***/
/********************************/
void AuraEffect::HandleModPowerRegen(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    // Update power regen values
    aurApp->GetTarget()->UpdatePowerRegeneration(Powers(GetMiscValue()));
}

void AuraEffect::HandleModPowerRegenPCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    HandleModPowerRegen(aurApp, mode, apply);
}

void AuraEffect::HandleModHealingPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateSpellHealingPercentTaken();
}

void AuraEffect::HandleModHealingDonePercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateSpellHealingPercentDone();
}

void AuraEffect::HandleModManaRegen(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    //Note: an increase in regen does NOT cause threat.
    target->ToPlayer()->UpdatePowerRegeneration(POWER_MANA);
}

void AuraEffect::HandleAuraModIncreaseHealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->HandleStatFlatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(GetAmount()), apply);
        target->ModifyHealth(GetAmount());
    }
    else
    {
        if (target->GetHealth() > 0)
        {
            int32 value = std::min<int32>(target->GetHealth() - 1, GetAmount());
            target->ModifyHealth(-value);
        }
        target->HandleStatFlatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(GetAmount()), apply);
    }
}

void AuraEffect::HandleAuraModIncreaseMaxHealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    float percent = target->GetHealthPct();

    target->HandleStatFlatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(GetAmount()), apply);

    // refresh percentage
    if (target->GetHealth() > 0)
    {
        uint32 newHealth = std::max<uint32>(target->CountPctFromMaxHealth(int32(percent)), 1);
        target->SetHealth(newHealth);
    }
}

void AuraEffect::HandleAuraModIncreaseEnergy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    Powers powerType = Powers(GetMiscValue());
    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + AsUnderlyingType(powerType));

    target->HandleStatFlatModifier(unitMod, TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModIncreaseEnergyPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    Powers powerType = Powers(GetMiscValue());
    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + AsUnderlyingType(powerType));

    // Save old powers for further calculation
    int32 oldPower = int32(target->GetPower(powerType));
    int32 oldMaxPower = int32(target->GetMaxPower(powerType));

    // Handle aura effect for max power
    if (apply)
    {
        float amount = float(GetAmount());
        target->ApplyStatPctModifier(unitMod, TOTAL_PCT, amount);
    }
    else
    {
        float amount = target->GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT, GetMiscValue());
        target->SetStatPctModifier(unitMod, TOTAL_PCT, amount);
    }

    // Calculate the current power change
    int32 change = target->GetMaxPower(powerType) - oldMaxPower;
    change = (oldPower + change) - target->GetPower(powerType);
    target->ModifyPower(powerType, change);
}

void AuraEffect::HandleAuraModIncreaseHealthPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Unit will keep hp% after MaxHealth being modified if unit is alive.
    float percent = target->GetHealthPct();
    if (apply)
        target->ApplyStatPctModifier(UNIT_MOD_HEALTH, TOTAL_PCT, float(GetAmount()));
    else
    {
        float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
        target->SetStatPctModifier(UNIT_MOD_HEALTH, TOTAL_PCT, amount);
    }

    if (target->GetHealth() > 0)
    {
        uint32 newHealth = std::max<uint32>(CalculatePct(target->GetMaxHealth(), percent), 1);
        target->SetHealth(newHealth);
    }
}

void AuraEffect::HandleAuraIncreaseBaseHealthPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->ApplyStatPctModifier(UNIT_MOD_HEALTH, BASE_PCT, float(GetAmount()));
    else
    {
        float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_BASE_HEALTH_PCT);
        target->SetStatPctModifier(UNIT_MOD_HEALTH, BASE_PCT, amount);
    }
}

/********************************/
/***          FIGHT           ***/
/********************************/

void AuraEffect::HandleAuraModParryPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateParryPercentage();
}

void AuraEffect::HandleAuraModDodgePercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateDodgePercentage();
}

void AuraEffect::HandleAuraModBlockPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateBlockPercentage();
}

void AuraEffect::HandleAuraModRegenInterrupt(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    HandleModManaRegen(aurApp, mode, apply);
}

void AuraEffect::HandleAuraModWeaponCritPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target)
        return;

    target->UpdateAllWeaponDependentCritAuras();
}

void AuraEffect::HandleModHitChance(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateHitChances();
}

void AuraEffect::HandleModSpellHitChance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateSpellHitChances();
    else
        target->m_modSpellHitChance += (apply) ? GetAmount(): (-GetAmount());
}

void AuraEffect::HandleModSpellCritChance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAllSpellCritChances();
    else
        target->m_baseSpellCritChance += (apply) ? GetAmount():-GetAmount();
}

void AuraEffect::HandleModSpellCritChanceShool(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (int school = SPELL_SCHOOL_NORMAL; school < MAX_SPELL_SCHOOL; ++school)
        if (GetMiscValue() & (1<<school))
            target->ToPlayer()->UpdateSpellCritChance(school);
}

void AuraEffect::HandleAuraModCritPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        target->m_baseSpellCritChance += (apply) ? GetAmount() : -GetAmount();
        return;
    }

    target->ToPlayer()->UpdateAllWeaponDependentCritAuras();

    // included in Player::UpdateSpellCritChance calculation
    target->ToPlayer()->UpdateAllSpellCritChances();
}

/********************************/
/***         ATTACK SPEED     ***/
/********************************/

// There are some SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK effects which cause spells to become instant casts.
// In this case we do not want these effects to affect the global haste of spells but only the casting time.
static constexpr uint16 CAST_HASTE_AMOUNT_THRESHOLD = 1000;

void AuraEffect::HandleModCastingSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    int32 spellGroupVal = target->GetHighestExclusiveSameEffectSpellGroupValue(this, GetAuraType());
    if (abs(spellGroupVal) >= abs(GetAmount()))
        return;

    if (spellGroupVal)
        target->ApplyCastTimePercentMod((float)GetAmount(), !apply, GetAmount() < CAST_HASTE_AMOUNT_THRESHOLD);

    target->ApplyCastTimePercentMod((float)GetAmount(), apply, GetAmount() < CAST_HASTE_AMOUNT_THRESHOLD);
}

void AuraEffect::HandleModMeleeRangedSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    //! ToDo: Haste auras with the same handler _CAN'T_ stack together
    Unit* target = aurApp->GetTarget();

    target->ApplyAttackTimePercentMod(BASE_ATTACK, (float)GetAmount(), apply);
    target->ApplyAttackTimePercentMod(OFF_ATTACK, (float)GetAmount(), apply);
    target->ApplyAttackTimePercentMod(RANGED_ATTACK, (float)GetAmount(), apply);

    if (GetAuraType() != SPELL_AURA_MOD_MELEE_RANGED_HASTE_2 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2)
    {
        target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), apply);
        target->ApplyHasteRegenMod(RANGED_ATTACK, (float)GetAmount(), apply);
    }
}

void AuraEffect::HandleModCombatSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    int32 spellGroupVal = target->GetHighestExclusiveSameEffectSpellGroupValue(this, SPELL_AURA_MELEE_SLOW);
    if (abs(spellGroupVal) >= abs(GetAmount()))
        return;

    if (spellGroupVal)
    {
        target->ApplyCastTimePercentMod(float(spellGroupVal), !apply);
        target->ApplyAttackTimePercentMod(BASE_ATTACK, float(spellGroupVal), !apply);
        target->ApplyAttackTimePercentMod(OFF_ATTACK, float(spellGroupVal), !apply);
        target->ApplyAttackTimePercentMod(RANGED_ATTACK, float(spellGroupVal), !apply);
    }
    target->ApplyCastTimePercentMod(float(GetAmount()), apply);
    target->ApplyAttackTimePercentMod(BASE_ATTACK, float(GetAmount()), apply);
    target->ApplyAttackTimePercentMod(OFF_ATTACK, float(GetAmount()), apply);
    target->ApplyAttackTimePercentMod(RANGED_ATTACK, float(GetAmount()), apply);
    if (GetAuraType() != SPELL_AURA_MOD_MELEE_RANGED_HASTE_2 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2)
    {
        target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), apply);
        target->ApplyHasteRegenMod(RANGED_ATTACK, (float)GetAmount(), apply);
    }
}

void AuraEffect::HandleModAttackSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    target->ApplyAttackTimePercentMod(BASE_ATTACK, float(GetAmount()), apply);
    if (GetAuraType() != SPELL_AURA_MOD_MELEE_RANGED_HASTE_2 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2)
        target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), apply);
    target->UpdateDamagePhysical(BASE_ATTACK);
}

void AuraEffect::HandleModMeleeSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    bool applyRegenPct = GetAuraType() != SPELL_AURA_MOD_MELEE_HASTE_3 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2;

    int32 spellGroupVal = target->GetHighestExclusiveSameEffectSpellGroupValue(this, GetAuraType());
    if (abs(spellGroupVal) >= abs(GetAmount()))
        return;

    if (spellGroupVal)
    {
        if (target->GetTypeId() == TYPEID_PLAYER && applyRegenPct && GetBase()->HasEffectType(SPELL_AURA_MOD_MELEE_HASTE_3))
            target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), !apply);
        else
        {
            target->ApplyAttackTimePercentMod(BASE_ATTACK, (float)GetAmount(), !apply);
            target->ApplyAttackTimePercentMod(OFF_ATTACK, (float)GetAmount(), !apply);
            if (applyRegenPct)
                target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), !apply);
        }
    }

    // Auras that only increase regeneration
    if (target->GetTypeId() == TYPEID_PLAYER && applyRegenPct && GetBase()->HasEffectType(SPELL_AURA_MOD_MELEE_HASTE_3))
        target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), apply);
    else
    {
        target->ApplyAttackTimePercentMod(BASE_ATTACK, (float)GetAmount(), apply);
        target->ApplyAttackTimePercentMod(OFF_ATTACK, (float)GetAmount(), apply);
        if (applyRegenPct)
            target->ApplyHasteRegenMod(BASE_ATTACK, (float)GetAmount(), apply);
    }
}

void AuraEffect::HandleAuraModRangedHaste(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    int32 spellGroupVal = target->GetHighestExclusiveSameEffectSpellGroupValue(this, GetAuraType());
    if (abs(spellGroupVal) >= abs(GetAmount()))
        return;

    if (spellGroupVal)
    {
        target->ApplyAttackTimePercentMod(RANGED_ATTACK, (float)GetAmount(), !apply);
        if (GetAuraType() != SPELL_AURA_MOD_MELEE_RANGED_HASTE_2 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2)
            target->ApplyHasteRegenMod(RANGED_ATTACK, (float)GetAmount(), !apply);
    }

    target->ApplyAttackTimePercentMod(RANGED_ATTACK, (float)GetAmount(), apply);
    if (GetAuraType() != SPELL_AURA_MOD_MELEE_RANGED_HASTE_2 && GetAuraType() != SPELL_AURA_MOD_RANGED_HASTE_2)
        target->ApplyHasteRegenMod(RANGED_ATTACK, (float)GetAmount(), apply);
}

/********************************/
/***       COMBAT RATING      ***/
/********************************/

void AuraEffect::HandleModRating(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
        if (GetMiscValue() & (1 << rating))
            target->ToPlayer()->ApplyRatingMod(CombatRating(rating), GetAmount(), apply);
}

void AuraEffect::HandleModRatingFromStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Just recalculate ratings
    for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
        if (GetMiscValue() & (1 << rating))
            target->ToPlayer()->ApplyRatingMod(CombatRating(rating), 0, apply);
}

/********************************/
/***        ATTACK POWER      ***/
/********************************/

void AuraEffect::HandleAuraModAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    target->HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModRangedAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if ((target->getClassMask() & CLASSMASK_WAND_USERS) != 0)
        return;

    target->HandleStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModAttackPowerPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    //UNIT_FIELD_ATTACK_POWER_MULTIPLIER = multiplier - 1
    if (apply)
        target->ApplyStatPctModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, float(GetAmount()));
    else
    {
        float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_ATTACK_POWER_PCT);
        target->SetStatPctModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, amount);
    }
}

void AuraEffect::HandleAuraModRangedAttackPowerPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if ((target->getClassMask() & CLASSMASK_WAND_USERS) != 0)
        return;

    //UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = multiplier - 1
    if (apply)
        target->ApplyStatPctModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_PCT, float(GetAmount()));
    else
    {
        float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT);
        target->SetStatPctModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_PCT, amount);
    }
}

void AuraEffect::HandleAuraModAttackPowerOfArmor(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Recalculate bonus
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAttackPowerAndDamage(false);
}

void AuraEffect::HandleOverrideSpellPowerByAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->ApplyModSignedFloatValue(PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_AP_PCT, float(GetAmount()), apply);
    target->UpdateSpellDamageAndHealingBonus();
}

/********************************/
/***        DAMAGE BONUS      ***/
/********************************/
void AuraEffect::HandleModDamageDone(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
        target->UpdateAllDamageDoneMods();

    // Magic damage modifiers implemented in Unit::SpellBaseDamageBonusDone
    // This information for client side use only
    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        uint16 baseField = GetAmount() >= 0 ? PLAYER_FIELD_MOD_DAMAGE_DONE_POS : PLAYER_FIELD_MOD_DAMAGE_DONE_NEG;
        for (uint16 i = 0; i < MAX_SPELL_SCHOOL; ++i)
            if (GetMiscValue() & (1 << i))
                target->ApplyModUInt32Value(baseField + i, GetAmount(), apply);

        if (Guardian* pet = target->ToPlayer()->GetGuardianPet())
            pet->UpdateAttackPowerAndDamage();
    }
}

void AuraEffect::HandleModDamagePercentDone(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // also handles spell group stacks
    if (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
        target->UpdateAllDamagePctDoneMods();

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        for (uint8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
        {
            if (GetMiscValue() & (1 << i))
            {
                // only aura type modifying PLAYER_FIELD_MOD_DAMAGE_DONE_PCT
                float amount = target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, 1 << i);
                target->SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + i, amount);
            }
        }
    }
}

void AuraEffect::HandleModOffhandDamagePercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // also handles spell group stacks
    target->UpdateDamagePctDoneMods(OFF_ATTACK);
}

void AuraEffect::HandleShieldBlockValue(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->HandleBaseModFlatValue(SHIELD_BLOCK_VALUE, float(GetAmount()), apply);
}

/********************************/
/***        POWER COST        ***/
/********************************/

void AuraEffect::HandleModPowerCostPCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    float amount = CalculatePct(1.0f, GetAmount());
    for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if (GetMiscValue() & (1 << i))
            target->ApplyModSignedFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + i, amount, apply);
}

void AuraEffect::HandleShieldBlockValuePercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    if (apply)
        target->ApplyBaseModPctValue(SHIELD_BLOCK_VALUE, float(GetAmount()));
    else
    {
        float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_SHIELD_BLOCKVALUE_PCT);
        target->SetBaseModPctValue(SHIELD_BLOCK_VALUE, amount);
    }
}

void AuraEffect::HandleModPowerCost(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if (GetMiscValue() & (1<<i))
            target->ApplyModInt32Value(UNIT_FIELD_POWER_COST_MODIFIER+i, GetAmount(), apply);
}

void AuraEffect::HandleArenaPreparation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    }
}

void AuraEffect::HandleNoReagentUseAura(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    flag96 mask;
    Unit::AuraEffectList const& noReagent = target->GetAuraEffectsByType(SPELL_AURA_NO_REAGENT_USE);
        for (Unit::AuraEffectList::const_iterator i = noReagent.begin(); i != noReagent.end(); ++i)
            mask |= (*i)->m_spellInfo->Effects[(*i)->m_effIndex].SpellClassMask;

    target->SetUInt32Value(PLAYER_NO_REAGENT_COST_1  , mask[0]);
    target->SetUInt32Value(PLAYER_NO_REAGENT_COST_1+1, mask[1]);
    target->SetUInt32Value(PLAYER_NO_REAGENT_COST_1+2, mask[2]);
}

void AuraEffect::HandleAuraRetainComboPoints(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // combo points was added in SPELL_EFFECT_ADD_COMBO_POINTS handler
    // remove only if aura expire by time (in case combo points amount change aura removed without combo points lost)
    if (!(apply) && GetBase()->GetDuration() == 0 && target->ToPlayer()->GetComboTarget())
        if (Unit* unit = ObjectAccessor::GetUnit(*target, target->ToPlayer()->GetComboTarget()))
            target->ToPlayer()->AddComboPoints(unit, -GetAmount());
}

/*********************************************************/
/***                    OTHERS                         ***/
/*********************************************************/

void AuraEffect::HandleAuraDummy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_REAPPLY)))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    // pet auras
    if (target->GetTypeId() == TYPEID_PLAYER && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        if (PetAura const* petSpell = sSpellMgr->GetPetAura(GetId(), m_effIndex))
        {
            if (apply)
                target->ToPlayer()->AddPetAura(petSpell);
            else
                target->ToPlayer()->RemovePetAura(petSpell);
        }
    }

    if (mode & (AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY))
    {
        // AT APPLY
        if (apply)
        {
            switch (GetId())
            {
                case 1515:                                      // Tame beast
                    // FIX_ME: this is 2.0.12 threat effect replaced in 2.1.x by dummy aura, must be checked for correctness
                    if (caster && target->CanHaveThreatList())
                        target->GetThreatManager().AddThreat(caster, 10.0f);
                    break;
                case 13139:                                     // net-o-matic
                    // root to self part of (root_target->charge->root_self sequence
                    if (caster)
                        caster->CastSpell(caster, 13138, this);
                    break;
                case 34026:   // kill command
                {
                    Unit* pet = target->GetGuardianPet();
                    if (!pet)
                        break;

                    target->CastSpell(target, 34027, this);

                    // set 3 stacks and 3 charges (to make all auras not disappear at once)
                    Aura* owner_aura = target->GetAura(34027, GetCasterGUID());
                    Aura* pet_aura  = pet->GetAura(58914, GetCasterGUID());
                    if (owner_aura)
                    {
                        owner_aura->SetStackAmount(owner_aura->GetSpellInfo()->StackAmount);
                        if (pet_aura)
                        {
                            pet_aura->SetCharges(0);
                            pet_aura->SetStackAmount(owner_aura->GetSpellInfo()->StackAmount);
                        }
                    }
                    break;
                }
                case 37096:                                     // Blood Elf Illusion
                {
                    if (caster)
                    {
                        if (caster->getGender() == GENDER_FEMALE)
                            caster->CastSpell(target, 37095, this); // Blood Elf Disguise
                        else
                            caster->CastSpell(target, 37093, this);
                    }
                    break;
                }
                case 39850:                                     // Rocket Blast
                    if (roll_chance_i(20))                       // backfire stun
                        target->CastSpell(target, 51581, this);
                    break;
                case 43873:                                     // Headless Horseman Laugh
                    target->PlayDistanceSound(11965);
                    break;
                case 46354:                                     // Blood Elf Illusion
                    if (caster)
                    {
                        if (caster->getGender() == GENDER_FEMALE)
                            caster->CastSpell(target, 46356, this);
                        else
                            caster->CastSpell(target, 46355, this);
                    }
                    break;
                case 46361:                                     // Reinforced Net
                    if (caster)
                        target->GetMotionMaster()->MoveFall();
                    break;
            }
        }
        // AT REMOVE
        else
        {
            if ((GetSpellInfo()->IsQuestTame()) && caster && caster->IsAlive() && target->IsAlive())
            {
                uint32 finalSpelId = 0;
                switch (GetId())
                {
                    case 19548: finalSpelId = 19597; break;
                    case 19674: finalSpelId = 19677; break;
                    case 19687: finalSpelId = 19676; break;
                    case 19688: finalSpelId = 19678; break;
                    case 19689: finalSpelId = 19679; break;
                    case 19692: finalSpelId = 19680; break;
                    case 19693: finalSpelId = 19684; break;
                    case 19694: finalSpelId = 19681; break;
                    case 19696: finalSpelId = 19682; break;
                    case 19697: finalSpelId = 19683; break;
                    case 19699: finalSpelId = 19685; break;
                    case 19700: finalSpelId = 19686; break;
                    case 30646: finalSpelId = 30647; break;
                    case 30653: finalSpelId = 30648; break;
                    case 30654: finalSpelId = 30652; break;
                    case 30099: finalSpelId = 30100; break;
                    case 30102: finalSpelId = 30103; break;
                    case 30105: finalSpelId = 30104; break;
                }

                if (finalSpelId)
                    caster->CastSpell(target, finalSpelId, this);
            }

            switch (m_spellInfo->SpellFamilyName)
            {
                case SPELLFAMILY_GENERIC:
                    switch (GetId())
                    {
                        case 2584: // Waiting to Resurrect
                            // Waiting to resurrect spell cancel, we must remove player from resurrect queue
                            if (target->GetTypeId() == TYPEID_PLAYER)
                            {
                                if (Battleground* bg = target->ToPlayer()->GetBattleground())
                                    bg->RemovePlayerFromResurrectQueue(target->GetGUID());
                                if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(target->GetMap(), target->GetZoneId()))
                                    bf->RemovePlayerFromResurrectQueue(target->GetGUID());
                            }
                            break;
                        case 36730:                                     // Flame Strike
                            target->CastSpell(target, 36731, this);
                            break;
                        case 43681: // Inactive
                        {
                            if (target->GetTypeId() != TYPEID_PLAYER || !aurApp->GetRemoveMode().HasFlag(AuraRemoveFlags::Expired))
                                return;

                            if (target->GetMap()->IsBattleground())
                                target->ToPlayer()->LeaveBattleground();
                            break;
                        }
                        case 42783: // Wrath of the Astromancer
                            target->CastSpell(target, GetAmount(), this);
                            break;
                        case 46308: // Burning Winds cast only at creatures at spawn
                            target->CastSpell(target, 47287, this);
                            break;
                        case 52172:  // Coyote Spirit Despawn Aura
                        case 60244:  // Blood Parrot Despawn Aura
                            target->CastSpell(nullptr, GetAmount(), this);
                            break;
                        case 91604: // Restricted Flight Area
                            if (aurApp->GetRemoveMode().HasFlag(AuraRemoveFlags::Expired))
                                target->CastSpell(target, 58601, true);
                            break;
                    }
                    break;
                case SPELLFAMILY_DEATHKNIGHT:
                    // Summon Gargoyle (Dismiss Gargoyle at remove)
                    if (GetId() == 61777)
                        target->CastSpell(target, GetAmount(), true);
                    break;
                default:
                    break;
            }
        }
    }

    // AT APPLY & REMOVE

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;
            switch (GetId())
            {
                // Recently Bandaged
                case 11196:
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, GetMiscValue(), apply);
                    break;
                // Unstable Power
                case 24658:
                {
                    uint32 spellId = 24659;
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(spellId);

                        for (uint32 i = 0; i < spell->StackAmount; ++i)
                            caster->CastSpell(target, spell->Id, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Restless Strength
                case 24661:
                {
                    uint32 spellId = 24662;
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(spellId);
                        for (uint32 i = 0; i < spell->StackAmount; ++i)
                            caster->CastSpell(target, spell->Id, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Tag Murloc
                case 30877:
                {
                    // Tag/untag Blacksilt Scout
                    target->SetEntry(apply ? 17654 : 17326);
                    break;
                }
                case 57819: // Argent Champion
                case 57820: // Ebon Champion
                case 57821: // Champion of the Kirin Tor
                case 57822: // Wyrmrest Champion
                case 93337: // Ramkahen
                case 93339: // Earthen Ring
                case 93341: // Hyjal
                case 93347: // Therazane
                case 93368: // Wildhammer Clan
                case 94158: // Dragonmaw Clan
                case 93827: // Darkspear Champion
                case 93830: // Bilgewater Champion
                case 94462: // Undercity Champion
                case 94463: // Thunder Bluff Champion
                case 93828: // Silvermoon Champion
                case 93825: // Orgrimmar Champion
                case 93821: // Gnomeregan Champion
                case 93816: // Gilneas Champion
                case 93806: // Darnassus Champion
                case 93811: // Exodar Champion
                case 93805: // Ironforge Champion
                case 93795: // Sturmwind Champion
                {
                    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                        break;

                    uint32 FactionID = 0;

                    if (apply)
                    {
                        switch (m_spellInfo->Id)
                        {
                            case 57819: FactionID = 1106; break; // Argent Crusade
                            case 57820: FactionID = 1098; break; // Knights of the Ebon Blade
                            case 57821: FactionID = 1090; break; // Kirin Tor
                            case 57822: FactionID = 1091; break; // The Wyrmrest Accord
                            case 93337: FactionID = 1173; break; // Ramkahen
                            case 93339: FactionID = 1135; break; // Earthen Ring
                            case 93341: FactionID = 1158; break; // Hyjal
                            case 93347: FactionID = 1171; break; // Therazane
                            case 93368: FactionID = 1174; break; // Wildhammer Clan
                            case 94158: FactionID = 1172; break; // Dragonmaw Clan
                            case 93827: FactionID = 530; break; // Darkspear Champion
                            case 93830: FactionID = 1133; break; // Bilgewater Champion
                            case 94462: FactionID = 68; break; // Undercity Champion
                            case 94463: FactionID = 81; break; // Thunder Bluff Champion
                            case 93828: FactionID = 911; break; // Silvermoon Champion
                            case 93825: FactionID = 76; break; // Orgrimmar Champion
                            case 93821: FactionID = 54; break; // Gnomeregan Champion
                            case 93816: FactionID = 1134; break; // Gilneas Champion
                            case 93806: FactionID = 69; break; // Darnassus Champion
                            case 93811: FactionID = 930; break; // Exodar Champion
                            case 93805: FactionID = 47; break; // Ironforge Champion
                            case 93795: FactionID = 72; break; // Sturmwind Champion
                        }
                    }
                    caster->ToPlayer()->SetChampioningFaction(FactionID);
                    break;
                }
                // LK Intro VO (1)
                case 58204:
                    if (target->GetTypeId() == TYPEID_PLAYER)
                    {
                        // Play part 1
                        if (apply)
                            target->PlayDirectSound(14970, target->ToPlayer());
                        // continue in 58205
                        else
                            target->CastSpell(target, 58205, true);
                    }
                    break;
                // LK Intro VO (2)
                case 58205:
                    if (target->GetTypeId() == TYPEID_PLAYER)
                    {
                        // Play part 2
                        if (apply)
                            target->PlayDirectSound(14971, target->ToPlayer());
                        // Play part 3
                        else
                            target->PlayDirectSound(14972, target->ToPlayer());
                    }
                    break;
                case 62061: // Festive Holiday Mount
                    if (target->HasAuraType(SPELL_AURA_MOUNTED))
                    {
                        uint32 creatureEntry = 0;
                        if (apply)
                        {
                            if (target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                                creatureEntry = 24906;
                            else
                                creatureEntry = 15665;
                        }
                        else
                            creatureEntry = target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetMiscValue();

                        if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creatureEntry))
                        {
                            CreatureModel model = *ObjectMgr::ChooseDisplayId(creatureInfo);
                            sObjectMgr->GetCreatureModelRandomGender(&model, creatureInfo);
                            target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, model.CreatureDisplayID);
                        }
                    }
                    break;
            }

            break;
        }
        case SPELLFAMILY_MAGE:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_PALADIN:
            // if (!(mode & AURA_EFFECT_HANDLE_REAL))
            //    break;
            break;
        case SPELLFAMILY_DEATHKNIGHT:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
            //    break;
            break;
        }
    }
}

void AuraEffect::HandleChannelDeathItem(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (apply || !aurApp->GetRemoveMode().HasFlag(AuraRemoveFlags::ByDeath))
        return;

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* plCaster = caster->ToPlayer();
    Unit* target = aurApp->GetTarget();

    // Item amount
    if (GetAmount() <= 0)
        return;

    if (GetSpellInfo()->Effects[m_effIndex].ItemType == 0)
        return;

    // Soul Shard
    if (GetSpellInfo()->Effects[m_effIndex].ItemType == 6265)
    {
        // Soul Shard only from units that grant XP or honor
        if (!plCaster->isHonorOrXPTarget(target) ||
            (target->GetTypeId() == TYPEID_UNIT && !target->ToCreature()->isTappedBy(plCaster)))
            return;
    }

    //Adding items
    uint32 noSpaceForCount = 0;
    uint32 count = GetAmount();

    ItemPosCountVec dest;
    InventoryResult msg = plCaster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, GetSpellInfo()->Effects[m_effIndex].ItemType, count, &noSpaceForCount);
    if (msg != EQUIP_ERR_OK)
    {
        count-=noSpaceForCount;
        plCaster->SendEquipError(msg, nullptr, nullptr, GetSpellInfo()->Effects[m_effIndex].ItemType);
        if (count == 0)
            return;
    }

    Item* newitem = plCaster->StoreNewItem(dest, GetSpellInfo()->Effects[m_effIndex].ItemType, true);
    if (!newitem)
    {
        plCaster->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
        return;
    }
    plCaster->SendNewItem(newitem, count, true, true);
}

void AuraEffect::HandleBindSight(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
        return;

    caster->ToPlayer()->SetViewpoint(target, apply);
}

void AuraEffect::HandleForceReaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    Player* player = target->ToPlayer();
    if (!player)
        return;

    uint32 factionId = GetMiscValue();
    ReputationRank factionRank = ReputationRank(GetAmount());

    player->GetReputationMgr().ApplyForceReaction(factionId, factionRank, apply);
    player->GetReputationMgr().SendForceReactions();

    // stop fighting at apply (if forced rank friendly) or at remove (if real rank friendly)
    if ((apply && factionRank >= REP_FRIENDLY) || (!apply && player->GetReputationRank(factionId) >= REP_FRIENDLY))
        player->StopAttackFaction(factionId);
}

void AuraEffect::HandleAuraEmpathy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    if (target->GetCreatureType() == CREATURE_TYPE_BEAST)
        target->ApplyModUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO, apply);
}

void AuraEffect::HandleAuraModFaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->SetFaction(GetMiscValue());
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION);

        if (target->IsPlayer())
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
    }
    else
    {
        target->RestoreFaction();
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION);

        if (target->IsPlayer())
            target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
    }
}

void AuraEffect::HandleComprehendLanguage(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_COMPREHEND_LANG);
    else
    {
        if (target->HasAuraType(GetAuraType()))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_COMPREHEND_LANG);
    }
}

void AuraEffect::HandleAuraConvertRune(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Player* player = target->ToPlayer();
    if (!player)
        return;

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return;

    /*
    * Converts a rune into a new rune type.
    * MiscValueA describes what rune type can be converted
    * MiscValueB describes into what rune the eligible rune will be converted into
    * Amount is the number of runes that will be converted if available
    */

    uint32 runes = GetAmount();

    if (apply)
    {
        for (uint32 i = 0; i < MAX_RUNES && runes; ++i)
        {
            if (RuneType(GetMiscValue()) != player->GetCurrentRune(i))
                continue;

            if (!player->GetRuneCooldown(i))
            {
                player->AddRuneByAuraEffect(i, RuneType(GetMiscValueB()), this, GetAuraType(), GetSpellInfo());
                --runes;
            }
        }
    }
    else
        player->RemoveRunesByAuraEffect(this);
}

void AuraEffect::HandleAuraLinked(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    Unit* target = aurApp->GetTarget();

    uint32 triggeredSpellId = sSpellMgr->GetSpellIdForDifficulty(m_spellInfo->Effects[m_effIndex].TriggerSpell, target);
    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggeredSpellId);
    if (!triggeredSpellInfo)
        return;

    if (mode & AURA_EFFECT_HANDLE_REAL)
    {
        if (apply)
        {
            Unit* caster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCaster() : target;

            if (!caster)
                return;

            // If amount avalible cast with basepoints (Crypt Fever for example)
            CastSpellExtraArgs args(this);

            if (GetAmount()) // If amount avalible cast with basepoints (Crypt Fever for example)
                args.AddSpellMod(SPELLVALUE_BASE_POINT0, GetAmount());

            caster->CastSpell(target, triggeredSpellId, args);
        }
        else
        {
            ObjectGuid casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCasterGUID() : target->GetGUID();
            target->RemoveAura(triggeredSpellId, casterGUID);
        }
    }
    else if (mode & AURA_EFFECT_HANDLE_REAPPLY && apply)
    {
        ObjectGuid casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCasterGUID() : target->GetGUID();
        // change the stack amount to be equal to stack amount of our aura
        if (Aura* triggeredAura = target->GetAura(triggeredSpellId, casterGUID))
            triggeredAura->ModStackAmount(GetBase()->GetStackAmount() - triggeredAura->GetStackAmount());
    }
}

void AuraEffect::HandleAuraOpenStable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER || !target->IsInWorld())
        return;

    if (apply)
        target->ToPlayer()->GetSession()->SendStablePet(target->GetGUID());

     // client auto close stable dialog at !apply aura
}

void AuraEffect::HandleAuraModFakeInebriation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->m_invisibilityDetect.AddFlag(INVISIBILITY_DRUNK);
        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, GetAmount());

        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FAKE_INEBRIATION, oldval + GetAmount());
        }
    }
    else
    {
        bool removeDetect = !target->HasAuraType(SPELL_AURA_MOD_FAKE_INEBRIATE);

        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, -GetAmount());

        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FAKE_INEBRIATION, oldval - GetAmount());

            if (removeDetect)
                removeDetect = !target->ToPlayer()->GetDrunkValue();
        }

        if (removeDetect)
            target->m_invisibilityDetect.DelFlag(INVISIBILITY_DRUNK);
    }

    // call functions which may have additional effects after changing state of unit
    if (target->IsInWorld())
        target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraOverrideSpells(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target || !target->IsInWorld())
        return;

    uint32 overrideId = uint32(GetMiscValue());

    if (apply)
    {
        target->SetUInt16Value(PLAYER_FIELD_BYTES2, PLAYER_BYTES_2_OVERRIDE_SPELLS_UINT16_OFFSET, overrideId);
        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->Spells[i])
                    target->AddTemporarySpell(spellId);
    }
    else
    {
        target->SetUInt16Value(PLAYER_FIELD_BYTES2, PLAYER_BYTES_2_OVERRIDE_SPELLS_UINT16_OFFSET, 0);
        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->Spells[i])
                    target->RemoveTemporarySpell(spellId);
    }
}

void AuraEffect::HandleAuraSetVehicle(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->IsInWorld())
        return;

    uint32 vehicleId = GetMiscValue();

    if (apply)
    {
        if (!target->CreateVehicleKit(vehicleId, 0))
            return;
    }
    else if (target->GetVehicleKit())
        target->RemoveVehicleKit();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        target->ToPlayer()->SendOnCancelExpectedVehicleRideAura();
}

void AuraEffect::HandlePreventResurrection(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (aurApp->GetTarget()->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        aurApp->GetTarget()->RemoveByteFlag(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_FLAGS, PLAYER_FIELD_BYTE_RELEASE_TIMER);
    else if (!aurApp->GetTarget()->GetMap()->Instanceable())
        aurApp->GetTarget()->SetByteFlag(PLAYER_FIELD_BYTES, PLAYER_FIELD_BYTES_OFFSET_FLAGS, PLAYER_FIELD_BYTE_RELEASE_TIMER);
}

void AuraEffect::HandleAuraModSpellPowerPct(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    // Calculations done in Unit::SpellBaseHealingBonusDone and Unit::SpellBaseDamageBonusDone
    target->SetFloatValue(PLAYER_FIELD_MOD_SPELL_POWER_PCT, target->GetTotalAuraMultiplier(SPELL_AURA_MOD_SPELL_POWER_PCT));
}

void AuraEffect::HandleMastery(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateMastery();
}

void AuraEffect::HandlePeriodicDummyAuraTick(Unit* target, Unit* caster) const
{
    switch (GetSpellInfo()->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch (GetId())
            {
                case 66149: // Bullet Controller Periodic - 10 Man
                case 68396: // Bullet Controller Periodic - 25 Man
                {
                    if (!caster)
                        break;

                    CastSpellExtraArgs args;
                    args.AddSpellMod(SPELLVALUE_MAX_TARGETS, urand(1, 6));

                    caster->CastSpell(target, 66152, args);
                    caster->CastSpell(target, 66153, args);
                    break;
                }
                case 62292: // Blaze (Pool of Tar)
                    // should we use custom damage?
                    target->CastSpell((Unit*)nullptr, m_spellInfo->Effects[m_effIndex].TriggerSpell, true);
                    break;
                case 62399: // Overload Circuit
                    if (target->GetMap()->IsDungeon() && int(target->GetAppliedAuras().count(62399)) >= (target->GetMap()->IsHeroic() ? 4 : 2))
                    {
                         target->CastSpell(target, 62475, true); // System Shutdown
                         if (Unit* veh = target->GetVehicleBase())
                             veh->CastSpell(target, 62475, true);
                    }
                    break;
                case 64821: // Fuse Armor (Razorscale)
                    if (GetBase()->GetStackAmount() == GetSpellInfo()->StackAmount)
                    {
                        target->CastSpell(target, 64774, GetCasterGUID());
                        target->RemoveAura(64821);
                    }
                    break;
            }
            break;
        case SPELLFAMILY_HUNTER:
        {
            // Explosive Shot
            if (GetSpellInfo()->SpellFamilyFlags[1] & 0x80000000)
            {
                if (caster)
                {
                    CastSpellExtraArgs args;
                    args.AddSpellMod(SpellValueMod(SPELLVALUE_BASE_POINT0), GetAmount());
                    args.SetTriggeringAura(this);
                    caster->CastSpell(target, 53352, args);
                }
                break;
            }
            switch (GetSpellInfo()->Id)
            {
                // Feeding Frenzy Rank 1
                case 53511:
                    if (target->GetVictim() && target->EnsureVictim()->HealthBelowPct(35))
                        target->CastSpell(target, 60096, this);
                    return;
                // Feeding Frenzy Rank 2
                case 53512:
                    if (target->GetVictim() && target->EnsureVictim()->HealthBelowPct(35))
                        target->CastSpell(target, 60097, this);
                    return;
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
            if (GetId() == 52179) // Astral Shift
            {
                // Periodic need for remove visual on stun/fear/silence lost
                if (!(target->GetUInt32Value(UNIT_FIELD_FLAGS)&(UNIT_FLAG_STUNNED|UNIT_FLAG_FLEEING|UNIT_FLAG_SILENCED)))
                    target->RemoveAurasDueToSpell(52179);
                break;
            }
            break;
        case SPELLFAMILY_DEATHKNIGHT:
            switch (GetId())
            {
                case 49016: // Hysteria
                    uint32 damage = uint32(target->CountPctFromMaxHealth(1));
                    Unit::DealDamage(target, target, damage, nullptr, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, false);
                    break;
            }
            // Blood of the North
            // Reaping
            // Death Rune Mastery
            // Blood Rites
            if (GetSpellInfo()->SpellIconID == 3041 || GetSpellInfo()->SpellIconID == 22
                || GetSpellInfo()->SpellIconID == 2622 || GetSpellInfo()->SpellIconID == 2724)
            {
                if (target->GetTypeId() != TYPEID_PLAYER)
                    return;
                if (target->ToPlayer()->getClass() != CLASS_DEATH_KNIGHT)
                    return;

                 // timer expired - remove death runes
                target->ToPlayer()->RemoveRunesByAuraEffect(this);
            }
            break;
        default:
            break;
    }
}

void AuraEffect::HandlePeriodicTriggerSpellAuraTick(Unit* target, Unit* caster) const
{
    // generic casting code with custom spells and target/caster customs
    uint32 triggerSpellId = GetSpellInfo()->Effects[GetEffIndex()].TriggerSpell;

    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);
    SpellInfo const* auraSpellInfo = GetSpellInfo();
    uint32 auraId = auraSpellInfo->Id;

    // specific code for cases with no trigger spell provided in field
    if (triggeredSpellInfo == nullptr)
    {
        switch (auraSpellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                switch (auraId)
                {
                    // Brood Affliction: Bronze
                    case 23170:
                        triggerSpellId = 23171;
                        break;
                    // Restoration
                    case 24379:
                    case 23493:
                    {
                        if (caster)
                        {
                            int32 heal = caster->CountPctFromMaxHealth(10);
                            HealInfo healInfo(caster, target, heal, auraSpellInfo, auraSpellInfo->GetSchoolMask());
                            caster->HealBySpell(healInfo);

                            /// @todo: should proc other auras?
                            if (int32 mana = caster->GetMaxPower(POWER_MANA))
                            {
                                mana /= 10;
                                caster->EnergizeBySpell(caster, 23493, mana, POWER_MANA);
                            }
                        }
                        return;
                    }
                    // Nitrous Boost
                    case 27746:
                        if (caster && target->GetPower(POWER_MANA) >= 10)
                        {
                            target->ModifyPower(POWER_MANA, -10);
                            target->SendEnergizeSpellLog(caster, 27746, 10, POWER_MANA);
                        }
                        else
                            target->RemoveAurasDueToSpell(27746);
                        return;
                    // Frost Blast
                    case 27808:
                        if (caster)
                        {
                            CastSpellExtraArgs args(this);
                            args.AddSpellMod(SpellValueMod(SPELLVALUE_BASE_POINT0), int32(target->CountPctFromMaxHealth(21)));
                            caster->CastSpell(target, triggerSpellId, args);
                        }
                        return;
                    // Inoculate Nestlewood Owlkin
                    case 29528:
                        if (target->GetTypeId() != TYPEID_UNIT) // prevent error reports in case ignored player target
                            return;
                        break;
                    // Feed Captured Animal
                    case 29917:
                        triggerSpellId = 29916;
                        break;
                    // Extract Gas
                    case 30427:
                    {
                        // move loot to player inventory and despawn target
                        if (caster && caster->GetTypeId() == TYPEID_PLAYER &&
                                target->GetTypeId() == TYPEID_UNIT &&
                                target->ToCreature()->GetCreatureTemplate()->type == CREATURE_TYPE_GAS_CLOUD)
                        {
                            Player* player = caster->ToPlayer();
                            Creature* creature = target->ToCreature();
                            // missing lootid has been reported on startup - just return
                            if (!creature->GetCreatureTemplate()->SkinLootId)
                                return;

                            player->AutoStoreLoot(creature->GetCreatureTemplate()->SkinLootId, LootTemplates_Skinning, true);

                            creature->DespawnOrUnsummon();
                        }
                        return;
                    }
                    // Quake
                    case 30576:
                        triggerSpellId = 30571;
                        break;
                    // Doom
                    /// @todo effect trigger spell may be independant on spell targets, and executed in spell finish phase
                    // so instakill will be naturally done before trigger spell
                    case 31347:
                    {
                        target->CastSpell(target, 31350, this);
                        target->KillSelf();
                        return;
                    }
                    // Spellcloth
                    case 31373:
                    {
                        // Summon Elemental after create item
                        target->SummonCreature(17870, 0, 0, 0, target->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 0);
                        return;
                    }
                    // Flame Quills
                    case 34229:
                    {
                        // cast 24 spells 34269-34289, 34314-34316
                        for (uint32 spell_id = 34269; spell_id != 34290; ++spell_id)
                            target->CastSpell(target, spell_id, this);
                        for (uint32 spell_id = 34314; spell_id != 34317; ++spell_id)
                            target->CastSpell(target, spell_id, this);
                        return;
                    }
                    // Remote Toy
                    case 37027:
                        triggerSpellId = 37029;
                        break;
                    // Eye of Grillok
                    case 38495:
                        triggerSpellId = 38530;
                        break;
                    // Absorb Eye of Grillok (Zezzak's Shard)
                    case 38554:
                    {
                        if (!caster || target->GetTypeId() != TYPEID_UNIT)
                            return;

                        caster->CastSpell(caster, 38495, this);

                        Creature* creatureTarget = target->ToCreature();
                        creatureTarget->DespawnOrUnsummon();
                        return;
                    }
                    // Tear of Azzinoth Summon Channel - it's not really supposed to do anything, and this only prevents the console spam
                    case 39857:
                        triggerSpellId = 39856;
                        break;
                    // Personalized Weather
                    case 46736:
                        triggerSpellId = 46737;
                        break;
                }
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                switch (auraId)
                {
                    // Lightning Shield (The Earthshatterer set trigger after cast Lighting Shield)
                    case 28820:
                    {
                        // Need remove self if Lightning Shield not active
                        if (!target->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_SHAMAN, 0x400, 0, 0))
                            target->RemoveAurasDueToSpell(28820);
                        return;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        // Spell exist but require custom code
        switch (auraId)
        {
            // Pursuing Spikes (Anub'arak)
            case 65920:
            case 65922:
            case 65923:
            {
                Unit* permafrostCaster = nullptr;
                Aura* permafrostAura = target->GetAura(66193);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67855);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67856);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67857);

                if (permafrostAura)
                    permafrostCaster = permafrostAura->GetCaster();

                if (permafrostCaster)
                {
                    if (Creature* permafrostCasterCreature = permafrostCaster->ToCreature())
                        permafrostCasterCreature->DespawnOrUnsummon(3000);

                    target->CastSpell(target, 66181, false);
                    target->RemoveAllAuras();
                    if (Creature* targetCreature = target->ToCreature())
                        targetCreature->DisappearAndDie();
                }
                break;
            }
            // Mana Tide
            case 16191:
            {
                CastSpellExtraArgs args(this);
                args.AddSpellMod(SPELLVALUE_BASE_POINT0, GetAmount());
                args.SetTriggeringAura(this);
                target->CastSpell(target, triggerSpellId, args);
                return;
            }
            // Negative Energy Periodic
            case 46284:
            {
                CastSpellExtraArgs args(this);
                args.AddSpellMod(SPELLVALUE_MAX_TARGETS, int32(_ticksDone / 10 + 1));
                args.SetTriggeringAura(this);
                target->CastSpell(nullptr, triggerSpellId, args);
                return;
            }
            // Slime Pool (Dreadscale & Acidmaw)
            case 66882:
            {
                CastSpellExtraArgs args(this);
                args.AddSpellMod(SPELLVALUE_RADIUS_MOD, int32((((float)_ticksDone / 60) * 0.9f + 0.1f) * 10000 * 2 / 3));
                args.SetTriggeringAura(this);
                target->CastSpell(nullptr, triggerSpellId, args);
                return;
            }
            // Slime Spray - temporary here until preventing default effect works again
            // added on 9.10.2010
            case 69508:
            {
                if (caster)
                    caster->CastSpell(target, triggerSpellId, caster->GetGUID());
                return;
            }
            case 24745: // Summon Templar, Trigger
            case 24747: // Summon Templar Fire, Trigger
            case 24757: // Summon Templar Air, Trigger
            case 24759: // Summon Templar Earth, Trigger
            case 24761: // Summon Templar Water, Trigger
            case 24762: // Summon Duke, Trigger
            case 24766: // Summon Duke Fire, Trigger
            case 24769: // Summon Duke Air, Trigger
            case 24771: // Summon Duke Earth, Trigger
            case 24773: // Summon Duke Water, Trigger
            case 24785: // Summon Royal, Trigger
            case 24787: // Summon Royal Fire, Trigger
            case 24791: // Summon Royal Air, Trigger
            case 24792: // Summon Royal Earth, Trigger
            case 24793: // Summon Royal Water, Trigger
            {
                // All this spells trigger a spell that requires reagents; if the
                // triggered spell is cast as "triggered", reagents are not consumed
                if (caster)
                    caster->CastSpell(target, triggerSpellId, false);
                return;
            }
        }
    }

    // Reget trigger spell proto
    triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);

    if (triggeredSpellInfo)
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? caster : target)
        {
            triggerCaster->CastSpell(target, triggeredSpellInfo->Id, this);
            TC_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellAuraTick: Spell %u Trigger %u", GetId(), triggeredSpellInfo->Id);
        }
    }
    else
        TC_LOG_WARN("spells", "AuraEffect::HandlePeriodicTriggerSpellAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
}

void AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick(Unit* target, Unit* caster) const
{
    uint32 triggerSpellId = GetSpellInfo()->Effects[m_effIndex].TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? caster : target)
        {
            CastSpellExtraArgs args(this);
            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                args.AddSpellMod(SpellValueMod(SPELLVALUE_BASE_POINT0 + i), GetAmount());
            triggerCaster->CastSpell(target, triggerSpellId, args);
            TC_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u Trigger %u", GetId(), triggeredSpellInfo->Id);
        }
    }
    else
        TC_LOG_WARN("spells","AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
}

void AuraEffect::HandlePeriodicDamageAurasTick(Unit* target, Unit* caster) const
{
    if (!target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // Consecrate ticks can miss and will not show up in the combat log
    // dynobj auras must always have a caster
    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        ASSERT_NOTNULL(caster)->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    // some auras remove at specific health level or more
    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        switch (GetSpellInfo()->Id)
        {
            case 43093: case 31956: case 38801:  // Grievous Wound
            case 35321: case 38363: case 39215:  // Gushing Wound
                if (target->IsFullHealth())
                {
                    target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            case 38772: // Grievous Wound
            {
                uint32 percent = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
                if (!target->HealthBelowPct(percent))
                {
                    target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            }
        }
    }

    CleanDamage cleanDamage = CleanDamage(0, 0, BASE_ATTACK, MELEE_HIT_NORMAL);

    // ignore negative values (can be result apply spellmods to aura damage
    uint32 damage = std::max(GetAmount(), 0);

    // Script Hook For HandlePeriodicDamageAurasTick -- Allow scripts to change the Damage pre class mitigation calculations
    sScriptMgr->ModifyPeriodicDamageAurasTick(target, caster, damage);

    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        // leave only target depending bonuses, rest is handled in calculate amount
        if (GetBase()->GetType() == DYNOBJ_AURA_TYPE)
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), damage, DOT, GetEffIndex(), GetBase()->GetStackAmount(), nullptr, this);

        switch (GetSpellInfo()->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
                switch (GetId())
                {
                    case 70911: // Unbound Plague
                    case 72854: // Unbound Plague
                    case 72855: // Unbound Plague
                    case 72856: // Unbound Plague
                        damage *= uint32(pow(1.25f, int32(_ticksDone)));
                        break;
                    default:
                        break;
                }
                break;
            case SPELLFAMILY_WARLOCK:
                // Curse of Agony
                if ((GetSpellInfo()->SpellFamilyFlags[0] & 0x400) && GetSpellInfo()->SpellIconID == 544)
                {
                    uint32 totalTick = GetTotalTicks();
                    // 1..4 ticks, 1/2 from normal tick damage
                    if (_ticksDone <= totalTick / 3)
                        damage = damage / 2;
                    // 9..12 ticks, 3/2 from normal tick damage
                    else if (_ticksDone > totalTick * 2 / 3)
                        damage += (damage + 1) / 2;           // +1 prevent 0.5 damage possible lost at 1..4 ticks
                                                              // 5..8 ticks have normal tick damage
                }

                // Drain Soul
                if (GetSpellInfo()->SpellFamilyFlags[0] & 0x00004000)
                    if (target->GetHealthPct() <= 25.0f)
                        AddPct(damage, 25);
                break;
            default:
                break;
        }
    }
    else // ceil obtained value, it may happen that 10 ticks for 10% damage may not kill owner
        damage = uint32(ceil(CalculatePct<float, float>(target->GetMaxHealth(), damage)));

    damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, DOT);

    bool crit = false;

    if (CanPeriodicTickCrit(caster))
        crit = roll_chance_f(GetCritChanceFor(caster, target));

    if (crit)
        damage = Unit::SpellCriticalDamageBonus(caster, m_spellInfo, damage);

    // Calculate armor mitigation
    if (Unit::IsDamageReducedByArmor(GetSpellInfo()->GetSchoolMask(), GetSpellInfo()))
    {
        uint32 damageReducedArmor = Unit::CalcArmorReducedDamage(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetAttackType(), GetBase()->GetCasterLevel());
        cleanDamage.mitigated_damage += damage - damageReducedArmor;
        damage = damageReducedArmor;
    }

    if (!GetSpellInfo()->HasAttribute(SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS))
    {
        if (GetSpellInfo()->Effects[GetEffIndex()].IsTargetingArea() || GetSpellInfo()->Effects[GetEffIndex()].IsAreaAuraEffect() || GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA || GetSpellInfo()->HasAttribute(SPELL_ATTR5_TREAT_AS_AREA_EFFECT))
            damage = target->CalculateAOEAvoidance(damage, m_spellInfo->SchoolMask, (caster && !caster->IsControlledByPlayer()) || GetSpellInfo()->HasAttribute(SPELL_ATTR7_TREAT_AS_NPC_AOE));
    }

    int32 dmg = damage;
    if (!GetSpellInfo()->HasAttribute(SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS) && caster && caster->CanApplyResilience())
        Unit::ApplyResilience(target, &dmg);
    damage = dmg;

    DamageInfo damageInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), DOT, BASE_ATTACK);
    Unit::CalcAbsorbResist(damageInfo);
    damage = damageInfo.GetDamage();

    uint32 absorb = damageInfo.GetAbsorb();
    uint32 resist = damageInfo.GetResist();
    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s attacked %s for %u dmg inflicted by %u absorb is %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), damage, GetId(), absorb);

    Unit::DealDamageMods(target, damage, &absorb);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DEAL_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKE_PERIODIC;

    uint32 hitMask = damageInfo.GetHitMask();
    if (damage)
    {
        hitMask |= crit ? PROC_HIT_CRITICAL : PROC_HIT_NORMAL;
        procVictim |= PROC_FLAG_TAKE_ANY_DAMAGE;
    }

    int32 overkill = damage - target->GetHealth();
    if (overkill < 0)
        overkill = 0;

    SpellPeriodicAuraLogInfo pInfo(this, damage, overkill, absorb, resist, 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    Unit::DealDamage(caster, target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), true);

    Unit::ProcSkillsAndAuras(caster, target, procAttacker, procVictim, PROC_SPELL_TYPE_DAMAGE, PROC_SPELL_PHASE_HIT, hitMask, nullptr, &damageInfo, nullptr);
}

void AuraEffect::HandlePeriodicHealthLeechAuraTick(Unit* target, Unit* caster) const
{
    if (!target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        ASSERT_NOTNULL(caster)->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    CleanDamage cleanDamage = CleanDamage(0, 0, GetSpellInfo()->GetAttackType(), MELEE_HIT_NORMAL);

    // ignore negative values (can be result apply spellmods to aura damage
    uint32 damage = std::max(GetAmount(), 0);

    // Script Hook For HandlePeriodicDamageAurasTick -- Allow scripts to change the Damage pre class mitigation calculations
    sScriptMgr->ModifyPeriodicDamageAurasTick(target, caster, damage);

    // dynobj auras must always have a caster
    if (GetBase()->GetType() == DYNOBJ_AURA_TYPE)
        damage = ASSERT_NOTNULL(caster)->SpellDamageBonusDone(target, GetSpellInfo(), damage, DOT, GetEffIndex(), GetBase()->GetStackAmount(), nullptr, this);

    damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, DOT);

    bool crit = false;

    if (CanPeriodicTickCrit(caster))
        crit = roll_chance_f(GetCritChanceFor(caster, target));

    if (crit)
        damage = Unit::SpellCriticalDamageBonus(caster, m_spellInfo, damage);

    // Calculate armor mitigation
    if (Unit::IsDamageReducedByArmor(GetSpellInfo()->GetSchoolMask(), GetSpellInfo()))
    {
        uint32 damageReducedArmor = Unit::CalcArmorReducedDamage(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetAttackType(), GetBase()->GetCasterLevel());
        cleanDamage.mitigated_damage += damage - damageReducedArmor;
        damage = damageReducedArmor;
    }

    if (!GetSpellInfo()->HasAttribute(SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS))
    {
        if (GetSpellInfo()->Effects[GetEffIndex()].IsTargetingArea() || GetSpellInfo()->Effects[GetEffIndex()].IsAreaAuraEffect() || GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA || GetSpellInfo()->HasAttribute(SPELL_ATTR5_TREAT_AS_AREA_EFFECT))
            damage = target->CalculateAOEAvoidance(damage, m_spellInfo->SchoolMask, (caster && !caster->IsControlledByPlayer()) || GetSpellInfo()->HasAttribute(SPELL_ATTR7_TREAT_AS_NPC_AOE));
    }

    int32 dmg = damage;
    if (!GetSpellInfo()->HasAttribute(SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS) && caster && caster->CanApplyResilience())
        Unit::ApplyResilience(target, &dmg);
    damage = dmg;

    DamageInfo damageInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), DOT, GetSpellInfo()->GetAttackType());
    Unit::CalcAbsorbResist(damageInfo);

    uint32 absorb = damageInfo.GetAbsorb();
    uint32 resist = damageInfo.GetResist();
    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s health leech of %s for %u dmg inflicted by %u abs is %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), damage, GetId(), absorb);

    // SendSpellNonMeleeDamageLog expects non-absorbed/non-resisted damage
    if (caster)
        caster->SendSpellNonMeleeDamageLog(target, GetId(), damage, GetSpellInfo()->GetSchoolMask(), absorb, resist, false, 0, crit);
    damage = damageInfo.GetDamage();

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DEAL_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKE_PERIODIC;

    uint32 hitMask = damageInfo.GetHitMask();
    if (damage)
    {
        hitMask |= crit ? PROC_HIT_CRITICAL : PROC_HIT_NORMAL;
        procVictim |= PROC_FLAG_TAKE_ANY_DAMAGE;
    }

    int32 new_damage = Unit::DealDamage(caster, target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), false);
    Unit::ProcSkillsAndAuras(caster, target, procAttacker, procVictim, PROC_SPELL_TYPE_DAMAGE, PROC_SPELL_PHASE_NONE, hitMask, nullptr, &damageInfo, nullptr);

    // process caster heal from now on (must be in world)
    if (!caster || !caster->IsAlive())
        return;

    float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    uint32 heal = uint32(caster->SpellHealingBonusDone(caster, GetSpellInfo(), uint32(new_damage * gainMultiplier), DOT, GetEffIndex(), GetBase()->GetStackAmount(), nullptr, this));
    heal = uint32(caster->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT));

    HealInfo healInfo(caster, caster, heal, GetSpellInfo(), GetSpellInfo()->GetSchoolMask());
    caster->HealBySpell(healInfo);

    caster->GetThreatManager().ForwardThreatForAssistingMe(caster, healInfo.GetEffectiveHeal() * 0.5f, GetSpellInfo());
    Unit::ProcSkillsAndAuras(caster, caster, PROC_FLAG_DEAL_PERIODIC, PROC_FLAG_TAKE_PERIODIC, PROC_SPELL_TYPE_HEAL, PROC_SPELL_PHASE_HIT, hitMask, nullptr, nullptr, &healInfo);
}

void AuraEffect::HandlePeriodicHealthFunnelAuraTick(Unit* target, Unit* caster) const
{
    if (!caster || !caster->IsAlive() || !target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    uint32 damage = std::max(GetAmount(), 0);
    // do not kill health donator
    if (caster->GetHealth() < damage)
        damage = caster->GetHealth() - 1;
    if (!damage)
        return;

    caster->ModifyHealth(-(int32)damage);
    TC_LOG_DEBUG("spells", "PeriodicTick: donator %u target %u damage %u.", caster->GetEntry(), target->GetEntry(), damage);

    float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    damage = int32(damage * gainMultiplier);

    HealInfo healInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetSchoolMask());
    caster->HealBySpell(healInfo);
    Unit::ProcSkillsAndAuras(caster, target, PROC_FLAG_DEAL_PERIODIC, PROC_FLAG_TAKE_PERIODIC, PROC_SPELL_TYPE_HEAL, PROC_SPELL_PHASE_HIT, PROC_HIT_NORMAL, nullptr, nullptr, &healInfo);
}

void AuraEffect::HandlePeriodicHealAurasTick(Unit* target, Unit* caster) const
{
    if (!target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->IsFullHealth())
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    uint32 damage = std::max(GetAmount(), 0);

    // Script Hook For HandlePeriodicHealAurasTick -- Allow scripts to change the Damage pre class mitigation calculations
    sScriptMgr->ModifyPeriodicDamageAurasTick(target, caster, damage);

    if (GetAuraType() == SPELL_AURA_OBS_MOD_HEALTH)
        damage = uint32(target->CountPctFromMaxHealth(damage));
    else
    {
        if (GetBase()->GetType() == DYNOBJ_AURA_TYPE)
            damage = caster->SpellHealingBonusDone(target, GetSpellInfo(), damage, DOT, GetEffIndex(), GetBase()->GetStackAmount(), nullptr, this) * caster->SpellHealingPctDone(target, m_spellInfo);
    }

    damage = target->SpellHealingBonusTaken(caster, GetSpellInfo(), damage, DOT);

    bool crit = false;

    if (CanPeriodicTickCrit(caster))
        crit = roll_chance_f(GetCritChanceFor(caster, target));

    if (crit)
        damage = Unit::SpellCriticalHealingBonus(caster, damage);

    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s heal of %s for %u health inflicted by %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), damage, GetId());

    uint32 heal = damage;

    HealInfo healInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo()->GetSchoolMask());
    Unit::CalcHealAbsorb(healInfo);
    Unit::DealHeal(healInfo);

    SpellPeriodicAuraLogInfo pInfo(this, heal, heal - healInfo.GetEffectiveHeal(), healInfo.GetAbsorb(), 0, 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    if (caster)
        target->GetThreatManager().ForwardThreatForAssistingMe(caster, float(healInfo.GetEffectiveHeal()) * 0.5f, GetSpellInfo());

    bool haveCastItem = !GetBase()->GetCastItemGUID().IsEmpty();

    // %-based heal - does not proc auras
    if (GetAuraType() == SPELL_AURA_OBS_MOD_HEALTH)
        return;

    uint32 procAttacker = PROC_FLAG_DEAL_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKE_PERIODIC;

    uint32 hitMask = crit ? PROC_HIT_CRITICAL : PROC_HIT_NORMAL;
    // ignore item heals
    if (!haveCastItem)
        Unit::ProcSkillsAndAuras(caster, target, procAttacker, procVictim, PROC_SPELL_TYPE_HEAL, PROC_SPELL_PHASE_HIT, hitMask, nullptr, nullptr, &healInfo);
}

void AuraEffect::HandlePeriodicManaLeechAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !caster->IsAlive() || !target->IsAlive() || target->GetPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 drainAmount = std::max(GetAmount(), 0);

    // Special case: draining x% of mana (up to a maximum of 2*x% of the caster's maximum mana)
    // It's mana percent cost spells, m_amount is percent drain from target
    if (m_spellInfo->ManaCostPercentage)
    {
        // max value
        int32 maxmana = CalculatePct(caster->GetMaxPower(powerType), drainAmount * 2.0f);
        ApplyPct(drainAmount, target->GetMaxPower(powerType));
        if (drainAmount > maxmana)
            drainAmount = maxmana;
    }

    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s power leech of %s for %u dmg inflicted by %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), drainAmount, GetId());

    int32 drainedAmount = -target->ModifyPower(powerType, -drainAmount);

    float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    SpellPeriodicAuraLogInfo pInfo(this, drainedAmount, 0, 0, 0, gainMultiplier, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gainAmount = int32(drainedAmount * gainMultiplier);
    int32 gainedAmount = 0;
    if (gainAmount)
    {
        gainedAmount = caster->ModifyPower(powerType, gainAmount);
        if (!GetSpellInfo()->HasAttribute(SPELL_ATTR4_NO_HELPFUL_THREAT))
            target->GetThreatManager().AddThreat(caster, float(gainedAmount) * 0.5f, GetSpellInfo(), true);
    }

    // Drain Mana - Mana Feed effect
    if (caster->GetGuardianPet() && m_spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && m_spellInfo->SpellFamilyFlags[0] & 0x00000010)
    {
        int32 manaFeedVal = 0;
        if (AuraEffect const* aurEff = GetBase()->GetEffect(EFFECT_1))
            manaFeedVal = aurEff->GetAmount();

        if (manaFeedVal > 0)
        {

            CastSpellExtraArgs args(this);
            args.AddSpellMod(SPELLVALUE_BASE_POINT0, int32(CalculatePct(gainedAmount, manaFeedVal)));
            args.SetTriggeringAura(this);
            caster->CastSpell(caster, 32554, args);
        }
    }
}

void AuraEffect::HandleObsModPowerAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType;
    if (GetMiscValue() == POWER_ALL)
        powerType = target->GetPowerType();
    else
        powerType = Powers(GetMiscValue());

    if (!target->IsAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    uint32 amount = std::max(GetAmount(), 0) * target->GetMaxPower(powerType) / 100;
    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s energize %s for %u dmg inflicted by %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), amount, GetId());

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->GetThreatManager().ForwardThreatForAssistingMe(caster, float(gain) * 0.5f, GetSpellInfo(), true);
}

void AuraEffect::HandlePeriodicEnergizeAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());
    if (!target->IsAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 amount = std::max(GetAmount(), 0);

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    TC_LOG_DEBUG("spells.periodic", "PeriodicTick: %s energize %s for %u dmg inflicted by %u",
        GetCasterGUID().ToString().c_str(), target->GetGUID().ToString().c_str(), amount, GetId());

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->GetThreatManager().ForwardThreatForAssistingMe(caster, float(gain) * 0.5f, GetSpellInfo(), true);
}

void AuraEffect::HandlePeriodicPowerBurnAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !target->IsAlive() || target->GetPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // ignore negative values (can be result apply spellmods to aura damage
    int32 damage = std::max(GetAmount(), 0);

    uint32 gain = uint32(-target->ModifyPower(powerType, -damage));

    float dmgMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    SpellInfo const* spellProto = GetSpellInfo();
    // maybe has to be sent different to client, but not by SMSG_PERIODICAURALOG
    SpellNonMeleeDamage damageInfo(caster, target, spellProto->Id, spellProto->SchoolMask);
    // no SpellDamageBonus for burn mana
    caster->CalculateSpellDamageTaken(&damageInfo, int32(gain * dmgMultiplier), spellProto);

    Unit::DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);

    caster->SendSpellNonMeleeDamageLog(&damageInfo);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DEAL_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKE_PERIODIC;

    uint32 hitMask      = createProcHitMask(&damageInfo, SPELL_MISS_NONE);
    uint32 spellTypeMask = PROC_SPELL_TYPE_NO_DMG_HEAL;
    if (damageInfo.damage)
    {
        procVictim |= PROC_FLAG_TAKE_ANY_DAMAGE;
        spellTypeMask |= PROC_SPELL_TYPE_DAMAGE;
    }

    caster->DealSpellDamage(&damageInfo, true);

    DamageInfo dotDamageInfo(damageInfo, DOT, BASE_ATTACK, hitMask);
    Unit::ProcSkillsAndAuras(caster, target, procAttacker, procVictim, spellTypeMask, PROC_SPELL_PHASE_HIT, hitMask, nullptr, &dotDamageInfo, nullptr);
}

void AuraEffect::HandleBreakableCCAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    int32 const damageLeft = GetAmount() - static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage());

    if (damageLeft <= 0)
        aurApp->GetTarget()->RemoveAura(aurApp);
    else
        SetAmount(damageLeft);
}

void AuraEffect::HandleProcTriggerSpellAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();
    if (GetSpellInfo()->HasAttribute(SPELL_ATTR8_TARGET_PROCS_ON_CASTER) && eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
        triggerTarget = eventInfo.GetActor();

    uint32 triggerSpellId = GetSpellInfo()->Effects[GetEffIndex()].TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        TC_LOG_DEBUG("spells", "AuraEffect::HandleProcTriggerSpellAuraProc: Triggering spell %u from aura %u proc", triggeredSpellInfo->Id, GetId());
        triggerCaster->CastSpell(triggerTarget, triggeredSpellInfo->Id, this);
    }
    else if (triggerSpellId && GetAuraType() != SPELL_AURA_DUMMY)
        TC_LOG_ERROR("spells","AuraEffect::HandleProcTriggerSpellAuraProc: Could not trigger spell %u from aura %u proc, because the spell does not have an entry in Spell.dbc.", triggerSpellId, GetId());
}

void AuraEffect::HandleProcTriggerSpellCopyAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();
    if (GetSpellInfo()->HasAttribute(SPELL_ATTR8_TARGET_PROCS_ON_CASTER) && eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
        triggerTarget = eventInfo.GetActor();

    SpellInfo const* triggeredSpellInfo = eventInfo.GetSpellInfo();
    if (!triggeredSpellInfo)
        return;

    TC_LOG_DEBUG("spells", "AuraEffect::HandleProcTriggerSpellAuraProc: Triggering spell %u from aura %u proc", triggeredSpellInfo->Id, GetId());
    triggerCaster->CastSpell(triggerTarget, triggeredSpellInfo->Id, this);
}

void AuraEffect::HandleProcTriggerSpellWithValueAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();

    uint32 triggerSpellId = GetSpellInfo()->Effects[m_effIndex].TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        CastSpellExtraArgs args(this);
        args.AddSpellMod(SPELLVALUE_BASE_POINT0, GetAmount());
        triggerCaster->CastSpell(triggerTarget, triggerSpellId, args);
        TC_LOG_DEBUG("spells", "AuraEffect::HandleProcTriggerSpellWithValueAuraProc: Triggering spell %u with value %d from aura %u proc", triggeredSpellInfo->Id, GetAmount(), GetId());
    }
    else
        TC_LOG_ERROR("spells","AuraEffect::HandleProcTriggerSpellWithValueAuraProc: Could not trigger spell %u from aura %u proc, because the spell does not have an entry in Spell.dbc.", triggerSpellId, GetId());
}

void AuraEffect::HandleProcTriggerDamageAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    if (!aurApp)
        return;

    Unit* target = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();
    if (triggerTarget->HasUnitState(UNIT_STATE_ISOLATED) || triggerTarget->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(triggerTarget, target);
        return;
    }

    SpellNonMeleeDamage damageInfo(target, triggerTarget, GetId(), GetSpellInfo()->SchoolMask);
    uint32 damage = target->SpellDamageBonusDone(triggerTarget, GetSpellInfo(), GetAmount(), SPELL_DIRECT_DAMAGE, GetEffIndex(), 1, nullptr, this);
    damage = triggerTarget->SpellDamageBonusTaken(target, GetSpellInfo(), damage, SPELL_DIRECT_DAMAGE);
    target->CalculateSpellDamageTaken(&damageInfo, damage, GetSpellInfo());
    Unit:: DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);
    target->SendSpellNonMeleeDamageLog(&damageInfo);
    TC_LOG_DEBUG("spells", "AuraEffect::HandleProcTriggerDamageAuraProc: Triggering %u spell damage from aura %u proc", damage, GetId());
    target->DealSpellDamage(&damageInfo, true);
}

void AuraEffect::HandleRaidProcFromChargeAuraProc(AuraApplication* aurApp, ProcEventInfo& /*eventInfo*/)
{
    Unit* target = aurApp->GetTarget();

    uint32 triggerSpellId;
    switch (GetId())
    {
        case 57949:            // Shiver
            triggerSpellId = 57952;
            //animationSpellId = 57951; dummy effects for jump spell have unknown use (see also 41637)
            break;
        case 59978:            // Shiver
            triggerSpellId = 59979;
            break;
        case 43593:            // Cold Stare
            triggerSpellId = 43594;
            break;
        default:
            TC_LOG_DEBUG("spells", "AuraEffect::HandleRaidProcFromChargeAuraProc: received not handled spell: %u", GetId());
            return;
    }

    int32 jumps = GetBase()->GetCharges();

    // current aura expire on proc finish
    GetBase()->SetCharges(0);
    GetBase()->SetUsingCharges(true);

    // next target selection
    if (jumps > 0)
    {
        if (Unit* caster = GetCaster())
        {
            float radius = GetSpellInfo()->Effects[GetEffIndex()].CalcRadius(caster);

            if (Unit* triggerTarget = target->GetNextRandomRaidMemberOrPet(radius))
            {
                target->CastSpell(triggerTarget, GetId(), { this, GetCasterGUID() });
                if (Aura* aura = triggerTarget->GetAura(GetId(), GetCasterGUID()))
                    aura->SetCharges(jumps);
            }
        }
    }

    TC_LOG_DEBUG("spells", "AuraEffect::HandleRaidProcFromChargeAuraProc: Triggering spell %u from aura %u proc", triggerSpellId, GetId());
    target->CastSpell(target, triggerSpellId, { this, GetCasterGUID() });
}

void AuraEffect::HandleRaidProcFromChargeWithValueAuraProc(AuraApplication* aurApp, ProcEventInfo& /*eventInfo*/)
{
    Unit* target = aurApp->GetTarget();

    // Currently only Prayer of Mending
    if (!(GetSpellInfo()->SpellFamilyName == SPELLFAMILY_PRIEST && GetSpellInfo()->SpellFamilyFlags[1] & 0x20))
    {
        TC_LOG_DEBUG("spells", "AuraEffect::HandleRaidProcFromChargeWithValueAuraProc: received not handled spell: %u", GetId());
        return;
    }
    uint32 triggerSpellId = 33110;

    int32 jumps = GetBase()->GetCharges();

    // current aura expire on proc finish
    GetBase()->SetCharges(0);
    GetBase()->SetUsingCharges(true);

    CastSpellExtraArgs args(this);
    args.OriginalCaster = GetCasterGUID();
    args.AddSpellMod(SPELLVALUE_BASE_POINT0, GetAmount());

    // next target selection
    if (jumps > 0)
    {
        if (Unit* caster = GetCaster())
        {
            float radius = GetSpellInfo()->Effects[GetEffIndex()].CalcRadius(caster);

            if (Unit* triggerTarget = target->GetNextRandomRaidMemberOrPet(radius))
            {
                target->CastSpell(triggerTarget, GetId(), args);
                if (Aura* aura = triggerTarget->GetAura(GetId(), GetCasterGUID()))
                    aura->SetCharges(jumps);
            }
        }
    }

    TC_LOG_DEBUG("spells", "AuraEffect::HandleRaidProcFromChargeWithValueAuraProc: Triggering spell %u from aura %u proc", triggerSpellId, GetId());
    target->CastSpell(target, triggerSpellId, args);
}

void AuraEffect::HandleProcOnPowerAmountAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();

    int32 powerAmountRequired = GetAmount();
    Powers powerRequired = Powers(GetMiscValue());

    if (!powerRequired || !powerAmountRequired)
    {
        TC_LOG_ERROR("spells", "AuraEffect::HandleProcOnPowerAmountAuraProc: Spell %u have 0 powerAmountRequired in EffectAmount[%d] or 0 powerRequired in EffectMiscValue, not handled custom case?", GetId(), GetEffIndex());
        return;
    }

    if (triggerCaster->GetPower(powerRequired) != powerAmountRequired)
        return;

    uint32 triggerSpellId = GetSpellInfo()->Effects[GetEffIndex()].TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        TC_LOG_DEBUG("spells", "AuraEffect::HandleProcOnPowerAmountAuraProc: Triggering spell %u from aura %u proc", triggeredSpellInfo->Id, GetId());
        triggerCaster->CastSpell(triggerTarget, triggeredSpellInfo->Id, true);
    }
    else
        TC_LOG_ERROR("spells", "AuraEffect::HandleProcOnPowerAmountAuraProc: Could not trigger spell %u from aura %u proc, because the spell does not have an entry in Spell.dbc.", triggerSpellId, GetId());
}

void AuraEffect::HandleAuraForceWeather(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target)
        return;

    if (apply)
        target->SendDirectMessage(WorldPackets::Misc::Weather(WeatherState(GetMiscValue()), 1.0f).Write());
    else
        target->GetMap()->SendZoneWeather(target->GetZoneId(), target);
}

void AuraEffect::HandleEnableAltPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    uint32 altPowerId = GetMiscValue();
    UnitPowerBarEntry const* powerEntry = sUnitPowerBarStore.LookupEntry(altPowerId);
    if (!powerEntry)
        return;

    Unit* target = aurApp->GetTarget();
    if (apply)
    {
        target->SetMaxPower(POWER_ALTERNATE_POWER, powerEntry->MaxPower);
        target->SetPower(POWER_ALTERNATE_POWER, powerEntry->StartPower);
        target->SetPowerBarID(altPowerId);
    }
    else
    {
        target->SetMaxPower(POWER_ALTERNATE_POWER, 0);
        target->SetPowerBarID(0);
    }
}

void AuraEffect::HandleModSpellCategoryCooldown(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (Player* player = aurApp->GetTarget()->ToPlayer())
        player->SendSpellCategoryCooldowns();
}

template TC_GAME_API void AuraEffect::GetTargetList(std::list<Unit*>&) const;
template TC_GAME_API void AuraEffect::GetTargetList(std::deque<Unit*>&) const;
template TC_GAME_API void AuraEffect::GetTargetList(std::vector<Unit*>&) const;

template TC_GAME_API void AuraEffect::GetApplicationList(std::list<AuraApplication*>&) const;
template TC_GAME_API void AuraEffect::GetApplicationList(std::deque<AuraApplication*>&) const;
template TC_GAME_API void AuraEffect::GetApplicationList(std::vector<AuraApplication*>&) const;
