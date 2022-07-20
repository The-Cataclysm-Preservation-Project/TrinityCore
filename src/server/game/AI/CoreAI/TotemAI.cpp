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

#include "TotemAI.h"
#include "Totem.h"
#include "Creature.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

static constexpr uint32 TargetSearchInterval = 400; // let's mimic one retail AI tick

int32 TotemAI::Permissible(Creature const* creature)
{
    if (creature->IsSummon() && creature->IsTotem())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

TotemAI::TotemAI(Creature* creature) : NullCreatureAI(creature), _spellInfo(nullptr), _spellRange(0.f), _targetSearchTimer(0) { }

void TotemAI::JustAppeared()
{
    NullCreatureAI::JustAppeared();

    _spellInfo = sSpellMgr->GetSpellInfo(me->m_spells[0]);
    if (!_spellInfo)
        return;

    if (_spellInfo->IsPassive())
        DoCastSelf(_spellInfo->Id);
    else
        _spellRange = _spellInfo->GetMaxRange(false);
}

void TotemAI::UpdateAI(uint32 diff)
{
    // Passive aura totems do not need any further casting
    if (!_spellInfo || _spellInfo->IsPassive())
        return;

    if (!me->IsAlive() || me->IsNonMeleeSpellCast(false))
        return;

    if (_targetSearchTimer <= diff)
    {
        Unit* target = SelectTotemTarget();

        if ((!target && !_spellInfo->NeedsExplicitUnitTarget()) || target)
        {
            me->CastSpell(target, _spellInfo->Id);
            if (target)
                _victimGUID = target->GetGUID();
        }
        else
            _targetSearchTimer = TargetSearchInterval;
    }
    else
        _targetSearchTimer -= diff;
}

Unit* TotemAI::SelectTotemTarget()
{
    // Some totems do cast AoE spells so we don't do any target selection
    if (!_spellInfo->NeedsExplicitUnitTarget())
        return nullptr;

    Unit* target = nullptr;
    if (!_victimGUID.IsEmpty())
    {
        // We have an active target. Check if we can still attack it
        target = ObjectAccessor::GetUnit(*me, _victimGUID);
        if (!target || !target->IsAlive() || target->isTargetableForAttack() || me->IsFriendlyTo(target) || !me->CanSeeOrDetect(target) || !me->IsWithinDistInMap(target, _spellRange))
        {
            _victimGUID = ObjectGuid::Empty;
            target = nullptr;
        }
    }

    if (!target)
    {
        // No valid target has been found so far. Let's try to find a new one
        Trinity::NearestAttackableUnitInObjectRangeCheck u_check(me, Coalesce<Unit const>(me->GetCreator(), me), _spellRange);
        Trinity::UnitLastSearcher<Trinity::NearestAttackableUnitInObjectRangeCheck> checker(me, target, u_check);
        Cell::VisitAllObjects(me, checker, _spellRange);
    }

    return target;
}
