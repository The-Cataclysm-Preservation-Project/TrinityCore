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
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_gen_".
 */

 /* ContentData
 npc_pet_gen_baby_blizzard_bear     100%    Baby Blizzard Bear sits down occasionally
 npc_pet_gen_egbert                 100%    Egbert run's around
 npc_pet_gen_pandaren_monk          100%    Pandaren Monk drinks and bows with you
 npc_pet_gen_mojo                   100%    Mojo follows you when you kiss it
 EndContentData */

#include "ScriptMgr.h"
#include "DBCStructure.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "PassiveAI.h"
#include "Player.h"
#include "ScriptedCreature.h"

enum BabyBlizzardBearMisc
{
    SPELL_BBB_PET_SIT = 61853,
    EVENT_BBB_PET_SIT = 1,
    EVENT_BBB_PET_SIT_INTER = 2
};

class npc_pet_gen_baby_blizzard_bear : public CreatureScript
{
public:
    npc_pet_gen_baby_blizzard_bear() : CreatureScript("npc_pet_gen_baby_blizzard_bear") {}

    struct npc_pet_gen_baby_blizzard_bearAI : public NullCreatureAI
    {
        npc_pet_gen_baby_blizzard_bearAI(Creature* creature) : NullCreatureAI(creature)
        {
            if (Unit* owner = me->GetCharmerOrOwner())
                me->FollowTarget(owner);
            _events.ScheduleEvent(EVENT_BBB_PET_SIT, urandms(10, 30));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Unit* owner = me->GetCharmerOrOwner())
                if (!me->IsWithinDist(owner, 25.f))
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BBB_PET_SIT:
                    me->CastSpell(me, SPELL_BBB_PET_SIT, false);
                    _events.ScheduleEvent(EVENT_BBB_PET_SIT_INTER, urandms(15, 30));
                    break;
                case EVENT_BBB_PET_SIT_INTER:
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                    _events.ScheduleEvent(EVENT_BBB_PET_SIT, urandms(10, 30));
                    break;
                default:
                    break;
                }
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_pet_gen_baby_blizzard_bearAI(creature);
    }
};

enum EgbertMisc
{
    SPELL_EGBERT = 40669,
    EVENT_RETURN = 3
};

class npc_pet_gen_egbert : public CreatureScript
{
public:
    npc_pet_gen_egbert() : CreatureScript("npc_pet_gen_egbert") {}

    struct npc_pet_gen_egbertAI : public NullCreatureAI
    {
        npc_pet_gen_egbertAI(Creature* creature) : NullCreatureAI(creature)
        {
            if (Unit* owner = me->GetCharmerOrOwner())
                if (owner->GetMap()->GetEntry()->ExpansionID > 1)
                    me->SetCanFly(true);
        }

        void Reset() override
        {
            _events.Reset();
            if (Unit* owner = me->GetCharmerOrOwner())
                me->FollowTarget(owner);
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            if (!_EnterEvadeMode(why))
                return;

            Reset();
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Unit* owner = me->GetCharmerOrOwner())
            {
                if (!me->IsWithinDist(owner, 40.f))
                {
                    me->RemoveAura(SPELL_EGBERT);
                    me->FollowTarget(owner);
                }
            }

            if (me->HasAura(SPELL_EGBERT))
                _events.ScheduleEvent(EVENT_RETURN, urandms(5, 20));

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RETURN:
                    me->RemoveAura(SPELL_EGBERT);
                    break;
                default:
                    break;
                }
            }
        }
    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_pet_gen_egbertAI(creature);
    }
};

enum PandarenMonkMisc
{
    SPELL_PANDAREN_MONK = 69800,
    EVENT_FOCUS = 1,
    EVENT_EMOTE = 2,
    EVENT_FOLLOW = 3,
    EVENT_DRINK = 4
};

class npc_pet_gen_pandaren_monk : public CreatureScript
{
public:
    npc_pet_gen_pandaren_monk() : CreatureScript("npc_pet_gen_pandaren_monk") {}

    struct npc_pet_gen_pandaren_monkAI : public NullCreatureAI
    {
        npc_pet_gen_pandaren_monkAI(Creature* creature) : NullCreatureAI(creature) { }

        void Reset() override
        {
            _events.Reset();
            _events.ScheduleEvent(EVENT_FOCUS, 1000);
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            if (!_EnterEvadeMode(why))
                return;

            Reset();
        }

        void ReceiveEmote(Player* /*player*/, uint32 emote) override
        {
            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            me->StopMoving();

            switch (emote)
            {
            case TEXT_EMOTE_BOW:
                _events.ScheduleEvent(EVENT_FOCUS, 1000);
                break;
            case TEXT_EMOTE_DRINK:
                _events.ScheduleEvent(EVENT_DRINK, 1000);
                break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Unit* owner = me->GetCharmerOrOwner())
                if (!me->IsWithinDist(owner, 30.f))
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FOCUS:
                    if (Unit* owner = me->GetCharmerOrOwner())
                        me->SetFacingToObject(owner);
                    _events.ScheduleEvent(EVENT_EMOTE, 1000);
                    break;
                case EVENT_EMOTE:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                    _events.ScheduleEvent(EVENT_FOLLOW, 1000);
                    break;
                case EVENT_FOLLOW:
                    if (Unit* owner = me->GetCharmerOrOwner())
                        me->FollowTarget(owner);
                    break;
                case EVENT_DRINK:
                    me->CastSpell(me, SPELL_PANDAREN_MONK, false);
                    break;
                default:
                    break;
                }
            }
        }
    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_pet_gen_pandaren_monkAI(creature);
    }
};

enum Mojo
{
    SAY_MOJO                = 0,

    SPELL_FEELING_FROGGY    = 43906,
    SPELL_SEDUCTION_VISUAL  = 43919
};

class npc_pet_gen_mojo : public CreatureScript
{
    public:
        npc_pet_gen_mojo() : CreatureScript("npc_pet_gen_mojo") { }

        struct npc_pet_gen_mojoAI : public ScriptedAI
        {
            npc_pet_gen_mojoAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset() override
            {
                _victimGUID.Clear();

                if (Unit* owner = me->GetOwner())
                    me->GetMotionMaster()->MoveFollow(owner, 0.0f, 0.0f);
            }

            void JustEngagedWith(Unit* /*who*/) override { }
            void UpdateAI(uint32 /*diff*/) override { }

            void ReceiveEmote(Player* player, uint32 emote) override
            {
                me->HandleEmoteCommand(emote);
                Unit* owner = me->GetOwner();
                if (emote != TEXT_EMOTE_KISS || !owner || owner->GetTypeId() != TYPEID_PLAYER ||
                    owner->ToPlayer()->GetTeam() != player->GetTeam())
                {
                    return;
                }

                Talk(SAY_MOJO, player);

                if (_victimGUID)
                    if (Player* victim = ObjectAccessor::GetPlayer(*me, _victimGUID))
                        victim->RemoveAura(SPELL_FEELING_FROGGY);

                _victimGUID = player->GetGUID();

                DoCast(player, SPELL_FEELING_FROGGY, true);
                DoCast(me, SPELL_SEDUCTION_VISUAL, true);
                me->GetMotionMaster()->MoveFollow(player, 0.0f, 0.0f);
            }

        private:
            ObjectGuid _victimGUID;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_gen_mojoAI(creature);
        }
};

// Lil' Ragnaros
enum LilRagnaros
{
    SPELL_DND_LR_2                      = 95804, // Root and emerge effect
    SPELL_DND_SUMMON_BASIC_CAMPFIRE     = 95811,
    SPELL_DND_LR                        = 95802,
    SPELL_SCORCHLING                    = 45887,
    SPELL_DND_LR_1                      = 95803, // Submerge effect
    SPELL_DND_DESPAWN_BASIC_CAMPFIRE    = 95813,

    EVENT_CHECK_PLAYER_DISTANCE         = 1,
};

class npc_pet_gen_lil_ragnaros : public CreatureScript
{
    public:
        npc_pet_gen_lil_ragnaros() : CreatureScript("npc_pet_gen_lil_ragnaros") { }

        struct npc_pet_gen_lil_ragnarosAI : public ScriptedAI
        {
            npc_pet_gen_lil_ragnarosAI(Creature* creature) : ScriptedAI(creature)
            {
                Initialize();
            }

            void Initialize()
            {
                _submerged = false;
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                DoCastSelf(SPELL_DND_LR_2, true);
                DoCastSelf(SPELL_DND_SUMMON_BASIC_CAMPFIRE, true);
                DoCastSelf(SPELL_DND_LR, true);
                DoCastSelf(SPELL_SCORCHLING, true);
                _events.ScheduleEvent(EVENT_CHECK_PLAYER_DISTANCE, Seconds(1));
            }

            void UpdateAI(uint32 diff) override
            {
                _events.Update(diff);

                if (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_PLAYER_DISTANCE:
                            if (Unit* owner = me->GetCharmerOrOwner())
                            {
                                if (me->GetDistance(owner) >= 20.0f && !_submerged)
                                {
                                    me->RemoveAllAuras();
                                    DoCastSelf(SPELL_DND_LR_1, true);
                                    DoCastSelf(SPELL_DND_DESPAWN_BASIC_CAMPFIRE, true);
                                    _submerged = true;
                                }
                                else if (me->GetDistance(owner) <= 5.0f && _submerged)
                                {
                                    me->RemoveAurasDueToSpell(SPELL_DND_LR_1);
                                    DoCastSelf(SPELL_DND_LR_2, true);
                                    DoCastSelf(SPELL_DND_SUMMON_BASIC_CAMPFIRE, true);
                                    DoCastSelf(SPELL_DND_LR, true);
                                    DoCastSelf(SPELL_SCORCHLING, true);
                                    _submerged = false;
                                }
                            }
                            _events.Repeat(Seconds(1));
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            EventMap _events;
            bool _submerged;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_gen_lil_ragnarosAI(creature);
        }
};

enum SoulTrader
{
    SAY_SOUL_TRADER_INTRO           = 0,

    SPELL_ETHEREAL_ONSUMMON         = 50052,
    SPELL_ETHEREAL_PET_REMOVE_AURA  = 50055
};

struct npc_pet_gen_soul_trader : public ScriptedAI
{
    npc_pet_gen_soul_trader(Creature* creature) : ScriptedAI(creature) { }

    void LeavingWorld() override
    {
        if (Unit* owner = me->GetOwner())
            DoCast(owner, SPELL_ETHEREAL_PET_REMOVE_AURA);
    }

    void JustAppeared() override
    {
        Talk(SAY_SOUL_TRADER_INTRO);
        if (Unit* owner = me->GetOwner())
            DoCast(owner, SPELL_ETHEREAL_ONSUMMON);
    }
};

void AddSC_generic_pet_scripts()
{
    new npc_pet_gen_baby_blizzard_bear();
    new npc_pet_gen_egbert();
    new npc_pet_gen_pandaren_monk();
    new npc_pet_gen_mojo();
    new npc_pet_gen_lil_ragnaros();
    RegisterCreatureAI(npc_pet_gen_soul_trader);
}
