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

 /* ScriptData
 SDName: Silverpine_Forest
 SD%Complete: 75 %
 SDComment:
 SDCategory: Silverpine Forest
 EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "Player.h"
#include "Vehicle.h"
#include "Unit.h"
#include "GridNotifiers.h"

 // Zone Silverpine Forest
class playerScript_silverpine_gilneas : public PlayerScript
{
public:
    playerScript_silverpine_gilneas() : PlayerScript("playerScript_silverpine_gilneas") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        // HACK FIX
        // We need this to make sure the player summons the NPC after logging in,
        // due to the fact that the aura is kept after logging off, adding the
        // aura again won't summon the NPCs. (This kind of behaviour should be implemented
        // at some point, simply adding the aura as not kept after logging off won't make it work)

        if (player->GetZoneId() == 130)
        {
            player->RemoveAura(83982); // Summon Agatha

            player->RemoveAura(85517); // Summon Godfrey
            player->RemoveAura(85518); // Summon Walden
            player->RemoveAura(85519); // Summon Ashbury

            player->RemoveAura(85870); // Summon Agatha
            player->RemoveAura(85869); // Summon Arthura
            player->RemoveAura(85871); // Summon Daschla
            player->RemoveAura(85868); // Summon Cromush
            player->RemoveAura(85874); // Song of Sylvanas
        }
    }

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 newArea) override
    {
        if (newArea == 233) // Ambermill
        {
            if (player->GetQuestStatus(27518) != QUEST_STATUS_COMPLETE)
                player->CastSpell(player, 85361, true);

            if (const Quest* Quest_Chapter_2 = sObjectMgr->GetQuestTemplate(27513))
            {
                if (player->GetQuestStatus(27512) == QUEST_STATUS_REWARDED && player->GetQuestStatus(27513) == QUEST_STATUS_NONE)
                    player->AddQuest(Quest_Chapter_2, nullptr);
            }

            if (const Quest* Quest_Chapter_3 = sObjectMgr->GetQuestTemplate(27518))
            {
                if (player->GetQuestStatus(27513) == QUEST_STATUS_REWARDED && player->GetQuestStatus(27518) == QUEST_STATUS_NONE)
                    player->AddQuest(Quest_Chapter_3, nullptr);
            }
        }
    }
};

// Worgen Renegade - 44793
class npc_worgen_renegade_44793 : public CreatureScript
{
public:
    npc_worgen_renegade_44793() : CreatureScript("npc_worgen_renegade_44793") { }

    enum eNPC
    {
        NPC_FORSAKEN_TROOPER1 = 44791,
        NPC_FORSAKEN_TROOPER2 = 44792,
        EVENT_CHECK_SHOWFIGHT = 101,
        EVENT_SPELL_QUEUE = 102,
        EVENT_SPELL_DAMAGE = 103,
        MOVE_TO_HOMEPOSITION,
    };

    struct npc_worgen_renegade_44793AI : public ScriptedAI
    {
        npc_worgen_renegade_44793AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;
        bool     m_isShowFight;

        void Reset() override
        {
            m_events.Reset();
            me->RemoveAura(73307);
            m_isShowFight = true;
            m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 1000);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (!caster)
                return;

            if (spell->Id == 84182)
                me->AddAura(73307, me);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            m_events.ScheduleEvent(EVENT_SPELL_QUEUE, urand(1500, 5000));
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* forsaken = me->FindNearestCreature(NPC_FORSAKEN_TROOPER1 || NPC_FORSAKEN_TROOPER2, 5.0f))
                    {
                        me->Attack(forsaken, true);
                        return;
                    }
                    else
                        m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SPELL_QUEUE:
                {
                    DoCastVictim(80365);
                    m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, 1000);
                    m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, 2000);
                    m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, 3000);
                    m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, 4000);
                    m_events.ScheduleEvent(EVENT_SPELL_QUEUE, urand(10000, 17500));
                    break;
                }

                case EVENT_SPELL_DAMAGE:
                    DoCastVictim(80367);
                    break;
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_renegade_44793AI(creature);
    }
};

// Forsaken Trooper - 44791, 44792
class npc_forsaken_trooper_44791 : public CreatureScript
{
public:
    npc_forsaken_trooper_44791() : CreatureScript("npc_forsaken_trooper_44791") { }

    enum eNPC
    {
        NPC_WORGEN_RENEGATE = 44793,
        EVENT_CHECK_SHOWFIGHT = 101,
        EVENT_SPELL_DAMAGE,
        MOVE_TO_HOMEPOSITION,
    };

    struct npc_forsaken_trooper_44791AI : public ScriptedAI
    {
        npc_forsaken_trooper_44791AI(Creature* creature) : ScriptedAI(creature) {}

        EventMap m_events;
        bool m_isShowFight;
        uint8 randomSpellIndex;

        void Reset() override
        {
            m_events.Reset();

            if (urand(0, 1) == 0)
                me->AI()->Talk(0);

            m_isShowFight = true;
            m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 1000);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen = me->FindNearestCreature(NPC_WORGEN_RENEGATE, 5.0f))
                    {
                        me->Attack(worgen, true);
                        m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                        return;
                    }

                    m_events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SPELL_DAMAGE:
                {
                    uint32 spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];

                    if (!spellId)
                    {
                        randomSpellIndex = 0;
                        spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];
                    }

                    if (!spellId)
                        break;

                    if (++randomSpellIndex > 8)
                        randomSpellIndex = 0;

                    DoCastVictim(spellId);

                    m_events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_trooper_44791AI(creature);
    }
};

// Grand Executor Mortuus - 44615
class npc_grand_executor_mortuus_44615 : public CreatureScript
{
public:
    npc_grand_executor_mortuus_44615() : CreatureScript("npc_grand_executor_mortuus_44615") { }

    enum eNPC
    {
        QUEST_THE_WARCHIEF_COMETH_26965 = 26965,
        NPC_GARROSH_HELLSCREAM_44629 = 44629,
        NPC_HIGH_WARLORD_CROMUSH_44640 = 44640,
        NPC_LADY_SYLVANAS_WINDRUNNER_44365 = 44365,
        NPC_HELLSCREEMS_ELITE_44636 = 44636,
        NPC_AGATHA_44608 = 44608,
        NPC_PORTAL_FROM_ORGRIMMAR_44630 = 44630,
        NPC_GRAND_EXECUTOR_MORTUUS_44615 = 44615,
        PLAYER_GUID = 99999,
        MOVE_CROMUSH_TO_SYLVANAS = 5405701,
        MOVE_CROMUSH_TO_HOME = 5405702,
        MOVE_GARROSH_TO_HOME = 5405703,
        SPELL_RAISE_FORSAKEN = 83173,
        SPELL_AIR_REVENANT_ENTRANCE = 55761,
        SPELL_SIMPLE_TELEPORT = 12980,
        ACTION_START_ANIM = 101,
        EVENT_START_ANIM,
        EVENT_AGATHA_RAISE_FORSAKEN = 201,
        EVENT_TALK_SEQUENCE = 301,
        EVENT_SUMMON_PORTAL = 401,
        EVENT_SUMMON_GARROSH,
    };

    struct npc_grand_executor_mortuus_44615AI : public ScriptedAI
    {
        npc_grand_executor_mortuus_44615AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;
        ObjectGuid  m_playerGUID;
        ObjectGuid  m_garroshGUID;
        ObjectGuid  m_cromushGUID;
        ObjectGuid  m_sylvanasGUID;
        ObjectGuid  m_agathaGUID;
        ObjectGuid  m_mortuusGUID;
        uint8   m_animPhase;
        ObjectGuid m_SpawnedGUID;
        ObjectGuid m_PortalGUID;
        std::list<ObjectGuid>spawnedList;
        std::list<ObjectGuid>portalList;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_THE_WARCHIEF_COMETH_26965)
            {
                me->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                me->GetAI()->DoAction(ACTION_START_ANIM);
            }
        }

        void Reset() override
        {
            m_events.Reset();
            m_playerGUID = ObjectGuid::Empty;
            m_animPhase = 0;
            spawnedList.clear();
            portalList.clear();
        }

        void SetGUID(ObjectGuid const& guid, int32 id) override
        {
            switch (id)
            {
            case PLAYER_GUID:
            {
                m_playerGUID = guid;
                break;
            }
            }
        }

        void JustSummoned(Creature* summon) override
        {
            spawnedList.push_back(summon->GetGUID());

            switch (summon->GetEntry())
            {
            case NPC_GARROSH_HELLSCREAM_44629:
            {
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
                m_garroshGUID = summon->GetGUID();
                break;
            }
            case NPC_HELLSCREEMS_ELITE_44636:
            {
                summon->CastSpell(summon, SPELL_SIMPLE_TELEPORT);
                break;
            }
            case NPC_PORTAL_FROM_ORGRIMMAR_44630:
            {
                portalList.push_back(summon->GetGUID());
                break;
            }
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
            case ACTION_START_ANIM:
            {
                if (!m_animPhase)
                {
                    if (Creature* sylvanas = me->FindNearestCreature(NPC_LADY_SYLVANAS_WINDRUNNER_44365, 100.0f))
                    {
                        if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA_44608, 100.0f))
                        {
                            m_animPhase = 1;
                            m_sylvanasGUID = sylvanas->GetGUID();
                            m_mortuusGUID = me->GetGUID();
                            m_agathaGUID = agatha->GetGUID();
                            m_events.ScheduleEvent(EVENT_START_ANIM, 500);
                        }
                    }
                }
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_START_ANIM:
                {
                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE, 1000);
                    m_events.ScheduleEvent(EVENT_SUMMON_PORTAL, 4000);
                    m_events.ScheduleEvent(EVENT_SUMMON_GARROSH, 7500);
                    break;
                }

                case EVENT_SUMMON_PORTAL:
                {
                    SummonPortalsFromOrgrimmar();
                    break;
                }

                case EVENT_SUMMON_GARROSH:
                {
                    SummonGarroshAndHisEliteGuards();
                    break;
                }

                case EVENT_TALK_SEQUENCE:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(0);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 1, 8000);
                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 25, 4500);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 1:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->SetFacingTo(0.808979f);

                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->GetMotionMaster()->MoveJump(1378.65f, 1044.23f, 53.8389f, 5.251f, 15.0f, 15.0f);

                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, m_cromushGUID))
                        cromush->SetVisible(true);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 2, 3500);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 2:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->SetFacingTo(3.924652f);

                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->AI()->Talk(0);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 3, 12000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 3:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                    {
                        garrosh->SetFacingTo(3.9670f);
                        garrosh->AI()->Talk(1);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 4, 7000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 4:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(2.426115f);
                        sylvanas->AI()->Talk(2);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 5, 5000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 5:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                    {
                        sylvanas->SetFacingTo(3.924652f);
                        sylvanas->AI()->Talk(3);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 6, 15000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 6:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(4);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 7, 4000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 7:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->AI()->Talk(2);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 8, 3000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 8:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(5);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 9, 6000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 9:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(6);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 10, 6000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 10:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(7);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 11, 9000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 11:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(8);

                    m_events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN, 3000);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(1235, 1366.80f, 1030.94f, 61.3620f, false, 1.0f);

                    m_events.ScheduleEvent(EVENT_AGATHA_RAISE_FORSAKEN + 1, 5000);
                    break;
                }

                case EVENT_AGATHA_RAISE_FORSAKEN + 1:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->CastSpell(me, SPELL_RAISE_FORSAKEN);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 12, 10000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 12:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, m_cromushGUID))
                        cromush->AI()->Talk(0);

                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(1235, 1364.02f, 1028.54f, 57.8534f, false, 1.0f);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 13, 3500);
                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 24, 3500);
                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 26, 3500);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 13:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->AI()->Talk(3);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 14, 13000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 14:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->SetFacingTo(0.855211f);

                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(9);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 15, 10000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 15:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->AI()->Talk(4);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 16, 6000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 16:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                        garrosh->AI()->Talk(5);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 17, 6000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 17:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(10);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 18, 5000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 18:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                    {
                        garrosh->SetFacingTo(5.251188f);
                        garrosh->AI()->Talk(6);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 19, 5000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 19:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                    {
                        garrosh->SetFacingTo(2.426115f);
                        garrosh->AI()->Talk(7);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 20, 14000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 20:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, m_cromushGUID))
                        cromush->AI()->Talk(1);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 21, 2500);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 21:
                {
                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                    {
                        garrosh->SetFacingTo(5.251188f);
                        garrosh->AI()->Talk(8);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 22, 7000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 22:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, m_cromushGUID))
                        cromush->GetMotionMaster()->MovePath(446403, false);

                    if (Creature* garrosh = ObjectAccessor::GetCreature(*me, m_garroshGUID))
                    {
                        garrosh->SetFacingTo(3.967066f);
                        garrosh->GetMotionMaster()->MovePath(446290, false);
                    }

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 23, 11000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 23:
                {
                    RemoveAllSpawnedCreatures();

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->KilledMonsterCredit(44629);
                        player->CompleteQuest(26965);
                    }

                    Reset();
                    break;
                }

                case EVENT_TALK_SEQUENCE + 24:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->SetFacingTo(0.855211f);

                    break;
                }

                case EVENT_TALK_SEQUENCE + 25:
                {
                    if (Creature* sylvanas = ObjectAccessor::GetCreature(*me, m_sylvanasGUID))
                        sylvanas->AI()->Talk(1);

                    break;
                }

                case EVENT_TALK_SEQUENCE + 26:
                {
                    if (Creature* mortuus = ObjectAccessor::GetCreature(*me, m_mortuusGUID))
                        mortuus->AI()->Talk(0);

                    m_events.ScheduleEvent(EVENT_TALK_SEQUENCE + 27, 3000);
                    break;
                }

                case EVENT_TALK_SEQUENCE + 27:
                {
                    std::list<Creature*> maleforsaken;
                    std::list<Creature*> femaleforsaken;
                    GetCreatureListWithEntryInGrid(maleforsaken, me, 44592, 100.0f);
                    GetCreatureListWithEntryInGrid(maleforsaken, me, 44593, 100.0f);

                    for (std::list<Creature*>::const_iterator itr = maleforsaken.begin(); itr != maleforsaken.end(); ++itr)
                        (*itr)->AI()->DoAction(0);
                    for (std::list<Creature*>::const_iterator itr = femaleforsaken.begin(); itr != femaleforsaken.end(); ++itr)
                        (*itr)->AI()->DoAction(0);
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

        void SummonPortalsFromOrgrimmar()
        {
            me->SummonCreature(NPC_PORTAL_FROM_ORGRIMMAR_44630, 1358.62f, 1054.72f, 53.1200f, 0, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_PORTAL_FROM_ORGRIMMAR_44630, 1393.27f, 1021.20f, 53.2225f, 0, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_PORTAL_FROM_ORGRIMMAR_44630, 1404.71f, 1063.73f, 60.5617f, 0, TEMPSUMMON_TIMED_DESPAWN, 300000);

            std::list<Creature*> portals;
            GetCreatureListWithEntryInGrid(portals, me, NPC_PORTAL_FROM_ORGRIMMAR_44630, 100.0f);

            for (std::list<Creature*>::const_iterator itr = portals.begin(); itr != portals.end(); ++itr)
                (*itr)->CastSpell(*itr, SPELL_AIR_REVENANT_ENTRANCE);
        }

        void SummonGarroshAndHisEliteGuards()
        {
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1387.90f, 1029.71f, 53.21853f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1389.79f, 1024.51f, 53.20833f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1388.05f, 1026.91f, 53.20833f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1388.16f, 1020.88f, 53.25523f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1385.79f, 1025.99f, 53.22593f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1386.69f, 1023.26f, 53.24393f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1384.33f, 1022.04f, 53.28123f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1391.10f, 1027.73f, 53.20483f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000);

            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1359.10f, 1046.55f, 52.97053f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1360.89f, 1051.81f, 53.19793f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1360.75f, 1048.84f, 53.12893f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1364.43f, 1053.62f, 53.29343f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1363.08f, 1048.15f, 53.22223f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1364.08f, 1050.84f, 53.29163f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1366.69f, 1050.31f, 53.34203f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            me->SummonCreature(NPC_HELLSCREEMS_ELITE_44636, 1357.85f, 1050.12f, 52.99823f, 5.253441f, TEMPSUMMON_TIMED_DESPAWN, 300000);

            me->SummonCreature(NPC_GARROSH_HELLSCREAM_44629, 1402.45f, 1061.62f, 60.56173f, 3.926991f, TEMPSUMMON_TIMED_DESPAWN, 300000);

            if (Creature* cromush = me->SummonCreature(NPC_HIGH_WARLORD_CROMUSH_44640, 1404.71f, 1063.73f, 60.5617f, 2.827433f, TEMPSUMMON_TIMED_DESPAWN, 300000))
            {
                m_cromushGUID = cromush->GetGUID();
                cromush->GetMotionMaster()->MovePath(446402, false);
            }
        }

        void RemoveAllSpawnedCreatures()
        {
            for (std::list<ObjectGuid>::const_iterator itr = spawnedList.begin(); itr != spawnedList.end(); itr++)
            {
                if (Creature* npc = ObjectAccessor::GetCreature(*me, (*itr)))
                {
                    if (npc->isMoving())
                        npc->DespawnOrUnsummon(1000);
                    else
                    {
                        npc->CastSpell(npc, SPELL_SIMPLE_TELEPORT);
                        npc->DespawnOrUnsummon(1000);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_grand_executor_mortuus_44615AI(creature);
    }
};

// Raise Forsaken - 83173
class spell_raise_forsaken_83173 : public SpellScriptLoader
{
public:
    spell_raise_forsaken_83173() : SpellScriptLoader("spell_raise_forsaken_83173") { }

    enum eSpell
    {
        NPC_FALLEN_HUMAN_44592 = 44592,
        NPC_FALLEN_HUMAN_44593 = 44593,
        SPELL_FEIGNED = 80636,
        SPELL_FLOATING = 87259
    };

    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    class spell_raise_forsaken_83173_SpellScript : public SpellScript
    {
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<uint32>entrys;
            entrys.push_back(NPC_FALLEN_HUMAN_44592);
            entrys.push_back(NPC_FALLEN_HUMAN_44593);
            targets.remove_if(IsNotInEntryList(entrys));
        }

        void Register()
        {
            OnObjectAreaTargetSelect.Register(&spell_raise_forsaken_83173_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_raise_forsaken_83173_SpellScript();
    }

    class spell_raise_forsaken_83173_AuraScript : public AuraScript
    {
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* fallenHuman = unit->ToCreature())
                {
                    fallenHuman->AddAura(SPELL_FLOATING, fallenHuman);
                    fallenHuman->SetFloatValue(UNIT_FIELD_HOVERHEIGHT, frand(4.5f, 6.5f));
                    fallenHuman->SetHover(true, false, false);
                }
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* fallenHuman = unit->ToCreature())
                {
                    fallenHuman->RemoveAura(SPELL_FLOATING);
                    fallenHuman->SetHover(false);
                    fallenHuman->AddAura(78351, fallenHuman);
                }
            }
        }

        void Register() override
        {
            OnEffectApply.Register(&spell_raise_forsaken_83173_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove.Register(&spell_raise_forsaken_83173_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_raise_forsaken_83173_AuraScript();
    }
};

// Forsaken Trooper Summon - 83149
class spell_forsaken_trooper_master_script_83149 : public SpellScriptLoader
{
public:
    spell_forsaken_trooper_master_script_83149() : SpellScriptLoader("spell_forsaken_trooper_master_script_83149") { }

    enum eSpell
    {
        SPELL_FORSAKEN_TROOPER_MALE_01 = 83150,
        SPELL_FORSAKEN_TROOPER_MALE_02 = 83163,
        SPELL_FORSAKEN_TROOPER_MALE_03 = 83164,
        SPELL_FORSAKEN_TROOPER_MALE_04 = 83165,
        SPELL_FORSAKEN_TROOPER_FEMALE_01 = 83152,
        SPELL_FORSAKEN_TROOPER_FEMALE_02 = 83166,
        SPELL_FORSAKEN_TROOPER_FEMALE_03 = 83167,
        SPELL_FORSAKEN_TROOPER_FEMALE_04 = 83168,

    };

    class spell_forsaken_trooper_master_script_83149_SpellScript : public SpellScript
    {
        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* unit = GetCaster())
            {
                if (Creature* fallenHuman = unit->ToCreature())
                {
                    if (fallenHuman->GetDisplayId() == 33978)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_01, true);

                    if (fallenHuman->GetDisplayId() == 33980)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_02, true);

                    if (fallenHuman->GetDisplayId() == 33979)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_03, true);

                    if (fallenHuman->GetDisplayId() == 33981)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_MALE_04, true);

                    if (fallenHuman->GetDisplayId() == 33982)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_01, true);

                    if (fallenHuman->GetDisplayId() == 33983)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_02, true);

                    if (fallenHuman->GetDisplayId() == 33984)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_03, true);

                    if (fallenHuman->GetDisplayId() == 33985)
                        fallenHuman->CastSpell(fallenHuman, SPELL_FORSAKEN_TROOPER_FEMALE_04, true);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget.Register(&spell_forsaken_trooper_master_script_83149_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_forsaken_trooper_master_script_83149_SpellScript();
    }
};

// Fallen Human - 44592, 44593
class npc_fallen_human_44592 : public CreatureScript
{
public:
    npc_fallen_human_44592() : CreatureScript("npc_fallen_human_44592") { }

    enum eSpell
    {
        SPELL_FEIGNED = 80636,
        EVENT_MOVE = 0,
        EVENT_TRANSFORM = 1,
    };

    struct npc_fallen_human_44592AI : public ScriptedAI
    {
        npc_fallen_human_44592AI(Creature* creature) : ScriptedAI(creature)
        {
            Done1 = false;
            Done2 = false;
        }

        EventMap m_events;
        bool Done1;
        bool Done2;

        void Reset() override
        {
            Done1 = false;
            Done2 = false;

            m_events.Reset();

            me->AddAura(SPELL_FEIGNED, me);
            me->RemoveAura(78351);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
            me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
                case EVENT_MOVE:
                {
                    if (Done2)
                        return;

                    if (urand(0, 1) == 0)
                        me->GetMotionMaster()->MovePath(4459200, false);
                    else
                        me->GetMotionMaster()->MovePath(4459201, false);

                    me->DespawnOrUnsummon(35000);

                    Done2 = true;
                    break;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            if (me->HasAura(78351))
                m_events.ScheduleEvent(EVENT_TRANSFORM, 1000);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_TRANSFORM:
                {
                    if (Done1)
                        return;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                    me->RemoveAura(SPELL_FEIGNED);
                    me->CastSpell(me, 83149, true);

                    m_events.ScheduleEvent(4, 1500);
                    Done1 = true;
                    break;
                }

                case 4:
                {
                    me->SetFacingTo(0.706837f);

                    m_events.ScheduleEvent(5, 2500);
                    break;
                }

                case 5:
                {
                    me->HandleEmoteCommand(66);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fallen_human_44592AI(creature);
    }
};

// Bat Handler Maggotbreath - 44825
class npc_bat_handler_maggotbreath_44825 : public CreatureScript
{
public:
    npc_bat_handler_maggotbreath_44825() : CreatureScript("npc_bat_handler_maggotbreath_44825") { }

    struct npc_bat_handler_maggotbreath_44825AI : public ScriptedAI
    {
        npc_bat_handler_maggotbreath_44825AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            QUEST_ITERATING_UPON_SUCCESS = 26998,
            SPELL_SUMMON_BAT = 83584,
            ACTION_OPTION_ID = 1,
        };

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            switch (gossipListId)
            {
                case ACTION_OPTION_ID:
                {
                    if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->CastSpell(player, SPELL_SUMMON_BAT);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                        me->AI()->Talk(0);
                        break;
                    }
                }
            }

            CloseGossipMenuFor(player);

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bat_handler_maggotbreath_44825AI(creature);
    }
};

// Forsaken Bat - 44821
class npc_forsaken_bat_44821 : public CreatureScript
{
public:
    npc_forsaken_bat_44821() : CreatureScript("npc_forsaken_bat_44821") { }

    enum eNPC
    {
        QUEST_ITERATING_UPON_SUCCESS = 26998,
        NPC_VILE_FIN_ORACLE = 1908,
        NPC_BAT_HANDLER_MAGGOTBREATH = 44825,
        WAYPOINT_QUEST_26998 = 4482101,
        SPELL_GO_HOME = 83594,
        EVENT_CHECK_FINISH = 101,
        EVENT_START_MOVEMENT,
        EVENT_GO_HOME2,
        EVENT_GO_HOME,
    };

    struct npc_forsaken_bat_44821AI : public ScriptedAI
    {
        npc_forsaken_bat_44821AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;
        Position m_homePos;
        Position m_homePos2;

        void Initialize()
        {
            m_playerGUID == 0;
            m_events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                    {
                        m_playerGUID = player->GetGUID();

                        Position m_homePos = me->GetPosition();
                    }
                }
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == WAYPOINT_MOTION_TYPE)
            {
                switch (id)
                {
                    m_homePos = me->GetPosition();

                case 0:
                {
                    me->SetSpeed(MOVE_RUN, 3.9f);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        player->KilledMonsterCredit(NPC_BAT_HANDLER_MAGGOTBREATH);
                    break;
                }

                case 1:
                {
                    m_homePos2 = me->GetPosition();
                    m_homePos2.m_positionZ + 40.0f;
                    break;
                }

                case 4:
                {
                    me->SetSpeed(MOVE_RUN, 2.4f);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        Talk(0);
                    break;
                }
                }

                if (type == POINT_MOTION_TYPE)
                {
                    if (id == 1234)
                        m_events.ScheduleEvent(EVENT_GO_HOME, 10);
                    else if (id == 1236)
                    {
                        me->GetVehicleKit()->RemoveAllPassengers();
                        me->DespawnOrUnsummon(100);
                    }
                }
            }
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
        {
            if (apply)
            {
                if (Player* player = passenger->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_ITERATING_UPON_SUCCESS) == QUEST_STATUS_INCOMPLETE)
                    {
                        m_playerGUID = player->GetGUID();
                        m_events.ScheduleEvent(EVENT_START_MOVEMENT, 2000);
                        return;
                    }
                }

                m_playerGUID == 0;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (player->GetReqKillOrCastCurrentCount(QUEST_ITERATING_UPON_SUCCESS, NPC_VILE_FIN_ORACLE) >= 50)
                        {
                            Talk(1);
                            me->CastSpell(me, SPELL_GO_HOME, true);
                            me->GetMotionMaster()->Clear();
                            m_events.ScheduleEvent(EVENT_GO_HOME2, 250);
                            return;
                        }
                    }

                    m_events.ScheduleEvent(EVENT_CHECK_FINISH, 1000);
                    break;
                }

                case EVENT_START_MOVEMENT:
                {
                    me->SetCanFly(true);
                    me->SetDisableGravity(true);
                    me->GetMotionMaster()->MovePath(WAYPOINT_QUEST_26998, false);
                    m_events.ScheduleEvent(EVENT_CHECK_FINISH, 1000);
                    break;
                }

                case EVENT_GO_HOME2:
                {
                    me->SetSpeed(MOVE_RUN, 3.5f);
                    me->GetMotionMaster()->MovePoint(1234, m_homePos2);
                    break;
                }

                case EVENT_GO_HOME:
                {
                    me->SetSpeed(MOVE_RUN, 2.0f);
                    me->GetMotionMaster()->MovePoint(1236, m_homePos);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_bat_44821AI(creature);
    }
};

// Agatha - 44951
class npc_agatha_44951 : public CreatureScript
{
public:
    npc_agatha_44951() : CreatureScript("npc_agatha_44951") { }

    enum eNPC
    {
        EVENT_CHAT_TO_PLAYER = 100,
        EVENT_CHECK_PLAYER,
        EVENT_CAST_AGGRO,
        EVENT_HEAL_COOLDOWN,
        EVENT_27099_RUN,

        NPC_HILLSBRAD_REFUGEE1 = 44954,
        NPC_HILLSBRAD_REFUGEE2 = 44966,
        NPC_FORSAKEN = 44959,

        SPELL_DOOMHOWL = 84012,
        SPELL_UNHOLY_DARKNESS = 84013,
        SPELL_UNHOLY_SMITE = 84014,

        SPELL_DEATH_WALK = 85451,
        SPELL_BROADCAST = 83978,
        SPELL_NOTIFY_AGATHA = 83990,
        SPELL_RISE_FORSAKEN = 83993,
        SPELL_KILL_CREDIT = 83996,
        SPELL_GENERAL_TRIGGER_84107 = 84107,
        SPELL_GENERAL_TRIGGER_84114 = 84114,

        QUEST_RISE_FORSAKEN = 27097,
        QUEST_NO_ESCAPE = 27099,
    };

    struct npc_agatha_44951AI : public ScriptedAI
    {
        npc_agatha_44951AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;
        ObjectGuid m_targetGUID;

        bool   m_doomhoulDone;
        bool   m_healCD;

        // VehicleID 1075

        void Initialize()
        {
            m_playerGUID = me->GetOwnerGUID();

            me->CastSpell(me, SPELL_DEATH_WALK, true);

            m_events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 30000);
            m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 2000);
        }

        void Reset() override
        {
            m_doomhoulDone = false;
            m_healCD = false;

            me->SetAnimationTier(AnimationTier::Hover);

            m_targetGUID = ObjectGuid::Empty;

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            me->SetReactState(REACT_DEFENSIVE);

            m_events.Reset();

            if (Creature* stalker = me->FindNearestCreature(45032, 25.0f))
                SetForQuest27099();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_NOTIFY_AGATHA:
            {
                me->CastSpell(caster, SPELL_RISE_FORSAKEN, true);

                me->AI()->Talk(1);

                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    player->CastSpell(player, SPELL_KILL_CREDIT);

                break;
            }

            case SPELL_GENERAL_TRIGGER_84114:
            {
                SetForQuest27099();
                break;
            }

            case 84107:
            {
                m_events.ScheduleEvent(EVENT_27099_RUN, 1000);
                break;
            }
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE && id == 1231)
                m_events.ScheduleEvent(EVENT_27099_RUN + 1, 100);

            if (type == WAYPOINT_MOTION_TYPE && id == 42)
                m_events.ScheduleEvent(EVENT_27099_RUN + 4, 100);
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
        {
            if (apply && passenger->ToPlayer())
                m_events.ScheduleEvent(EVENT_27099_RUN + 3, 100);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->GetGUID() != m_targetGUID)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                {
                    if (Unit* unit = player->GetVictim())
                    {
                        if (unit->GetGUID() != m_targetGUID)
                            JustEnteredCombat(unit);
                    }
                }
            }
        }

        void JustEngagedWith(Unit* who) override
        {
            m_doomhoulDone = false;

            m_targetGUID = who->GetGUID();

            me->CastSpell(who, SPELL_UNHOLY_SMITE);
            m_events.RescheduleEvent(EVENT_CAST_AGGRO, urand(1000, 4000));
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHAT_TO_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        me->AI()->Talk(0);

                    m_events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 60000);
                    break;
                }

                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (player->IsAlive() || player->IsInWorld())
                        {
                            if (CheckQuestStatus(player))
                            {
                                if (!m_healCD && player->GetHealthPct() < 90.0f)
                                {
                                    me->CastSpell(player, SPELL_UNHOLY_DARKNESS);

                                    m_healCD = true;

                                    m_events.ScheduleEvent(EVENT_HEAL_COOLDOWN, 2000);
                                }

                                if (Unit* unit = player->GetVictim())
                                {
                                    if (unit->GetGUID() != m_targetGUID)
                                        JustEnteredCombat(unit);
                                }

                                else if (player->IsInCombat())
                                {
                                    if (Unit* unit = player->GetSelectedUnit())
                                    {
                                        if (unit->GetGUID() != m_targetGUID)
                                            JustEnteredCombat(unit);
                                    }
                                }
                            }
                        }

                        m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                        break;
                    }
                }

                case EVENT_CAST_AGGRO:
                {
                    if (Creature* target = ObjectAccessor::GetCreature(*me, m_targetGUID))
                    {
                        if (target->IsAlive())
                        {
                            if (target->GetEntry() == NPC_HILLSBRAD_REFUGEE1 || target->GetEntry() == NPC_HILLSBRAD_REFUGEE2)
                            {
                                if (!m_doomhoulDone)
                                {
                                    me->CastSpell(me, SPELL_DOOMHOWL);

                                    m_doomhoulDone = true;
                                    break;
                                }
                            }
                        }
                    }
                    m_events.ScheduleEvent(EVENT_CAST_AGGRO, urand(1000, 6000));
                    break;
                }

                case EVENT_HEAL_COOLDOWN:
                {
                    m_healCD = false;
                    break;
                }

                case EVENT_27099_RUN:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        Position pos = player->GetPosition();

                        me->GetMotionMaster()->MovePoint(1231, pos);
                    }
                    break;
                }

                case EVENT_27099_RUN + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->ExitVehicle();

                        me->CastSpell(player, 84112, true); // camera spell

                        Talk(3);
                    }

                    m_events.ScheduleEvent(EVENT_27099_RUN + 2, 1500);
                    break;
                }

                case EVENT_27099_RUN + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

                        me->CastSpell(player, 84109); // ride vehicle, script_effect

                        player->EnterVehicle(me);

                        Talk(4);
                    }
                    break;
                }

                case EVENT_27099_RUN + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        player->KilledMonsterCredit(44951);

                    me->GetMotionMaster()->MovePath(4495101, false);
                    break;
                }

                case EVENT_27099_RUN + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        player->ExitVehicle();

                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

        bool CheckQuestStatus(Player* player)
        {
            if (player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RISE_FORSAKEN) == QUEST_STATUS_COMPLETE)
                return true;
            else if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_COMPLETE)
                return true;

            return false;
        }

        void SetForQuest27099()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->MovePoint(1234, 982.57f, 671.04f, 77.298f);
            m_events.CancelEvent(EVENT_CHAT_TO_PLAYER);
            me->AI()->Talk(2);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_agatha_44951AI(creature);
    }
};

// Hillsbrand Refugee - 44954, 44966
class npc_hillsbrad_refugee_44966 : public CreatureScript
{
public:
    npc_hillsbrad_refugee_44966() : CreatureScript("npc_hillsbrad_refugee_44966") { }

    enum eNPC
    {
        NPC_AGATHA = 44951,
        SPELL_NOTIFY_AGATHA = 83990,
    };

    struct npc_hillsbrad_refugee_44966AI : public ScriptedAI
    {
        npc_hillsbrad_refugee_44966AI(Creature* creature) : ScriptedAI(creature) { }

        void JustSummoned(Creature* summon) override
        {
            uint32 rol = urand(0, 100);

            if (rol < 50)
            {
                summon->AI()->Talk(0);
                summon->DespawnOrUnsummon(10000);
            }
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* agatha = me->FindNearestCreature(NPC_AGATHA, 30.0f))
            {
                me->CastSpell(agatha, SPELL_NOTIFY_AGATHA, true);

                uint32 m_forsakenSpell;

                if (me->getGender() == GENDER_MALE)
                    m_forsakenSpell = RAND(83998, 83999, 84000, 84001);
                else
                    m_forsakenSpell = RAND(84002, 84003, 84004, 84005);

                me->CastSpell(me, m_forsakenSpell);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_hillsbrad_refugee_44966AI(creature);
    }
};

enum eQuest27099
{
    NPC_AGATHA = 44951,
    QUEST_NO_ESCAPE = 27099,
    NPC_BLOODFANG = 44990,
    NPC_CROWLEY = 44989,
    NPC_PHIN_ODELIC = 44993,
    NPC_BARTOLO_GINSETTI = 44994,
    NPC_LOREMASTER_DIBBS = 44995,
    NPC_MAGISTRATE_HENRY_MALEB = 44996,
    NPC_CARETAKER_SMITHERS = 44997,
    NPC_SOPHIA_ZWOSKI = 45002,
    SPELL_FORCE_CAST_FENRIS_CAMERA = 84113,
    SPELL_SUMMON_BLOODFANG = 84054,
    SPELL_SUMMON_CROWLEY = 84055,
    SPELL_SUMMON_PHIN_ODELIC = 84056,
    SPELL_SUMMON_BARTOLO_GINSETTI = 84057,
    SPELL_SUMMON_LOREMASTER_DIBBS = 84058,
    SPELL_SUMMON_MAGISTRATE_HENRY_MALEB = 84059,
    SPELL_SUMMON_CARETAKER_SMITHERS = 84060,
    SPELL_SUMMON_SOPHIA = 84061,
    SPELL_SUMMON_AGATHA = 83982,
    SPELL_CONVERSATION_TRIGGER_84076 = 84076, // start conversation for maleb
    SPELL_CONVERSATION_TRIGGER_84077 = 84077, // start drinking poison
    SPELL_ARMORE_CAMERA = 84104,
    SPELL_GENERAL_TRIGGER_84102 = 84102,      // start conversation for crowley
    SPELL_GENERAL_TRIGGER_84107 = 84107,      // finish video.. starting agatha RUN
    SPELL_GENERAL_TRIGGER_84114 = 84114,      // von player an agatha.. n:262723
    SOUND_WORGEN_HOWLING = 17671,
    EVENT_CHECK_QUEST = 101,
    EVENT_CHECK_PLAYER,
    EVENT_START_ANIMATION,
    EVENT_MASTER_RESET,
    EVENT_DRINKING,
    EVENT_MORPHING,
    EVENT_HOWLING,
    EVENT_MOVE_TO_STARTPOINT,
    EVENT_CHANGE_TO_SEAT_2,
    EVENT_TRIGGER_84102,
    EVENT_ANIMATION = 200,
};

// Fenris Keep Stalker - 45032
class npc_fenris_keep_stalker_45032 : public CreatureScript
{
public:
    npc_fenris_keep_stalker_45032() : CreatureScript("npc_fenris_keep_stalker_45032") { }

    struct npc_fenris_keep_stalker_45032AI : public ScriptedAI
    {
        npc_fenris_keep_stalker_45032AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;
        ObjectGuid m_playerGUID;
        ObjectGuid m_bloodfangGUID;
        ObjectGuid m_crowleyGUID;
        ObjectGuid m_phinOdelicGUID;
        ObjectGuid m_bartoloGinsettiGUID;
        ObjectGuid m_loremasterDibbsGUID;
        ObjectGuid m_henryMalebGUID;
        ObjectGuid m_caretakersSmithersGUID;
        ObjectGuid m_sophiaGUID;

        bool m_animationIsStarted;

        void Reset() override
        {
            m_playerGUID == 0;
            m_bloodfangGUID == 0;
            m_crowleyGUID == 0;
            m_phinOdelicGUID == 0;
            m_bartoloGinsettiGUID == 0;
            m_loremasterDibbsGUID == 0;
            m_henryMalebGUID == 0;
            m_caretakersSmithersGUID == 0;
            m_sophiaGUID == 0;

            m_events.Reset();
            m_events.ScheduleEvent(EVENT_CHECK_QUEST, 1000);
            m_animationIsStarted = false;
        }

        void JustSummoned(Creature* summon) override
        {
            switch (summon->GetEntry())
            {
            case NPC_BLOODFANG:
                m_bloodfangGUID = summon->GetGUID();
                break;
            case NPC_CROWLEY:
                m_crowleyGUID = summon->GetGUID();
                break;
            case NPC_PHIN_ODELIC:
                m_phinOdelicGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            case NPC_BARTOLO_GINSETTI:
                m_bartoloGinsettiGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            case NPC_LOREMASTER_DIBBS:
                m_loremasterDibbsGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            case NPC_MAGISTRATE_HENRY_MALEB:
                m_henryMalebGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            case NPC_CARETAKER_SMITHERS:
                m_caretakersSmithersGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            case NPC_SOPHIA_ZWOSKI:
                m_sophiaGUID = summon->GetGUID();
                SetCreatureValues(summon);
                break;
            }

            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_GENERAL_TRIGGER_84107:
            {
                m_events.RescheduleEvent(EVENT_MASTER_RESET, 30000);
                break;
            }

            case SPELL_GENERAL_TRIGGER_84102:
            {
                if (Creature* camera = me->FindNearestCreature(45003, 25.0f))
                {
                    if (Creature* crowley = ObjectAccessor::GetCreature(*me, m_crowleyGUID))
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        {
                            // camera spell is wrong
                            // camera->CastSpell(crowley->GetPositionX(), crowley->GetPositionY(), crowley->GetPositionZ(), 83763, true);
                        }
                    }
                }
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_QUEST:
                {
                    if (!m_animationIsStarted)
                    {
                        if (Player* player = me->SelectNearestPlayer(15.0f))
                        {
                            if (abs(player->GetPositionZ() - me->GetPositionZ()) < 1.0f)
                            {
                                if (player->GetQuestStatus(QUEST_NO_ESCAPE) == QUEST_STATUS_INCOMPLETE)
                                {
                                    m_playerGUID = player->GetGUID();
                                    m_animationIsStarted = true;
                                    m_events.ScheduleEvent(EVENT_START_ANIMATION, 100);
                                    m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                                    m_events.ScheduleEvent(EVENT_MASTER_RESET, 250000);
                                }
                            }
                        }
                    }

                    m_events.ScheduleEvent(EVENT_CHECK_QUEST, 1000);
                    break;
                }

                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (player->IsAlive() || player->IsInWorld())
                        {
                            m_events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                            break;
                        }
                    }
                    break;
                }

                case EVENT_MASTER_RESET:
                {
                    DespawnAllSummons();
                    Reset();
                    break;
                }

                case EVENT_START_ANIMATION:
                {
                    me->CastSpell({ 994.530f, 687.740f, 74.8984f }, SPELL_SUMMON_BLOODFANG, true);
                    me->CastSpell({ 994.562f, 691.186f, 74.8984f }, SPELL_SUMMON_CROWLEY, true);
                    me->CastSpell({ 1002.520f, 693.027f, 76.1922f }, SPELL_SUMMON_PHIN_ODELIC, true);
                    me->CastSpell({ 1002.760f, 687.195f, 76.1922f }, SPELL_SUMMON_BARTOLO_GINSETTI, true);
                    me->CastSpell({ 1002.800f, 684.256f, 76.1922f }, SPELL_SUMMON_LOREMASTER_DIBBS, true);
                    me->CastSpell({ 1000.670f, 689.759f, 76.1922f }, SPELL_SUMMON_MAGISTRATE_HENRY_MALEB, true);
                    me->CastSpell({ 1002.700f, 695.775f, 76.1922f }, SPELL_SUMMON_CARETAKER_SMITHERS, true);
                    me->CastSpell({ 1004.530f, 688.619f, 76.1922f }, SPELL_SUMMON_SOPHIA, true);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        me->CastSpell(player, SPELL_FORCE_CAST_FENRIS_CAMERA, true);

                    break;
                }
                }
            }
        }

        void DespawnAllSummons()
        {
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_bloodfangGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_crowleyGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_phinOdelicGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_bartoloGinsettiGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_loremasterDibbsGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_henryMalebGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_caretakersSmithersGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, m_sophiaGUID))
                creature->DespawnOrUnsummon(10);
            if (Creature* creature = me->FindNearestCreature(45003, 30.0f))
                creature->DespawnOrUnsummon(10);
        }

        void SetCreatureValues(Creature* npc)
        {
            Position pos = npc->GetPosition();
            pos.SetOrientation(3.33f);
            npc->SetHomePosition(pos);
            npc->GetMotionMaster()->MoveTargetedHome();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fenris_keep_stalker_45032AI(creature);
    }
};

// Lord Darius Crowley - 44989
class npc_lord_darius_crowley_44989 : public CreatureScript
{
public:
    npc_lord_darius_crowley_44989() : CreatureScript("npc_lord_darius_crowley_44989") { }

    struct npc_lord_darius_crowley_44989AI : public ScriptedAI
    {
        npc_lord_darius_crowley_44989AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_GENERAL_TRIGGER_84102:
            {
                m_events.ScheduleEvent(EVENT_ANIMATION, 500);
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ANIMATION + 0:
                {
                    Talk(0);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 1, 4700);
                    break;
                }
                case EVENT_ANIMATION + 1:
                {
                    Talk(1);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 2, 4700);
                    break;
                }
                case EVENT_ANIMATION + 2:
                {
                    Talk(2);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 3, 4700);
                    break;
                }
                case EVENT_ANIMATION + 3:
                {
                    Talk(3);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 4, 4700);
                    break;
                }
                case EVENT_ANIMATION + 4:
                {
                    Talk(4);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 5, 6100);
                    break;
                }
                case EVENT_ANIMATION + 5:
                {
                    Talk(5);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 6, 9500);
                    break;
                }
                case EVENT_ANIMATION + 6:
                {
                    me->CastSpell(me, SPELL_CONVERSATION_TRIGGER_84076, true);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lord_darius_crowley_44989AI(creature);
    }
};

// Phin Odelic - 44993
class npc_phin_odelic_44993 : public CreatureScript
{
public:
    npc_phin_odelic_44993() : CreatureScript("npc_phin_odelic_44993") { }

    struct npc_phin_odelic_44993AI : public ScriptedAI
    {
        npc_phin_odelic_44993AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84094))
                    me->RemoveAura(84094);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DRINKING:
                {
                    // place for emote
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84094);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_phin_odelic_44993AI(creature);
    }
};

// Bartolo Ginsetti - 44994
class npc_bartolo_ginsetti_44994 : public CreatureScript
{
public:
    npc_bartolo_ginsetti_44994() : CreatureScript("npc_bartolo_ginsetti_44994") { }

    struct npc_bartolo_ginsetti_44994AI : public ScriptedAI
    {
        npc_bartolo_ginsetti_44994AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84095))
                    me->RemoveAura(84095);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DRINKING:
                {
                    // place for emote
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84095);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bartolo_ginsetti_44994AI(creature);
    }
};

// Loremaster Dibbs - 44995
class npc_loremaster_dibbs_44995 : public CreatureScript
{
public:
    npc_loremaster_dibbs_44995() : CreatureScript("npc_loremaster_dibbs_44995") { }

    struct npc_loremaster_dibbs_44995AI : public ScriptedAI
    {
        npc_loremaster_dibbs_44995AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        uint64 m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID = 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84096))
                    me->RemoveAura(84096);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DRINKING:
                {
                    // place for emote
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84096);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_loremaster_dibbs_44995AI(creature);
    }
};

// Magistrate Henry Maleb - 44996
class npc_magistrate_henry_maleb_44996 : public CreatureScript
{
public:
    npc_magistrate_henry_maleb_44996() : CreatureScript("npc_magistrate_henry_maleb_44996") { }

    struct npc_magistrate_henry_maleb_44996AI : public ScriptedAI
    {
        npc_magistrate_henry_maleb_44996AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84076:
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    me->CastSpell(player, SPELL_ARMORE_CAMERA, true);

                m_events.ScheduleEvent(EVENT_ANIMATION, 500);
                break;
            }

            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84097))
                    me->RemoveAura(84097);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ANIMATION:
                {
                    Talk(0);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 1, 10800);
                    break;
                }

                case EVENT_ANIMATION + 1:
                {
                    Talk(1);
                    m_events.ScheduleEvent(EVENT_ANIMATION + 2, 3500);
                    break;
                }

                case EVENT_ANIMATION + 2:
                {
                    me->CastSpell(me, SPELL_CONVERSATION_TRIGGER_84077, true);
                    break;
                }

                case EVENT_DRINKING:
                {
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84097);
                    me->CastSpell(me, SPELL_GENERAL_TRIGGER_84107, true);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_magistrate_henry_maleb_44996AI(creature);
    }
};

// Caretaker Smithers - 44997
class npc_caretaker_smithers_44997 : public CreatureScript
{
public:
    npc_caretaker_smithers_44997() : CreatureScript("npc_caretaker_smithers_44997") { }

    struct npc_caretaker_smithers_44997AI : public ScriptedAI
    {
        npc_caretaker_smithers_44997AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84098))
                    me->RemoveAura(84098);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DRINKING:
                {
                    // place for emote
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84098);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_caretaker_smithers_44997AI(creature);
    }
};

// Sophia Zwoski - 45002
class npc_sophia_zwoski_45002 : public CreatureScript
{
public:
    npc_sophia_zwoski_45002() : CreatureScript("npc_sophia_zwoski_45002") { }

    struct npc_sophia_zwoski_45002AI : public ScriptedAI
    {
        npc_sophia_zwoski_45002AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        bool   m_isWorgen;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            m_isWorgen = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
            case SPELL_CONVERSATION_TRIGGER_84077:
            {
                m_events.ScheduleEvent(EVENT_DRINKING, 100);
                break;
            }
            }

            if (!m_isWorgen)
            {
                if (me->HasAura(84099))
                    me->RemoveAura(84099);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DRINKING:
                {
                    // place for emote
                    m_events.ScheduleEvent(EVENT_MORPHING, 3000);
                    break;
                }

                case EVENT_MORPHING:
                {
                    m_isWorgen = true;
                    me->CastSpell(me, 84099);
                    m_events.ScheduleEvent(EVENT_HOWLING, 1500);
                    break;
                }

                case EVENT_HOWLING:
                {
                    me->PlayDirectSound(SOUND_WORGEN_HOWLING);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sophia_zwoski_45002AI(creature);
    }
};

// Fenris Keep Camera - 45003
class npc_camera_45003 : public CreatureScript
{
public:
    npc_camera_45003() : CreatureScript("npc_camera_45003") { }

    struct npc_camera_45003AI : public ScriptedAI
    {
        npc_camera_45003AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        EventMap m_events;
        ObjectGuid m_playerGUID;

        void Initialize()
        {
            m_playerGUID == 0;
        }

        void Reset() override
        {
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
        {
            if (apply && seatId == 0)
                m_events.ScheduleEvent(EVENT_MOVE_TO_STARTPOINT, 100);

            if (apply && seatId == 1)
                passenger->SetFacingTo(0.0f);

            if (!apply && seatId == 1)
                me->DespawnOrUnsummon(1000);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE && id == 1230)
                m_events.ScheduleEvent(EVENT_CHANGE_TO_SEAT_2, 100);
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_MOVE_TO_STARTPOINT:
                {
                    me->GetMotionMaster()->MovePoint(1230, 980.7f, 689.14f, 76.9f);
                    break;
                }

                case EVENT_CHANGE_TO_SEAT_2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->EnterVehicle(me, 1);
                        me->CastSpell(player, 84091, true);
                    }

                    m_events.ScheduleEvent(EVENT_TRIGGER_84102, 1500);
                    break;
                }

                case EVENT_TRIGGER_84102:
                {
                    me->CastSpell(me, SPELL_GENERAL_TRIGGER_84102, true);
                    break;
                }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_camera_45003AI(creature);
    }
};

// Lady Sylvanas Windrunner (Quest Giver) - 44365
class npc_lady_sylvanas_windrunner_44365 : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner_44365() : CreatureScript("npc_lady_sylvanas_windrunner_44365") { }

    enum eNPC
    {
        QUEST_THE_WARCHIEFS_FLEET_27065 = 27065,
        QUEST_ORCS_ARE_IN_ORDER = 27096,
        QUEST_RISE_FORSAKEN = 27097,
        QUEST_NO_ESCAPE = 27099,
        QUEST_LORDAERON = 27098,
        QUEST_TO_FORSAKEN = 27290,

        SPELL_SUMMON_AGATHA = 83982,
        SPELL_DEATH_WALK = 85451,
        SPELL_RIDE_VEHICLE = 84109,

        SPELL_SEE_QUEST_INVIS_5 = 84241,
        SPELL_SUMMON_SYLVANAS_HORSE = 84128,
        SPELL_SUMMON_OWN_HORSE = 84164,

        PLAYER_GUID = 99999,
        PLAYER_HORSE = 45041,
    };

    struct npc_lady_sylvanas_windrunner_44365AI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunner_44365AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;
        ObjectGuid  m_playerGUID;
        ObjectGuid  m_sylvanasGUID;
        ObjectGuid m_playerhorseGUID;
        ObjectGuid m_SpawnedGUID;

        uint8   m_animPhase;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_THE_WARCHIEFS_FLEET_27065)
                player->AddAura(SPELL_SEE_QUEST_INVIS_5, player);
            else if (quest->GetQuestId() == QUEST_RISE_FORSAKEN)
                player->CastSpell(player, SPELL_SUMMON_AGATHA, true);
            else if (quest->GetQuestId() == QUEST_LORDAERON)
            {
                me->AddAura(84184, player);
                player->CastSpell(player, SPELL_SUMMON_SYLVANAS_HORSE, true);
            }
            else if (quest->GetQuestId() == QUEST_TO_FORSAKEN)
            {
                if (Creature* arthura = player->FindNearestCreature(44610, 40.0f, true))
                {
                    if (Creature* arthura = player->SummonCreature(44610, arthura->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0, 0, ObjectGuid::Empty))
                    {
                        player->ExitVehicle();
                        arthura->CastSpell(player, SPELL_RIDE_VEHICLE);
                        player->EnterVehicle(arthura);
                    }
                }
            }

        }

        void QuestReward(Player* player, Quest const* quest, uint32 /*opt*/) override
        {
            if (quest->GetQuestId() == QUEST_ORCS_ARE_IN_ORDER)
                player->RemoveAura(SPELL_SEE_QUEST_INVIS_5);

            if (quest->GetQuestId() == QUEST_NO_ESCAPE)
                player->RemoveAura(SPELL_DEATH_WALK);
        }

        void Reset() override
        {
            m_events.Reset();
            m_playerGUID = ObjectGuid::Empty;
        }

        void SetGUID(ObjectGuid const& guid, int32 id) override
        {
            switch (id)
            {
            case PLAYER_GUID:
            {
                m_playerGUID = guid;
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lady_sylvanas_windrunner_44365AI(creature);
    }
};

// Lady Sylvanas Windrunner (Rider) - 45051
class npc_lady_sylvanas_windrunner_45051 : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner_45051() : CreatureScript("npc_lady_sylvanas_windrunner_45051") { }

    enum eNPC
    {
        QUEST_LORDAERON = 27098,
        DATA_LORDAERON_RIDE = 54,
        EVENT_CHAT_TO_PLAYER = 544,
        EVENT_CHECK_PLAYER_STATUS = 543,
        EVENT_START_FOLLOW = 542,
        NPC_FORSAKEN_WARHORSE = 45057,
        SYLVANAS_PATH_ID = 4505101,
        EVENT_CHECK_WORGEN = 700,
        EVENT_CHECK_FINISH = 701,
        EVENT_CHECK_DREADGUARD = 702,

        SPELL_HEARTSTRIKE = 84182
    };

    struct npc_lady_sylvanas_windrunner_45051AI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunner_45051AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            _playerGUID = ObjectGuid::Empty;
            _horseGUID = ObjectGuid::Empty;
            _events.Reset();
        }

        void IsSummonedBy(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
                _playerGUID = player->GetGUID();

            if (Creature* playerhorse = me->FindNearestCreature(45057, 10.0f))
                _horseGUID = playerhorse->GetGUID();

            if (Creature* agatha = me->SummonCreature(44608, 1364.84f, 1029.26f, 58.8997f, 0.855211f, TEMPSUMMON_TIMED_DESPAWN, 25000))
            {
                agatha->GetMotionMaster()->MovePath(446080, false);
                agatha->AddAura(84183, agatha);

                if (Creature* daschla = me->SummonCreature(44609, 1367.04f, 1025.92f, 56.4914f, 0.855211f, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    daschla->GetMotionMaster()->MoveFollow(agatha, 5.0f, M_PI / 2.0f, false, true, false);
                    daschla->AddAura(84183, daschla);
                }

                if (Creature* arthura = me->SummonCreature(44610, 1361.92f, 1032.05f, 56.4914f, 0.696561f, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    arthura->GetMotionMaster()->MoveFollow(agatha, 5.0f, M_PI * 1.5f, false, true, false);
                    arthura->AddAura(84183, arthura);
                }
            }
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case DATA_LORDAERON_RIDE:
            {
                _events.ScheduleEvent(EVENT_START_FOLLOW, 0);
                break;
            }
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
            {
                if (Creature* playerhorse = ObjectAccessor::GetCreature(*me, _horseGUID))
                {
                    playerhorse->AI()->SetData(55, 55);
                    me->SetFacingTo(4.40509f);
                    me->StopMoving();
                    _events.ScheduleEvent(EVENT_CHECK_FINISH, 1000);
                }
            }
        }

        void SetGUID(ObjectGuid const& guid, int32 id) override
        {
            switch (id)
            {
            case NPC_FORSAKEN_WARHORSE:
            {
                _horseGUID = guid;
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_START_FOLLOW:
                {
                    if (Creature* playerhorse = ObjectAccessor::GetCreature(*me, _horseGUID))
                        me->GetMotionMaster()->MoveFollow(playerhorse, 2.5f, M_PI * 1.5f, false, true, false);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER, 6500);

                    _events.ScheduleEvent(EVENT_CHECK_WORGEN, 1000);
                    _events.ScheduleEvent(EVENT_CHECK_DREADGUARD, 500);
                    break;
                }

                case EVENT_CHECK_WORGEN:
                {
                    if (Creature* worgen = me->FindNearestCreature(44793, 30.0f, true))
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            return;

                        me->HandleEmoteCommand(384);

                        me->CastSpell(worgen, SPELL_HEARTSTRIKE, false);
                    }
                    else
                        me->HandleEmoteCommand(0);

                    _events.ScheduleEvent(EVENT_CHECK_WORGEN, 3000);
                    break;
                }

                case EVENT_CHECK_DREADGUARD:
                {
                    std::list<Creature*> dreadguards;
                    GetCreatureListWithEntryInGrid(dreadguards, me, 45588, 10.0f);

                    for (std::list<Creature*>::const_iterator itr = dreadguards.begin(); itr != dreadguards.end(); ++itr)
                        (*itr)->AI()->SetData(54, 54);

                    _events.ScheduleEvent(EVENT_CHECK_DREADGUARD, 500);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 1, 3500);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 2, 3800);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 2:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(2);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 3, 7200);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 3:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->AI()->Talk(3);
                        me->AddAura(84184, player);
                    }

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 4, 3800);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->AI()->Talk(4);
                        me->AddAura(84184, player);
                    }

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 5, 6800);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 5:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(5);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 6, 7300);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 6:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(6);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 7, 8300);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 7:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(7);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 8, 9300);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 8:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(8);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 9, 8800);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 9:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(9);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 10, 4800);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 10:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(10);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 11, 4300);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 11:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(11);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 12, 8300);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 12:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(12);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 13, 6000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 13: // And though
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(13);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 14, 10000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 14: // With Arthas
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(14);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 15, 10200);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 15: // From
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(15);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 16, 8200);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 16: // Our goal
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(16);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 17, 9000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 17: // Lich dead
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(17);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 18, 5000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 18:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(18);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 19, 7000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 19: // But
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        me->AddAura(84184, player);
                        me->AI()->Talk(19);
                    }

                    if (Creature* agatha = me->SummonCreature(44608, 569.11f, 1550.09f, 117.9347f, 0.855211f, TEMPSUMMON_TIMED_DESPAWN, 23000))
                    {
                        agatha->GetMotionMaster()->MovePath(446081, false);
                        agatha->AddAura(84183, agatha);

                        if (Creature* daschla = me->SummonCreature(44609, 569.11f, 1550.09f, 117.9347f, 0.855211f, TEMPSUMMON_TIMED_DESPAWN, 23000))
                        {
                            daschla->GetMotionMaster()->MoveFollow(agatha, 5.0f, M_PI / 2.5f, false, true, false);
                            daschla->AddAura(84183, agatha);
                        }

                        if (Creature* arthura = me->SummonCreature(44610, 569.11f, 1550.09f, 117.9347f, 0.696561f, TEMPSUMMON_TIMED_DESPAWN, 23000))
                        {
                            arthura->GetMotionMaster()->MoveFollow(agatha, 5.0f, M_PI * 2.0f, false, true, false);
                            arthura->AddAura(84183, agatha);
                        }
                    }

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 20, 10500);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 20: // I will never
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(20);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 21, 4500);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 21: // Lordaeron
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->AI()->Talk(21);

                    _events.ScheduleEvent(EVENT_CHAT_TO_PLAYER + 22, 2000);
                    break;
                }

                case EVENT_CHAT_TO_PLAYER + 22:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        me->GetMotionMaster()->MovePoint(1, 501.42f, 1565.80f, 128.3090f, false, 3.5f);
                    break;
                }

                case EVENT_CHECK_FINISH:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        if (player->GetQuestStatus(QUEST_LORDAERON) == QUEST_STATUS_COMPLETE)
                        {
                            player->RemoveAura(84184);
                            me->DespawnOrUnsummon();
                        }
                    }
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
        ObjectGuid _playerGUID;
        ObjectGuid _horseGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lady_sylvanas_windrunner_45051AI(creature);
    }
};

// Forsaken Warhorse - 45057
class npc_forsaken_warhorse_45057 : public CreatureScript
{
public:
    npc_forsaken_warhorse_45057() : CreatureScript("npc_forsaken_warhorse_45057") { }

    enum eNPC
    {
        QUEST_LORDAERON = 27098,
        DATA_LORDAERON_RIDE = 54,
        EVENT_WARHORSE_DESPAWN = 546,
        EVENT_CAST_CAMERA = 547,
        EVENT_RIDE_WARHORSE = 548,
        SYLVANAS_ACCOMPANIED_TO_THE_SEPULCHER = 45051,
        SPELL_RIDE_VEHICLE = 84109,
        SPELL_CAMERA = 84112,
        SPELL_RIDE_VEHICLE_HARD_CODED = 46598,
        SPELL_EJECT_ALL_PASSENGERS = 50630,
        NPC_SYLVANAS = 45051,
    };

    struct npc_forsaken_warhorse_AI : public ScriptedAI
    {
        npc_forsaken_warhorse_AI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
            _playerGUID = ObjectGuid::Empty;
            _events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_LORDAERON) == QUEST_STATUS_INCOMPLETE)
                {
                    if (Vehicle* vehicle = me->GetVehicleKit())
                    {
                        _playerGUID = player->GetGUID();
                        _events.ScheduleEvent(EVENT_RIDE_WARHORSE, 0);
                    }
                }
            }
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case 55:
            {
                _events.ScheduleEvent(EVENT_WARHORSE_DESPAWN, 0);
                break;
            }
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
            {
                if (player->GetQuestStatus(QUEST_LORDAERON) == QUEST_STATUS_INCOMPLETE)
                {
                    player->FailQuest(QUEST_LORDAERON);
                    player->NearTeleportTo(1379.41f, 1039.00f, 53.0639f, 3.7848f);
                }
            }
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
        {
            if (apply && passenger->GetTypeId() == TYPEID_PLAYER)
            {
                if (Creature* sylvanas = me->FindNearestCreature(NPC_SYLVANAS, 10.0f, true))
                {
                    sylvanas->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());
                    sylvanas->AI()->SetData(54, 54);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RIDE_WARHORSE:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    {
                        player->ExitVehicle();
                        me->CastSpell(player, SPELL_RIDE_VEHICLE); // ride vehicle, script_effect
                        player->EnterVehicle(me);

                        me->GetMotionMaster()->MovePath(4505701, false);
                    }
                    break;
                }

                case EVENT_WARHORSE_DESPAWN:
                {
                    me->RemoveAllAuras();
                    me->GetVehicleKit()->RemoveAllPassengers();

                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        player->KilledMonsterCredit(SYLVANAS_ACCOMPANIED_TO_THE_SEPULCHER);

                    me->DespawnOrUnsummon(1000);

                    break;
                }
                default:
                    break;
                }
            }
        }

    private:
        EventMap _events;
        ObjectGuid _playerGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_warhorse_AI(creature);
    }
};

// Dreadguard (Cosmetic) - 45588
class npc_dreadguard_cosmetic : public CreatureScript
{
public:
    npc_dreadguard_cosmetic() : CreatureScript("npc_dreadguard_cosmetic") { }

    struct npc_dreadguard_cosmeticAI : public ScriptedAI
    {
        npc_dreadguard_cosmeticAI(Creature* creature) : ScriptedAI(creature) { }

        bool Done;

        void Reset()
        {
            _events.Reset();
            Done = false;
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case 54:
            {
                _events.ScheduleEvent(1, 100);
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case 1:
                {
                    if (Done)
                        return;

                    me->HandleEmoteCommand(66);

                    _events.ScheduleEvent(1, 6000);

                    Done = true;
                    break;
                }

                case 2:
                {
                    if (Done)
                    {
                        Done = false;
                    }
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dreadguard_cosmeticAI(creature);
    }
};

// Apothecary Wormcrud - 44912
class npc_apothecary_wormcrud : public CreatureScript
{
public:
    npc_apothecary_wormcrud() : CreatureScript("npc_apothecary_wormcrud") { }

    struct npc_apothecary_wormcrudAI : public ScriptedAI
    {
        npc_apothecary_wormcrudAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            _events.Reset();
            _events.ScheduleEvent(1, urand(3500, 7500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case 1:
                {
                    if (Creature* seaorc = me->SummonCreature(44942, 1069.88f, 1530.59f, 28.4103f, 0, TEMPSUMMON_TIMED_DESPAWN, 70000))
                        seaorc->GetMotionMaster()->MovePath(449421, false);

                    _events.ScheduleEvent(1, urand(3500, 7500));
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_apothecary_wormcrudAI(creature);
    }
};



/* Gilneas Part */

// Arthura - 45318
class npc_arthura_45318 : public CreatureScript
{
public:
    npc_arthura_45318() : CreatureScript("npc_arthura_45318") { }

    struct npc_arthura_45318AI : public ScriptedAI
    {
        npc_arthura_45318AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            QUEST_TO_FORSAKEN = 27290,
        };

        ObjectGuid  m_playerGUID;

        void Reset() override
        {
            _events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                {
                    m_playerGUID = player->GetGUID();

                    _events.ScheduleEvent(1, 2000);
                    _events.ScheduleEvent(2, 4000);
                }
            }
        }

        void WaypointReached(uint32 waypointId, uint32 pathId) override
        {
            if (pathId == 446100)
            {
                if (waypointId == 21)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->KilledMonsterCredit(44610);
                        me->GetVehicleKit()->RemoveAllPassengers();

                        _events.ScheduleEvent(3, 2000);
                    }
                }

            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case 1:
                    {
                        me->GetMotionMaster()->MovePath(446100, false);
                        break;
                    }

                    case 2:
                    {
                        me->AI()->Talk(0);
                        break;
                    }

                    case 3:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_arthura_45318AI(creature);
    }
};

// Deathstalker Commander Belmont, Deathstalkers - 45312, 45313
class npc_deathstalkers_belmont : public CreatureScript
{
public:
    npc_deathstalkers_belmont() : CreatureScript("npc_deathstalkers_belmont") { }

    struct npc_deathstalkers_belmontAI : public ScriptedAI
    {
        npc_deathstalkers_belmontAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {

            EVENT_SCENE = 1,
        };

        ObjectGuid  m_playerGUID;

        void Reset()
        {
            _events.Reset();

            me->SetSheath(SHEATH_STATE_MELEE);

            me->SetPowerType(Powers::POWER_ENERGY);
            me->SetMaxPower(Powers::POWER_ENERGY, 100);

        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_deathstalkers_belmontAI(creature);
    }
};

// Forsaken Catapult - 45282
class npc_forsaken_catapult_45282 : public CreatureScript
{
public:
    npc_forsaken_catapult_45282() : CreatureScript("npc_forsaken_catapult_45282") { }

    struct npc_forsaken_catapult_45282AI : public ScriptedAI
    {
        npc_forsaken_catapult_45282AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_BARREL = 1,
            EVENT_BREATH = 2,
            EVENT_BREATH_CD = 3,

            SPELL_THROW_BARREL = 85147,
            SPELL_FIRE_BREATH = 84807,
        };

        bool BreathCD;

        Position pos;

        void Reset() override
        {
            _events.Reset();

            BreathCD = false;

            _events.ScheduleEvent(EVENT_BARREL, urand(7500, 12500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->FindNearestCreature(45375, 5.0f, true))
            {
                if (BreathCD)
                    return;

                _events.ScheduleEvent(EVENT_BREATH, 1000);
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BARREL:
                    {
                        std::list<Creature*> targetList;
                        GetCreatureListWithEntryInGrid(targetList, me, 45375, 150.0f);
                        Trinity::Containers::RandomResize(targetList, 1);

                        for (auto itr : targetList)
                            me->CastSpell(itr, SPELL_THROW_BARREL, false);

                        _events.ScheduleEvent(EVENT_BARREL, urand(7500, 12500));
                        break;
                    }

                    case EVENT_BREATH:
                    {
                        DoCastVictim(SPELL_FIRE_BREATH, false);

                        BreathCD = true;

                        _events.ScheduleEvent(EVENT_BREATH_CD, 8000);
                        break;
                    }

                    case EVENT_BREATH_CD:
                    {
                        BreathCD = false;
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_catapult_45282AI(creature);
    }
};

// Forsaken Blightspreader - 45374
class npc_forsaken_blightspreader_45374 : public CreatureScript
{
public:
    npc_forsaken_blightspreader_45374() : CreatureScript("npc_forsaken_blightspreader_45374") { }

    struct npc_forsaken_blightspreader_45374AI : public ScriptedAI
    {
        npc_forsaken_blightspreader_45374AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_CONVERSATION = 1,
        };

        bool mounted;

        Position pos;

        void Reset() override
        {
            _events.Reset();

            pos = me->GetPosition();

            if (Creature* catapult = me->SummonCreature(45282, pos, TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (mounted)
                    return;

                me->EnterVehicle(catapult);

                mounted = true;
            }

            _events.ScheduleEvent(EVENT_CONVERSATION, urand(1000, 5000));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CONVERSATION:
                    {
                        me->AI()->Talk(0);

                        _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 90000));
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_blightspreader_45374AI(creature);
    }
};

// Forsaken Sharpshooter - 45376
class npc_forsaken_sharpshooter_45376 : public CreatureScript
{
public:
    npc_forsaken_sharpshooter_45376() : CreatureScript("npc_forsaken_sharpshooter_45376") { }

    struct npc_forsaken_sharpshooter_45376AI : public ScriptedAI
    {
        npc_forsaken_sharpshooter_45376AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_SPELL_DAMAGE = 1,
        };

        uint8 randomSpellIndex;
        float Orientation;

        void Reset() override
        {
            _events.Reset();

            Orientation = me->GetOrientation();

            me->SetFacingTo(Orientation, true);

            me->AddUnitState(UNIT_STATE_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(1000, 1200));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPELL_DAMAGE:
                    {
                        std::list<Creature*> worgen;
                        GetCreatureListWithEntryInGrid(worgen, me, 45375, 50.0f);
                        Trinity::Containers::RandomResize(worgen, 1);

                        uint32 spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];

                        if (!spellId)
                        {
                            randomSpellIndex = 0;
                            spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];
                        }

                        if (!spellId)
                            break;

                        if (++randomSpellIndex > 8)
                            randomSpellIndex = 0;

                        for (auto itr : worgen)
                        {
                            if (itr->IsAlive())
                            {
                                JustEngagedWith(itr);
                                me->SetFacingToObject(itr, true);
                                me->CastSpell(itr, spellId, false);
                            }
                        }

                        _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(1000, 1500));
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_sharpshooter_45376AI(creature);
    }
};

// Worgen Rusher - 45375
class npc_worgen_rusher_45375 : public CreatureScript
{
public:
    npc_worgen_rusher_45375() : CreatureScript("npc_worgen_rusher_45375") { }

    struct npc_worgen_rusher_45375AI : public ScriptedAI
    {
        npc_worgen_rusher_45375AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_START_PATH = 1,
            EVENT_CHARGE = 2,
            EVENT_DISAPPEAR = 3,

            SPELL_CHARGE = 75002,
        };

        Position pos;

        void Reset() override
        {
            _events.Reset();

            pos = me->GetPosition();

            me->SetCorpseDelay(-299000);
            me->SetRespawnDelay(-299000);

            me->SetAttackTime(BASE_ATTACK, 1500 + urand(0, 850));

            _events.ScheduleEvent(EVENT_START_PATH, urand(1000, 28000));
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->SummonCreature(45375, pos, TEMPSUMMON_MANUAL_DESPAWN);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (Creature* vanguard = me->FindNearestCreature(45280, 15.0f, true))
            {
                if (!me->IsInCombat() && !vanguard->IsInCombat())
                {
                    if (!vanguard->HasAura(86378))
                    {
                        Position pos = vanguard->GetPosition();
                        me->GetMotionMaster()->MovePoint(1, pos, false, 9.0f);
                        me->Attack(vanguard, true);
                        vanguard->Attack(me, true);
                    }
                }
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_PATH:
                    {
                        if (me->IsInCombat() || me->isDead())
                            return;

                        std::vector<uint32> pathIds = { 453750, 453751, 453752, 453753, 453754 };

                        uint32 chosenPathId = Trinity::Containers::SelectRandomContainerElement(pathIds);

                        me->GetMotionMaster()->MovePath(chosenPathId, false);
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_rusher_45375AI(creature);
    }
};

// Forsaken Vanguard - 45280
class npc_forsaken_vanguard_45280 : public CreatureScript
{
public:
    npc_forsaken_vanguard_45280() : CreatureScript("npc_forsaken_vanguard_45280") { }

    struct npc_forsaken_vanguard_45280AI : public ScriptedAI
    {
        npc_forsaken_vanguard_45280AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_CHECK_SHOWFIGHT = 1,
            EVENT_SPELL_DAMAGE = 2,
            MOVE_TO_HOMEPOSITION = 3,
        };

        uint8 randomSpellIndex;

        void Reset() override
        {
            _events.Reset();

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            me->SetAttackTime(BASE_ATTACK, 2000 + urand(0, 850));

            me->SetPowerType(Powers::POWER_RAGE);
            me->SetMaxPower(Powers::POWER_RAGE, 1000);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen1 = me->FindNearestCreature(46063, 5.0f))
                    {
                        me->CastSpell(worgen1, 355, true);
                        me->Attack(worgen1, true);
                    }

                    if (Creature* worgen2 = me->FindNearestCreature(46064, 5.0f))
                    {
                        me->CastSpell(worgen2, 355, true);
                        me->Attack(worgen2, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SPELL_DAMAGE:
                {
                    uint32 spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];

                    if (!spellId)
                    {
                        randomSpellIndex = 0;
                        spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];
                    }

                    if (!spellId)
                        break;

                    if (++randomSpellIndex > 8)
                        randomSpellIndex = 0;

                    DoCastVictim(spellId);

                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);

                    _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_vanguard_45280AI(creature);
    }
};

// Worgen Rebel - 45292
class npc_worgen_rebel_45292 : public CreatureScript
{
public:
    npc_worgen_rebel_45292() : CreatureScript("npc_worgen_rebel_45292") { }

    struct npc_worgen_rebel_45292AI : public ScriptedAI
    {
        npc_worgen_rebel_45292AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_CHECK_SHOWFIGHT = 1,
            EVENT_SPELL_DAMAGE = 2,
            MOVE_TO_HOMEPOSITION = 3,
        };

        uint8 randomSpellIndex;

        void Reset() override
        {
            _events.Reset();

            me->SetAttackTime(BASE_ATTACK, 1500 + urand(0, 850));

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            me->SetPowerType(Powers::POWER_RAGE);
            me->SetMaxPower(Powers::POWER_RAGE, 1000);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen1 = me->FindNearestCreature(46063, 5.0f))
                    {
                        me->CastSpell(worgen1, 355, true);
                        me->Attack(worgen1, true);
                    }

                    if (Creature* worgen2 = me->FindNearestCreature(46064, 5.0f))
                    {
                        me->CastSpell(worgen2, 355, true);
                        me->Attack(worgen2, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SPELL_DAMAGE:
                {
                    uint32 spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];

                    if (!spellId)
                    {
                        randomSpellIndex = 0;
                        spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];
                    }

                    if (!spellId)
                        break;

                    if (++randomSpellIndex > 8)
                        randomSpellIndex = 0;

                    DoCastVictim(spellId);

                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);

                    _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_rebel_45292AI(creature);
    }
};






// Bloodfang Scout - 45536
class npc_bloodfang_scout_45536 : public CreatureScript
{
public:
    npc_bloodfang_scout_45536() : CreatureScript("npc_bloodfang_scout_45536") { }

    struct npc_bloodfang_scout_45536AI : public ScriptedAI
    {
        npc_bloodfang_scout_45536AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_SINISTER_STRIKE = 1,
            EVENT_SPRINT = 2,

            SPELL_SINISTER_STRIKE = 60195,
            SPELL_SPRINT = 66060
        };

        void Reset()
        {
            _events.Reset();

            me->SetPowerType(Powers::POWER_ENERGY);
            me->SetMaxPower(Powers::POWER_ENERGY, 100);
        }

        void JustEngagedWith(Unit* who) override
        {
            _events.ScheduleEvent(EVENT_SINISTER_STRIKE, 2000);
            _events.ScheduleEvent(EVENT_SPRINT, 6000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SINISTER_STRIKE:
                {
                    DoCastVictim(SPELL_SINISTER_STRIKE);
                    _events.ScheduleEvent(EVENT_SINISTER_STRIKE, urand(4000, 6000));
                    break;
                }

                case EVENT_SPRINT:
                {
                    me->CastSpell(me, SPELL_SPRINT, false);
                    _events.ScheduleEvent(EVENT_SPRINT, urand(14000, 18000));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bloodfang_scout_45536AI(creature);
    }
};

// Worgen Battlemage - 45537
class npc_worgen_battlemage_45537 : public CreatureScript
{
public:
    npc_worgen_battlemage_45537() : CreatureScript("npc_worgen_battlemage_45537") { }

    struct npc_worgen_battlemage_45537AI : public ScriptedAI
    {
        npc_worgen_battlemage_45537AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_FIREBALL = 1,
            EVENT_FROSTBOLT = 2,

            SPELL_FIREBALL = 79854,
            SPELL_FROSTBOLT = 116,
            SPELL_MOLTEN_CORE = 79849
        };

        void Reset()
        {
            _events.Reset();

            if (!me->HasAura(SPELL_MOLTEN_CORE))
                me->CastSpell(me, SPELL_MOLTEN_CORE, false);
        }

        void JustEngagedWith(Unit* who) override
        {
            _events.ScheduleEvent(EVENT_FIREBALL, 500);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (Creature* arthura = me->FindNearestCreature(45314, 40.0f, true))
                _events.ScheduleEvent(EVENT_FROSTBOLT, 500);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FIREBALL:
                {
                    DoCastVictim(SPELL_FIREBALL);
                    _events.ScheduleEvent(EVENT_FIREBALL, urand(3100, 3500));
                    break;
                }

                case EVENT_FROSTBOLT:
                {
                    if (Creature* arthura = me->FindNearestCreature(45314, 40.0f, true))
                    {
                        me->CastSpell(arthura, SPELL_FROSTBOLT, false);

                        _events.ScheduleEvent(EVENT_FROSTBOLT, 2100);
                    }
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_worgen_battlemage_45537AI(creature);
    }
};

// Deathstalker Commander Belmont - 45474
class npc_deathstalker_belmont_45474 : public CreatureScript
{
public:
    npc_deathstalker_belmont_45474() : CreatureScript("npc_deathstalker_belmont_45474") { }

    struct npc_deathstalker_belmont_45474AI : public ScriptedAI
    {
        npc_deathstalker_belmont_45474AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            QUEST_GREAT_ESCAPE = 27438,

            EVENT_SCENE = 1,
        };

        ObjectGuid  m_playerGUID;
        ObjectGuid  m_arthuraGUID;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_GREAT_ESCAPE)
            {
                me->GetAI()->SetGUID(player->GetGUID(), m_playerGUID);

                player->SummonCreature(45314, -2118.54f, 932.51f, 4.0295f, 3.573078f, TEMPSUMMON_MANUAL_DESPAWN);

                player->SummonCreature(45474, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 22000, 0, ObjectGuid::Empty);
            }
        }

        void Reset()
        {
            _events.Reset();

            me->SetPowerType(Powers::POWER_ENERGY);
            me->SetMaxPower(Powers::POWER_ENERGY, 100);

            if (me->GetAreaId() == 5436)
                me->AddAura(68442, me);

            if (me->IsSummon())
                _events.ScheduleEvent(EVENT_SCENE, 0);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
                m_playerGUID = player->GetGUID();
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SCENE:
                {
                    _events.ScheduleEvent(EVENT_SCENE + 1, 3500);
                    break;
                }

                case EVENT_SCENE + 1:
                {
                    me->AI()->Talk(0);
                    me->RemoveAura(68442);

                    _events.ScheduleEvent(EVENT_SCENE + 2, 6000);
                    break;
                }

                case EVENT_SCENE + 2:
                {
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_SCENE + 3, 4000);
                    break;
                }

                case EVENT_SCENE + 3:
                {
                    me->AI()->Talk(2);

                    _events.ScheduleEvent(EVENT_SCENE + 4, 3500);
                    break;
                }

                case EVENT_SCENE + 4:
                {
                    me->CastSpell(me, 86603, true);
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_deathstalker_belmont_45474AI(creature);
    }
};

// Arthura - 45314
class npc_arthura_45314 : public CreatureScript
{
public:
    npc_arthura_45314() : CreatureScript("npc_arthura_45314") { }

    struct npc_arthura_45314AI : public ScriptedAI
    {
        npc_arthura_45314AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            QUEST_GREAT_ESCAPE = 27438,

            EVENT_SCENE = 1,
        };

        ObjectGuid  m_playerGUID;
        ObjectGuid  m_godfreyGUID;

        void Reset() override
        {
            _events.Reset();

            if (!me->IsSummon())
                me->RemoveVehicleKit();

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    m_playerGUID = player->GetGUID();
            }

            _events.ScheduleEvent(EVENT_SCENE, 1000);
        }

        void WaypointReached(uint32 waypointId, uint32 pathId) override
        {
            if (pathId == 453740)
            {
                if (waypointId == 15)
                    _events.ScheduleEvent(EVENT_SCENE + 4, 1000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SCENE:
                {
                    me->AI()->Talk(0);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->ExitVehicle();
                        me->CastSpell(player, 84109);
                        player->EnterVehicle(me);
                    }

                    if (Creature* godfrey = me->SummonCreature(44369, -2121.61f, 931.01f, 1.1034f, 0.4878f, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        m_godfreyGUID = godfrey->GetGUID();

                        godfrey->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                        godfrey->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                        godfrey->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                        godfrey->ExitVehicle();
                        me->CastSpell(godfrey, 84109);
                        godfrey->EnterVehicle(me);
                    }

                    _events.ScheduleEvent(EVENT_SCENE + 1, 15000);
                    break;
                }

                case EVENT_SCENE + 1:
                {
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_SCENE + 2, 2000);
                    break;
                }

                case EVENT_SCENE + 2:
                {
                    me->GetMotionMaster()->MovePath(453740, false);

                    _events.ScheduleEvent(EVENT_SCENE + 3, 13000);
                    break;
                }

                case EVENT_SCENE + 3:
                {
                    me->AI()->Talk(2);
                    break;
                }

                case EVENT_SCENE + 4:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->CompleteQuest(27438);
                        me->GetVehicleKit()->RemoveAllPassengers();

                        if (Creature* godfrey = ObjectAccessor::GetCreature(*me, m_godfreyGUID))
                            godfrey->DespawnOrUnsummon();
                    }

                    me->DespawnOrUnsummon(1000);
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_arthura_45314AI(creature);
    }
};

/* Gilneas Part Finish */

// Lady Sylvanas Windrunner (Quest Giver) - 45525
class npc_lady_sylvanas_windrunner_45525 : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner_45525() : CreatureScript("npc_lady_sylvanas_windrunner_45525") { }

    enum eNPC
    {
        QUEST_RISE_GODFREY = 27472,
        EVENT_SCENE = 1,

        PLAYER_GUID = 99999,
    };

    struct npc_lady_sylvanas_windrunner_45525AI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunner_45525AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap _events;
        ObjectGuid  m_playerGUID;
        ObjectGuid  m_agathaGUID;
        ObjectGuid  m_arthuraGUID;
        ObjectGuid  m_daschlaGUID;
        ObjectGuid  m_cromushGUID;
        ObjectGuid  m_godfreyGUID;
        ObjectGuid  m_ashburyGUID;
        ObjectGuid  m_waldenGUID;
        std::list<ObjectGuid>spawnedList;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_RISE_GODFREY)
                player->SummonCreature(45525, me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0, 0, ObjectGuid::Empty);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Player* player = summoner->ToPlayer())
            {
                m_playerGUID = player->GetGUID();

                me->Mount(29257);

                _events.ScheduleEvent(EVENT_SCENE, 1000);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            spawnedList.push_back(summon->GetGUID());
        }

        void Reset() override
        {
            _events.Reset();
            spawnedList.clear();
            m_playerGUID = ObjectGuid::Empty;
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SCENE:
                {
                    me->AI()->Talk(0);

                    if (Creature* agatha = me->SummonCreature(45609, -193.28f, 1282.36f, 42.8246f, 4.18617f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_agathaGUID = agatha->GetGUID();

                    if (Creature* arthura = me->SummonCreature(45611, -203.30f, 1275.54f, 43.1805f, 0.164935f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_arthuraGUID = arthura->GetGUID();

                    if (Creature* daschla = me->SummonCreature(45610, -194.35f, 1269.22f, 45.1161f, 1.93601f, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        daschla->RemoveFlag(UNIT_NPC_FLAGS, 2);
                        m_daschlaGUID = daschla->GetGUID();
                    }

                    if (Creature* cromush = me->SummonCreature(45593, -179.77f, 1279.76f, 46.000f, 3.07554f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_cromushGUID = cromush->GetGUID();

                    if (Creature* godfrey = me->SummonCreature(45606, -198.58f, 1275.18f, 39.9967f, 0.164935f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_godfreyGUID = godfrey->GetGUID();

                    if (Creature* ashbury = me->SummonCreature(45607, -198.32f, 1271.95f, 40.8185f, 0.164935f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_ashburyGUID = ashbury->GetGUID();

                    if (Creature* walden = me->SummonCreature(45608, -197.39f, 1279.18f, 39.5602f, 0.164935f, TEMPSUMMON_MANUAL_DESPAWN))
                        m_waldenGUID = walden->GetGUID();

                    _events.ScheduleEvent(EVENT_SCENE + 1, 1000);
                    break;
                }

                case EVENT_SCENE + 1:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->GetMotionMaster()->MovePoint(1, -193.28f, 1282.36f, 46.8246f, false);

                    if (Creature* arthura = ObjectAccessor::GetCreature(*me, m_arthuraGUID))
                        arthura->GetMotionMaster()->MovePoint(1, -203.30f, 1275.54f, 46.8246f, false);

                    if (Creature* daschla = ObjectAccessor::GetCreature(*me, m_daschlaGUID))
                        daschla->GetMotionMaster()->MovePoint(1, -194.35f, 1269.22f, 46.8246f, false);

                    _events.ScheduleEvent(EVENT_SCENE + 2, 3000);
                    break;
                }

                case EVENT_SCENE + 2:
                {
                    if (Creature* agatha = ObjectAccessor::GetCreature(*me, m_agathaGUID))
                        agatha->CastSpell(agatha, 85197, true);

                    if (Creature* arthura = ObjectAccessor::GetCreature(*me, m_arthuraGUID))
                        arthura->CastSpell(arthura, 85197, true);

                    if (Creature* daschla = ObjectAccessor::GetCreature(*me, m_daschlaGUID))
                        daschla->CastSpell(daschla, 85197, true);

                    _events.ScheduleEvent(EVENT_SCENE + 3, 13000);
                    break;
                }

                case EVENT_SCENE + 3:
                {
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_SCENE + 4, 8000);
                    break;
                }

                case EVENT_SCENE + 4:
                {
                    if (Creature* cromush = ObjectAccessor::GetCreature(*me, m_cromushGUID))
                        cromush->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_SCENE + 5, 3000);
                    break;
                }

                case EVENT_SCENE + 5:
                {
                    me->AI()->Talk(2);

                    _events.ScheduleEvent(EVENT_SCENE + 6, 18000);
                    break;
                }

                case EVENT_SCENE + 6:
                {
                    me->AI()->Talk(3);

                    _events.ScheduleEvent(EVENT_SCENE + 7, 6000);
                    break;
                }

                case EVENT_SCENE + 7:
                {
                    me->AI()->Talk(4);

                    _events.ScheduleEvent(EVENT_SCENE + 8, 6000);
                    break;
                }

                case EVENT_SCENE + 8:
                {
                    me->AI()->Talk(5);

                    _events.ScheduleEvent(EVENT_SCENE + 9, 2000);
                    break;
                }

                case EVENT_SCENE + 9:
                {
                    me->GetMotionMaster()->MovePoint(1, -151.93f, 1270.57f, 51.5560f, false, 2.5f);

                    _events.ScheduleEvent(EVENT_SCENE + 10, 8000);
                    break;
                }

                case EVENT_SCENE + 10:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        me->SetFacingToObject(player, true);

                    _events.ScheduleEvent(EVENT_SCENE + 11, 2000);
                    break;
                }

                case EVENT_SCENE + 11:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->KilledMonsterCredit(45617);
                        RemoveAllSpawnedCreatures();
                        me->DespawnOrUnsummon();
                    }
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

        void RemoveAllSpawnedCreatures()
        {
            for (std::list<ObjectGuid>::const_iterator itr = spawnedList.begin(); itr != spawnedList.end(); itr++)
            {
                if (Creature* npc = ObjectAccessor::GetCreature(*me, (*itr)))
                    npc->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lady_sylvanas_windrunner_45525AI(creature);
    }
};

// Ashbury, Godfrey, Walden - 45607, 45606, 45608
class npc_ashgodwal_45606 : public CreatureScript
{
public:
    npc_ashgodwal_45606() : CreatureScript("npc_ashgodwal_45606") { }

    enum eSpell
    {
        EVENT_MOVE = 0,
        EVENT_TRANSFORM = 1,
        EVENT_SCENE = 2,
    };

    struct npc_ashgodwal_45606AI : public ScriptedAI
    {
        npc_ashgodwal_45606AI(Creature* creature) : ScriptedAI(creature)
        {
            Done1 = false;
        }

        EventMap m_events;
        bool Done1;

        void Reset() override
        {
            Done1 = false;

            m_events.Reset();

            me->RemoveAura(78351);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
            me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            if (me->HasAura(78351))
                m_events.ScheduleEvent(EVENT_TRANSFORM, 1000);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_TRANSFORM:
                {
                    if (Done1)
                        return;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);

                    if (me->GetEntry() == 45606)
                        me->CastSpell(me, 85198, true);
                    else if (me->GetEntry() == 45607)
                        me->CastSpell(me, 85200, true);
                    else
                        me->CastSpell(me, 85201, true);

                    m_events.ScheduleEvent(EVENT_SCENE, 1000);
                    Done1 = true;
                    break;
                }

                case EVENT_SCENE:
                {
                    me->AddAura(68442, me);

                    if (me->GetEntry() == 45606)
                        me->AI()->Talk(0);

                    m_events.ScheduleEvent(EVENT_SCENE + 1, 18000);
                    break;
                }

                case EVENT_SCENE + 1:
                {
                    if (me->GetEntry() == 45606)
                    {
                        me->RemoveAura(68442);
                        me->AI()->Talk(1);
                    }

                    m_events.ScheduleEvent(EVENT_SCENE + 2, 4500);
                    break;
                }

                case EVENT_SCENE + 2:
                {
                    if (me->GetEntry() == 45606)
                        me->AI()->Talk(2);

                    m_events.ScheduleEvent(EVENT_SCENE + 3, 4500);
                    break;
                }

                case EVENT_SCENE + 3:
                {
                    if (me->GetEntry() == 45606)
                        me->AI()->Talk(3);

                    m_events.ScheduleEvent(EVENT_SCENE + 4, 14500);
                    break;
                }

                case EVENT_SCENE + 4:
                {
                    if (me->GetEntry() == 45606)
                    {
                        me->AI()->Talk(4);
                        me->GetMotionMaster()->MovePath(456060, false);
                    }
                    else if (me->GetEntry() == 45607)
                    {
                        me->RemoveAura(68442);
                        if (Creature* godfrey = me->FindNearestCreature(45606, 15.0f, true))
                            me->GetMotionMaster()->MoveFollow(godfrey, 2.0f, M_PI * 1.5f, false, true, false);
                    }
                    else
                    {
                        me->RemoveAura(68442);
                        if (Creature* godfrey = me->FindNearestCreature(45606, 15.0f, true))
                            me->GetMotionMaster()->MoveFollow(godfrey, 2.0f, M_PI / 2.0f, false, true, false);
                    }

                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ashgodwal_45606AI(creature);
    }
};

// Raise Forsaken - 85197
class spell_raise_forsaken_85197 : public SpellScriptLoader
{
public:
    spell_raise_forsaken_85197() : SpellScriptLoader("spell_raise_forsaken_85197") { }

    enum eSpell
    {
        NPC_ASHBURY = 45607,
        NPC_GODFREY = 45606,
        NPC_WALDEN = 45608
    };

    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    class spell_raise_forsaken_85197_SpellScript : public SpellScript
    {
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<uint32>entrys;
            entrys.push_back(NPC_ASHBURY);
            entrys.push_back(NPC_GODFREY);
            entrys.push_back(NPC_WALDEN);
            targets.remove_if(IsNotInEntryList(entrys));
        }

        void Register()
        {
            OnObjectAreaTargetSelect.Register(&spell_raise_forsaken_85197_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect.Register(&spell_raise_forsaken_85197_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect.Register(&spell_raise_forsaken_85197_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_raise_forsaken_85197_SpellScript();
    }

    class spell_raise_forsaken_85197_AuraScript : public AuraScript
    {
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* corpses = unit->ToCreature())
                {
                    corpses->AddAura(87259, corpses);
                    corpses->SetFloatValue(UNIT_FIELD_HOVERHEIGHT, 2.5f);
                    corpses->SetHover(true, false, false);
                }
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* corpses = unit->ToCreature())
                {
                    corpses->RemoveAura(87259);
                    corpses->SetHover(false);
                    corpses->AddAura(78351, corpses);
                }
            }
        }

        void Register() override
        {
            OnEffectApply.Register(&spell_raise_forsaken_85197_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply.Register(&spell_raise_forsaken_85197_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply.Register(&spell_raise_forsaken_85197_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove.Register(&spell_raise_forsaken_85197_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove.Register(&spell_raise_forsaken_85197_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove.Register(&spell_raise_forsaken_85197_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_raise_forsaken_85197_AuraScript();
    }
};

// Lady Sylvanas Windrunner (Quest Giver) - 45617
class npc_lady_sylvanas_windrunner_45617 : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner_45617() : CreatureScript("npc_lady_sylvanas_windrunner_45617") { }

    enum eNPC
    {
        SPELL_SUMMON_SYLVANAS = 85864,
        SPELL_SUMMON_AGATHA = 85870,
        SPELL_SUMMON_ARTHURA = 85869,
        SPELL_SUMMON_DASCHLA = 85871,
        SPELL_SUMMON_CROMUSH = 85868,

        QUEST_CITIES_IN_DUST = 27601,
    };

    struct npc_lady_sylvanas_windrunner_45617AI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunner_45617AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap _events;
        ObjectGuid  m_playerGUID;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_CITIES_IN_DUST)
            {
                player->CastSpell(me, SPELL_SUMMON_SYLVANAS, false);
                player->CastSpell(player, SPELL_SUMMON_AGATHA, false);
                player->CastSpell(player, SPELL_SUMMON_ARTHURA, false);
                player->CastSpell(player, SPELL_SUMMON_DASCHLA, false);
                player->CastSpell(player, SPELL_SUMMON_CROMUSH, false);
            }
        }

        void Reset() override
        {
            _events.Reset();
            m_playerGUID = ObjectGuid::Empty;
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lady_sylvanas_windrunner_45617AI(creature);
    }
};

// Forsaken Catapult - 45198
class npc_forsaken_catapult : public CreatureScript
{
public:
    npc_forsaken_catapult() : CreatureScript("npc_forsaken_catapult") { }

    struct npc_forsaken_catapultAI : public ScriptedAI
    {
        npc_forsaken_catapultAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_THROW_BARREL = 85147,

            EVENT_BARREL = 1,
        };

        void Reset()
        {
            _events.Reset();

            _events.ScheduleEvent(EVENT_BARREL, 4000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BARREL:
                {
                    Position pos = me->GetRandomPoint(me->GetPosition(), urand(50.0f, 120.0f));

                    me->CastSpell(pos, SPELL_THROW_BARREL, true);

                    _events.ScheduleEvent(EVENT_BARREL, 4000);
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_forsaken_catapultAI(creature);
    }
};

// Ambermill Dimensional Portal 45752
class npc_ambermill_portal_45752 : public CreatureScript
{
public:
    npc_ambermill_portal_45752() : CreatureScript("npc_ambermill_portal_45752") { }

    struct npc_ambermill_portal_45752AI : public ScriptedAI
    {
        npc_ambermill_portal_45752AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            QUEST_TDW_CHAPTER_2 = 27513,
            SPELL_TELEPORT = 85360,
            ACTION_OPTION_ID = 0,
        };

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            switch (gossipListId)
            {
            case ACTION_OPTION_ID:
            {
                if (player->GetQuestStatus(QUEST_TDW_CHAPTER_2) == QUEST_STATUS_INCOMPLETE)
                    player->CastSpell(player, SPELL_TELEPORT, true);
                break;
            }
            }

            CloseGossipMenuFor(player);

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ambermill_portal_45752AI(creature);
    }
};

// Archmage Ataeric - 2120
class npc_archmage_ataeric_2120 : public CreatureScript
{
public:
    npc_archmage_ataeric_2120() : CreatureScript("npc_archmage_ataeric_2120") { }

    struct npc_archmage_ataeric_2120AI : public ScriptedAI
    {
        npc_archmage_ataeric_2120AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_SHIELD = 87251,
            SPELL_BEAM = 85373,
            SPELL_SUMMON_ARCANE = 87253,
            SPELL_ARCANE_BLAST = 87249,
            SPELL_BUBBLE_BREAK = 85400,
            SPELL_KILLCREDIT = 85376,

            EVENT_START_EVENT = 1,
            EVENT_POWER_20 = 2,
            EVENT_POWER_40 = 3,
            EVENT_POWER_60 = 4,
            EVENT_POWER_80 = 5,
            EVENT_POWER_100 = 6,
            EVENT_SUMMON_FIEND = 7,
            EVENT_CAST_BEAM = 8,
            EVENT_FINISH = 9
        };

        bool Done_20;
        bool Done_40;
        bool Done_60;
        bool Done_80;
        bool Done_100;

        ObjectGuid m_playerGUID;
        std::list<ObjectGuid>spawnedList;

        Position pos1 = { -136.69f, 1077.04f, 82.5790f, 6.223499f };
        Position pos2 = { -136.72f, 1081.99f, 72.2496f, 6.223499f };
        Position pos3 = { -149.40f, 1078.45f, 78.5251f, 6.223499f };
        Position pos4 = { -121.98f, 1077.88f, 78.6668f, 6.223499f };

        Position pos5 = { -106.33f, 1075.27f, 78.6668f, 6.223499f };
        Position pos6 = { -100.51f, 1077.41f, 68.1010f, 6.223499f };
        Position pos7 = { -167.68f, 1080.21f, 78.6668f, 6.223499f };
        Position pos8 = { -172.98f, 1077.65f, 68.1010f, 6.223499f };

        void Reset() override
        {
            Done_20 = false;
            Done_40 = false;
            Done_60 = false;
            Done_80 = false;
            Done_100 = false;

            RemoveAllSpawnedCreatures();

            _events.Reset();

            spawnedList.clear();
            m_playerGUID == 0;

            me->AddUnitState(UNIT_STATE_ROOT);
            me->AddAura(SPELL_SHIELD, me);

            me->SetPower(me->GetPowerType(), 0, true);

            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);

            me->SummonCreature(46661, pos1, TEMPSUMMON_MANUAL_DESPAWN);

            me->CastSpell(me, SPELL_BEAM, true);
        }

        void JustSummoned(Creature* summon) override
        {
            spawnedList.push_back(summon->GetGUID());
        }

        void JustEngagedWith(Unit* who) override
        {
            m_playerGUID = who->GetGUID();

            me->AI()->Talk(0);

            _events.ScheduleEvent(EVENT_START_EVENT, 1500);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->GetPower(me->GetPowerType()) == 100 && Done_100 == false)
                _events.ScheduleEvent(EVENT_POWER_100, 1000);

            if (me->GetPower(me->GetPowerType()) >= 80 && Done_80 == false)
                _events.ScheduleEvent(EVENT_POWER_80, 1000);

            if (me->GetPower(me->GetPowerType()) >= 60 && Done_60 == false)
                _events.ScheduleEvent(EVENT_POWER_60, 1000);

            if (me->GetPower(me->GetPowerType()) >= 40 && Done_40 == false)
                _events.ScheduleEvent(EVENT_POWER_40, 1000);

            if (me->GetPower(me->GetPowerType()) >= 20 && Done_20 == false)
                _events.ScheduleEvent(EVENT_POWER_20, 1000);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_START_EVENT:
                {
                    me->AI()->Talk(6);

                    _events.ScheduleEvent(EVENT_SUMMON_FIEND, 5500);
                    break;
                }

                case EVENT_CAST_BEAM:
                {
                    me->CastStop();

                    me->CastSpell(me, SPELL_BEAM, true);
                    break;
                }

                case EVENT_SUMMON_FIEND:
                {
                    me->CastSpell(me, SPELL_SUMMON_ARCANE, true);

                    _events.CancelEvent(EVENT_SUMMON_FIEND);

                    // The more power the shield is charged, the more inestable it becomes and the faster arcane fiends come out

                    if (me->GetPower(me->GetPowerType()) >= 80 && Done_100 == false)
                        _events.ScheduleEvent(EVENT_SUMMON_FIEND, urand(2500, 5500));

                    if (me->GetPower(me->GetPowerType()) >= 60 && Done_80 == false)
                        _events.ScheduleEvent(EVENT_SUMMON_FIEND, urand(3500, 6500));

                    if (me->GetPower(me->GetPowerType()) >= 40 && Done_60 == false)
                        _events.ScheduleEvent(EVENT_SUMMON_FIEND, urand(3500, 6500));

                    if (me->GetPower(me->GetPowerType()) >= 20 && Done_40 == false)
                        _events.ScheduleEvent(EVENT_SUMMON_FIEND, urand(4500, 7500));

                    if (me->GetPower(me->GetPowerType()) < 20 && Done_20 == false)
                        _events.ScheduleEvent(EVENT_SUMMON_FIEND, urand(4500, 8500));

                    break;
                }

                case EVENT_POWER_20:
                {
                    if (Done_20)
                        return;

                    me->SummonCreature(46661, pos2, TEMPSUMMON_MANUAL_DESPAWN);

                    _events.ScheduleEvent(EVENT_CAST_BEAM, 2000);

                    Done_20 = true;
                    break;
                }

                case EVENT_POWER_40:
                {
                    if (Done_40)
                        return;

                    me->AI()->Talk(1);

                    me->SummonCreature(46661, pos3, TEMPSUMMON_MANUAL_DESPAWN);

                    _events.ScheduleEvent(EVENT_CAST_BEAM, 2000);

                    Done_40 = true;
                    break;
                }

                case EVENT_POWER_60:
                {
                    if (Done_60)
                        return;

                    me->AI()->Talk(2);

                    me->SummonCreature(46661, pos4, TEMPSUMMON_MANUAL_DESPAWN);

                    _events.ScheduleEvent(EVENT_CAST_BEAM, 2000);

                    Done_60 = true;
                    break;
                }

                case EVENT_POWER_80:
                {
                    if (Done_80)
                        return;

                    me->AI()->Talk(3);

                    me->SummonCreature(46661, pos5, TEMPSUMMON_MANUAL_DESPAWN);
                    me->SummonCreature(46661, pos6, TEMPSUMMON_MANUAL_DESPAWN);
                    me->SummonCreature(46661, pos7, TEMPSUMMON_MANUAL_DESPAWN);
                    me->SummonCreature(46661, pos8, TEMPSUMMON_MANUAL_DESPAWN);

                    _events.ScheduleEvent(EVENT_CAST_BEAM, 2000);

                    Done_80 = true;
                    break;
                }

                case EVENT_POWER_100:
                {
                    if (Done_100)
                        return;

                    me->AI()->Talk(4);

                    _events.CancelEvent(EVENT_SUMMON_FIEND);

                    _events.ScheduleEvent(EVENT_FINISH, 6000);

                    Done_100 = true;
                    break;
                }

                case EVENT_FINISH:
                {
                    Position posdaschla = { -136.02f, 1106.96f, 66.72f, 4.70453f };

                    me->CastStop();

                    me->RemoveAura(SPELL_SHIELD);

                    me->CastSpell(me, SPELL_BUBBLE_BREAK, true);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->SummonCreature(45775, posdaschla, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -107.47f, 1121.98f, 64.411f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -116.16f, 1113.05f, 64.102f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -108.26f, 1112.52f, 64.504f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -99.20f, 1111.92f, 64.882f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -88.71f, 1111.22f, 65.199f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -93.80f, 1114.16f, 65.014f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -103.44f, 1115.82f, 64.674f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -111.98f, 1116.95f, 64.516f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -112.86f, 1121.96f, 64.655f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -107.47f, 1121.98f, 64.411f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -94.89f, 1122.05f, 64.709f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -89.28f, 1122.07f, 64.892f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -152.60f, 1113.93f, 59.828f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -158.65f, 1115.08f, 56.910f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -164.83f, 1112.09f, 56.001f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -171.07f, 1114.98f, 56.252f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -175.80f, 1112.0f, 56.177f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -166.90f, 1119.5f, 56.390f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -162.01f, 1119.54f, 56.827f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -132.47f, 1113.83f, 62.728f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -139.47f, 1113.94f, 63.014f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -136.26f, 1116.56f, 62.702f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -132.24f, 1119.88f, 62.648f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45777, -139.46f, 1120.39f, 62.745f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);

                        player->SummonCreature(45778, -82.96f, 1123.06f, 65.055f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45778, -100.70f, 1122.97f, 64.482f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45778, -118.44f, 1122.90f, 64.831f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45778, -155.75f, 1123.83f, 59.151f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45778, -172.29f, 1123.18f, 56.793f, 4.6462f, TEMPSUMMON_TIMED_DESPAWN, 60000);

                        player->SummonCreature(45779, -135.783005f, 1069.9799f, 66.1443f, 1.551942f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -153.32f, 1045.61f, 69.52f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -182.46f, 1044.06f, 61.80f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -179.72f, 1021.57f, 65.12f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -213.89f, 1022.92f, 68.03f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -204.55f, 990.23f, 68.04f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -212.53f, 981.60f, 71.09f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -188.49f, 986.71f, 65.54f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -156.70f, 1005.30f, 65.12f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -140.13f, 997.25f, 67.97f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -129.76f, 1011.04f, 68.12f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -122.28f, 1048.54f, 67.76f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -105.28f, 1045.77f, 67.16f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -114.08f, 1007.09f, 66.33f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -90.34f, 970.78f, 69.73f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -63.75f, 975.13f, 67.05f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -83.60f, 955.85f, 68.24f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -113.12f, 941.53f, 67.85f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -134.47f, 964.25f, 67.48f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -143.16f, 945.58f, 67.33f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -155.53f, 928.65f, 65.56f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -157.66f, 903.40f, 64.81f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -131.47f, 916.78f, 66.92f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        player->SummonCreature(45781, -102.66f, 878.40f, 63.81f, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }

                    DoCastAOE(SPELL_KILLCREDIT, false);

                    _events.ScheduleEvent(EVENT_FINISH + 1, 1000);
                    break;
                }

                case EVENT_FINISH + 1:
                {
                    me->AI()->Talk(5);

                    me->CastSpell(me, 87255, true);

                    RemoveAllSpawnedCreatures();

                    me->SetCorpseDelay(1000);
                    me->DespawnOrUnsummon(1000);
                    break;
                }
                }
            }

            UpdateVictim();
        }

        void RemoveAllSpawnedCreatures()
        {
            for (std::list<ObjectGuid>::const_iterator itr = spawnedList.begin(); itr != spawnedList.end(); itr++)
            {
                if (Creature* npc = ObjectAccessor::GetCreature(*me, (*itr)))
                    npc->DespawnOrUnsummon();
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_archmage_ataeric_2120AI(creature);
    }
};

// Arcane Fiend - 45766
class npc_arcane_fiend_45766 : public CreatureScript
{
public:
    npc_arcane_fiend_45766() : CreatureScript("npc_arcane_fiend_45766") { }

    struct npc_arcane_fiend_45766AI : public ScriptedAI
    {
        npc_arcane_fiend_45766AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_ARCANE_BLAST = 87249,

            EVENT_START_EVENT = 1
        };

        void Reset() override
        {
            _events.Reset();
        }

        void JustAppeared() override
        {
            me->CastSpell(me, 98174, true);
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->CastSpell(me, SPELL_ARCANE_BLAST, true);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_arcane_fiend_45766AI(creature);
    }
};

// Dimensional Beam - 85373
class spell_dimensional_beam_85373 : public SpellScriptLoader
{
public:
    spell_dimensional_beam_85373() : SpellScriptLoader("spell_dimensional_beam_85373") { }

    enum eSpell
    {
        NPC_TRIGGER_TARGET = 46661
    };

    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    class spell_dimensional_beam_85373_SpellScript : public SpellScript
    {
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<uint32>entrys;
            entrys.push_back(NPC_TRIGGER_TARGET);
            targets.remove_if(IsNotInEntryList(entrys));
        }

        void Register()
        {
            OnObjectAreaTargetSelect.Register(&spell_dimensional_beam_85373_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dimensional_beam_85373_SpellScript();
    }
};

// Daschla, Forsaken Vanguard - 45775, 47777
class npc_daschla_forsaken : public CreatureScript
{
public:
    npc_daschla_forsaken() : CreatureScript("npc_daschla_forsaken") { }

    struct npc_daschla_forsakenAI : public ScriptedAI
    {
        npc_daschla_forsakenAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_SCENE = 1,
        };

        ObjectGuid  m_playerGUID;

        void Reset()
        {
            _events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            _events.ScheduleEvent(EVENT_SCENE, 2000);
        }

        void WaypointReached(uint32 waypointId, uint32 pathId) override
        {
            if (pathId == 457750 && waypointId == 2)
                _events.ScheduleEvent(EVENT_SCENE + 3, 1000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SCENE:
                {
                    if (me->GetEntry() == 45777)
                        _events.ScheduleEvent(EVENT_SCENE + 1, 2000);

                    if (me->GetEntry() == 45775)
                    {
                        me->AI()->Talk(0);
                        _events.ScheduleEvent(EVENT_SCENE + 2, 2000);
                    }

                    break;
                }

                case EVENT_SCENE + 1:
                {
                    std::vector<uint32> pathIds = { 457770, 457771, 457772, 457773 };

                    uint32 chosenPathId = Trinity::Containers::SelectRandomContainerElement(pathIds);

                    me->GetMotionMaster()->MovePath(chosenPathId, false);
                    break;
                }

                case EVENT_SCENE + 2:
                {
                    me->GetMotionMaster()->MovePath(457750, false);
                    break;
                }

                case EVENT_SCENE + 3:
                {
                    me->SetFacingTo(1.523814f);
                    me->GetMotionMaster()->MovePoint(1, -135.229f, 1056.57f, 73.6036f, false, 4.0f);
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_SCENE + 4, 2000);
                    break;
                }

                case EVENT_SCENE + 4:
                {
                    me->CastSpell(me, 85411, false);
                    break;
                }
                }
            }

            UpdateVictim();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_daschla_forsakenAI(creature);
    }
};

// Raise Forsaken - 85411
class spell_raise_forsaken_85411 : public SpellScriptLoader
{
public:
    spell_raise_forsaken_85411() : SpellScriptLoader("spell_raise_forsaken_85411") { }

    enum eSpell
    {
        NPC_FALLEN_MAGI_45781 = 45781,
        NPC_FALLEN_ATAERIC_45779 = 45779,
        SPELL_FLOATING = 87259
    };

    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    class spell_raise_forsaken_85411_SpellScript : public SpellScript
    {
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<uint32>entrys;
            entrys.push_back(NPC_FALLEN_MAGI_45781);
            entrys.push_back(NPC_FALLEN_ATAERIC_45779);
            targets.remove_if(IsNotInEntryList(entrys));
        }

        void Register()
        {
            OnObjectAreaTargetSelect.Register(&spell_raise_forsaken_85411_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_raise_forsaken_85411_SpellScript();
    }

    class spell_raise_forsaken_85411_AuraScript : public AuraScript
    {
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* corpses = unit->ToCreature())
                {
                    corpses->AddAura(87259, corpses);
                    corpses->SetFloatValue(UNIT_FIELD_HOVERHEIGHT, 2.5f);
                    corpses->SetHover(true, false, false);
                }
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unit = GetTarget())
            {
                if (Creature* corpses = unit->ToCreature())
                {
                    corpses->RemoveAura(87259);
                    corpses->SetHover(false);
                    corpses->AddAura(78351, corpses);
                }
            }
        }

        void Register() override
        {
            OnEffectApply.Register(&spell_raise_forsaken_85411_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove.Register(&spell_raise_forsaken_85411_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_raise_forsaken_85411_AuraScript();
    }
};

// Archmage Ataeric and Magi - 45779, 45781
class npc_ataeric_and_magi : public CreatureScript
{
public:
    npc_ataeric_and_magi() : CreatureScript("npc_ataeric_and_magi") { }

    enum eSpell
    {
        SPELL_DEATH = 58951,

        EVENT_DIE = 1,
        EVENT_APPEAR = 2,
        EVENT_TRANSFORM = 3,
        EVENT_SALUTE = 4
    };

    struct npc_ataeric_and_magiAI : public ScriptedAI
    {
        npc_ataeric_and_magiAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap m_events;
        bool Done1;
        bool Done2;

        void Reset() override
        {
            Done1 = false;
            Done2 = false;

            m_events.Reset();

            me->RemoveAura(78351);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
            me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);

            if (me->GetEntry() == 45779)
            {
                me->SetVisible(false);
                m_events.ScheduleEvent(EVENT_DIE, 0);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            if (me->HasAura(78351))
                m_events.ScheduleEvent(EVENT_TRANSFORM, 1000);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DIE:
                {
                    if (Done1)
                        return;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                    me->CastSpell(me, SPELL_DEATH, true);

                    m_events.ScheduleEvent(EVENT_APPEAR, 2000);

                    Done1 = true;
                    break;
                }

                case EVENT_APPEAR:
                {
                    me->SetVisible(true);
                    break;
                }

                case EVENT_TRANSFORM:
                {
                    if (Done2)
                        return;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAND_STATE_DEAD);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);

                    if (me->GetEntry() == 45779)
                        me->CastSpell(me, 85410, true);
                    else
                        me->CastSpell(me, 85409, true);

                    m_events.ScheduleEvent(EVENT_SALUTE, 2500);

                    Done2 = true;
                    break;
                }

                case EVENT_SALUTE:
                {
                    me->HandleEmoteCommand(113);
                    break;
                }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ataeric_and_magiAI(creature);
    }
};

// Lord Godfrey - 45878
class npc_godfrey_45878 : public CreatureScript
{
public:
    npc_godfrey_45878() : CreatureScript("npc_godfrey_45878") { }

    struct npc_godfrey_45878AI : public ScriptedAI
    {
        npc_godfrey_45878AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            NPC_INN = 45937,
            NPC_CHAPEL = 45938,
            NPC_TOWNHALL = 45939,

            SPELL_MOLTEN_ARMOR = 79849,

            SPELL_FIREBALL = 79854,
            SPELL_FLAMESTRIKE = 79856,
            SPELL_BLASTWAVE = 79857,
            SPELL_FIREBLAST = 79855,

            SPELL_NOTIFY_CHAT = 83990,
            SPELL_NOTIFY_KILL = 84145,
            SPELL_NOTIFY_DEMPSEY = 84195,
            SPELL_KILL = 85685,

            EVENT_CONVERSATION = 1,
            EVENT_FIREBALL = 2,
            EVENT_FIREBLAST = 3,
            EVENT_FLAMESTRIKE = 4,
            EVENT_BLASTWAVE = 5,
            EVENT_KILL = 6,
            EVENT_CONVO_DEMPSEY = 7,
            EVENT_CONVO_LORNA = 100,
            EVENT_FINAL_LORNA = 150,

            QUEST_BELLY_OF_BEAST = 27575,
            QUEST_ON_HER_MAJESTY_SECRET_SERVICE = 27594
        };

        bool Inn_Done;
        bool Chapel_Done;
        bool Townhall_Done;
        bool Lorna_Done;
        bool LornaFinal_Done;
        bool LornaFinal2_Done;

        ObjectGuid m_playerGUID;

        void QuestReward(Player* player, Quest const* quest, uint32 /*opt*/) override
        {
            if (quest->GetQuestId() == QUEST_BELLY_OF_BEAST)
                player->CastSpell(player, 85678, false);
        }

        void Reset() override
        {
            m_playerGUID = me->GetOwnerGUID();

            Lorna_Done = false;
            LornaFinal_Done = false;
            LornaFinal2_Done = false;

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (player->GetQuestStatus(27550) == QUEST_STATUS_INCOMPLETE)
                {
                    Inn_Done = false;
                    Chapel_Done = false;
                    Townhall_Done = false;
                }
                else
                {
                    Inn_Done = true;
                    Chapel_Done = true;
                    Townhall_Done = true;
                }

                if (player->GetQuestStatus(QUEST_ON_HER_MAJESTY_SECRET_SERVICE) == QUEST_STATUS_COMPLETE && !LornaFinal2_Done)
                    _events.ScheduleEvent(EVENT_FINAL_LORNA + 1, 0);
            }

            me->SetReactState(REACT_ASSIST);

            _events.Reset();

            if (!me->HasAura(SPELL_MOLTEN_ARMOR))
                me->CastSpell(me, SPELL_MOLTEN_ARMOR, false);

            _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 90000));
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_NOTIFY_CHAT)
            {
                if (caster->GetEntry() == NPC_INN)
                {
                    if (Inn_Done)
                        return;

                    me->AI()->Talk(3);

                    Inn_Done = true;
                }

                if (caster->GetEntry() == NPC_CHAPEL)
                {
                    if (Chapel_Done)
                        return;

                    me->AI()->Talk(4);

                    Chapel_Done = true;
                }

                if (caster->GetEntry() == NPC_TOWNHALL)
                {
                    if (Townhall_Done)
                        return;

                    me->AI()->Talk(5);

                    Townhall_Done = true;
                }
            }

            if (spell->Id == SPELL_NOTIFY_KILL)
            {
                if (urand(0, 1) == 1)
                {
                    me->AI()->Talk(2);

                    me->CastSpell(caster, SPELL_KILL, true);
                }
                else
                    me->AI()->Talk(1);
            }

            if (spell->Id == SPELL_NOTIFY_DEMPSEY)
            {
                if (Creature* dempsey = me->FindNearestCreature(45977, 15.0f, true))
                {
                    me->SetFacingToObject(dempsey);
                    me->AddUnitState(UNIT_STATE_ROOT);
                    me->HandleEmoteCommand(25);

                    _events.CancelEvent(EVENT_CONVERSATION);
                }

                _events.ScheduleEvent(EVENT_CONVO_DEMPSEY, 0);
            }
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetEntry() == 45997 && me->GetDistance2d(who) <= 15.0f && !Lorna_Done)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                {
                    if (player->GetQuestStatus(QUEST_ON_HER_MAJESTY_SECRET_SERVICE) == QUEST_STATUS_INCOMPLETE)
                        _events.ScheduleEvent(EVENT_CONVO_LORNA, 0);
                }
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
                _events.ScheduleEvent(EVENT_CONVO_LORNA + 1, 0);
        }

        void JustEngagedWith(Unit* who) override
        {
            _events.ScheduleEvent(EVENT_FIREBALL, 0);
            _events.ScheduleEvent(EVENT_FIREBLAST, 3000);
            _events.ScheduleEvent(EVENT_FLAMESTRIKE, 6000);
            _events.ScheduleEvent(EVENT_BLASTWAVE, 8000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (!player->HasAura(85517))
                    me->DespawnOrUnsummon();
            }

            if (me->HasAura(78351) && !LornaFinal_Done)
            {
                LornaFinal_Done = true;
                _events.ScheduleEvent(EVENT_FINAL_LORNA, 0);
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONVERSATION:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        me->AI()->Talk(0, player);

                    _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 90000));
                    break;
                }

                case EVENT_FIREBALL:
                {
                    DoCastVictim(SPELL_FIREBALL, false);
                    _events.ScheduleEvent(EVENT_FIREBALL, 2000);
                    break;
                }

                case EVENT_FIREBLAST:
                {
                    DoCastVictim(SPELL_FIREBLAST, false);
                    _events.ScheduleEvent(EVENT_FIREBLAST, 6000);
                    break;
                }

                case EVENT_FLAMESTRIKE:
                {
                    DoCastVictim(SPELL_FLAMESTRIKE, false);
                    _events.ScheduleEvent(EVENT_FLAMESTRIKE, 7500);
                    break;
                }

                case EVENT_BLASTWAVE:
                {
                    DoCastVictim(SPELL_BLASTWAVE, false);
                    _events.ScheduleEvent(EVENT_BLASTWAVE, 8500);
                    break;
                }

                case EVENT_CONVO_DEMPSEY:
                {
                    me->AI()->Talk(6);

                    _events.ScheduleEvent(EVENT_CONVO_DEMPSEY + 1, 12000);
                    break;
                }

                case EVENT_CONVO_DEMPSEY + 1:
                {
                    me->AI()->Talk(7);

                    if (Creature* dempsey = me->FindNearestCreature(45977, 15.0f))
                    {
                        me->SetFacingToObject(dempsey);
                        me->ClearUnitState(UNIT_STATE_ROOT);

                        me->CastSpell(dempsey, SPELL_KILL, true);

                        _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 90000));
                    }
                    break;
                }

                case EVENT_CONVO_LORNA:
                {
                    me->GetMotionMaster()->Clear(true);

                    if (me->HasUnitState(UNIT_STATE_FOLLOW))
                        me->ClearUnitState(UNIT_STATE_FOLLOW);

                    me->GetMotionMaster()->MovePoint(1, -796.84f, 1346.82f, 33.7479f, false, 5.5f);

                    if (Creature* walden = me->FindNearestCreature(45879, 25.0f))
                    {
                        walden->GetMotionMaster()->Clear(true);

                        if (walden->HasUnitState(UNIT_STATE_FOLLOW))
                            walden->ClearUnitState(UNIT_STATE_FOLLOW);

                        walden->GetMotionMaster()->MovePoint(1, -794.12f, 1344.57f, 33.7292f, false, 5.5f);
                    }

                    if (Creature* ashbury = me->FindNearestCreature(45880, 25.0f))
                    {
                        ashbury->GetMotionMaster()->Clear(true);

                        if (ashbury->HasUnitState(UNIT_STATE_FOLLOW))
                            ashbury->ClearUnitState(UNIT_STATE_FOLLOW);

                        ashbury->GetMotionMaster()->MovePoint(1, -800.02f, 1349.56f, 33.8369f, false, 5.5f);
                    }

                    Lorna_Done = true;

                    _events.ScheduleEvent(EVENT_CONVO_LORNA + 2, 2000);
                    break;
                }

                case EVENT_CONVO_LORNA + 1:
                {
                    if (Creature* lorna = me->FindNearestCreature(45997, 25.0f))
                        me->SetFacingToObject(lorna);

                    me->AddUnitState(UNIT_STATE_ROOT);
                    break;
                }

                case EVENT_CONVO_LORNA + 2:
                {
                    me->AI()->Talk(8);

                    _events.ScheduleEvent(EVENT_CONVO_LORNA + 3, 13000);
                    break;
                }

                case EVENT_CONVO_LORNA + 3:
                {
                    me->AI()->Talk(9);

                    _events.ScheduleEvent(EVENT_CONVO_LORNA + 4, 4000);
                    break;
                }

                case EVENT_CONVO_LORNA + 4:
                {
                    me->AI()->Talk(10);

                    _events.ScheduleEvent(EVENT_CONVO_LORNA + 5, 6000);
                    break;
                }

                case EVENT_CONVO_LORNA + 5:
                {
                    me->AI()->Talk(11);
                    break;
                }

                case EVENT_FINAL_LORNA:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        Position pos;

                        pos = me->GetPosition();

                        me->AI()->Talk(12);

                        me->GetMotionMaster()->MoveFollow(player, 5.0f, M_PI / 2.0f, true, true, false);

                        if (Creature* walden = me->FindNearestCreature(45879, 50.0f))
                            walden->GetMotionMaster()->MoveFollow(player, 5.0f, M_PI, true, true, false);

                        if (Creature* ashbury = me->FindNearestCreature(45880, 50.0f))
                            ashbury->GetMotionMaster()->MoveFollow(player, 5.0f, M_PI * 1.5f, true, true, false);

                        _events.ScheduleEvent(EVENT_FINAL_LORNA + 1, 500);
                    }
                    break;
                }

                case EVENT_FINAL_LORNA + 1:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        LornaFinal2_Done = true;

                        Position pos;

                        pos = me->GetPosition();

                        me->CreateVehicleKit(1170, 45878);

                        me->SummonCreature(45997, pos, TEMPSUMMON_MANUAL_DESPAWN);
                    }
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_godfrey_45878AI(creature);
    }
};

// Lord Walden - 45879
class npc_walden_45879 : public CreatureScript
{
public:
    npc_walden_45879() : CreatureScript("npc_walden_45879") { }

    struct npc_walden_45879AI : public ScriptedAI
    {
        npc_walden_45879AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_INNER_FIRE = 48168,

            SPELL_HOLY_SMITE = 9734,
            SPELL_PENANCE = 54518,
            SPELL_HOLY_FIRE = 85563,
            SPELL_HOLY_NOVA = 85965,
            SPELL_POWER_WORD = 11974,
            SPELL_HEAL = 83849,
            SPELL_MENDING = 33076,

            EVENT_CONVERSATION = 1,
            EVENT_HOLY_FIRE = 2,
            EVENT_HOLY_SMITE = 3,
            EVENT_PENANCE = 4,
            EVENT_HOLY_NOVA = 5,
            EVENT_CHECK_PLAYER = 6,
            EVENT_HEAL_COOLDOWN = 7,
            EVENT_SHIELD_COOLDOWN = 8,
            EVENT_CONVO_LORNA = 100
        };

        ObjectGuid m_playerGUID;
        ObjectGuid m_targetGUID;

        bool m_healCD;
        bool m_shieldCD;

        void Reset() override
        {
            m_healCD = false;
            m_shieldCD = false;

            _events.Reset();

            m_playerGUID = ObjectGuid::Empty;
            m_targetGUID = ObjectGuid::Empty;

            m_playerGUID = me->GetOwnerGUID();

            me->SetReactState(REACT_ASSIST);

            if (!me->HasAura(SPELL_INNER_FIRE))
                me->CastSpell(me, SPELL_INNER_FIRE, false);

            _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
            _events.ScheduleEvent(EVENT_CONVERSATION, urand(45000, 90000));
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->GetGUID() != m_targetGUID)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                {
                    if (Unit* unit = player->GetVictim())
                    {
                        if (unit->GetGUID() != m_targetGUID)
                            JustEnteredCombat(unit);
                    }
                }
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
                _events.ScheduleEvent(EVENT_CONVO_LORNA + 1, 0);
        }

        void JustEngagedWith(Unit* who) override
        {
            _events.ScheduleEvent(EVENT_PENANCE, 1000);
            _events.ScheduleEvent(EVENT_HOLY_FIRE, 3500);
            _events.ScheduleEvent(EVENT_HOLY_SMITE, 5500);
            _events.ScheduleEvent(EVENT_HOLY_NOVA, 9000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (!player->HasAura(85518))
                    me->DespawnOrUnsummon();
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONVERSATION:
                {
                    me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 75000));
                    break;
                }

                case EVENT_CHECK_PLAYER:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (player->IsAlive() || player->IsInWorld())
                        {
                            if (player->GetHealthPct() <= 50.0f)
                            {
                                if (m_healCD)
                                    return;

                                me->CastSpell(player, SPELL_HEAL, true);

                                m_healCD = true;
                                _events.ScheduleEvent(EVENT_HEAL_COOLDOWN, 1000);
                            }

                            if (player->GetHealthPct() >= 50.0f)
                            {
                                if (me->IsInCombat() || (player->IsInCombat()))
                                {
                                    if (m_shieldCD)
                                        return;

                                    if (urand(0, 1) == 1)
                                        me->CastSpell(player, SPELL_POWER_WORD);
                                    else
                                        me->CastSpell(player, SPELL_MENDING);

                                    m_shieldCD = true;
                                    _events.ScheduleEvent(EVENT_SHIELD_COOLDOWN, 8000);
                                }
                            }
                        }
                    }
                    _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                    break;
                }

                case EVENT_HEAL_COOLDOWN:
                {
                    m_healCD = false;
                    break;
                }

                case EVENT_SHIELD_COOLDOWN:
                {
                    m_shieldCD = false;
                    break;
                }

                case EVENT_HOLY_FIRE:
                {
                    DoCastVictim(SPELL_HOLY_FIRE, false);
                    _events.ScheduleEvent(EVENT_HOLY_FIRE, 8500);
                    break;
                }

                case EVENT_HOLY_SMITE:
                {
                    DoCastVictim(SPELL_HOLY_SMITE, false);
                    _events.ScheduleEvent(EVENT_HOLY_SMITE, 2000);
                    break;
                }

                case EVENT_PENANCE:
                {
                    DoCastVictim(SPELL_PENANCE, false);
                    _events.ScheduleEvent(EVENT_PENANCE, 7500);
                    break;
                }

                case EVENT_HOLY_NOVA:
                {
                    DoCastVictim(SPELL_HOLY_NOVA, false);
                    _events.ScheduleEvent(EVENT_HOLY_NOVA, 8500);
                    break;
                }

                case EVENT_CONVO_LORNA + 1:
                {
                    if (Creature* lorna = me->FindNearestCreature(45997, 25.0f))
                        me->SetFacingToObject(lorna);

                    me->AddUnitState(UNIT_STATE_ROOT);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_walden_45879AI(creature);
    }
};

// Baron Ashbury - 45880
class npc_ashbury_45880 : public CreatureScript
{
public:
    npc_ashbury_45880() : CreatureScript("npc_ashbury_45880") { }

    struct npc_ashbury_45880AI : public ScriptedAI
    {
        npc_ashbury_45880AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_SHADOWFORM = 16592,

            SPELL_SHADOW_WORD = 72318,
            SPELL_MIND_BLAST = 13860,
            SPELL_MIND_FLAY = 78751,
            SPELL_PSYCHIC_SCREAM = 22884,
            SPELL_DISPERSION = 79811,

            EVENT_CONVERSATION = 1,
            EVENT_SHADOW_WORD = 2,
            EVENT_MIND_BLAST = 3,
            EVENT_MIND_FLAY = 4,
            EVENT_PSYCHIC_SCREAM = 5,
            EVENT_DISPERSION = 6,
            EVENT_CONVO_LORNA = 100
        };

        ObjectGuid m_playerGUID;
        ObjectGuid m_targetGUID;

        bool m_dispersionCD;

        void Reset() override
        {
            m_dispersionCD = false;

            _events.Reset();

            me->SetReactState(REACT_ASSIST);

            m_playerGUID = ObjectGuid::Empty;
            m_targetGUID = ObjectGuid::Empty;

            m_playerGUID = me->GetOwnerGUID();

            if (!me->HasAura(SPELL_SHADOWFORM))
                me->CastSpell(me, SPELL_SHADOWFORM, false);

            _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 90000));
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
                _events.ScheduleEvent(EVENT_CONVO_LORNA + 1, 0);
        }

        void JustEngagedWith(Unit* who) override
        {
            m_targetGUID = who->GetGUID();

            _events.ScheduleEvent(EVENT_MIND_BLAST, 1000);
            _events.ScheduleEvent(EVENT_MIND_FLAY, 3000);
            _events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, 9000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (!player->HasAura(85519))
                    me->DespawnOrUnsummon();
            }

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->HealthBelowPct(50.0f))
            {
                if (m_dispersionCD)
                    return;

                me->CastSpell(me, EVENT_DISPERSION, false);

                m_dispersionCD = true;
            }

            if (Creature* target = ObjectAccessor::GetCreature(*me, m_targetGUID))
            {
                if (!target->HasAura(SPELL_SHADOW_WORD))
                    _events.ScheduleEvent(EVENT_SHADOW_WORD, 0);
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONVERSATION:
                {
                    me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_CONVERSATION, urand(60000, 75000));
                    break;
                }

                case EVENT_SHADOW_WORD:
                {
                    DoCastVictim(SPELL_SHADOW_WORD, false);
                    break;
                }

                case EVENT_MIND_BLAST:
                {
                    DoCastVictim(SPELL_MIND_BLAST, false);
                    _events.ScheduleEvent(EVENT_MIND_BLAST, 7500);
                    break;
                }

                case EVENT_MIND_FLAY:
                {
                    DoCastVictim(SPELL_MIND_FLAY, false);
                    _events.ScheduleEvent(EVENT_MIND_FLAY, 2500);
                    break;
                }

                case EVENT_PSYCHIC_SCREAM:
                {
                    m_dispersionCD = false;
                    DoCastVictim(SPELL_PSYCHIC_SCREAM, false);
                    _events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, 12000);
                    break;
                }

                case EVENT_CONVO_LORNA + 1:
                {
                    if (Creature* lorna = me->FindNearestCreature(45997, 25.0f))
                        me->SetFacingToObject(lorna);

                    me->AddUnitState(UNIT_STATE_ROOT);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ashbury_45880AI(creature);
    }
};

// Kill Credit Pyrewood - 45937, 45938, 45939
class npc_killcredit_pyrewood : public CreatureScript
{
public:
    npc_killcredit_pyrewood() : CreatureScript("npc_killcredit_pyrewood") { }

    struct npc_killcredit_pyrewoodAI : public ScriptedAI
    {
        npc_killcredit_pyrewoodAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            NPC_INN = 45937,
            NPC_CHAPEL = 45938,
            NPC_TOWNHALL = 45939,

            SPELL_FIRE = 52855,

            EVENT_SCENE = 1,
        };

        ObjectGuid  m_playerGUID;

        void Reset()
        {
            _events.Reset();
            m_playerGUID = ObjectGuid::Empty;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            _events.ScheduleEvent(EVENT_SCENE + 1, 0);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == 85600)
            {
                m_playerGUID = caster->GetGUID();
                _events.ScheduleEvent(EVENT_SCENE, 0);
                _events.ScheduleEvent(EVENT_SCENE + 1, 0);

                if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                {
                    if (Creature* godfrey = player->FindNearestCreature(45878, 10.0f, true))
                        me->CastSpell(godfrey, 83990, true);
                }
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
            case 1:
            {
                me->CastSpell(me, SPELL_FIRE, true);
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SCENE:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (me->GetEntry() == NPC_INN)
                            player->KilledMonsterCredit(NPC_INN);
                        else if (me->GetEntry() == NPC_CHAPEL)
                            player->KilledMonsterCredit(NPC_CHAPEL);
                        else if (me->GetEntry() == NPC_TOWNHALL)
                            player->KilledMonsterCredit(NPC_TOWNHALL);
                    }
                    break;
                }

                case EVENT_SCENE + 1:
                {
                    if (me->GetEntry() == NPC_INN)
                    {
                        std::list<Creature*> inns;
                        GetCreatureListWithEntryInGrid(inns, me, NPC_INN, 50.0f);

                        for (std::list<Creature*>::const_iterator itr = inns.begin(); itr != inns.end(); ++itr)
                            (*itr)->AI()->DoAction(1);
                    }

                    if (me->GetEntry() == NPC_CHAPEL)
                    {
                        std::list<Creature*> chapels;
                        GetCreatureListWithEntryInGrid(chapels, me, NPC_CHAPEL, 50.0f);

                        for (std::list<Creature*>::const_iterator itr = chapels.begin(); itr != chapels.end(); ++itr)
                            (*itr)->AI()->DoAction(1);
                    }

                    if (me->GetEntry() == NPC_TOWNHALL)
                    {
                        std::list<Creature*> townhalls;
                        GetCreatureListWithEntryInGrid(townhalls, me, NPC_TOWNHALL, 50.0f);

                        for (std::list<Creature*>::const_iterator itr = townhalls.begin(); itr != townhalls.end(); ++itr)
                            (*itr)->AI()->DoAction(1);
                    }
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_killcredit_pyrewoodAI(creature);
    }
};

// Toss Torch - 85600
class spell_toss_torch_85600 : public SpellScriptLoader
{
public:
    spell_toss_torch_85600() : SpellScriptLoader("spell_toss_torch_85600") { }

    enum eSpell
    {
        NPC_INN = 45937,
        NPC_CHAPEL = 45938,
        NPC_TOWNHALL = 45939,
    };

    class IsNotInEntryList
    {
    public:
        explicit IsNotInEntryList(std::list<uint32>entrys) : _entrys(entrys) { }

        bool operator()(WorldObject* obj) const
        {
            if (Creature* target = obj->ToCreature())
                for (std::list<uint32>::const_iterator itr = _entrys.begin(); itr != _entrys.end(); ++itr)
                    if (target->GetEntry() == *itr)
                        return false;

            return true;
        }

    private:
        std::list<uint32> _entrys;
    };

    class spell_toss_torch_85600_SpellScript : public SpellScript
    {
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<uint32>entrys;
            entrys.push_back(NPC_TOWNHALL);
            entrys.push_back(NPC_INN);
            entrys.push_back(NPC_CHAPEL);
            targets.remove_if(IsNotInEntryList(entrys));
        }

        void Register()
        {
            OnObjectAreaTargetSelect.Register(&spell_toss_torch_85600_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_toss_torch_85600_SpellScript();
    }
};

// 7th Legion Scout - 45883
class npc_legion_scout_45883 : public CreatureScript
{
public:
    npc_legion_scout_45883() : CreatureScript("npc_legion_scout_45883") { }

    struct npc_legion_scout_45883AI : public ScriptedAI
    {
        npc_legion_scout_45883AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_STEALTH = 84442,

            SPELL_HEMORRHAGE = 79863,
            SPELL_BACKSTAB = 79862,
            SPELL_FAN_OF_KNIVES = 79584,
            SPELL_EVISCERATE = 79852,
            SPELL_SHADOWSTEP = 80576,
            SPELL_EVASION = 79853,

            EVENT_HEMORRHAGE = 1,
            EVENT_FAN_OF_KNIVES = 2,
            EVENT_BACKSTAB = 3,
            EVENT_EVISCERATE = 4,
            EVENT_SHADOWSTEP = 5,
            EVENT_EVASION = 6
        };

        ObjectGuid m_targetGUID;

        bool m_backstabCD;
        bool m_evasionCD;

        void Reset() override
        {
            m_backstabCD = false;
            m_evasionCD = false;

            me->SetPowerType(Powers::POWER_ENERGY);
            me->SetMaxPower(Powers::POWER_ENERGY, 100);

            _events.Reset();

            m_targetGUID = ObjectGuid::Empty;

            if (!me->HasAura(SPELL_STEALTH))
                me->CastSpell(me, SPELL_STEALTH, false);
        }

        void JustEngagedWith(Unit* who) override
        {
            m_targetGUID = who->GetGUID();

            if (urand(0, 2) == 2)
                _events.ScheduleEvent(EVENT_SHADOWSTEP, 0);

            _events.ScheduleEvent(EVENT_HEMORRHAGE, 2000);
            _events.ScheduleEvent(EVENT_FAN_OF_KNIVES, 5000);
            _events.ScheduleEvent(EVENT_EVISCERATE, 7000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HealthBelowPct(50.0f))
            {
                if (urand(0, 3) == 3)
                    _events.ScheduleEvent(EVENT_EVASION, 0);
            }

            if (Creature* target = ObjectAccessor::GetCreature(*me, m_targetGUID))
            {
                if (target->IsAlive() && me->isInBack(target))
                    _events.ScheduleEvent(EVENT_BACKSTAB, 0);

                if (me->IsWithinMeleeRange(target))
                    me->RemoveAura(SPELL_STEALTH);
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_HEMORRHAGE:
                {
                    DoCastVictim(SPELL_HEMORRHAGE, false);
                    _events.ScheduleEvent(EVENT_HEMORRHAGE, 6000);
                    break;
                }

                case EVENT_BACKSTAB:
                {
                    if (m_backstabCD)
                        return;

                    DoCastVictim(SPELL_BACKSTAB, false);

                    _events.ScheduleEvent(EVENT_BACKSTAB, 5400);

                    m_backstabCD = true;
                    break;
                }

                case EVENT_FAN_OF_KNIVES:
                {
                    m_backstabCD = false;
                    DoCastVictim(SPELL_FAN_OF_KNIVES, false);
                    _events.ScheduleEvent(EVENT_FAN_OF_KNIVES, 8500);
                    break;
                }

                case EVENT_EVISCERATE:
                {
                    m_evasionCD = false;
                    DoCastVictim(SPELL_EVISCERATE, false);
                    _events.ScheduleEvent(EVENT_EVISCERATE, urand(8000, 12000));
                    break;
                }

                case EVENT_SHADOWSTEP:
                {
                    if (Creature* target = ObjectAccessor::GetCreature(*me, m_targetGUID))
                    {
                        me->CastSpell(target, SPELL_SHADOWSTEP, false);
                        _events.ScheduleEvent(EVENT_BACKSTAB, 500);
                    }
                    break;
                }

                case EVENT_EVASION:
                {
                    if (m_evasionCD)
                        return;

                    me->CastSpell(me, SPELL_EVASION, false);
                    m_evasionCD = true;
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_legion_scout_45883AI(creature);
    }
};

// Cowering Trooper - 45910
class npc_cowering_trooper_45910 : public CreatureScript
{
public:
    npc_cowering_trooper_45910() : CreatureScript("npc_cowering_trooper_45910") { }

    struct npc_cowering_trooper_45910AI : public ScriptedAI
    {
        npc_cowering_trooper_45910AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_FLEE = 1
        };

        ObjectGuid  m_playerGUID;

        void Reset()
        {
            if (urand(0, 1) == 1)
                me->CastSpell(me, 78087, true);
            else
                me->CastSpell(me, 91166, true);

            m_playerGUID = ObjectGuid::Empty;

            _events.Reset();
        }

        void OnSpellClick(Unit* clicker, bool& /*result*/) override
        {
            m_playerGUID = clicker->GetGUID();

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (player->GetQuestStatus(27547) == QUEST_STATUS_INCOMPLETE)
                {
                    if (me->HasAura(78087))
                        me->RemoveAura(78087);
                    else
                        me->RemoveAura(91166);

                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

                    me->AI()->Talk(0);

                    player->KilledMonsterCredit(45910);

                    _events.ScheduleEvent(EVENT_FLEE, 2000);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FLEE:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        if (Creature* godfrey = player->FindNearestCreature(45878, 10.0f, true))
                            me->CastSpell(godfrey, 84145, true);

                        me->GetMotionMaster()->MoveFleeing(player, 2500);
                    }

                    me->DespawnOrUnsummon(2500);
                    break;
                }
                }
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_cowering_trooper_45910AI(creature);
    }
};

// Sean Dempsey - 45977
class npc_dempsey_45977 : public CreatureScript
{
public:
    npc_dempsey_45977() : CreatureScript("npc_dempsey_45977") { }

    struct npc_dempsey_45977AI : public ScriptedAI
    {
        npc_dempsey_45977AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_KNEEL = 68442,
            SPELL_NOTIFY_DEMPSEY = 84195,

            EVENT_CONVO_DEMPSEY = 1
        };

        bool p_Done;

        EventMap _events;
        ObjectGuid m_playerGUID;

        void Reset() override
        {
            p_Done = false;

            m_playerGUID = me->GetOwnerGUID();

            _events.Reset();

            _events.ScheduleEvent(EVENT_CONVO_DEMPSEY, 1000);

            me->AddAura(SPELL_KNEEL, me);

            if (Creature* godfrey = me->FindNearestCreature(45878, 15.0f, true))
                me->SetFacingToObject(godfrey);

            me->AddUnitState(UNIT_STATE_ROOT);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONVO_DEMPSEY:
                {
                    me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_CONVO_DEMPSEY + 1, 4000);
                    break;
                }

                case EVENT_CONVO_DEMPSEY + 1:
                {
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_CONVO_DEMPSEY + 2, 3000);
                    break;
                }

                case EVENT_CONVO_DEMPSEY + 2:
                {
                    if (Creature* godfrey = me->FindNearestCreature(45878, 15.0f, true))
                        me->CastSpell(godfrey, SPELL_NOTIFY_DEMPSEY, true);

                    _events.ScheduleEvent(EVENT_CONVO_DEMPSEY + 3, 8000);
                    break;
                }

                case EVENT_CONVO_DEMPSEY + 3:
                {
                    me->AI()->Talk(2);
                    me->DespawnOrUnsummon(10000);
                    break;
                }
                }
            }

            UpdateVictim();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dempsey_45977AI(creature);
    }
};

// Commander Lorna Crowley - 45997
class npc_lorna_45997 : public CreatureScript
{
public:
    npc_lorna_45997() : CreatureScript("npc_lorna_45997") { }

    struct npc_lorna_45997AI : public ScriptedAI
    {
        npc_lorna_45997AI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            SPELL_COMMANDING_SHOUT = 80983,
            SPELL_SUNDERING_CLEAVE = 85240,
            SPELL_THUNDERCLAP = 81140,
            SPELL_CHARGE = 22120,
            SPELL_NOTIFY_GODFREY_2 = 79164,

            EVENT_SUNDERING_CLEAVE = 1,
            EVENT_THUNDERCLAP = 2,
            EVENT_CHARGE = 3,
            EVENT_CHECK_PLAYER = 4,
            EVENT_START_FIGHT = 5,
            EVENT_FINAL = 6,
            EVENT_CONVERSATION = 7,

            QUEST_ON_HER_MAJESTY_SECRET_SERVICE = 27594
        };

        ObjectGuid m_playerGUID;

        bool m_Done;
        bool m_FinalDone;

        void Reset() override
        {
            _events.Reset();

            m_Done = false;
            m_FinalDone = false;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

            m_playerGUID = ObjectGuid::Empty;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetEntry() == 45878)
                me->EnterVehicle(summoner);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TypeID::TYPEID_PLAYER && me->GetDistance2d(who) <= 15.0f && !m_Done)
            {
                m_playerGUID == who->GetGUID();

                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_ON_HER_MAJESTY_SECRET_SERVICE) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (Creature* godfrey = me->FindNearestCreature(45878, 25.0f))
                        {
                            me->SetFacingToObject(godfrey);
                            _events.ScheduleEvent(EVENT_CONVERSATION, 0);
                            m_Done = true;
                        }
                    }
                }
            }
        }

        void JustEngagedWith(Unit* who) override
        {
            _events.ScheduleEvent(EVENT_SUNDERING_CLEAVE, 2000);
            _events.ScheduleEvent(EVENT_THUNDERCLAP, 4000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->GetHealthPct() <= 10.0f && !m_FinalDone)
                _events.ScheduleEvent(EVENT_FINAL, 0);

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (me->IsInCombat() && me->GetDistance2d(player) >= 5.0f)
                    me->CastSpell(player, SPELL_CHARGE, false);
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SUNDERING_CLEAVE:
                {
                    DoCastVictim(SPELL_SUNDERING_CLEAVE, false);
                    _events.ScheduleEvent(EVENT_SUNDERING_CLEAVE, 4000);
                    break;
                }

                case EVENT_THUNDERCLAP:
                {
                    DoCastVictim(SPELL_THUNDERCLAP, false);
                    _events.ScheduleEvent(EVENT_THUNDERCLAP, 7500);
                    break;
                }

                case EVENT_CONVERSATION:
                {
                    me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_CONVERSATION + 1, 7000);
                    break;
                }

                case EVENT_CONVERSATION + 1:
                {
                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_CONVERSATION + 2, 5000);
                    break;
                }

                case EVENT_CONVERSATION + 2:
                {
                    me->AI()->Talk(2);

                    _events.ScheduleEvent(EVENT_CONVERSATION + 3, 16000);
                    break;
                }

                case EVENT_CONVERSATION + 3:
                {
                    me->AI()->Talk(3);

                    me->CastSpell(me, SPELL_COMMANDING_SHOUT, false);

                    _events.ScheduleEvent(EVENT_START_FIGHT, 1000);
                    break;
                }

                case EVENT_START_FIGHT:
                {
                    Position pos;

                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                    me->SetReactState(REACT_AGGRESSIVE);

                    if (Creature* walden = me->FindNearestCreature(45879, 25.0f))
                    {
                        walden->ClearUnitState(UNIT_STATE_ROOT);
                        walden->Attack(me, false);
                    }

                    if (Creature* ashbury = me->FindNearestCreature(45880, 25.0f))
                    {
                        ashbury->ClearUnitState(UNIT_STATE_ROOT);
                        ashbury->Attack(me, false);
                    }

                    if (Creature* godfrey = me->FindNearestCreature(45878, 25.0f))
                    {
                        pos = godfrey->GetPosition();

                        me->Attack(godfrey, true);
                        me->CastSpell(godfrey, SPELL_CHARGE, true);

                        godfrey->ClearUnitState(UNIT_STATE_ROOT);
                        godfrey->Attack(me, false);
                    }

                    me->SetInCombatWithZone();

                    break;
                }

                case EVENT_FINAL:
                {
                    m_FinalDone = true;

                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                    me->SetHealth(10.0f);

                    me->AddUnitState(UNIT_STATE_ROOT);
                    me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);

                    me->AI()->Talk(4);

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 50.0f);

                    for (Player* player : playerList)
                    {
                        if (player->GetQuestStatus(QUEST_ON_HER_MAJESTY_SECRET_SERVICE) == QUEST_STATUS_INCOMPLETE)
                        {
                            player->KilledMonsterCredit(46002);

                            if (Creature* godfrey = me->FindNearestCreature(45878, 50.0f))
                            {
                                me->AddAura(78351, godfrey);
                                me->DespawnOrUnsummon(2500);
                            }
                        }
                    }
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lorna_45997AI(creature);
    }
};

// Lady Sylvanas Windrunner (Final quest) - 46026
class npc_lady_sylvanas_windrunner_46026 : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner_46026() : CreatureScript("npc_lady_sylvanas_windrunner_46026") { }

    enum eNPC
    {
        EVENT_SONG = 1,
        EVENT_START_EVENT = 2,

        SPELL_SUMMON_SYLVANAS = 85864,
        SPELL_SUMMON_CROWLEY = 85877,
        SPELL_SUMMON_BLOODFANG = 85878,
        SPELL_SUMMON_GODFREY = 85925,
        SPELL_SUMMON_WALDEN = 85926,
        SPELL_SUMMON_ASHBURY = 85927,
        SPELL_SYLVANAS_SONG = 85874,
        SPELL_NOTIFY_ALL = 83990,
        SPELL_NOTIFY_ALL2 = 84145,
        SPELL_NOTIFY_DEMPSEY = 84195,

        SOUND_SYLVANAS_SONG = 10896,

        QUEST_CITIES_IN_DUST = 27601,
    };

    struct npc_lady_sylvanas_windrunner_46026AI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunner_46026AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap _events;
        ObjectGuid  m_playerGUID;

        void Reset() override
        {
            _events.Reset();

            m_playerGUID = ObjectGuid::Empty;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);

            me->CastSpell(me, SPELL_SYLVANAS_SONG, false);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            m_playerGUID = summoner->GetGUID();

            me->SetCharm(me, true);

            _events.ScheduleEvent(EVENT_SONG, 1500);
            _events.ScheduleEvent(EVENT_START_EVENT, 2000);
        }

        void WaypointReached(uint32 waypointId, uint32 pathId) override
        {
            if (pathId == 460260)
            {
                if (waypointId == 8)
                    _events.ScheduleEvent(EVENT_START_EVENT + 2, 2000);
            }

            if (pathId == 460261)
            {
                if (waypointId == 11)
                    _events.ScheduleEvent(EVENT_START_EVENT + 7, 2000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (!player->HasAura(SPELL_SUMMON_SYLVANAS))
                    me->DespawnOrUnsummon();
            }

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SONG:
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                        DoPlaySoundToSet(player, SOUND_SYLVANAS_SONG);

                    _events.ScheduleEvent(EVENT_SONG, 168000);
                    break;
                }

                case EVENT_START_EVENT:
                {
                    me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_START_EVENT + 1, 3000);
                    break;
                }

                case EVENT_START_EVENT + 1:
                {
                    me->ClearUnitState(UNIT_STATE_ROOT);

                    me->AI()->Talk(21);

                    me->GetMotionMaster()->MovePath(460260, false);
                    break;
                }

                case EVENT_START_EVENT + 2:
                {
                    me->AddUnitState(UNIT_STATE_ROOT);

                    me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_START_EVENT + 3, 5500);
                    break;
                }

                case EVENT_START_EVENT + 3:
                {
                    me->AI()->Talk(2);

                    _events.ScheduleEvent(EVENT_START_EVENT + 4, 5500);
                    break;
                }

                case EVENT_START_EVENT + 4:
                {
                    me->AI()->Talk(3);

                    _events.ScheduleEvent(EVENT_START_EVENT + 5, 9000);
                    break;
                }

                case EVENT_START_EVENT + 5:
                {
                    me->AI()->Talk(4);

                    _events.ScheduleEvent(EVENT_START_EVENT + 6, 4000);
                    break;
                }

                case EVENT_START_EVENT + 6:
                {
                    me->ClearUnitState(UNIT_STATE_ROOT);

                    me->GetMotionMaster()->MovePath(460261, false);
                    break;
                }

                case EVENT_START_EVENT + 7:
                {
                    me->AddUnitState(UNIT_STATE_ROOT);

                    me->SetFacingTo(3.058213f);

                    me->AI()->Talk(5);

                    if (Creature* agatha = me->FindNearestCreature(46034, 200.0f, true))
                        agatha->AI()->DoAction(1);

                    if (Creature* arthura = me->FindNearestCreature(46032, 200.0f, true))
                        arthura->AI()->DoAction(1);

                    if (Creature* daschla = me->FindNearestCreature(46033, 200.0f, true))
                        daschla->AI()->DoAction(1);

                    if (Creature* cromush = me->FindNearestCreature(46031, 200.0f, true))
                        cromush->AI()->DoAction(1);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->CastSpell(player, SPELL_SUMMON_CROWLEY, false);
                        player->CastSpell(player, SPELL_SUMMON_BLOODFANG, false);
                    }

                    _events.ScheduleEvent(EVENT_START_EVENT + 8, 20000);
                    break;
                }

                case EVENT_START_EVENT + 8:
                {
                    me->AI()->Talk(6);

                    _events.ScheduleEvent(EVENT_START_EVENT + 9, 12000);
                    break;
                }

                case EVENT_START_EVENT + 9:
                {
                    me->AI()->Talk(7);

                    _events.ScheduleEvent(EVENT_START_EVENT + 10, 18000);
                    break;
                }

                case EVENT_START_EVENT + 10:
                {
                    me->AI()->Talk(8);

                    _events.ScheduleEvent(EVENT_START_EVENT + 11, 20000);
                    break;
                }

                case EVENT_START_EVENT + 11:
                {
                    me->AI()->Talk(9);

                    _events.ScheduleEvent(EVENT_START_EVENT + 12, 4000);
                    break;
                }

                case EVENT_START_EVENT + 12:
                {
                    me->AI()->Talk(10);

                    _events.ScheduleEvent(EVENT_START_EVENT + 13, 10000);
                    break;
                }

                case EVENT_START_EVENT + 13:
                {
                    me->AI()->Talk(11);

                    _events.ScheduleEvent(EVENT_START_EVENT + 14, 10000);
                    break;
                }

                case EVENT_START_EVENT + 14:
                {
                    me->AI()->Talk(12);

                    _events.ScheduleEvent(EVENT_START_EVENT + 15, 10000);
                    break;
                }

                case EVENT_START_EVENT + 15:
                {
                    me->AI()->Talk(13);

                    if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
                    {
                        player->CastSpell(player, SPELL_SUMMON_GODFREY, false);
                        player->CastSpell(player, SPELL_SUMMON_WALDEN, false);
                        player->CastSpell(player, SPELL_SUMMON_ASHBURY, false);
                    }

                    _events.ScheduleEvent(EVENT_START_EVENT + 16, 18000);
                    break;
                }

                case EVENT_START_EVENT + 16:
                {
                    me->AI()->Talk(14);

                    _events.ScheduleEvent(EVENT_START_EVENT + 17, 4000);
                    break;
                }

                case EVENT_START_EVENT + 17:
                {
                    me->AI()->Talk(15);

                    _events.ScheduleEvent(EVENT_START_EVENT + 18, 8000);
                    break;
                }

                case EVENT_START_EVENT + 18:
                {
                    me->AI()->Talk(16);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lady_sylvanas_windrunner_46026AI(creature);
    }
};

// Agatha, Arthura, Daschla, Cromush - 46034, 46032, 46033, 46031
class npc_agardacro_46034 : public CreatureScript
{
public:
    npc_agardacro_46034() : CreatureScript("npc_agardacro_46034") { }

    enum eSpell
    {
        EVENT_START_EVENT = 1,

        SPELL_SUMMON_AGATHA = 85870,
        SPELL_SUMMON_ARTHURA = 85869,
        SPELL_SUMMON_DASCHLA = 85871,
        SPELL_SUMMON_CROMUSH = 85868,
    };

    struct npc_agardacro_46034AI : public ScriptedAI
    {
        npc_agardacro_46034AI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;

        ObjectGuid  m_playerGUID;

        void Reset() override
        {
            m_events.Reset();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            m_playerGUID = summoner->GetGUID();

            if (me->GetEntry() == 46031)
                me->Mount(14575);
            else
            {
                me->SetAnimationTier(AnimationTier::Hover);
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
            {
                me->SetCharm(me, true);

                me->AddUnitState(UNIT_STATE_ROOT);

                me->SetFacingTo(3.113983f);

                if (me->GetEntry() == 46031)
                    me->Dismount();
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
            case EVENT_START_EVENT:
            {
                if (!me->GetEntry() == 46031)
                {
                    me->SetFloatValue(UNIT_FIELD_HOVERHEIGHT, 0.0f);
                    me->SetHover(false, false, true);
                    me->SetDisableGravity(true, false, true);
                    me->SetCanFly(true, false);
                }
                else
                    me->HandleEmoteCommand(375);

                if (me->GetEntry() == 46034)
                    me->GetMotionMaster()->MovePoint(1, -703.60f, 1547.73f, 35.4921f, false, 10.0f);
                else if (me->GetEntry() == 46032)
                    me->GetMotionMaster()->MovePoint(1, -703.45f, 1552.40f, 32.2814f, false, 10.0f);
                else if (me->GetEntry() == 46033)
                    me->GetMotionMaster()->MovePoint(1, -704.10f, 1543.33f, 32.2814f, false, 10.0f);
                else if (me->GetEntry() == 46031)
                    me->GetMotionMaster()->MovePoint(1, -699.37f, 1550.38f, 27.9772f, false, 10.0f);

                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            if (Player* player = ObjectAccessor::GetPlayer(*me, m_playerGUID))
            {
                if (me->GetEntry() == 46034 && !player->HasAura(SPELL_SUMMON_AGATHA))
                    me->DespawnOrUnsummon();
                else if (me->GetEntry() == 46032 && !player->HasAura(SPELL_SUMMON_ARTHURA))
                    me->DespawnOrUnsummon();
                else if (me->GetEntry() == 46033 && !player->HasAura(SPELL_SUMMON_DASCHLA))
                    me->DespawnOrUnsummon();
                else if (me->GetEntry() == 46031 && !player->HasAura(SPELL_SUMMON_CROMUSH))
                    me->DespawnOrUnsummon();
            }

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {

                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_agardacro_46034AI(creature);
    }
};

// Lord Darius Crowley, Bloodfang - 46036, 46035
class npc_battle_crowblood : public CreatureScript
{
public:
    npc_battle_crowblood() : CreatureScript("npc_battle_crowblood") { }

    enum eSpell
    {
        EVENT_START_EVENT = 1,
    };

    struct npc_battle_crowbloodAI : public ScriptedAI
    {
        npc_battle_crowbloodAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap _events;

        void Reset() override
        {
            _events.Reset();

            _events.ScheduleEvent(EVENT_START_EVENT, 0);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
                _events.ScheduleEvent(EVENT_START_EVENT + 1, 1000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_START_EVENT:
                {
                    if (me->GetEntry() == 46035)
                        me->GetMotionMaster()->MovePoint(1, -716.16f, 1545.94f, 28.0112f, false, 3.0f);

                    if (me->GetEntry() == 46036)
                        me->GetMotionMaster()->MovePoint(1, -715.85f, 1550.13f, 28.0112f, false, 3.0f);
                    break;
                }

                case EVENT_START_EVENT + 1:
                {
                    if (me->GetEntry() == 46036)
                        me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_START_EVENT + 2, 28000);
                    break;
                }

                case EVENT_START_EVENT + 2:
                {
                    if (me->GetEntry() == 46036)
                        me->AI()->Talk(1);

                    _events.ScheduleEvent(EVENT_START_EVENT + 3, 14000);
                    break;
                }

                case EVENT_START_EVENT + 3:
                {
                    if (me->GetEntry() == 46036)
                    {
                        me->AI()->Talk(2);

                        if (Creature* godfrey = me->FindNearestCreature(46028, 50.0f, true))
                            me->SetFacingToObject(godfrey);
                    }

                    _events.ScheduleEvent(EVENT_START_EVENT + 4, 30000);
                    break;
                }

                case EVENT_START_EVENT + 4:
                {
                    if (me->GetEntry() == 46036)
                    {
                        me->AI()->Talk(3);

                        if (Creature* sylvanas = me->FindNearestCreature(46026, 50.0f, true))
                            me->SetFacingToObject(sylvanas);
                    }
                    else
                        me->AI()->Talk(0);

                    _events.ScheduleEvent(EVENT_START_EVENT + 5, 3000);
                    _events.ScheduleEvent(EVENT_START_EVENT + 6, 15000);
                    break;
                }

                case EVENT_START_EVENT + 5:
                {
                    if (!me->GetEntry() == 46036)
                        me->GetMotionMaster()->MovePoint(2, -784.94f, 1549.20f, 28.0107f, false, 12.0f);
                    break;
                }

                case EVENT_START_EVENT + 6:
                {
                    if (me->GetEntry() == 46036)
                    {
                        me->AI()->Talk(4);

                        if (Creature* lorna = me->FindNearestCreature(46027, 50.0f, true))
                            me->SetFacingToObject(lorna);
                    }

                    _events.ScheduleEvent(EVENT_START_EVENT + 5, 15000);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_crowbloodAI(creature);
    }
};

// Admiral Hatchet, Warlord Torok, Orc Sea Dogs - 46055, 46056, 46057
class npc_battle_sea_orcs : public CreatureScript
{
public:
    npc_battle_sea_orcs() : CreatureScript("npc_battle_sea_orcs") { }

    struct npc_battle_sea_orcsAI : public ScriptedAI
    {
        npc_battle_sea_orcsAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_SINISTER_STRIKE = 1,
            EVENT_SPRINT = 2,
            EVENT_BLOODTHIRST = 3,
            EVENT_REND = 4,
            EVENT_SLAM = 5,
            EVENT_BATTLE_SHOUT = 6,
            EVENT_SHATTERING_THROW = 7,
            EVENT_CHECK_SHOWFIGHT = 8,
            MOVE_TO_HOMEPOSITION = 9,

            SPELL_SINISTER_STRIKE = 60195,
            SPELL_SPRINT = 66060,

            SPELL_BLOODTHIRST = 79878,
            SPELL_REND = 11977,
            SPELL_SLAM = 79881,
            SPELL_BATTLE_SHOUT = 32064,
            SPELL_SHATTERING_THROW = 79883,
        };

        void Reset() override
        {
            _events.Reset();

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            me->SetAttackTime(BASE_ATTACK, 1500 + urand(0, 850));

            if (me->GetEntry() == 46057)
            {
                me->SetPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
            }
            else
            {
                me->SetPowerType(Powers::POWER_RAGE);
                me->SetMaxPower(Powers::POWER_RAGE, 1000);

                if (!me->HasAura(SPELL_BATTLE_SHOUT))
                    me->CastSpell(me, SPELL_BATTLE_SHOUT, false);
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* who) override
        {
            if (me->GetEntry() == 46057)
            {
                _events.ScheduleEvent(EVENT_SINISTER_STRIKE, urand(3500, 6500));
                _events.ScheduleEvent(EVENT_SPRINT, urand(14000, 18000));
            }
            else
            {
                _events.ScheduleEvent(EVENT_BLOODTHIRST, urand(3500, 6500));
                _events.ScheduleEvent(EVENT_REND, urand(14000, 18000));
                _events.ScheduleEvent(EVENT_SLAM, urand(3500, 6500));
                _events.ScheduleEvent(SPELL_SHATTERING_THROW, urand(14000, 18000));
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen1 = me->FindNearestCreature(46050, 5.0f))
                    {
                        me->CastSpell(worgen1, 355, true);
                        me->Attack(worgen1, true);
                    }

                    if (Creature* worgen2 = me->FindNearestCreature(46060, 5.0f))
                    {
                        me->CastSpell(worgen2, 355, true);
                        me->Attack(worgen2, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SINISTER_STRIKE:
                {
                    DoCastVictim(SPELL_SINISTER_STRIKE);
                    _events.ScheduleEvent(EVENT_SINISTER_STRIKE, urand(3500, 6500));
                    break;
                }

                case EVENT_SPRINT:
                {
                    me->CastSpell(me, SPELL_SPRINT, false);
                    _events.ScheduleEvent(EVENT_SPRINT, urand(14000, 18000));
                    break;
                }

                case EVENT_BLOODTHIRST:
                {
                    DoCastVictim(SPELL_BLOODTHIRST);
                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);
                    _events.ScheduleEvent(EVENT_BLOODTHIRST, urand(3500, 6500));
                    break;
                }

                case EVENT_REND:
                {
                    DoCastVictim(SPELL_REND);
                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);
                    _events.ScheduleEvent(EVENT_REND, urand(8500, 12500));
                    break;
                }

                case EVENT_SLAM:
                {
                    DoCastVictim(SPELL_SLAM);
                    me->ModifyPowerPct(POWER_RAGE, -30.0f, true);
                    _events.ScheduleEvent(EVENT_SLAM, urand(5500, 8500));
                    break;
                }

                case EVENT_SHATTERING_THROW:
                {
                    DoCastVictim(SPELL_SHATTERING_THROW);
                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);
                    _events.ScheduleEvent(EVENT_SHATTERING_THROW, urand(3500, 6500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_sea_orcsAI(creature);
    }
};

// Forsaken Troopers - 46053, 46054
class npc_battle_forsaken_troopers : public CreatureScript
{
public:
    npc_battle_forsaken_troopers() : CreatureScript("npc_battle_forsaken_troopers") { }

    struct npc_battle_forsaken_troopersAI : public ScriptedAI
    {
        npc_battle_forsaken_troopersAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_CHECK_SHOWFIGHT = 1,
            EVENT_SPELL_DAMAGE = 2,
            MOVE_TO_HOMEPOSITION = 3,
        };

        uint8 randomSpellIndex;

        void Reset() override
        {
            _events.Reset();

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            me->SetAttackTime(BASE_ATTACK, 1500 + urand(0, 850));

            me->SetPowerType(Powers::POWER_RAGE);
            me->SetMaxPower(Powers::POWER_RAGE, 1000);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen1 = me->FindNearestCreature(46063, 5.0f))
                    {
                        me->CastSpell(worgen1, 355, true);
                        me->Attack(worgen1, true);
                    }

                    if (Creature* worgen2 = me->FindNearestCreature(46064, 5.0f))
                    {
                        me->CastSpell(worgen2, 355, true);
                        me->Attack(worgen2, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_SPELL_DAMAGE:
                {
                    uint32 spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];

                    if (!spellId)
                    {
                        randomSpellIndex = 0;
                        spellId = me->GetCreatureTemplate()->spells[randomSpellIndex];
                    }

                    if (!spellId)
                        break;

                    if (++randomSpellIndex > 8)
                        randomSpellIndex = 0;

                    DoCastVictim(spellId);

                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);

                    _events.ScheduleEvent(EVENT_SPELL_DAMAGE, urand(4000, 8500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_forsaken_troopersAI(creature);
    }
};

// Archmage Ataeric, Forsaken Mage - 46049, 46048
class npc_battle_forsaken_mages : public CreatureScript
{
public:
    npc_battle_forsaken_mages() : CreatureScript("npc_battle_forsaken_mages") { }

    struct npc_battle_forsaken_magesAI : public ScriptedAI
    {
        npc_battle_forsaken_magesAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_FIREBALL = 1,
            EVENT_FIREBLAST = 2,
            EVENT_FLAMESTRIKE = 3,
            EVENT_BLASTWAVE = 4,

            EVENT_FROSTBOLT = 5,
            EVENT_ICE_LANCE = 6,
            EVENT_BLIZZARD = 7,
            EVENT_FROST_NOVA = 8,

            EVENT_CHECK_SHOWFIGHT = 9,
            MOVE_TO_HOMEPOSITION = 10,

            SPELL_MOLTEN_CORE = 79849,
            SPELL_FIREBALL = 79854,
            SPELL_FIREBLAST = 79855,
            SPELL_FLAMESTRIKE = 79846,
            SPELL_BLAST_WAVE = 79847,

            SPELL_FROST_ARMOR = 79865,
            SPELL_FROSTBOLT = 79858,
            SPELL_ICE_LANCE = 79859,
            SPELL_BLIZZARD = 79860,
            SPELL_FROST_NOVA = 79850,
        };

        void Reset() override
        {
            _events.Reset();

            me->AddUnitState(UNIT_STATE_ROOT);

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            if (me->GetEntry() == 46049)
            {
                if (!me->HasAura(SPELL_MOLTEN_CORE))
                    me->CastSpell(me, SPELL_MOLTEN_CORE, false);
            }
            else
            {
                if (!me->HasAura(SPELL_FROST_ARMOR))
                    me->CastSpell(me, SPELL_FROST_ARMOR, false);
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* who) override
        {
            if (me->GetEntry() == 46049)
            {
                _events.ScheduleEvent(EVENT_FIREBALL, 0);
                _events.ScheduleEvent(EVENT_FIREBLAST, urand(3000, 4000));
                _events.ScheduleEvent(EVENT_FLAMESTRIKE, urand(6000, 8000));
                _events.ScheduleEvent(EVENT_BLASTWAVE, urand(8000, 10000));
            }
            else
            {
                _events.ScheduleEvent(EVENT_FROSTBOLT, 0);
                _events.ScheduleEvent(EVENT_ICE_LANCE, urand(3000, 4000));
                _events.ScheduleEvent(EVENT_BLIZZARD, urand(6000, 8000));
                _events.ScheduleEvent(EVENT_FROST_NOVA, urand(8000, 10000));
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* worgen1 = me->FindNearestCreature(46063, 5.0f))
                    {
                        me->CastSpell(worgen1, 355, true);
                        me->Attack(worgen1, true);
                    }

                    if (Creature* worgen2 = me->FindNearestCreature(46064, 5.0f))
                    {
                        me->CastSpell(worgen2, 355, true);
                        me->Attack(worgen2, true);
                    }

                    if (me->GetEntry() == 46049)
                    {
                        if (Creature* worgen3 = me->FindNearestCreature(46059, 5.0f))
                        {
                            me->CastSpell(worgen3, 355, true);
                            me->Attack(worgen3, true);
                        }

                        if (Creature* worgen4 = me->FindNearestCreature(46061, 5.0f))
                        {
                            me->CastSpell(worgen4, 355, true);
                            me->Attack(worgen4, true);
                        }

                        if (Creature* worgen5 = me->FindNearestCreature(46062, 5.0f))
                        {
                            me->CastSpell(worgen5, 355, true);
                            me->Attack(worgen5, true);
                        }
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_FIREBALL:
                {
                    DoCastVictim(SPELL_FIREBALL, false);
                    _events.ScheduleEvent(EVENT_FIREBALL, 3100);
                    break;
                }

                case EVENT_FIREBLAST:
                {
                    DoCastVictim(SPELL_FIREBLAST, false);
                    _events.ScheduleEvent(EVENT_FIREBLAST, 6000);
                    break;
                }

                case EVENT_FLAMESTRIKE:
                {
                    DoCastVictim(SPELL_FLAMESTRIKE, false);
                    _events.ScheduleEvent(EVENT_FLAMESTRIKE, 7500);
                    break;
                }

                case EVENT_BLASTWAVE:
                {
                    DoCastVictim(SPELL_BLAST_WAVE, false);
                    _events.ScheduleEvent(EVENT_BLASTWAVE, 8500);
                    break;
                }

                case EVENT_FROSTBOLT:
                {
                    DoCastVictim(SPELL_FROSTBOLT, false);
                    _events.ScheduleEvent(EVENT_FROSTBOLT, 2100);
                    break;
                }

                case EVENT_ICE_LANCE:
                {
                    DoCastVictim(SPELL_ICE_LANCE, false);
                    _events.ScheduleEvent(EVENT_ICE_LANCE, 6000);
                    break;
                }

                case EVENT_BLIZZARD:
                {
                    DoCastVictim(SPELL_BLIZZARD, false);
                    _events.ScheduleEvent(EVENT_BLIZZARD, 7500);
                    break;
                }

                case EVENT_FROST_NOVA:
                {
                    DoCastVictim(SPELL_FROST_NOVA, false);
                    _events.ScheduleEvent(EVENT_FROST_NOVA, 8500);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_forsaken_magesAI(creature);
    }
};

// Phin Odelic, Loremaster Dibbs, Magistrate Henry Haleb - 46059, 46061, 46062
class npc_battle_worgen_mages : public CreatureScript
{
public:
    npc_battle_worgen_mages() : CreatureScript("npc_battle_worgen_mages") { }

    struct npc_battle_worgen_magesAI : public ScriptedAI
    {
        npc_battle_worgen_magesAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_ARCANE_BLAST = 1,
            EVENT_ARCANE_MISSILES = 2,
            EVENT_ARCANE_EXPLOSION = 3,
            EVENT_SLOW = 4,

            EVENT_CHECK_SHOWFIGHT = 5,
            MOVE_TO_HOMEPOSITION = 6,

            SPELL_MAGE_ARMOR = 6117,
            SPELL_ARCANE_BLAST = 79868,
            SPELL_ARCANE_MISSILES = 79869,
            SPELL_ARCANE_EXPLOSION = 22938,
            SPELL_SLOW = 79880
        };

        void Reset() override
        {
            _events.Reset();

            me->AddUnitState(UNIT_STATE_ROOT);

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            if (!me->HasAura(SPELL_MAGE_ARMOR))
                me->CastSpell(me, SPELL_MAGE_ARMOR, false);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_ARCANE_BLAST, 0);
            _events.ScheduleEvent(EVENT_ARCANE_MISSILES, urand(3000, 4000));
            _events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, urand(6000, 8000));
            _events.ScheduleEvent(EVENT_SLOW, urand(8000, 10000));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* horde1 = me->FindNearestCreature(46049, 5.0f))
                    {
                        me->CastSpell(horde1, 355, true);
                        me->Attack(horde1, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_ARCANE_BLAST:
                {
                    DoCastVictim(SPELL_ARCANE_BLAST, false);
                    _events.ScheduleEvent(EVENT_ARCANE_BLAST, 2600);
                    break;
                }

                case EVENT_ARCANE_MISSILES:
                {
                    DoCastVictim(SPELL_ARCANE_MISSILES, false);
                    _events.ScheduleEvent(EVENT_ARCANE_MISSILES, 6000);
                    break;
                }

                case EVENT_ARCANE_EXPLOSION:
                {
                    DoCastVictim(SPELL_ARCANE_EXPLOSION, false);
                    _events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, 7500);
                    break;
                }

                case EVENT_SLOW:
                {
                    DoCastVictim(SPELL_SLOW, false);
                    _events.ScheduleEvent(EVENT_SLOW, 8500);
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_worgen_magesAI(creature);
    }
};

// Bloodfang Berserker, Hillsbrad Worgen - 46050, 46063, 46064 
class npc_battle_worgens : public CreatureScript
{
public:
    npc_battle_worgens() : CreatureScript("npc_battle_worgens") { }

    struct npc_battle_worgensAI : public ScriptedAI
    {
        npc_battle_worgensAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_UPPERCUT = 1,
            EVENT_ENRAGE = 2,
            EVENT_SLAM = 3,
            EVENT_SKIN = 4,
            EVENT_CHECK_SHOWFIGHT = 5,
            MOVE_TO_HOMEPOSITION = 6,

            SPELL_UPPERCUT = 46064,
            SPELL_ENRAGE = 32714,
            SPELL_SKIN = 80367,
            SPELL_SLAM = 79881,
        };

        void Reset() override
        {
            _events.Reset();

            me->SetAttackTime(BASE_ATTACK, 1500 + urand(0, 850));

            _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 0);

            me->SetPowerType(Powers::POWER_RAGE);
            me->SetMaxPower(Powers::POWER_RAGE, 1000);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_UPPERCUT, urand(3500, 6500));
            _events.ScheduleEvent(EVENT_ENRAGE, urand(14000, 18000));

            if (!me->GetEntry() == 46050)
                _events.ScheduleEvent(EVENT_SKIN, urand(6000, 8000));

            if (me->GetEntry() == 46050)
                _events.ScheduleEvent(EVENT_SLAM, urand(6000, 8000));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_SHOWFIGHT:
                {
                    if (me->IsInCombat() || me->isDead())
                        return;

                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                        return;
                    }

                    if (Creature* forsaken1 = me->FindNearestCreature(46055, 5.0f))
                    {
                        me->CastSpell(forsaken1, 355, true);
                        me->Attack(forsaken1, true);
                    }

                    if (Creature* forsaken2 = me->FindNearestCreature(46056, 5.0f))
                    {
                        me->CastSpell(forsaken2, 355, true);
                        me->Attack(forsaken2, true);
                    }

                    if (Creature* forsaken3 = me->FindNearestCreature(46057, 5.0f))
                    {
                        me->CastSpell(forsaken3, 355, true);
                        me->Attack(forsaken3, true);
                    }

                    if (Creature* forsaken4 = me->FindNearestCreature(46053, 5.0f))
                    {
                        me->CastSpell(forsaken4, 355, true);
                        me->Attack(forsaken4, true);
                    }

                    if (Creature* forsaken5 = me->FindNearestCreature(46054, 5.0f))
                    {
                        me->CastSpell(forsaken5, 355, true);
                        me->Attack(forsaken5, true);
                    }

                    _events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                    break;
                }

                case EVENT_UPPERCUT:
                {
                    DoCastVictim(SPELL_UPPERCUT);
                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);
                    _events.ScheduleEvent(EVENT_UPPERCUT, urand(3500, 6500));
                    break;
                }

                case EVENT_ENRAGE:
                {
                    me->CastSpell(me, SPELL_ENRAGE, false);
                    me->ModifyPowerPct(POWER_RAGE, -20.0f, true);
                    _events.ScheduleEvent(EVENT_ENRAGE, urand(14000, 18000));
                    break;
                }

                case EVENT_SKIN:
                {
                    me->CastSpell(me, SPELL_SKIN, false);
                    me->ModifyPowerPct(POWER_RAGE, -25.0f, true);
                    _events.ScheduleEvent(EVENT_SKIN, urand(6000, 8000));
                    break;
                }

                case EVENT_SLAM:
                {
                    me->CastSpell(me, SPELL_SLAM, false);
                    me->ModifyPowerPct(POWER_RAGE, -40.0f, true);
                    _events.ScheduleEvent(EVENT_SLAM, urand(14000, 18000));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_worgensAI(creature);
    }
};

// Forsaken Catapult - 46052
class npc_battle_forsaken_catapult : public CreatureScript
{
public:
    npc_battle_forsaken_catapult() : CreatureScript("npc_battle_forsaken_catapult") { }

    struct npc_battle_forsaken_catapultAI : public ScriptedAI
    {
        npc_battle_forsaken_catapultAI(Creature* creature) : ScriptedAI(creature) { }

        enum eNPC
        {
            EVENT_BARREL = 1,

            SPELL_THROW_BARREL = 85147,
        };

        bool Done;

        void Reset() override
        {
            _events.Reset();

            Done = false;

            me->AddUnitState(UNIT_STATE_ROOT);

            me->SetPowerType(Powers::POWER_ENERGY);
            me->SetMaxPower(Powers::POWER_ENERGY, 100);

            _events.ScheduleEvent(EVENT_BARREL, urand(7500, 12500));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (Done == false)
            {
                Position pos = me->GetPosition();

                if (Creature* apothecary = me->SummonCreature(46483, pos, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    me->CastSpell(apothecary, 84109); // ride vehicle, script_effect
                    apothecary->CastSpell(me, 108582);
                    apothecary->EnterVehicle(me, 0);

                    Done = true;
                }
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BARREL:
                {
                    std::list<Creature*> targetList;
                    GetCreatureListWithEntryInGrid(targetList, me, 46063, 200.0f);
                    GetCreatureListWithEntryInGrid(targetList, me, 46064, 200.0f);
                    Trinity::Containers::RandomResize(targetList, 1);

                    for (auto itr : targetList)
                        me->CastSpell(itr, SPELL_THROW_BARREL, false);

                    _events.ScheduleEvent(EVENT_BARREL, urand(7500, 12500));
                    break;
                }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_forsaken_catapultAI(creature);
    }
};


/*
Arthura on Gilneas - 84941*/



/* Eject passengers 3-8 -> 83477 */

// Horde Hauler - 44731
class npc_horde_hauler : public CreatureScript
{
public:
    npc_horde_hauler() : CreatureScript("npc_horde_hauler") { }

    enum eNPC
    {
        NPC_HORDE_ENGINEER = 44734,
        NPC_SUBDUED_FOREST_ETTIN = 44737,
        NPC_FORSAKEN_TROOPER_F = 44732,
        NPC_FORSAKEN_TROOPER_M = 44733,

        CHAIN_RIGHT = 83467,
        CHAIN_LEFT = 83464,
        ANIMKIT = 91298,
        SPELL_EJECT = 83477,

        EVENT_TO_FORSAKEN_HIGH_COMMAND = 1,
        EVENT_TO_THE_SEPULCHER = 2,
        EVENT_TO_THE_FORSAKEN_FRONT = 3,
        EVENT_TO_DESPAWN = 4,
        EVENT_YELL_1 = 5,
        EVENT_YELL_2 = 6,
        EVENT_YELL_3 = 7,
    };

    struct npc_horde_haulerAI : public ScriptedAI
    {
        npc_horde_haulerAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap m_events;

        void Reset() override
        {
            m_events.Reset();

            m_events.ScheduleEvent(EVENT_TO_FORSAKEN_HIGH_COMMAND, 5000);
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
        {
            if (!apply)
                return;

            if (seatId == 2)
            {
                if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                    engineer->AI()->Talk(0);
            }
        }

        void WaypointReached(uint32 waypointId, uint32 pathId) override
        {
            if (pathId == 447310)
            {
                if (waypointId == 5)
                {
                    m_events.ScheduleEvent(EVENT_TO_THE_SEPULCHER, 15000);
                    m_events.ScheduleEvent(EVENT_YELL_1, 1000);
                }
            }

            if (pathId == 447311)
            {
                if (waypointId == 10)
                {
                    m_events.ScheduleEvent(EVENT_TO_THE_FORSAKEN_FRONT, 15000);
                    m_events.ScheduleEvent(EVENT_YELL_2, 1000);
                }
            }

            if (pathId == 447312)
            {
                if (waypointId == 5)
                {
                    m_events.ScheduleEvent(EVENT_TO_DESPAWN, 15000);
                    m_events.ScheduleEvent(EVENT_YELL_3, 1000);
                }
            }

            if (pathId == 447313)
            {
                if (waypointId == 5)
                    me->DespawnOrUnsummon(10000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_TO_FORSAKEN_HIGH_COMMAND:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, CHAIN_LEFT, true);
                            engineer->AddAura(ANIMKIT, engineer);

                            me->GetMotionMaster()->MovePath(447310, false);
                        }
                    }
                    break;
                }

                case EVENT_YELL_1:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(1);
                    break;
                }

                case EVENT_TO_THE_SEPULCHER:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, CHAIN_LEFT, true);
                            engineer->AddAura(ANIMKIT, engineer);

                            me->GetMotionMaster()->MovePath(447311, false);
                        }
                    }
                    break;
                }

                case EVENT_YELL_2:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(2);
                    break;
                }

                case EVENT_TO_THE_FORSAKEN_FRONT:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, CHAIN_LEFT, true);
                            engineer->AddAura(ANIMKIT, engineer);

                            me->GetMotionMaster()->MovePath(447312, false);
                        }
                    }
                    break;
                }

                case EVENT_YELL_3:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f, true))
                        engineer->AI()->Talk(3);

                    me->CastSpell(me, SPELL_EJECT, true);
                    break;
                }

                case EVENT_TO_DESPAWN:
                {
                    if (Creature* engineer = me->FindNearestCreature(NPC_HORDE_ENGINEER, 15.0f))
                    {
                        if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_FOREST_ETTIN, 15.0f))
                        {
                            engineer->CastSpell(ettin, CHAIN_RIGHT, true);
                            engineer->CastSpell(ettin, CHAIN_LEFT, true);
                            engineer->AddAura(ANIMKIT, engineer);

                            me->GetMotionMaster()->MovePath(447313, false);
                        }
                    }
                    break;
                }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_horde_haulerAI(creature);
    }
};

// Spell Summon Generic
class spell_generic_summon_silverpine : public SpellScriptLoader
{
public:
    spell_generic_summon_silverpine() : SpellScriptLoader("spell_generic_summon") { }

    enum eSpells
    {
        SPELL_SUMMON_AGATHA_83982 = 83982,
        SPELL_SUMMON_GODFREY = 85517,
        SPELL_SUMMON_WALDEN = 85518,
        SPELL_SUMMON_ASHBURY = 85519,
        SPELL_SUMMON_AGATHA_85870 = 85870,
        SPELL_SUMMON_ARTHURA = 85869,
        SPELL_SUMMON_DASCHLA = 85871,
        SPELL_SUMMON_CROMUSH = 85868,
    };

    class spell_generic_summon_silverpine_SpellScript : public SpellScript
    {
        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_SUMMON_AGATHA_83982,
                    SPELL_SUMMON_GODFREY,
                    SPELL_SUMMON_WALDEN,
                    SPELL_SUMMON_ASHBURY,
                    SPELL_SUMMON_AGATHA_85870,
                    SPELL_SUMMON_ARTHURA,
                    SPELL_SUMMON_DASCHLA,
                    SPELL_SUMMON_CROMUSH,
                });
        }

        bool Load() override
        {
            dummySpellId = GetSpellInfo()->Id;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (TempSummon* creature = GetCaster()->ToTempSummon())
            {
                if (Unit* player = creature->GetSummoner())
                {
                    if (!creature)
                        player->CastSpell(player, dummySpellId, false);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget.Register(&spell_generic_summon_silverpine_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SUMMON);
        }

    private:
        uint32 dummySpellId;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_generic_summon_silverpine_SpellScript();
    }
};

void AddSC_silverpine_forest()
{
    // Zone
    new playerScript_silverpine_gilneas();

    // Npc
    new npc_worgen_renegade_44793();
    new npc_forsaken_trooper_44791();

    new npc_grand_executor_mortuus_44615();
    new npc_fallen_human_44592();

    new npc_bat_handler_maggotbreath_44825();
    new npc_forsaken_bat_44821();

    new npc_agatha_44951();
    new npc_hillsbrad_refugee_44966();
    new npc_fenris_keep_stalker_45032();
    new npc_lord_darius_crowley_44989();
    new npc_phin_odelic_44993();
    new npc_bartolo_ginsetti_44994();
    new npc_loremaster_dibbs_44995();
    new npc_magistrate_henry_maleb_44996();
    new npc_caretaker_smithers_44997();
    new npc_sophia_zwoski_45002();
    new npc_camera_45003();

    new npc_lady_sylvanas_windrunner_44365();
    new npc_lady_sylvanas_windrunner_45051();
    new npc_forsaken_warhorse_45057();

    new npc_arthura_45318();

    new npc_deathstalkers_belmont();
    new npc_forsaken_sharpshooter_45376();
    new npc_forsaken_catapult_45282();
    new npc_forsaken_blightspreader_45374();
    new npc_worgen_rusher_45375();
    new npc_forsaken_vanguard_45280();
    new npc_worgen_rebel_45292();

    new npc_bloodfang_scout_45536();
    new npc_worgen_battlemage_45537();

    new npc_deathstalker_belmont_45474();
    new npc_arthura_45314();

    new npc_lady_sylvanas_windrunner_45525();
    new npc_ashgodwal_45606();

    new npc_lady_sylvanas_windrunner_45617();
    new npc_forsaken_catapult();

    new npc_ambermill_portal_45752();
    new npc_archmage_ataeric_2120();
    new npc_arcane_fiend_45766();
    new npc_daschla_forsaken();
    new npc_ataeric_and_magi();

    new npc_godfrey_45878();
    new npc_walden_45879();
    new npc_ashbury_45880();
    new npc_killcredit_pyrewood();
    new npc_legion_scout_45883();
    new npc_cowering_trooper_45910();
    new npc_dempsey_45977();
    new npc_lorna_45997();

    new npc_lady_sylvanas_windrunner_46026();
    new npc_agardacro_46034();
    new npc_battle_crowblood();

    new npc_battle_sea_orcs();
    new npc_battle_forsaken_troopers();
    new npc_battle_forsaken_mages();
    new npc_battle_forsaken_catapult();
    new npc_battle_worgen_mages();
    new npc_battle_worgens();

    // Cosmetic
    new npc_horde_hauler();
    new npc_dreadguard_cosmetic();
    new npc_apothecary_wormcrud();

    // Spell
    new spell_raise_forsaken_83173();
    new spell_forsaken_trooper_master_script_83149();
    new spell_raise_forsaken_85197();
    new spell_dimensional_beam_85373();
    new spell_raise_forsaken_85411();
    new spell_toss_torch_85600();
    new spell_generic_summon_silverpine();
}
