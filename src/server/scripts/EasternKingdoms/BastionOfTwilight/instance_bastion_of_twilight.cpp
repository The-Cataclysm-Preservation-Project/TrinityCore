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

#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "Containers.h"
#include "bastion_of_twilight.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "TemporarySummon.h"

namespace BastionOfTwilight
{
ObjectData const creatureData[] =
{
    { BOSS_HALFUS_WYRMBREAKER,          DATA_HALFUS_WYRMBREAKER             },
    { BOSS_THERALION,                   DATA_THERALION                      },
    { BOSS_VALIONA,                     DATA_VALIONA                        },
    { BOSS_IGNACIOUS,                   DATA_IGNACIOUS                      },
    { BOSS_FELUDIUS,                    DATA_FELUDIUS                       },
    { BOSS_TERRASTRA,                   DATA_TERRASTRA                      },
    { BOSS_ARION,                       DATA_ARION                          },
    { BOSS_ELEMENTIUM_MONSTROSITY,      DATA_ELEMENTIUM_MONSTROSITY         },
    { BOSS_CHOGALL,                     DATA_CHOGALL                        },
    { BOSS_SINESTRA,                    DATA_SINESTRA                       },
    { NPC_PROTO_BEHEMOTH,               DATA_PROTO_BEHEMOTH                 },
    { NPC_SLATE_DRAGON,                 DATA_SLATE_DRAGON                   },
    { NPC_NETHER_SCION,                 DATA_NETHER_SCION                   },
    { NPC_STORM_RIDER,                  DATA_STORM_RIDER                    },
    { NPC_TIME_WARDEN,                  DATA_TIME_WARDEN                    },
    { NPC_ASCENDANT_COUNCIL_CONTROLLER, DATA_ASCENDANT_COUNCIL_CONTROLLER   },
    { NPC_CORRUPTION,                   DATA_CORRUPTION                     },
    { NPC_CHOGALL,                      DATA_CHOGALL_EVENT                  },
    { 0,                                0                                   } // END
};

ObjectData const gameobjectData[] =
{
    { GO_GRIM_BATOL_RAID_TRAP_DOOR, DATA_GRIM_BATOL_RAID_TRAP_DOOR  },
    { GO_WHELP_CAGE,                DATA_WHELP_CAGE                 },
    { 0,                            0                               } // END
};

DoorData const doorData[] =
{
    { GO_HALFUS_ENTRANCE,                   DATA_HALFUS_WYRMBREAKER,        DOOR_TYPE_ROOM      },
    { GO_HALFUS_EXIT,                       DATA_HALFUS_WYRMBREAKER,        DOOR_TYPE_PASSAGE   },
    { GO_DRAGON_SIBLINGS_DOOR_ENTRANCE,     DATA_THERALION_AND_VALIONA,     DOOR_TYPE_ROOM      },
    { GO_DRAGON_SIBLINGS_DOOR_EXIT,         DATA_THERALION_AND_VALIONA,     DOOR_TYPE_PASSAGE   },
    { GO_ASCENDANT_COUNCIL_ENTRANCE,        DATA_ASCENDANT_COUNCIL,         DOOR_TYPE_ROOM      },
    { GO_ASCENDANT_COUNCIL_EXIT,            DATA_ASCENDANT_COUNCIL,         DOOR_TYPE_PASSAGE   },
    { GO_CHOGALL_ENTRANCE,                  DATA_CHOGALL,                   DOOR_TYPE_ROOM      },
    { 0,                                    0,                              DOOR_TYPE_ROOM      } // END
};

enum MapEvents
{
    EVENT_ID_NETHER_SCION_FREED = 26077,
    EVENT_ID_SLATE_DRAGON_FREED = 26078,
    EVENT_ID_STORM_RIDER_FREED  = 26079,
    EVENT_ID_TIME_WARDEN_FREED  = 26064
};

Position const BreathFlightTargetStalkerSortPos = { -740.677f, -592.328f, 859.455f };

class instance_bastion_of_twilight final : public InstanceMapScript
{
    public:
        instance_bastion_of_twilight() : InstanceMapScript(BoTScriptName, 671) { }

        struct instance_bastion_of_twilight_InstanceMapScript final : public InstanceScript
        {
            instance_bastion_of_twilight_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(!map->IsHeroic() ? EncounterCountNormal : EncounterCountHeroic); // Sinestra only in heroic mode
                LoadDoorData(doorData);
                LoadObjectData(creatureData, gameobjectData);
                _activeDragonFlags = DRAGON_FLAG_ALL_ACTIVE;

                if (!map->IsHeroic())
                {
                    _activeDragonFlags = 0;
                    std::vector<BoTHalfusDragonFlags> activeDragons = { DRAGON_FLAG_NETHER_SCION, DRAGON_FLAG_SLATE_DRAGON, DRAGON_FLAG_STORM_RIDER, DRAGON_FLAG_TIME_WARDEN, DRAGON_FLAG_EMERALD_WHELPS };
                    Trinity::Containers::RandomResize(activeDragons, 3);
                    for (BoTHalfusDragonFlags flag : activeDragons)
                        _activeDragonFlags |= flag;
                }

                if ((_activeDragonFlags & DRAGON_FLAG_TIME_WARDEN) != 0)
                    events.ScheduleEvent(EVENT_CAST_DANCING_FLAMES, 1s);
            }

            void OnCreatureCreate(Creature* creature) override
            {
                InstanceScript::OnCreatureCreate(creature);

                switch (creature->GetEntry())
                {
                    case NPC_INVISIBLE_STALKER:
                        if (creature->GetPositionZ() < 850.0f)
                            _dancingFlamesInvisibleStalkerGUIDs.insert(creature->GetGUID());
                        break;
                    case NPC_COLLAPSING_TWILIGHT_PORTAL:
                        if (Creature* valiona = GetCreature(DATA_VALIONA))
                            valiona->AI()->JustSummoned(creature);

                        _collapsingTwilightPortalGUIDs.insert(creature->GetGUID());
                        break;
                    case NPC_CONVECTIVE_FLAMES:
                    case NPC_TWILIGHT_SENTRY:
                    case NPC_TWILIGHT_RIFT:
                        if (Creature* valiona = GetCreature(DATA_VALIONA))
                            valiona->AI()->JustSummoned(creature);
                        break;
                    case NPC_DAZZLING_DESTRUCTION_STALKER:
                    case NPC_FABULOUS_FLAMES:
                        if (Creature* theralion = GetCreature(DATA_THERALION))
                            theralion->AI()->JustSummoned(creature);
                        break;
                    case NPC_VALIONA_DUMMY:
                        _valionaDummyGUIDs.insert(creature->GetGUID());
                        if (creature->GetOrientation() == 0.0f) // Blizzard uses a single dummy with 0.0f orientation as aura target dummy
                            _valionaAuraDummyGUID = creature->GetGUID();
                        break;
                    case NPC_UNSTABLE_TWILIGHT:
                        _unstableTwilightGUIDs.insert(creature->GetGUID());
                        break;
                    case BOSS_FELUDIUS:
                    case BOSS_IGNACIOUS:
                    case BOSS_ARION:
                    case BOSS_TERRASTRA:
                        if (creature->isDead() && GetBossState(DATA_ASCENDANT_COUNCIL) != DONE)
                            creature->Respawn();
                        break;
                    case NPC_SPIKED_TENTACLE_TRIGGER:
                        if (Creature* chogall = GetCreature(DATA_CHOGALL))
                            chogall->AI()->JustSummoned(creature);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureRemove(Creature* creature) override
            {
                InstanceScript::OnCreatureRemove(creature);
                if (creature->GetEntry() == NPC_INVISIBLE_STALKER)
                    _dancingFlamesInvisibleStalkerGUIDs.erase(creature->GetGUID());
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                InstanceScript::OnGameObjectCreate(go);
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_HALFUS_WYRMBREAKER:
                        if (state == DONE)
                        {
                            if (Creature* chogall = GetCreature(DATA_CHOGALL_EVENT))
                                chogall->AI()->DoAction(ACTION_TALK_HALFUS_WYRMBREAKER_DEAD);
                        }
                        break;
                    case DATA_THERALION_AND_VALIONA:
                        if (state == FAIL)
                        {
                            for (ObjectGuid guid : _valionaDummyGUIDs)
                                if (Creature* creature = instance->GetCreature(guid))
                                    creature->DespawnOrUnsummon(0ms, 30s);

                            for (ObjectGuid guid : _unstableTwilightGUIDs)
                                if (Creature* creature = instance->GetCreature(guid))
                                    creature->DespawnOrUnsummon(0ms, 30s);

                            _valionaDummyGUIDs.clear();
                            _collapsingTwilightPortalGUIDs.clear();
                        }
                        else if (state == DONE)
                        {
                            for (ObjectGuid guid : _valionaDummyGUIDs)
                                if (Creature* creature = instance->GetCreature(guid))
                                    creature->DespawnOrUnsummon();

                            for (ObjectGuid guid : _unstableTwilightGUIDs)
                                if (Creature* creature = instance->GetCreature(guid))
                                    creature->DespawnOrUnsummon();

                            events.ScheduleEvent(EVENT_CHOGALL_TALK_THERALION_AND_VALIONA_DEAD, 6s);
                        }
                        break;
                    default:
                        break;
                }
                return true;
            }

            void SetData(uint32 type, uint32 /*data*/) override
            {
                switch (type)
                {
                    case DATA_AT_HALFUS_WYRMBREAKER_INTRO:
                        if (GetBossState(DATA_HALFUS_WYRMBREAKER) != DONE)
                            if (Creature* chogall = GetCreature(DATA_CHOGALL_EVENT))
                                chogall->AI()->DoAction(ACTION_TALK_HALFUS_WYRMBREAKER_INTRO);
                        break;
                    case DATA_AT_THERALION_AND_VALIONA_INTRO:
                        if (GetBossState(DATA_THERALION_AND_VALIONA) != DONE)
                        {
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_THERALION_AND_VALIONA_INTRO);

                            if (Creature* theralion = GetCreature(DATA_THERALION))
                                theralion->AI()->DoAction(ACTION_START_ARGUMENT_INTRO);

                            if (Creature* valiona = GetCreature(DATA_VALIONA))
                                valiona->AI()->DoAction(ACTION_START_ARGUMENT_INTRO);
                        }
                        break;
                    case DATA_AT_ASCENDANT_COUNCIL_INTRO_1:
                        if (GetBossState(DATA_ASCENDANT_COUNCIL) != DONE)
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_ASCENDANT_COUNCIL_INTRO_1);
                        break;
                    case DATA_AT_ASCENDANT_COUNCIL_INTRO_2:
                        if (GetBossState(DATA_ASCENDANT_COUNCIL) != DONE)
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_ASCENDANT_COUNCIL_INTRO_2);
                        break;
                    case DATA_AT_ASCENDANT_COUNCIL_INTRO_3:
                        if (GetBossState(DATA_ASCENDANT_COUNCIL) != DONE)
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_ASCENDANT_COUNCIL_INTRO_3);
                        break;
                    case DATA_AT_CHOGALL_INTRO:
                        if (GetBossState(DATA_CHOGALL) != DONE)
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_CHOGALL_INTRO);
                        break;
                    default:
                        break;
                }
            }

            ObjectGuid GetGuidData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_RANDOM_VALIONA_DUMMY:
                        return Trinity::Containers::SelectRandomContainerElement(_valionaDummyGUIDs);
                    case DATA_VALIONA_AURA_DUMMY:
                        return _valionaAuraDummyGUID;
                    default:
                        break;
                }

                return ObjectGuid::Empty;
            }

            uint32 GetData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_COLLAPSING_TWILIGHT_PORTAL_COUNT:
                    {
                        uint8 portalCount = 0;
                        for (ObjectGuid guid : _collapsingTwilightPortalGUIDs)
                            if (instance->GetCreature(guid))
                                portalCount++;
                        return portalCount;
                    }
                    case DATA_FULL_HEROIC_ID:
                        return false;
                    case DATA_ACTIVE_DRAGON_FLAGS:
                        return _activeDragonFlags;
                    default:
                        return 0;
                }
                return 0;
            }

            void Update(uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CAST_DANCING_FLAMES:
                        {
                            if (_dancingFlamesInvisibleStalkerGUIDs.size() < 1)
                            {
                                events.Repeat(1s);
                                break;
                            }

                            // Between one and three stalkers cast the dancing flames at the same time. We now have to select a range within the guidset
                            uint32 castingStalkerNum = std::min<uint32>(urand(1, 3), (_dancingFlamesInvisibleStalkerGUIDs.size() - 1));
                            GuidSet::const_iterator startIterator = _dancingFlamesInvisibleStalkerGUIDs.begin();
                            GuidSet::const_iterator endIterator = _dancingFlamesInvisibleStalkerGUIDs.begin();
                            std::advance(endIterator, castingStalkerNum);

                            if (_dancingFlamesInvisibleStalkerGUIDs.size() > (castingStalkerNum))
                            {
                                uint32 offset = urand(0, _dancingFlamesInvisibleStalkerGUIDs.size() - castingStalkerNum);
                                std::advance(startIterator, offset);
                                std::advance(endIterator, offset);
                            }

                            for (GuidSet::const_iterator itr = startIterator; itr != endIterator; ++itr)
                                if (Creature* cataclysmStalker = instance->GetCreature(*itr))
                                    cataclysmStalker->CastSpell(cataclysmStalker, SPELL_DANCING_FLAMES_VISUAL);

                            events.Repeat(500ms, 1s);
                            break;
                        }
                        case EVENT_CHOGALL_TALK_THERALION_AND_VALIONA_DEAD:
                            if (Creature* chogall = GetCreature(DATA_CHOGALL))
                                chogall->AI()->DoAction(ACTION_TALK_THERALION_AND_VALIONA_DEAD);
                            break;
                        default:
                            break;
                    }
                }
            }

            void ProcessEvent(WorldObject* objTarget, uint32 eventId, WorldObject* /*invoker*/) override
            {
                switch (eventId)
                {
                    // These events are sent by the Free Dragon spells in Halfus Wyrmbreaker's encounter. Though it makes zero sense to use them in this context, we will just
                    // do so to follow Blizzard's intention. Probably some intern's doing or so.
                    case EVENT_ID_SLATE_DRAGON_FREED:
                    case EVENT_ID_NETHER_SCION_FREED:
                    case EVENT_ID_STORM_RIDER_FREED:
                    case EVENT_ID_TIME_WARDEN_FREED:
                        if (objTarget->IsCreature())
                            if (CreatureAI* ai = objTarget->ToCreature()->AI())
                                ai->DoAction(ACTION_DRAGON_FREED);
                        break;
                    default:
                        break;
                }
            }

        private:
            EventMap events;
            GuidSet _dancingFlamesInvisibleStalkerGUIDs;
            GuidSet _valionaDummyGUIDs;
            GuidSet _unstableTwilightGUIDs;
            GuidSet _collapsingTwilightPortalGUIDs;
            ObjectGuid _valionaAuraDummyGUID;
            uint8 _activeDragonFlags;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_bastion_of_twilight_InstanceMapScript(map);
        }
};
}

void AddSC_instance_bastion_of_twilight()
{
    using namespace BastionOfTwilight;
    new instance_bastion_of_twilight();
}
