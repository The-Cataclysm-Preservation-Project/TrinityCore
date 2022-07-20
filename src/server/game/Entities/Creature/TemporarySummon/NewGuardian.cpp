
#include "NewGuardian.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "NewPet.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellInfo.h"

NewGuardian::NewGuardian(SummonPropertiesEntry const* properties, Unit* summoner, bool isWorldObject, bool isTotem) :
    NewTemporarySummon(properties, summoner, isWorldObject, isTotem), _isUsingRealStats(false)
{
    m_unitTypeMask |= UNIT_MASK_GUARDIAN;
}

void NewGuardian::AddToWorld()
{
    // Setting the guid before adding to world to reduce building unnecessary object update packets
    SetCreatorGUID(GetInternalSummonerGUID());

    NewTemporarySummon::AddToWorld();
}

void NewGuardian::Update(uint32 diff)
{
    if (_scalingAuraUpdateTimer.has_value())
    {
        if (*_scalingAuraUpdateTimer <= Milliseconds(diff))
        {
            UpdateScalingAuras();
            _scalingAuraUpdateTimer = 1s;
        }
        else
            *_scalingAuraUpdateTimer -= Milliseconds(diff);
    }

    NewTemporarySummon::Update(diff);
}

bool NewGuardian::HandlePreSummonActions(Unit const* summoner, uint8 creatureLevel, uint8 maxSummons)
{
    if (!NewTemporarySummon::HandlePreSummonActions(summoner, creatureLevel, maxSummons))
        return false;

    // Guardians inherit their summoner's faction and level unless a flag forbids it or the properties override it
    if (summoner)
    {
        if (!_summonProperties || _summonProperties->Faction == 0)
            SetFaction(summoner->GetFaction());

        if (!_summonProperties || !_summonProperties->GetFlags().HasFlag(SummonPropertiesFlags::UseCreatureLevel))
            SetLevel(summoner->getLevel());
    }

    InitializeStats();

    return true;
}

void NewGuardian::InitializeStats()
{
    CreatureTemplate const* creatureInfo = GetCreatureTemplate();
    if (!creatureInfo)
        return;

    SetMeleeDamageSchool(SpellSchools(creatureInfo->dmgschool));

    uint32 creatureIdForStats = creatureInfo->Entry;
    if (IsPet() && ToNewPet()->IsHunterPet())
    {
        creatureIdForStats = 1; // hunter pet level stats are stored under creatureId 1 in pet_levelstats
        SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS, CLASS_WARRIOR);
        SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_GENDER, GENDER_NONE);
        SetAttackTime(BASE_ATTACK, BASE_ATTACK_TIME);
        SetAttackTime(OFF_ATTACK, BASE_ATTACK_TIME);
        SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
        SetSheath(SHEATH_STATE_MELEE);
        SetPowerType(POWER_FOCUS);
    }
    else
    {
        SetAttackTime(BASE_ATTACK, creatureInfo->BaseAttackTime);
        SetAttackTime(OFF_ATTACK, creatureInfo->BaseAttackTime);
        SetAttackTime(RANGED_ATTACK, creatureInfo->RangeAttackTime);
    }

    if (PetLevelInfo const* petLevelInfo = sObjectMgr->GetPetLevelInfo(creatureIdForStats, getLevel()))
    {
        for (uint8 i = 0; i < MAX_STATS; ++i)
            SetCreateStat(Stats(i), float(petLevelInfo->stats[i]));

        if (petLevelInfo->armor > 0)
            SetStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, float(petLevelInfo->armor));

        SetMaxHealth(petLevelInfo->health);
        SetCreateHealth(petLevelInfo->health);
        SetCreateMana(petLevelInfo->mana);

        _isUsingRealStats = true;
    }
    else
    {
        // Guardian has no pet level data, fall back to default creature behavior of Creature::UpdateEntry
        uint32 previousHealth = GetHealth();
        UpdateLevelDependantStats();
        if (previousHealth > 0)
            SetHealth(previousHealth);

        SetMeleeDamageSchool(SpellSchools(creatureInfo->dmgschool));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_HOLY, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_HOLY]));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_FIRE, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_FIRE]));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_NATURE, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_NATURE]));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_FROST, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_FROST]));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_SHADOW, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_SHADOW]));
        SetStatFlatModifier(UNIT_MOD_RESISTANCE_ARCANE, BASE_VALUE, float(creatureInfo->resistance[SPELL_SCHOOL_ARCANE]));
    }

    SetCanModifyStats(true);
    UpdateAllStats();
    SetFullHealth();
}

void NewGuardian::HandlePostSummonActions()
{
    NewTemporarySummon::HandlePostSummonActions();

    CastPassiveAuras();
}

// ------------- private methods

void NewGuardian::CastPassiveAuras()
{
    CreatureTemplate const* creatureInfo = GetCreatureTemplate();
    if (!creatureInfo)
        return;

    CreatureFamilyEntry const* creatureFamilyEntry = sCreatureFamilyStore.LookupEntry(creatureInfo->family);
    if (!creatureFamilyEntry)
        return;

    PetFamilySpellsStore::const_iterator petSpellStore = sPetFamilySpellsStore.find(creatureFamilyEntry->ID);
    if (petSpellStore == sPetFamilySpellsStore.end())
        return;

    for (uint32 spellId : petSpellStore->second)
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
        {
            if (spellInfo->IsPassive())
            {
                CastSpell(this, spellId);
                if (spellInfo->HasAttribute(SPELL_ATTR4_OWNER_POWER_SCALING))
                    _scalingAuras.push_back(spellId);
            }
        }
    }

    if (!_scalingAuras.empty())
        _scalingAuraUpdateTimer = 1s;
}

void NewGuardian::UpdateScalingAuras()
{
    for (uint32 spellId : _scalingAuras)
        if (Aura* aura = GetAura(spellId))
            aura->RecalculateAmountOfEffects();
}
