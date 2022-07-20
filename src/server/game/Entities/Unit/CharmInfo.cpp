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

#include "CharmInfo.h"
#include "Creature.h"
#include "PetAI.h"
#include "MotionMaster.h"
#include "MoveSpline.h"
#include "NewPet.h"
#include "SpellInfo.h"

CharmInfo::CharmInfo(Unit* unit) :
    _unit(unit), _commandState(COMMAND_FOLLOW), _petnumber(0), _oldReactState(REACT_PASSIVE),
    _isCommandAttack(false), _isCommandFollow(false), _isAtStay(false), _isFollowing(false), _isReturning(false)
{
    for (uint8 i = 0; i < MAX_SPELL_CHARM; ++i)
        _charmspells[i].SetActionAndType(0, ACT_DISABLED);

    if (Creature* creature = _unit->ToCreature())
    {
        _oldReactState = creature->GetReactState();
        creature->SetReactState(REACT_PASSIVE);
    }
}

CharmInfo::~CharmInfo() { }

void CharmInfo::RestoreState()
{
    if (Creature* creature = _unit->ToCreature())
        creature->SetReactState(_oldReactState);
}

void CharmInfo::InitPetActionBar()
{
    // the first 3 SpellOrActions are attack, follow and moveTo
    SetActionBar(ACTION_BAR_INDEX_ATTACK, COMMAND_ATTACK, ACT_COMMAND);
    SetActionBar(ACTION_BAR_INDEX_FOLLOW, COMMAND_FOLLOW, ACT_COMMAND);
    SetActionBar(ACTION_BAR_INDEX_MOVE_TO, COMMAND_MOVE_TO, ACT_COMMAND);

    // middle 4 SpellOrActions are spells/special attacks/abilities
    for (uint32 i = 0; i < ACTION_BAR_INDEX_PET_SPELL_END-ACTION_BAR_INDEX_PET_SPELL_START; ++i)
        SetActionBar(ACTION_BAR_INDEX_PET_SPELL_START + i, 0, ACT_PASSIVE);

    // last 3 SpellOrActions are reactions
    SetActionBar(ACTION_BAR_INDEX_ASSIST, REACT_ASSIST, ACT_REACTION);
    SetActionBar(ACTION_BAR_INDEX_DEFENSIVE, REACT_DEFENSIVE, ACT_REACTION);
    SetActionBar(ACTION_BAR_INDEX_PASSIVE, REACT_PASSIVE, ACT_REACTION);
}

void CharmInfo::InitEmptyActionBar(bool withAttack /*= true*/)
{
    if (withAttack)
        SetActionBar(ACTION_BAR_INDEX_START, COMMAND_ATTACK, ACT_COMMAND);
    else
        SetActionBar(ACTION_BAR_INDEX_START, 0, ACT_PASSIVE);
    for (uint32 x = ACTION_BAR_INDEX_START+1; x < ACTION_BAR_INDEX_END; ++x)
        SetActionBar(x, 0, ACT_PASSIVE);
}

void CharmInfo::InitPossessCreateSpells()
{
    if (!_unit->IsCreature())
    {
        InitEmptyActionBar();
        return;
    }

    Creature* creature = _unit->ToCreature();

    if (!creature->GetCreatureTemplate()->StaticFlags.HasFlag(CREATURE_STATIC_FLAG_2_FULL_SPELL_LIST))
        InitEmptyActionBar();

    for (uint8 i = 0; i < MAX_CREATURE_SPELLS; ++i)
    {
        uint32 spellId = creature->m_spells[i];
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (spellInfo)
        {
            if (spellInfo->HasAttribute(SPELL_ATTR5_NOT_AVAILABLE_WHILE_CHARMED))
                continue;

            if (spellInfo->IsPassive())
                _unit->CastSpell(_unit, spellInfo->Id, true);
            else
                AddSpellToActionBar(spellInfo, ACT_PASSIVE, i % MAX_UNIT_ACTION_BAR_INDEX);
        }
    }
}

void CharmInfo::InitCharmCreateSpells()
{
    if (_unit->GetTypeId() == TYPEID_PLAYER)                // charmed players don't have spells
    {
        InitEmptyActionBar();
        return;
    }

    InitPetActionBar();

    for (uint32 x = 0; x < MAX_SPELL_CHARM; ++x)
    {
        uint32 spellId = _unit->ToCreature()->m_spells[x];
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

        if (!spellInfo)
        {
            _charmspells[x].SetActionAndType(spellId, ACT_DISABLED);
            continue;
        }

        if (spellInfo->HasAttribute(SPELL_ATTR5_NOT_AVAILABLE_WHILE_CHARMED))
            continue;

        if (spellInfo->IsPassive())
        {
            _unit->CastSpell(_unit, spellInfo->Id, true);
            _charmspells[x].SetActionAndType(spellId, ACT_PASSIVE);
        }
        else
        {
            _charmspells[x].SetActionAndType(spellId, ACT_DISABLED);

            ActiveStates newstate = ACT_PASSIVE;

            if (!spellInfo->IsAutocastable())
                newstate = ACT_PASSIVE;
            else
            {
                if (spellInfo->NeedsExplicitUnitTarget())
                {
                    newstate = ACT_ENABLED;
                    ToggleCreatureAutocast(spellInfo, true);
                }
                else
                    newstate = ACT_DISABLED;
            }

            AddSpellToActionBar(spellInfo, newstate);
        }
    }
}

bool CharmInfo::AddSpellToActionBar(SpellInfo const* spellInfo, ActiveStates newstate, uint8 preferredSlot)
{
    uint32 spell_id = spellInfo->Id;
    uint32 first_id = spellInfo->GetFirstRankSpell()->Id;

    ASSERT(preferredSlot < MAX_UNIT_ACTION_BAR_INDEX);
    // new spell rank can be already listed
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (uint32 action = PetActionBar[i].GetAction())
        {
            if (PetActionBar[i].IsActionBarForSpell() && sSpellMgr->GetFirstSpellInChain(action) == first_id)
            {
                PetActionBar[i].SetAction(spell_id);
                return true;
            }
        }
    }

    // or use empty slot in other case
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        uint8 j = (preferredSlot + i) % MAX_UNIT_ACTION_BAR_INDEX;
        if (!PetActionBar[j].GetAction() && PetActionBar[j].IsActionBarForSpell())
        {
            SetActionBar(j, spell_id, newstate == ACT_DECIDE ? spellInfo->IsAutocastable() ? ACT_ENABLED : ACT_PASSIVE : newstate);
            return true;
        }
    }
    return false;
}

bool CharmInfo::RemoveSpellFromActionBar(uint32 spell_id)
{
    uint32 first_id = sSpellMgr->GetFirstSpellInChain(spell_id);

    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (uint32 action = PetActionBar[i].GetAction())
        {
            if (PetActionBar[i].IsActionBarForSpell() && sSpellMgr->GetFirstSpellInChain(action) == first_id)
            {
                SetActionBar(i, 0, ACT_PASSIVE);
                return true;
            }
        }
    }

    return false;
}

void CharmInfo::ToggleCreatureAutocast(SpellInfo const* spellInfo, bool apply)
{
    if (spellInfo->IsPassive())
        return;

    for (uint32 x = 0; x < MAX_SPELL_CHARM; ++x)
        if (spellInfo->Id == _charmspells[x].GetAction())
            _charmspells[x].SetType(apply ? ACT_ENABLED : ACT_DISABLED);
}

void CharmInfo::SetPetNumber(uint32 petnumber, bool statwindow)
{
    _petnumber = petnumber;
    if (statwindow)
        _unit->SetUInt32Value(UNIT_FIELD_PETNUMBER, _petnumber);
    else
        _unit->SetUInt32Value(UNIT_FIELD_PETNUMBER, 0);
}

void CharmInfo::SetCommandState(CommandStates state, Unit* target /*= nullptr*/, Position const* destination /*= nullptr*/)
{
    switch (state)
    {
        case COMMAND_FOLLOW:
            if (Unit* owner = _unit->GetCharmerOrOwner())
            {
                _unit->AttackStop();
                _unit->InterruptNonMeleeSpells(false);
                _unit->FollowTarget(owner);
                SetIsCommandAttack(false);
                SetIsAtStay(false);
                SetIsReturning(true);
                SetIsCommandFollow(true);
                SetIsFollowing(false);
            }
            break;
        case COMMAND_MOVE_TO:
            if (destination)
            {
                _unit->GetMotionMaster()->MovePoint(0, *destination);
                _unit->GetMotionMaster()->Clear(MOTION_SLOT_IDLE);
                _unit->GetMotionMaster()->MoveIdle();

                SetIsCommandAttack(false);
                SetIsAtStay(true);
                SetIsCommandFollow(false);
                SetIsFollowing(false);
                SetIsReturning(false);
                SaveStayPosition();
            }
            break;
        case COMMAND_STAY:
            if (_unit->GetMotionMaster()->GetCurrentSlot() != MOTION_SLOT_CONTROLLED)
                _unit->StopMoving();
            _unit->GetMotionMaster()->Clear(MOTION_SLOT_IDLE);
            _unit->GetMotionMaster()->Clear(MOTION_SLOT_ACTIVE);
            _unit->GetMotionMaster()->MoveIdle();

            SetIsCommandAttack(false);
            SetIsAtStay(true);
            SetIsCommandFollow(false);
            SetIsFollowing(false);
            SetIsReturning(false);
            SaveStayPosition();
            break;
        case COMMAND_ATTACK:
            // Attack commands are no real commands and instead they just command the pet to chase and attack their victim. That's why we return from this case
            if (target)
            {
                Unit* owner = _unit->GetCharmerOrOwner();
                if (!owner || !owner->IsValidAttackTarget(target))
                    return;

                // Can't attack if owner is pacified
                if (owner->HasAuraType(SPELL_AURA_MOD_PACIFY))
                {
                    // pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                    /// @todo Send proper error message to client
                    return;
                }

                if (_unit->GetVictim() != target)
                {
                    if (_unit->GetVictim())
                        _unit->AttackStop();

                    if (_unit->IsCreature())
                    {
                        SetIsCommandAttack(true);
                        SetIsAtStay(false);
                        SetIsFollowing(false);
                        SetIsCommandFollow(false);
                        SetIsReturning(false);

                        Creature* creature = _unit->ToCreature();
                        if (CreatureAI* ai = creature->AI())
                        {
                            if (PetAI* petAI = dynamic_cast<PetAI*>(ai))
                                petAI->_AttackStart(target); // force target switch
                            else
                                ai->AttackStart(target);
                        }
                    }
                    else // charmed player
                    {
                        SetIsCommandAttack(true);
                        SetIsAtStay(false);
                        SetIsFollowing(false);
                        SetIsCommandFollow(false);
                        SetIsReturning(false);

                        _unit->Attack(target, true);
                    }
                }
            }
            return;
        default:
            break;
    }

    _commandState = state;
}

void CharmInfo::LoadPetActionBar(const std::string& data, NewPet const* pet)
{
    Tokenizer tokens(data, ' ');

    if (tokens.size() != (ACTION_BAR_INDEX_END-ACTION_BAR_INDEX_START) * 2)
        return;                                             // non critical, will reset to default

    InitPetActionBar();

    uint8 index = ACTION_BAR_INDEX_START;
    Tokenizer::const_iterator iter = tokens.begin();
    for (; index < ACTION_BAR_INDEX_END; ++iter, ++index)
    {
        // use unsigned cast to avoid sign negative format use at long-> ActiveStates (int) conversion
        ActiveStates type  = ActiveStates(atol(*iter));
        ++iter;
        uint32 action = atoul(*iter);

        PetActionBar[index].SetActionAndType(action, type);

        // check correctness
        if (PetActionBar[index].IsActionBarForSpell())
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(PetActionBar[index].GetAction());
            if (!spellInfo || !pet->HasSpell(spellInfo->Id))
                SetActionBar(index, 0, ACT_PASSIVE);
            else if (!spellInfo->IsAutocastable())
                SetActionBar(index, PetActionBar[index].GetAction(), ACT_PASSIVE);
        }
    }
}

void CharmInfo::BuildActionBar(WorldPacket* data)
{
    for (uint32 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
        *data << uint32(PetActionBar[i].packedData);
}

void CharmInfo::BuildActionBar(std::array<uint32, 10>& actionButtons)
{
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
        actionButtons[i] = PetActionBar[i].packedData;
}

void CharmInfo::SetSpellAutocast(SpellInfo const* spellInfo, bool state)
{
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (spellInfo->Id == PetActionBar[i].GetAction() && PetActionBar[i].IsActionBarForSpell())
        {
            PetActionBar[i].SetType(state ? ACT_ENABLED : ACT_DISABLED);
            break;
        }
    }
}

void CharmInfo::SetIsCommandAttack(bool val)
{
    _isCommandAttack = val;
}

bool CharmInfo::IsCommandAttack()
{
    return _isCommandAttack;
}

void CharmInfo::SetIsCommandFollow(bool val)
{
    _isCommandFollow = val;
}

bool CharmInfo::IsCommandFollow()
{
    return _isCommandFollow;
}

void CharmInfo::SaveStayPosition()
{
    //! At this point a new spline destination is enabled because of Unit::StopMoving()
    G3D::Vector3 stayPos = _unit->movespline->FinalDestination();

    if (_unit->movespline->onTransport)
        if (TransportBase* transport = _unit->GetDirectTransport())
            transport->CalculatePassengerPosition(stayPos.x, stayPos.y, stayPos.z);

    _stayPosition = Position(stayPos.x, stayPos.y, stayPos.z);
}

void CharmInfo::GetStayPosition(Position& position)
{
    position = _stayPosition;
}

void CharmInfo::SetIsAtStay(bool val)
{
    _isAtStay = val;
}

bool CharmInfo::IsAtStay()
{
    return _isAtStay;
}

void CharmInfo::SetIsFollowing(bool val)
{
    _isFollowing = val;
}

bool CharmInfo::IsFollowing()
{
    return _isFollowing;
}

void CharmInfo::SetIsReturning(bool val)
{
    _isReturning = val;
}

bool CharmInfo::IsReturning()
{
    return _isReturning;
}

