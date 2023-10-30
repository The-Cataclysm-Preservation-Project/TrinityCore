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

#include "Group.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "CharacterCache.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DynamicObject.h"
#include "Formulas.h"
#include "GameObject.h"
#include "GroupMgr.h"
#include "Guild.h"
#include "InstanceSaveMgr.h"
#include "LFGMgr.h"
#include "LootMgr.h"
#include "LootPackets.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "PartyPackets.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "Random.h"
#include "TerrainMgr.h"
#include "SpellAuras.h"
#include "UpdateData.h"
#include "UpdateFieldFlags.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

Roll::Roll(ObjectGuid _guid, LootItem const& li) : itemGUID(_guid), itemid(li.itemid),
itemRandomPropId(li.randomPropertyId), itemRandomSuffix(li.randomSuffix), itemCount(li.count),
totalPlayersRolling(0), totalNeed(0), totalGreed(0), totalPass(0), itemSlot(0),
rollVoteMask(ROLL_ALL_TYPE_NO_DISENCHANT) { }

Roll::~Roll() { }

void Roll::setLoot(Loot* pLoot)
{
    link(pLoot, this);
}

Loot* Roll::getLoot()
{
    return getTarget();
}

Group::Group() : m_leaderGuid(), m_leaderName(""), m_groupFlags(GROUP_FLAG_NONE), m_markerMask(0),
m_dungeonDifficulty(DUNGEON_DIFFICULTY_NORMAL), m_raidDifficulty(RAID_DIFFICULTY_10MAN_NORMAL),
m_bgGroup(nullptr), m_bfGroup(nullptr), m_lootMethod(FREE_FOR_ALL), m_lootThreshold(ITEM_QUALITY_UNCOMMON), m_looterGuid(),
m_masterLooterGuid(), m_subGroupsCounts(nullptr), m_guid(), m_counter(0), m_dbStoreId(0)
{
    m_disenchantInfo.Initialize();

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i].Clear();
}

Group::~Group()
{
    if (m_bgGroup)
    {
        TC_LOG_DEBUG("bg.battleground", "Group::~Group: battleground group being deleted.");
        if (m_bgGroup->GetBgRaid(ALLIANCE) == this) m_bgGroup->SetBgRaid(ALLIANCE, nullptr);
        else if (m_bgGroup->GetBgRaid(HORDE) == this) m_bgGroup->SetBgRaid(HORDE, nullptr);
        else TC_LOG_ERROR("misc", "Group::~Group: battleground group is not linked to the correct battleground.");
    }

    // this may unload some instance saves
    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        for (BoundInstancesMap::iterator itr2 = m_boundInstances[i].begin(); itr2 != m_boundInstances[i].end(); ++itr2)
            itr2->second.save->RemoveGroup(this);

    // Sub group counters clean up
    delete[] m_subGroupsCounts;
}

bool Group::Create(Player* leader)
{
    ObjectGuid leaderGuid = leader->GetGUID();
    ObjectGuid::LowType lowguid = sGroupMgr->GenerateGroupId();

    m_guid = ObjectGuid(HighGuid::Group, lowguid);
    m_leaderGuid = leaderGuid;
    m_leaderName = leader->GetName();
    leader->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

    if (isBGGroup() || isBFGroup())
        m_groupFlags = GROUP_MASK_BGRAID;

    if (m_groupFlags & GROUP_FLAG_RAID)
        _initRaidSubGroupsCounter();

    if (!isLFGGroup())
        m_lootMethod = GROUP_LOOT;

    m_lootThreshold = ITEM_QUALITY_UNCOMMON;
    m_looterGuid = leaderGuid;
    m_masterLooterGuid.Clear();

    m_dungeonDifficulty = DUNGEON_DIFFICULTY_NORMAL;
    m_raidDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;

    if (!isBGGroup() && !isBFGroup())
    {
        m_dungeonDifficulty = leader->GetDungeonDifficulty();
        m_raidDifficulty = leader->GetRaidDifficulty();

        m_dbStoreId = sGroupMgr->GenerateNewGroupDbStoreId();

        sGroupMgr->RegisterGroupDbStoreId(m_dbStoreId, this);

        // Store group in database
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP);

        uint8 index = 0;

        stmt->setUInt32(index++, m_dbStoreId);
        stmt->setUInt32(index++, m_leaderGuid.GetCounter());
        stmt->setUInt8(index++, uint8(m_lootMethod));
        stmt->setUInt32(index++, m_looterGuid.GetCounter());
        stmt->setUInt8(index++, uint8(m_lootThreshold));
        stmt->setUInt64(index++, m_targetIcons[0].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[1].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[2].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[3].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[4].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[5].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[6].GetRawValue());
        stmt->setUInt64(index++, m_targetIcons[7].GetRawValue());
        stmt->setUInt8(index++, uint8(m_groupFlags));
        stmt->setUInt32(index++, uint8(m_dungeonDifficulty));
        stmt->setUInt32(index++, uint8(m_raidDifficulty));
        stmt->setUInt32(index++, m_masterLooterGuid.GetCounter());

        CharacterDatabase.Execute(stmt);

        Group::ConvertLeaderInstancesToGroup(leader, this, false);

        bool addMemberResult = AddMember(leader);
        ASSERT(addMemberResult); // If the leader can't be added to a new group because it appears full, something is clearly wrong.
    }
    else if (!AddMember(leader))
        return false;

    return true;
}

void Group::LoadGroupFromDB(Field* fields)
{
    m_dbStoreId = fields[16].GetUInt32();
    m_guid = ObjectGuid(HighGuid::Group, sGroupMgr->GenerateGroupId());
    m_leaderGuid = ObjectGuid(HighGuid::Player, fields[0].GetUInt32());

    // group leader not exist
    if (!sCharacterCache->GetCharacterNameByGuid(m_leaderGuid, m_leaderName))
        return;

    m_lootMethod = LootMethod(fields[1].GetUInt8());
    m_looterGuid = ObjectGuid(HighGuid::Player, fields[2].GetUInt32());
    m_lootThreshold = ItemQualities(fields[3].GetUInt8());

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i].Set(fields[4 + i].GetUInt64());

    m_groupFlags = GroupFlags(fields[12].GetUInt8());
    if (m_groupFlags & GROUP_FLAG_RAID)
        _initRaidSubGroupsCounter();

    uint32 diff = fields[13].GetUInt8();
    if (diff >= MAX_DUNGEON_DIFFICULTY)
        m_dungeonDifficulty = DUNGEON_DIFFICULTY_NORMAL;
    else
        m_dungeonDifficulty = Difficulty(diff);

    uint32 r_diff = fields[14].GetUInt8();
    if (r_diff >= MAX_RAID_DIFFICULTY)
       m_raidDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;
    else
       m_raidDifficulty = Difficulty(r_diff);

    m_masterLooterGuid = ObjectGuid(HighGuid::Player, fields[15].GetUInt32());

    if (m_groupFlags & GROUP_FLAG_LFG)
        sLFGMgr->_LoadFromDB(fields, GetGUID());
}

void Group::LoadMemberFromDB(ObjectGuid::LowType guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles)
{
    MemberSlot member;
    member.guid = ObjectGuid(HighGuid::Player, guidLow);

    // skip non-existed member
    if (!sCharacterCache->GetCharacterNameByGuid(member.guid, member.name) ||
        !sCharacterCache->GetPlayerGuildIdByGUID(member.guid, member.guildId))
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
        stmt->setUInt32(0, guidLow);
        CharacterDatabase.Execute(stmt);
        return;
    }

    member.group = subgroup;
    member.flags = memberFlags;
    member.roles = roles;

    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subgroup);

    sLFGMgr->SetupGroupMember(member.guid, GetGUID());
}

void Group::ConvertToLFG()
{
    m_groupFlags = GroupFlags(m_groupFlags | GROUP_FLAG_LFG | GROUP_FLAG_LFG_RESTRICTED);
    m_lootMethod = NEED_BEFORE_GREED;
    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
}

void Group::ConvertToLFR()
{
    m_groupFlags  = GroupFlags(m_groupFlags | GROUP_FLAG_LFG | GROUP_FLAG_LFG_RESTRICTED | GROUP_FLAG_RAID);
    m_lootMethod = NEED_BEFORE_GREED;

    _initRaidSubGroupsCounter();

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
}

void Group::ConvertToRaid()
{
    m_groupFlags = GroupFlags(m_groupFlags | GROUP_FLAG_RAID);

    _initRaidSubGroupsCounter();

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateVisibleGameobjectsOrSpellClicks();
}

void Group::ConvertToGroup()
{
    if (m_memberSlots.size() > 5)
        return; // What message error should we send?

    m_groupFlags = GroupFlags(GROUP_FLAG_NONE);

    if (m_subGroupsCounts)
    {
        delete[] m_subGroupsCounts;
        m_subGroupsCounts = nullptr;
    }

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateVisibleGameobjectsOrSpellClicks();
}

bool Group::AddInvite(Player* player)
{
    if (!player || player->GetGroupInvite())
        return false;
    Group* group = player->GetGroup();
    if (group && (group->isBGGroup() || group->isBFGroup()))
        group = player->GetOriginalGroup();
    if (group)
        return false;

    RemoveInvite(player);

    m_invitees.insert(player);

    player->SetGroupInvite(this);

    sScriptMgr->OnGroupInviteMember(this, player->GetGUID());

    return true;
}

bool Group::AddLeaderInvite(Player* player)
{
    if (!AddInvite(player))
        return false;

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    return true;
}

void Group::RemoveInvite(Player* player)
{
    if (player)
    {
        m_invitees.erase(player);
        player->SetGroupInvite(nullptr);
    }
}

void Group::RemoveAllInvites()
{
    for (InvitesList::iterator itr=m_invitees.begin(); itr != m_invitees.end(); ++itr)
        if (*itr)
            (*itr)->SetGroupInvite(nullptr);

    m_invitees.clear();
}

Player* Group::GetInvited(ObjectGuid guid) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetGUID() == guid)
            return (*itr);
    }
    return nullptr;
}

Player* Group::GetInvited(const std::string& name) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetName() == name)
            return (*itr);
    }
    return nullptr;
}

bool Group::AddMember(Player* player)
{
    // Get first not-full group
    uint8 subGroup = 0;
    if (m_subGroupsCounts)
    {
        bool groupFound = false;
        for (; subGroup < MAX_RAID_SUBGROUPS; ++subGroup)
        {
            if (m_subGroupsCounts[subGroup] < MAXGROUPSIZE)
            {
                groupFound = true;
                break;
            }
        }
        // We are raid group and no one slot is free
        if (!groupFound)
            return false;
    }

    MemberSlot member;
    member.guid      = player->GetGUID();
    member.name      = player->GetName();
    member.group     = subGroup;
    member.flags     = 0;
    member.roles     = 0;
    member.guildId   = player->GetGuildId();
    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subGroup);

    player->SetGroupInvite(nullptr);
    if (player->GetGroup())
    {
        if (isBGGroup() || isBFGroup()) // if player is in group and he is being added to BG raid group, then call SetBattlegroundRaid()
            player->SetBattlegroundOrBattlefieldRaid(this, subGroup);
        else //if player is in bg raid and we are adding him to normal group, then call SetOriginalGroup()
            player->SetOriginalGroup(this, subGroup);
    }
    else //if player is not in group, then call set group
        player->SetGroup(this, subGroup);

    // if the same group invites the player back, cancel the homebind timer
    player->m_InstanceValid = player->CheckInstanceValidity(false);

    if (!isRaidGroup())                                      // reset targetIcons for non-raid-groups
    {
        for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
            m_targetIcons[i].Clear();
    }

    // insert into the table if we're not a battleground group
    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP_MEMBER);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, member.guid.GetCounter());
        stmt->setUInt8(2, member.flags);
        stmt->setUInt8(3, member.group);
        stmt->setUInt8(4, member.roles);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
    SendRaidMarkerUpdateToPlayer(player->GetGUID());
    sScriptMgr->OnGroupAddMember(this, player->GetGUID());

    if (!IsLeader(player->GetGUID()) && !isBGGroup() && !isBFGroup())
    {
        // reset the new member's instances, unless he is currently in one of them
        // including raid/heroic instances that they are not permanently bound to!
        player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false);
        player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true);

        if (player->getLevel() >= LEVELREQUIREMENT_HEROIC)
        {
            if (player->GetDungeonDifficulty() != GetDungeonDifficulty())
            {
                player->SetDungeonDifficulty(GetDungeonDifficulty());
                player->SendDungeonDifficulty(true);
            }
            if (player->GetRaidDifficulty() != GetRaidDifficulty())
            {
                player->SetRaidDifficulty(GetRaidDifficulty());
                player->SendRaidDifficulty(true);
            }
        }
    }
    player->SetGroupUpdateFlag(GROUP_UPDATE_FULL);
    if (player->GetPet())
        player->SetGroupUpdateFlag(GROUP_UPDATE_PET);

    UpdatePlayerOutOfRange(player);

    // quest related GO state dependent from raid membership
    if (isRaidGroup())
        player->UpdateVisibleGameobjectsOrSpellClicks();

    player->FailAchievementCriteria(AchievementCriteriaFailEvent::ModifyPartyStatus, 0);

    {
        // Broadcast new player group member fields to rest of the group
        player->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);

        UpdateData groupData(player->GetMapId());
        WorldPacket groupDataPacket;

        // Broadcast group members' fields to player
        for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (itr->GetSource() == player)
                continue;

            if (Player* existingMember = itr->GetSource())
            {
                if (player->HaveAtClient(existingMember))
                {
                    existingMember->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                    existingMember->BuildValuesUpdateBlockForPlayer(&groupData, player);
                    existingMember->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                }

                if (existingMember->HaveAtClient(player))
                {
                    UpdateData newData(player->GetMapId());
                    WorldPacket newDataPacket;
                    player->BuildValuesUpdateBlockForPlayer(&newData, existingMember);
                    if (newData.HasData())
                    {
                        newData.BuildPacket(&newDataPacket);
                        existingMember->SendDirectMessage(&newDataPacket);
                    }
                }
            }
        }

        if (groupData.HasData())
        {
            groupData.BuildPacket(&groupDataPacket);
            player->SendDirectMessage(&groupDataPacket);
        }

        player->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
    }

    if (m_disenchantInfo.MaxDisenchantSkillLevel < player->GetSkillValue(SKILL_ENCHANTING))
    {
        m_disenchantInfo.MaxDisenchantSkillLevel = player->GetSkillValue(SKILL_ENCHANTING);
        m_disenchantInfo.DisenchanterGUID = player->GetGUID();
    }

    return true;
}

bool Group::RemoveMember(ObjectGuid guid, const RemoveMethod& method /*= GROUP_REMOVEMETHOD_DEFAULT*/, ObjectGuid kicker /*= 0*/, char const* reason /*= nullptr*/)
{
    BroadcastGroupUpdate();

    sScriptMgr->OnGroupRemoveMember(this, guid, method, kicker, reason);

    Player* player = ObjectAccessor::FindConnectedPlayer(guid);
    if (player)
    {
        for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* groupMember = itr->GetSource())
            {
                if (groupMember->GetGUID() == guid)
                    continue;

                groupMember->RemoveAllGroupBuffsFromCaster(guid);
                player->RemoveAllGroupBuffsFromCaster(groupMember->GetGUID());
            }
        }
    }

    // LFG group vote kick handled in scripts
    if (isLFGGroup() && method == GROUP_REMOVEMETHOD_KICK)
        return !m_memberSlots.empty();

    if (player)
        player->FailAchievementCriteria(AchievementCriteriaFailEvent::ModifyPartyStatus, 0);

    // remove member and change leader (if need) only if strong more 2 members _before_ member remove (BG/BF allow 1 member group)
    if (GetMembersCount() > ((isBGGroup() || isLFGGroup() || isBFGroup()) ? 1u : 2u))
    {
        if (player)
        {
            // Battleground group handling
            if (isBGGroup() || isBFGroup())
                player->RemoveFromBattlegroundOrBattlefieldRaid();
            else
            // Regular group
            {
                if (player->GetOriginalGroup() == this)
                    player->SetOriginalGroup(nullptr);
                else
                    player->SetGroup(nullptr);

                // quest related GO state dependent from raid membership
                player->UpdateVisibleGameobjectsOrSpellClicks();
            }

            WorldPacket data;

            if (method == GROUP_REMOVEMETHOD_KICK || method == GROUP_REMOVEMETHOD_KICK_LFG)
            {
                data.Initialize(SMSG_GROUP_UNINVITE, 0);
                player->SendDirectMessage(&data);
            }

            // Do we really need to send this opcode?
            SendUpdateDestroyGroupToPlayer(player);

            _homebindIfInstance(player);
        }

        // Remove player from group in DB
        if (!isBGGroup() && !isBFGroup())
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
            stmt->setUInt32(0, guid.GetCounter());
            CharacterDatabase.Execute(stmt);
            DelinkMember(guid);
        }

        // Reevaluate group enchanter if the leaving player had enchanting skill or the player is offline
        if (!player || player->GetSkillValue(SKILL_ENCHANTING))
            ResetMaxEnchantingLevel();

        // Remove player from loot rolls
        for (Rolls::iterator it = RollId.begin(); it != RollId.end(); ++it)
        {
            Roll* roll = it->get();
            Roll::PlayerVote::iterator itr2 = roll->playerVote.find(guid);
            if (itr2 == roll->playerVote.end())
                continue;

            if (itr2->second == GREED || itr2->second == DISENCHANT)
                --roll->totalGreed;
            else if (itr2->second == NEED)
                --roll->totalNeed;
            else if (itr2->second == PASS)
                --roll->totalPass;

            if (itr2->second != NOT_VALID)
                --roll->totalPlayersRolling;

            roll->playerVote.erase(itr2);

            CountRollVote(guid, roll->itemGUID, MAX_ROLL_TYPE);
        }

        // Update subgroups
        member_witerator slot = _getMemberWSlot(guid);
        if (slot != m_memberSlots.end())
        {
            SubGroupCounterDecrease(slot->group);
            m_memberSlots.erase(slot);
        }

        // Pick new leader if necessary
        if (m_leaderGuid == guid)
        {
            for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
            {
                if (ObjectAccessor::FindConnectedPlayer(itr->guid))
                {
                    ChangeLeader(itr->guid);
                    break;
                }
            }
        }

        SendUpdate();

        SendRaidMarkerUpdateToPlayer(guid, true);

        if (isLFGGroup() && GetMembersCount() == 1)
        {
            Player* leader = ObjectAccessor::FindConnectedPlayer(GetLeaderGUID());
            uint32 mapId = sLFGMgr->GetDungeonMapId(GetGUID());
            if (!mapId || !leader || (leader->IsAlive() && leader->GetMapId() != mapId))
            {
                Disband();
                return false;
            }
        }

        if (m_memberMgr.getSize() < ((isLFGGroup() || isBGGroup()) ? 1u : 2u))
            Disband();

        return true;
    }
    // If group size before player removal <= 2 then disband it
    else
    {
        Disband();
        return false;
    }
}

void Group::ChangeLeader(ObjectGuid newLeaderGuid)
{
    member_witerator slot = _getMemberWSlot(newLeaderGuid);

    if (slot == m_memberSlots.end())
        return;

    Player* newLeader = ObjectAccessor::FindConnectedPlayer(slot->guid);

    // Don't allow switching leader to offline players
    if (!newLeader)
        return;

    sScriptMgr->OnGroupChangeLeader(this, newLeaderGuid, m_leaderGuid);

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

        // Remove the groups permanent instance bindings
        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin(); itr != m_boundInstances[i].end();)
            {
                // Do not unbind saves of instances that already had map created (a newLeader entered)
                // forcing a new instance with another leader requires group disbanding (confirmed on retail)
                if (itr->second.perm && !sMapMgr->FindMap(itr->first, itr->second.save->GetInstanceId()))
                {
                    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_PERM_BINDING);
                    stmt->setUInt32(0, m_dbStoreId);
                    stmt->setUInt32(1, itr->second.save->GetInstanceId());
                    trans->Append(stmt);

                    itr->second.save->RemoveGroup(this);
                    m_boundInstances[i].erase(itr++);
                }
                else
                    ++itr;
            }
        }

        // Copy the permanent binds from the new leader to the group
        Group::ConvertLeaderInstancesToGroup(newLeader, this, true);

        // Update the group leader
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_LEADER);

        stmt->setUInt32(0, newLeader->GetGUID().GetCounter());
        stmt->setUInt32(1, m_dbStoreId);

        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);
    }

    if (Player* oldLeader = ObjectAccessor::FindConnectedPlayer(m_leaderGuid))
        oldLeader->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

    newLeader->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);
    m_leaderGuid = newLeader->GetGUID();
    m_leaderName = newLeader->GetName();
    ToggleGroupMemberFlag(slot, MEMBER_FLAG_ASSISTANT, false);

    WorldPacket data(SMSG_GROUP_SET_LEADER, m_leaderName.size()+1);
    data << slot->name;
    BroadcastPacket(&data, true);
}

/// convert the player's binds to the group
void Group::ConvertLeaderInstancesToGroup(Player* player, Group* group, bool switchLeader)
{
    // copy all binds to the group, when changing leader it's assumed the character
    // will not have any solo binds
    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
    {
        for (Player::BoundInstancesMap::iterator itr = player->m_boundInstances[i].begin(); itr != player->m_boundInstances[i].end();)
        {
            if (!switchLeader || !group->GetBoundInstance(itr->second.save->GetDifficulty(), itr->first))
                if (itr->second.extendState) // not expired
                    group->BindToInstance(itr->second.save, itr->second.perm, false);

            // permanent binds are not removed
            if (switchLeader && !itr->second.perm)
            {
                // increments itr in call
                player->UnbindInstance(itr, Difficulty(i), false);
            }
            else
                ++itr;
        }
    }

    /* if group leader is in a non-raid dungeon map and nobody is actually bound to this map then the group can "take over" the instance *
    * (example: two-player group disbanded by disconnect where the player reconnects within 60 seconds and the group is reformed)       */
    if (Map* playerMap = player->GetMap())
        if (!switchLeader && playerMap->IsNonRaidDungeon())
            if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(playerMap->GetInstanceId()))
                if (save->GetGroupCount() == 0 && save->GetPlayerCount() == 0)
                {
                    TC_LOG_DEBUG("maps", "Group::ConvertLeaderInstancesToGroup: Group for player %s is taking over unbound instance map %d with Id %d", player->GetName().c_str(), playerMap->GetId(), playerMap->GetInstanceId());
                    // if nobody is saved to this, then the save wasn't permanent
                    group->BindToInstance(save, false, false);
                }
}

void Group::Disband(bool hideDestroy /* = false */)
{
    sScriptMgr->OnGroupDisband(this);

    Player* player;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player)
            continue;

        //we cannot call _removeMember because it would invalidate member iterator
        //if we are removing player from battleground raid
        if (isBGGroup() || isBFGroup())
            player->RemoveFromBattlegroundOrBattlefieldRaid();
        else
        {
            //we can remove player who is in battleground from his original group
            if (player->GetOriginalGroup() == this)
                player->SetOriginalGroup(nullptr);
            else
                player->SetGroup(nullptr);
        }

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateVisibleGameobjectsOrSpellClicks();

        if (!player->GetSession())
            continue;

        WorldPacket data;
        if (!hideDestroy)
        {
            data.Initialize(SMSG_GROUP_DESTROYED, 0);
            player->SendDirectMessage(&data);
        }

        //we already removed player from group and in player->GetGroup() is his original group, send update
        if (Group* group = player->GetGroup())
            group->SendUpdate();
        else
            SendUpdateDestroyGroupToPlayer(player);

        _homebindIfInstance(player);

    }

    SetGroupMarkerMask(0x20);
    SendRaidMarkerUpdate();
    RemoveMarker();
    RemoveAllMarkerFromList();

    RollId.clear();
    m_memberSlots.clear();

    RemoveAllInvites();

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER_ALL);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);

        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, false, nullptr);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, nullptr);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);
        stmt->setUInt32(0, m_dbStoreId);
        CharacterDatabase.Execute(stmt);

        sGroupMgr->FreeGroupDbStoreId(this);
    }

    sGroupMgr->RemoveGroup(this);
    delete this;
}

/*********************************************************/
/***                   LOOT SYSTEM                     ***/
/*********************************************************/

void Group::SendLootStartRoll(uint32 countDown, uint32 mapid, Roll const& r)
{
    WorldPacket data(SMSG_LOOT_START_ROLL, (8+4+4+4+4+4+4+1));
    data << uint64(r.itemGUID);                             // guid of rolled item
    data << uint32(mapid);                                  // 3.3.3 mapid
    data << uint32(r.itemSlot);                             // itemslot
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId.Id);                  // item random property ID
    data << uint32(r.itemCount);                            // items in stack
    data << uint32(countDown);                              // the countdown time to choose "need" or "greed"
    data << uint8(r.rollVoteMask);                          // roll type mask
    data << uint8(r.totalPlayersRolling);                   // maybe the number of players rolling for it???

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second == NOT_EMITED_YET)
            p->SendDirectMessage(&data);
    }
}

void Group::SendLootStartRollToPlayer(uint32 countDown, uint32 mapId, Player* p, bool canNeed, Roll const& r)
{
    if (!p || !p->GetSession())
        return;

    WorldPacket data(SMSG_LOOT_START_ROLL, (8 + 4 + 4 + 4 + 4 + 4 + 4 + 1));
    data << uint64(r.itemGUID);                             // guid of rolled item
    data << uint32(mapId);                                  // 3.3.3 mapid
    data << uint32(r.itemSlot);                             // itemslot
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId.Id);                  // item random property ID
    data << uint32(r.itemCount);                            // items in stack
    data << uint32(countDown);                              // the countdown time to choose "need" or "greed"
    uint8 voteMask = r.rollVoteMask;
    if (!canNeed)
        voteMask &= ~ROLL_FLAG_TYPE_NEED;
    data << uint8(voteMask);                                // roll type mask
    data << uint8(r.totalPlayersRolling);                   // maybe the number of players rolling for it???

    p->SendDirectMessage(&data);
}

void Group::SendLootRoll(ObjectGuid sourceGuid, ObjectGuid targetGuid, int32 rollNumber, uint8 rollType, Roll const& roll, bool autoPass)
{
    WorldPacket data(SMSG_LOOT_ROLL, (8+4+8+4+4+4+1+1+1));
    data << uint64(sourceGuid);                             // guid of the item rolled
    data << uint32(roll.itemSlot);                          // slot
    data << uint64(targetGuid);
    data << uint32(roll.itemid);                            // the itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomSuffix);                  // randomSuffix
    data << uint32(roll.itemRandomPropId.Id);               // Item random property ID
    data << int32(rollNumber);                              // 0: "Need for: [item name]" - 1: "you passed on: [item name]"      Roll number
    data << uint8(rollType);                                // 0: "Need for: [item name]" 0: "You have selected need for [item name] 1: need roll 2: greed roll
    data << uint8(autoPass);                                // 1: "You automatically passed on: %s because you cannot loot that item." - Possibly used in need befor greed

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->SendDirectMessage(&data);
    }
}

void Group::SendLootRollWon(ObjectGuid sourceGuid, ObjectGuid targetGuid, int32 rollNumber, uint8 rollType, Roll const& roll)
{
    WorldPacket data(SMSG_LOOT_ROLL_WON, (8+4+4+4+4+8+1+1));
    data << uint64(sourceGuid);                             // guid of the item rolled
    data << uint32(roll.itemSlot);                          // slot
    data << uint32(roll.itemid);                            // the itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomSuffix);                  // randomSuffix
    data << uint32(roll.itemRandomPropId.Id);               // Item random property
    data << uint64(targetGuid);                             // guid of the player who won.
    data << int32(rollNumber);                              // rollnumber realted to SMSG_LOOT_ROLL
    data << uint8(rollType);                                // rollType related to SMSG_LOOT_ROLL

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            p->SendDirectMessage(&data);
    }
}

void Group::SendLootAllPassed(Roll const& roll)
{
    WorldPacket data(SMSG_LOOT_ALL_PASSED, (8+4+4+4+4));
    data << uint64(roll.itemGUID);                             // Guid of the item rolled
    data << uint32(roll.itemSlot);                             // Item loot slot
    data << uint32(roll.itemid);                               // The itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomPropId.Id);                  // Item random property ID
    data << uint32(roll.itemRandomSuffix);                     // Item random suffix ID

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(itr->first);
        if (!player || !player->GetSession())
            continue;

        if (itr->second != NOT_VALID)
            player->SendDirectMessage(&data);
    }
}

// notify group members which player is the allowed looter for the given creature
void Group::SendLooter(Creature* creature, Player* groupLooter)
{
    ASSERT(creature);

    WorldPackets::Loot::LootList lootList;

    lootList.Owner = creature->GetGUID();

    if (GetLootMethod() == MASTER_LOOT && creature->loot.hasOverThresholdItem())
        lootList.Master = GetMasterLooterGuid();

    if (groupLooter)
        lootList.RoundRobinWinner = groupLooter->GetGUID();

    BroadcastPacket(lootList.Write(), false);
}

bool CanRollOnItem(const LootItem& item, Player const* player)
{
    // Players can't roll on unique items if they already reached the maximum quantity of that item
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(item.itemid);
    if (!proto)
        return false;

    uint32 itemCount = player->GetItemCount(item.itemid);
    if (proto->GetMaxCount() > 0 && itemCount >= proto->GetMaxCount())
        return false;

    if (!item.AllowedForPlayer(player))
        return false;

    return true;
}

void Group::GroupLoot(Loot* loot, WorldObject* pLootedObject)
{
    std::vector<LootItem>::iterator i;
    ItemTemplate const* item;
    uint8 itemSlot = 0;

    for (i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //TC_LOG_DEBUG("misc", "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        //roll for over-threshold item if it's one-player loot
        if (item->GetQuality() >= uint32(m_lootThreshold))
        {
            ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>().Generate());

            std::unique_ptr<Roll> r = std::make_unique<Roll>(newitemGUID, *i);

            //a vector is filled with only near party members
            for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member || !member->GetSession())
                    continue;
                if (member->IsAtGroupRewardDistance(pLootedObject))
                {
                    r->totalPlayersRolling++;
                    RollVote vote = member->GetPassOnGroupLoot() ? PASS : NOT_EMITED_YET;
                    if (!CanRollOnItem(*i, member))
                    {
                        vote = PASS;
                        ++r->totalPass;
                    }
                    r->playerVote[member->GetGUID()] = vote;
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_disenchantInfo.MaxDisenchantSkillLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                loot->items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == PASS)
                            SendLootRoll(newitemGUID, p->GetGUID(), -1, ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

                RollId.push_back(std::move(r));

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = pLootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
        }
        else
            i->is_underthreshold = true;
    }

    for (i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //TC_LOG_DEBUG("misc", "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>().Generate());

        std::unique_ptr<Roll> r = std::make_unique<Roll>(newitemGUID, *i);

        //a vector is filled with only near party members
        for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member || !member->GetSession())
                continue;

            if (member->IsAtGroupRewardDistance(pLootedObject) && i->AllowedForPlayer(member))
            {
                r->totalPlayersRolling++;
                r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

            RollId.push_back(std::move(r));

            if (Creature* creature = pLootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = pLootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
    }
}

void Group::NeedBeforeGreed(Loot* loot, WorldObject* lootedObject)
{
    ItemTemplate const* item;
    uint8 itemSlot = 0;
    for (std::vector<LootItem>::iterator i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);

        //roll for over-threshold item if it's one-player loot
        if (item->GetQuality() >= uint32(m_lootThreshold))
        {
            ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>().Generate());

            std::unique_ptr<Roll> r = std::make_unique<Roll>(newitemGUID, *i);

            for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* playerToRoll = itr->GetSource();
                if (!playerToRoll || !playerToRoll->GetSession())
                    continue;

                if (playerToRoll->IsAtGroupRewardDistance(lootedObject) && i->AllowedForPlayer(playerToRoll))
                {
                    r->totalPlayersRolling++;
                    if (playerToRoll->GetPassOnGroupLoot())
                    {
                        r->playerVote[playerToRoll->GetGUID()] = PASS;
                        r->totalPass++;
                        // can't broadcast the pass now. need to wait until all rolling players are known.
                    }
                    else
                        r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_disenchantInfo.MaxDisenchantSkillLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                if (item->GetFlags2() & ITEM_FLAG2_CAN_ONLY_ROLL_GREED)
                    r->rollVoteMask &= ~ROLL_FLAG_TYPE_NEED;

                loot->items[itemSlot].is_blocked = true;

                //Broadcast Pass and Send Rollstart
                for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                {
                    Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
                    if (!p || !p->GetSession())
                        continue;

                    if (itr->second == PASS)
                        SendLootRoll(newitemGUID, p->GetGUID(), -1, ROLL_PASS, *r);
                    else
                        SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollNeedForItem(item), *r);
                }

                RollId.push_back(std::move(r));

                if (Creature* creature = lootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = lootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
        }
        else
            i->is_underthreshold = true;
    }

    for (std::vector<LootItem>::iterator i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        ObjectGuid newitemGUID = ObjectGuid::Create<HighGuid::Item>(sObjectMgr->GetGenerator<HighGuid::Item>().Generate());

        std::unique_ptr<Roll> r = std::make_unique<Roll>(newitemGUID, *i);

        for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* playerToRoll = itr->GetSource();
            if (!playerToRoll || !playerToRoll->GetSession())
                continue;

            if (playerToRoll->IsAtGroupRewardDistance(lootedObject) && i->AllowedForPlayer(playerToRoll))
            {
                r->totalPlayersRolling++;
                r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            //Broadcast Pass and Send Rollstart
            for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
            {
                Player* p = ObjectAccessor::FindConnectedPlayer(itr->first);
                if (!p || !p->GetSession())
                    continue;

                if (itr->second == PASS)
                    SendLootRoll(newitemGUID, p->GetGUID(), -1, ROLL_PASS, *r);
                else
                    SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollNeedForItem(item) == EQUIP_ERR_OK, *r);
            }

            RollId.push_back(std::move(r));

            if (Creature* creature = lootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = lootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
    }
}

void Group::MasterLoot(Loot* loot, WorldObject* pLootedObject)
{
    TC_LOG_DEBUG("network", "Group::MasterLoot (SMSG_LOOT_MASTER_LIST)");

    for (std::vector<LootItem>::iterator i = loot->items.begin(); i != loot->items.end(); ++i)
    {
        if (i->freeforall)
            continue;

        i->is_blocked = !i->is_underthreshold;
    }

    for (std::vector<LootItem>::iterator i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i)
    {
        if (!i->follow_loot_rules)
            continue;

        i->is_blocked = !i->is_underthreshold;
    }

    uint32 real_count = 0;

    WorldPacket data(SMSG_LOOT_MASTER_LIST, 1 + GetMembersCount() * 8);
    data << uint8(GetMembersCount());

    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* looter = itr->GetSource();
        if (!looter->IsInWorld())
            continue;

        if (looter->IsAtGroupRewardDistance(pLootedObject))
        {
            data << uint64(looter->GetGUID());
            ++real_count;
        }
    }

    data.put<uint8>(0, real_count);

    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* looter = itr->GetSource();
        if (looter->IsAtGroupRewardDistance(pLootedObject))
            looter->SendDirectMessage(&data);
    }
}

void Group::CountRollVote(ObjectGuid playerGUID, ObjectGuid Guid, uint8 Choice)
{
    Rolls::iterator rollI = GetRoll(Guid);
    if (rollI == RollId.end())
        return;
    Roll* roll = rollI->get();

    Roll::PlayerVote::iterator itr = roll->playerVote.find(playerGUID);
    // this condition means that player joins to the party after roll begins
    if (itr == roll->playerVote.end())
        return;

    if (roll->getLoot())
        if (roll->getLoot()->items.empty())
            return;

    switch (Choice)
    {
        case ROLL_PASS:                                     // Player choose pass
            SendLootRoll(ObjectGuid::Empty, playerGUID, -1, ROLL_PASS, *roll);
            ++roll->totalPass;
            itr->second = PASS;
            break;
        case ROLL_NEED:                                     // player choose Need
            SendLootRoll(ObjectGuid::Empty, playerGUID, 0, 0, *roll);
            ++roll->totalNeed;
            itr->second = NEED;
            break;
        case ROLL_GREED:                                    // player choose Greed
            SendLootRoll(ObjectGuid::Empty, playerGUID, -7, ROLL_GREED, *roll);
            ++roll->totalGreed;
            itr->second = GREED;
            break;
        case ROLL_DISENCHANT:                               // player choose Disenchant
            SendLootRoll(ObjectGuid::Empty, playerGUID, -8, ROLL_DISENCHANT, *roll);
            ++roll->totalGreed;
            itr->second = DISENCHANT;
            break;
    }

    if (roll->totalPass + roll->totalNeed + roll->totalGreed >= roll->totalPlayersRolling)
        CountTheRoll(rollI, nullptr);
}

//called when roll timer expires
void Group::EndRoll(Loot* pLoot, Map* allowedMap)
{
    for (Rolls::iterator itr = RollId.begin(); itr != RollId.end();)
    {
        if ((*itr)->getLoot() == pLoot) {
            CountTheRoll(itr, allowedMap);           //i don't have to edit player votes, who didn't vote ... he will pass
            itr = RollId.begin();
        }
        else
            ++itr;
    }
}

void Group::CountTheRoll(Rolls::iterator rollI, Map* allowedMap)
{
    Roll* roll = rollI->get();
    if (!roll->isValid())                                   // is loot already deleted ?
    {
        RollId.erase(rollI);
        delete roll;
        return;
    }

    //end of the roll
    if (roll->totalNeed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            ObjectGuid maxguid = ObjectGuid::Empty;
            Player* player = nullptr;

            for (Roll::PlayerVote::const_iterator itr=roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != NEED)
                    continue;

                player = ObjectAccessor::FindPlayer(itr->first);
                if (!player || (allowedMap != nullptr && player->FindMap() != allowedMap))
                {
                    --roll->totalNeed;
                    continue;
                }

                uint8 randomN = urand(1, 100);
                SendLootRoll(ObjectGuid::Empty, itr->first, randomN, ROLL_NEED, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                }
            }

            if (!maxguid.IsEmpty())
            {
                SendLootRollWon(ObjectGuid::Empty, maxguid, maxresul, ROLL_NEED, *roll);
                player = ObjectAccessor::FindConnectedPlayer(maxguid);

                if (player && player->GetSession())
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT, roll->itemid, maxresul);

                    ItemPosCountVec dest;
                    LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
                    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                    if (msg == EQUIP_ERR_OK)
                    {
                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->unlootedCount--;
                        player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, item->GetAllowedLooters());
                    }
                    else
                    {
                        item->is_blocked = false;
                        item->rollWinnerGUID = player->GetGUID();
                        player->SendEquipError(msg, nullptr, nullptr, roll->itemid);
                    }
                }
            }
            else
                roll->totalNeed = 0;
        }
    }

    if (roll->totalNeed == 0 && roll->totalGreed > 0) // if (roll->totalNeed == 0 && ...), not else if, because numbers can be modified above if player is on a different map
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            ObjectGuid maxguid = ObjectGuid::Empty;
            Player* player = nullptr;
            RollVote rollvote = NOT_VALID;

            Roll::PlayerVote::iterator itr;
            for (itr = roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != GREED && itr->second != DISENCHANT)
                    continue;

                player = ObjectAccessor::FindPlayer(itr->first);
                if (!player || (allowedMap != nullptr && player->FindMap() != allowedMap))
                {
                    --roll->totalGreed;
                    continue;
                }

                uint8 randomN = urand(1, 100);
                SendLootRoll(ObjectGuid::Empty, itr->first, randomN, itr->second, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                    rollvote = itr->second;
                }
            }

            if (!maxguid.IsEmpty())
            {
                SendLootRollWon(ObjectGuid::Empty, maxguid, maxresul, rollvote, *roll);
                player = ObjectAccessor::FindConnectedPlayer(maxguid);

                if (player && player->GetSession())
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT, roll->itemid, maxresul);

                    LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);

                    if (rollvote == GREED)
                    {
                        ItemPosCountVec dest;
                        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                        if (msg == EQUIP_ERR_OK)
                        {
                            item->is_looted = true;
                            roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                            roll->getLoot()->unlootedCount--;
                            player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, item->GetAllowedLooters());
                        }
                        else
                        {
                            item->is_blocked = false;
                            item->rollWinnerGUID = player->GetGUID();
                            player->SendEquipError(msg, nullptr, nullptr, roll->itemid);
                        }
                    }
                    else if (rollvote == DISENCHANT)
                    {
                        WorldPackets::Loot::DisenchantCredit disenchantCredit;
                        disenchantCredit.Disenchanter = m_disenchantInfo.DisenchanterGUID;
                        disenchantCredit.Item.ItemID = item->itemid;
                        disenchantCredit.Item.RandomPropertiesID = item->randomPropertyId.Type == ItemRandomEnchantmentType::Property ? item->randomPropertyId.Id : -int32(item->randomPropertyId.Id);
                        disenchantCredit.Item.RandomPropertiesSeed = item->randomSuffix;
                        BroadcastPacket(disenchantCredit.Write(), false);

                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->unlootedCount--;
                        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(roll->itemid);
                        ASSERT(pProto);
                        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, 13262); // Disenchant

                        ItemPosCountVec dest;
                        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                        if (msg == EQUIP_ERR_OK)
                            player->AutoStoreLoot(pProto->DisenchantID, LootTemplates_Disenchant, true);
                        else // If the player's inventory is full, send the disenchant result in a mail.
                        {
                            Loot loot;
                            loot.FillLoot(pProto->DisenchantID, LootTemplates_Disenchant, player, true);

                            uint32 max_slot = loot.GetMaxSlotInLootFor(player);
                            for (uint32 i = 0; i < max_slot; ++i)
                            {
                                LootItem* lootItem = loot.LootItemInSlot(i, player);
                                player->SendEquipError(msg, nullptr, nullptr, lootItem->itemid);
                                player->SendItemRetrievalMail(lootItem->itemid, lootItem->count);
                            }
                        }
                    }
                }
            }
            else
                roll->totalGreed = 0;
        }
    }

    if (roll->totalNeed == 0 && roll->totalGreed == 0) // if, not else, because numbers can be modified above if player is on a different map
    {
        SendLootAllPassed(*roll);

        // remove is_blocked so that the item is lootable by all players
        LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
        item->is_blocked = false;
    }

    RollId.erase(rollI);
    delete roll;
}

void Group::SetTargetIcon(uint8 id, ObjectGuid whoGuid, ObjectGuid targetGuid)
{
    if (id >= TARGETICONCOUNT)
        return;

    // clean other icons
    if (targetGuid)
        for (int i=0; i<TARGETICONCOUNT; ++i)
            if (m_targetIcons[i] == targetGuid)
                SetTargetIcon(i, ObjectGuid::Empty, ObjectGuid::Empty);

    m_targetIcons[id] = targetGuid;

    WorldPacket data(MSG_RAID_TARGET_UPDATE, (1+8+1+8));
    data << uint8(0);                                       // set targets
    data << uint64(whoGuid);
    data << uint8(id);
    data << uint64(targetGuid);
    BroadcastPacket(&data, true);
}

void Group::SendTargetIconList(WorldSession* session)
{
    if (!session)
        return;

    WorldPacket data(MSG_RAID_TARGET_UPDATE, (1+TARGETICONCOUNT*9));
    data << uint8(1);                                       // list targets

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
    {
        if (m_targetIcons[i].IsEmpty())
            continue;

        data << uint8(i);
        data << uint64(m_targetIcons[i]);
    }

    session->SendPacket(&data);
}

void Group::SendRaidMarkerUpdate()
{
    WorldPacket data(SMSG_RAID_MARKERS_CHANGED, 4);
    data << uint32(m_markerMask);

    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindPlayer(itr->guid);
        if (!player || !player->GetSession())
            continue;

        player->SendDirectMessage(&data);
    }
}

void Group::SendRaidMarkerUpdateToPlayer(ObjectGuid playerGUID, bool remove)
{
    Player* player = ObjectAccessor::FindPlayer(playerGUID);
    if (!player || !player->GetSession())
        return;

    WorldPacket data(SMSG_RAID_MARKERS_CHANGED, 4);
    data << uint32(remove ? 0 : m_markerMask);
    player->SendDirectMessage(&data);
}

DynamicObject* Group::GetRaidMarkerBySpellId(uint32 spellId)
{
    for (RaidMarkerInfo itr : m_raidMarkers)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr.summonerGuid))
            if (DynamicObject* dynObject = ObjectAccessor::GetDynamicObject(*player, itr.markerGuid))
                if (dynObject->GetEntry() == spellId)
                    return dynObject;
    }

    return nullptr;
}

void Group::RemoveRaidMarkerFromList(ObjectGuid markerGuid)
{
    for (RaidMarkerList::const_iterator itr = m_raidMarkers.begin(); itr != m_raidMarkers.end();)
    {
        if (itr->markerGuid == markerGuid)
        {
            m_raidMarkers.erase(itr);
            itr = m_raidMarkers.begin();
        }
        else
            itr++;
    }
}

void Group::RemoveMarker()
{
    for (uint32 i = 0; i < 5; ++i)
    {
        uint32 mask = 1 << i;
        uint32 spellId = SPELL_RAID_MARKER + i;

        if (HasMarker(mask))
            continue;

        if (DynamicObject* dynObject = GetRaidMarkerBySpellId(spellId))
        {
            RemoveRaidMarkerFromList(dynObject->GetGUID());
            dynObject->RemoveFromWorld();
        }
    }
}

void Group::SendUpdate()
{
    for (member_witerator witr = m_memberSlots.begin(); witr != m_memberSlots.end(); ++witr)
        SendUpdateToPlayer(witr->guid, &(*witr));
}

void Group::SendUpdateToPlayer(ObjectGuid playerGUID, MemberSlot* slot)
{
    Player* player = ObjectAccessor::FindConnectedPlayer(playerGUID);

    if (!player || !player->GetSession() || player->GetGroup() != this)
        return;

    // if MemberSlot wasn't provided
    if (!slot)
    {
        member_witerator witr = _getMemberWSlot(playerGUID);

        if (witr == m_memberSlots.end()) // if there is no MemberSlot for such a player
            return;

        slot = &(*witr);
    }

    WorldPackets::Party::PartyUpdate partyUpdate;
    partyUpdate.PartyFlags = m_groupFlags;
    partyUpdate.Subgroup = slot->group;
    partyUpdate.Flags = slot->flags;
    partyUpdate.RolesAssigned = slot->roles;

    if (isLFGGroup())
    {
        partyUpdate.LfgInfo.MyLfgFlags = sLFGMgr->GetState(m_guid) == lfg::LFG_STATE_FINISHED_DUNGEON ? 2 : 0; // Todo: 'saved' status (1)
        partyUpdate.LfgInfo.LfgSlot = sLFGMgr->GetLFGDungeonEntry(sLFGMgr->GetDungeon(m_guid));
        partyUpdate.LfgInfo.LfgAborted = false; // todo
    }

    partyUpdate.PartyGUID = m_guid;
    partyUpdate.LeaderGUID = m_leaderGuid;
    partyUpdate.SequenceNum = m_counter++;

    for (MemberSlot const& memberSlot : m_memberSlots)
    {
        if (slot->guid == memberSlot.guid)
            continue;

        WorldPackets::Party::PartyPlayerInfo& playerInfo = partyUpdate.PlayerList.emplace_back();

        Player* member = ObjectAccessor::FindConnectedPlayer(memberSlot.guid);
        playerInfo.Connected = (member && !member->GetSession()->PlayerLogout()) ? MEMBER_STATUS_ONLINE : MEMBER_STATUS_OFFLINE;
        playerInfo.Connected |= ((isBGGroup() || isBFGroup()) ? MEMBER_STATUS_PVP : 0);

        playerInfo.Name = memberSlot.name;
        playerInfo.Guid = memberSlot.guid;
        playerInfo.Subgroup = memberSlot.group;
        playerInfo.Flags = memberSlot.flags;
        playerInfo.RolesAssigned = memberSlot.roles;
    }

    if (!partyUpdate.PlayerList.empty())
    {
        partyUpdate.LootSettings.LootMethod = m_lootMethod;
        partyUpdate.LootSettings.LootMaster = m_lootMethod == MASTER_LOOT ? m_masterLooterGuid : ObjectGuid::Empty;
        partyUpdate.LootSettings.LootThreshold = m_lootThreshold;

        partyUpdate.DifficultySettings.DungeonDifficulty = m_dungeonDifficulty;
        partyUpdate.DifficultySettings.RaidDifficulty = m_raidDifficulty;
    }

    player->SendDirectMessage(partyUpdate.Write());
}

void Group::SendUpdateDestroyGroupToPlayer(Player* player)
{
    WorldPackets::Party::PartyUpdate partyUpdate;
    partyUpdate.PartyFlags = GROUP_FLAG_DESTROYED;
    partyUpdate.PartyGUID = m_guid;
    partyUpdate.SequenceNum = m_counter++;
    player->SendDirectMessage(partyUpdate.Write());
}

void Group::UpdatePlayerOutOfRange(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    WorldPackets::Party::PartyMemberState packet;
    packet.Initialize(player);
    packet.Write();

    Player* member;
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        member = itr->GetSource();
        if (member && member != player && (!member->IsInMap(player) || !member->IsWithinDist(player, member->GetSightRange(), false)))
            member->SendDirectMessage(packet.GetRawPacket());
    }
}

void Group::BroadcastAddonMessagePacket(WorldPacket const* packet, const std::string& prefix, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player || (ignore != 0 && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (WorldSession* session = player->GetSession())
            if (session && (group == -1 || itr->getSubGroup() == group))
                if (session->IsAddonRegistered(prefix))
                    session->SendPacket(packet);
    }
}

void Group::BroadcastPacket(WorldPacket const* packet, bool ignorePlayersInBGRaid, int group, ObjectGuid ignoredPlayer)
{
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player || (!ignoredPlayer.IsEmpty() && player->GetGUID() == ignoredPlayer) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (player->GetSession() && (group == -1 || itr->getSubGroup() == group))
            player->SendDirectMessage(packet);
    }
}

void Group::BroadcastReadyCheck(WorldPacket const* packet)
{
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (player && player->GetSession())
            if (IsLeader(player->GetGUID()) || IsAssistant(player->GetGUID()))
                player->SendDirectMessage(packet);
    }
}

void Group::OfflineReadyCheck()
{
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || !player->GetSession())
        {
            WorldPacket data(MSG_RAID_READY_CHECK_CONFIRM, 9);
            data << uint64(citr->guid);
            data << uint8(0);
            BroadcastReadyCheck(&data);
        }
    }
}

bool Group::_setMembersGroup(ObjectGuid guid, uint8 group)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return false;

    slot->group = group;

    SubGroupCounterIncrease(group);

    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, guid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }

    return true;
}

bool Group::IsGuildGroupFor(Player* player)
{
    if (!IsMember(player->GetGUID()))
        return false;

    if (!player->GetGuildId())
        return false;

    if (!player->GetMap()->IsDungeon() &&
        !(player->InArena() && player->GetBattleground()->isRated()))
        return false;

    if (GetMembersCountOfGuild(player->GetGuildId()) < GetNeededMembersOfSameGuild((player->InArena() && player->GetBattleground()->isRated())
        ? player->GetBattleground()->GetArenaType() : 0, player->GetMap()))
        return false;

    return true;
}

uint32 Group::GetMembersCountOfGuild(uint32 guildId)
{
    uint32 count = 0;

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guildId == guildId)
            count++;

    return count;
}

uint32 Group::GetNeededMembersOfSameGuild(uint8 arenaType, Map const *map)
{
    // For arena (100% of member)
    if (arenaType && map->IsBattleArena())
        return arenaType;

    if (map->IsNonRaidDungeon())
        return 3;

    if (map->IsRaid())
    {
        if (map->GetEntry()->Expansion() == 0) // classic
            return 10;

        if (map->GetEntry()->Expansion() == 1) // TBC
            return 8;

        if (map->Is25ManRaid())
            return 20;

        return 8;
    }

    return 0;
}

float Group::GetGuildXpRateForPlayer(Player* player)
{
    if (!IsMember(player->GetGUID()))
        return 0.0f;

    if (!player->GetGuildId())
        return 0.0f;

    if (!player->GetMap()->IsDungeon() &&
        !(player->InArena() && player->GetBattleground()->isRated()))
        return 0.0f;

    if (player->GetMap()->IsNonRaidDungeon())
    {
        switch (GetMembersCountOfGuild(player->GetGuildId()))
        {
            case 3:
                return 0.5f;
            case 4:
                return 1.0f;
            case 5:
                return 1.25f;
            default:
                return 0.0f;
        }
    }

    return 1.0f;
}

bool Group::MemberLevelIsInRange(uint32 levelMin, uint32 levelMax)
{
    for (auto itr : m_memberSlots)
        if (Player* member = ObjectAccessor::FindPlayer(itr.guid))
            if (member->getLevel() < levelMin || member->getLevel() > levelMax)
                return false;

    return true;
}

void Group::UpdateGuildFor(ObjectGuid guid, uint32 guildId)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (itr->guid == guid)
            itr->guildId = guildId; // Change guild ID

                                    // Update guild flag
        if (itr->guildId == guildId)
            if (Player* member = ObjectAccessor::FindPlayer(itr->guid))
                if (Guild* guild = member->GetGuild())
                    guild->HandleGuildPartyRequest(member->GetSession());
    }
}

bool Group::SameSubGroup(Player const* member1, Player const* member2) const
{
    if (!member1 || !member2)
        return false;

    if (member1->GetGroup() != this || member2->GetGroup() != this)
        return false;
    else
        return member1->GetSubGroup() == member2->GetSubGroup();
}

// Allows setting sub groups both for online or offline members
void Group::ChangeMembersGroup(ObjectGuid guid, uint8 group)
{
    // Only raid groups have sub groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Abort if the player is already in the target sub group
    uint8 prevSubGroup = GetMemberGroup(guid);
    if (prevSubGroup == group)
        return;

    // Update the player slot with the new sub group setting
    slot->group = group;

    // Increase the counter of the new sub group..
    SubGroupCounterIncrease(group);

    // ..and decrease the counter of the previous one
    SubGroupCounterDecrease(prevSubGroup);

    // Preserve new sub group in database for non-raid groups
    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, guid.GetCounter());

        CharacterDatabase.Execute(stmt);
    }

    // In case the moved player is online, update the player object with the new sub group references
    if (Player* player = ObjectAccessor::FindConnectedPlayer(guid))
    {
        if (player->GetGroup() == this)
            player->GetGroupRef().setSubGroup(group);
        else
        {
            // If player is in BG raid, it is possible that he is also in normal raid - and that normal raid is stored in m_originalGroup reference
            prevSubGroup = player->GetOriginalSubGroup();
            player->GetOriginalGroupRef().setSubGroup(group);
        }
    }

    // Broadcast the changes to the group
    SendUpdate();
}

// Retrieve the next Round-Roubin player for the group
//
// No update done if loot method is FFA.
//
// If the RR player is not yet set for the group, the first group member becomes the round-robin player.
// If the RR player is set, the next player in group becomes the round-robin player.
//
// If ifneed is true,
//      the current RR player is checked to be near the looted object.
//      if yes, no update done.
//      if not, he loses his turn.
void Group::UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed)
{
    // round robin style looting applies for all low
    // quality items in each loot method except free for all
    if (GetLootMethod() == FREE_FOR_ALL)
        return;

    ObjectGuid oldLooterGUID = GetLooterGuid();
    member_citerator guid_itr = _getMemberCSlot(oldLooterGUID);
    if (guid_itr != m_memberSlots.end())
    {
        if (ifneed)
        {
            // not update if only update if need and ok
            Player* looter = ObjectAccessor::FindPlayer(guid_itr->guid);
            if (looter && looter->IsAtGroupRewardDistance(pLootedObject))
                return;
        }
        ++guid_itr;
    }

    // search next after current
    Player* pNewLooter = nullptr;
    for (member_citerator itr = guid_itr; itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
            if (player->IsAtGroupRewardDistance(pLootedObject))
            {
                pNewLooter = player;
                break;
            }
    }

    if (!pNewLooter)
    {
        // search from start
        for (member_citerator itr = m_memberSlots.begin(); itr != guid_itr; ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
                if (player->IsAtGroupRewardDistance(pLootedObject))
                {
                    pNewLooter = player;
                    break;
                }
        }
    }

    if (pNewLooter)
    {
        if (oldLooterGUID != pNewLooter->GetGUID())
        {
            SetLooterGuid(pNewLooter->GetGUID());
            SendUpdate();
        }
    }
    else
    {
        SetLooterGuid(ObjectGuid::Empty);
        SendUpdate();
    }
}

GroupJoinBattlegroundResult Group::CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 /*MaxPlayerCount*/, bool isRated, uint32 arenaSlot)
{
    // check if this group is LFG group
    if (isLFGGroup())
        return ERR_LFG_CANT_USE_BATTLEGROUND;

    BattlemasterListEntry const* bgEntry = sBattlemasterListStore.LookupEntry(bgOrTemplate->GetTypeID());
    if (!bgEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;            // shouldn't happen

    // check for min / max count
    uint32 memberscount = GetMembersCount();

    if (memberscount > bgEntry->MaxGroupSize)                // no MinPlayerCount for battlegrounds
        return ERR_BATTLEGROUND_NONE;                        // ERR_GROUP_JOIN_BATTLEGROUND_TOO_MANY handled on client side

    // get a player as reference, to compare other players' stats to (arena team id, queue id based on level, etc.)
    Player* reference = ASSERT_NOTNULL(GetFirstMember())->GetSource();
    // no reference found, can't join this way
    if (!reference)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    PvPDifficultyEntry const* bracketEntry = sDBCManager.GetBattlegroundBracketByLevel(bgOrTemplate->GetMapId(), reference->getLevel());
    if (!bracketEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    uint32 arenaTeamId = reference->GetArenaTeamId(arenaSlot);
    uint32 team = reference->GetTeam();

    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // check every member of the group to be able to join
    memberscount = 0;
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next(), ++memberscount)
    {
        Player* member = itr->GetSource();
        // offline member? don't let join
        if (!member)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't allow cross-faction join as group
        if (member->GetTeam() != team)
            return ERR_BATTLEGROUND_JOIN_TIMED_OUT;
        // not in the same battleground level braket, don't let join
        PvPDifficultyEntry const* memberBracketEntry = sDBCManager.GetBattlegroundBracketByLevel(bracketEntry->MapID, member->getLevel());
        if (memberBracketEntry != bracketEntry)
            return ERR_BATTLEGROUND_JOIN_RANGE_INDEX;
        // don't let join rated matches if the arena team id doesn't match
        if (isRated && member->GetArenaTeamId(arenaSlot) != arenaTeamId)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't let join if someone from the group is already in that bg queue
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeId))
            return ERR_BATTLEGROUND_JOIN_FAILED;            // not blizz-like
        // don't let join if someone from the group is in bg queue random
        if (bgOrTemplate->GetTypeID() != BATTLEGROUND_AA && member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeIdRandom))
            return ERR_IN_RANDOM_BG;
        // don't let join to bg queue random if someone from the group is already in bg queue
        if (bgOrTemplate->GetTypeID() == BATTLEGROUND_RB && member->InBattlegroundQueue())
            return ERR_IN_NON_RANDOM_BG;
        // check for deserter debuff in case not arena queue
        if (bgOrTemplate->GetTypeID() != BATTLEGROUND_AA && !member->CanJoinToBattleground(bgOrTemplate))
            return ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
        // check if member can join any more battleground queues
        if (!member->HasFreeBattlegroundQueueId())
            return ERR_BATTLEGROUND_TOO_MANY_QUEUES;        // not blizz-like
        // check if someone in party is using dungeon system
        if (member->isUsingLfg())
            return ERR_LFG_CANT_USE_BATTLEGROUND;
        // check Freeze debuff
        if (member->HasAura(9454))
            return ERR_BATTLEGROUND_JOIN_FAILED;
    }

    // only check for MinPlayerCount since MinPlayerCount == MaxPlayerCount for arenas...
    if (bgOrTemplate->isArena() && memberscount != MinPlayerCount)
        return ERR_ARENA_TEAM_PARTY_SIZE;

    return ERR_BATTLEGROUND_NONE;
}

//===================================================
//============== Roll ===============================
//===================================================

void Roll::targetObjectBuildLink()
{
    // called from link()
    getTarget()->addLootValidatorRef(this);
}

void Group::SetDungeonDifficulty(Difficulty difficulty)
{
    m_dungeonDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_dungeonDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player->GetSession())
            continue;

        player->SetDungeonDifficulty(difficulty);
        player->SendDungeonDifficulty(true);
    }
}

void Group::SetRaidDifficulty(Difficulty difficulty)
{
    m_raidDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_raidDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player->GetSession())
            continue;

        player->SetRaidDifficulty(difficulty);
        player->SendRaidDifficulty(true);
    }
}

bool Group::InCombatToInstance(uint32 instanceId)
{
    for (GroupReference* itr = GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (player && player->GetInstanceId() == instanceId && !player->getAttackers().empty() && (player->GetMap()->IsRaidOrHeroicDungeon()))
            for (std::set<Unit*>::const_iterator i = player->getAttackers().begin(); i != player->getAttackers().end(); ++i)
                if ((*i) && (*i)->GetTypeId() == TYPEID_UNIT && (*i)->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
                    return true;
    }
    return false;
}

void Group::ResetInstances(uint8 method, bool isRaid, Player* SendMsgTo)
{
    if (isBGGroup() || isBFGroup())
        return;

    // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY, INSTANCE_RESET_GROUP_DISBAND

    // we assume that when the difficulty changes, all instances that can be reset will be
    Difficulty diff = GetDifficulty(isRaid);

    for (BoundInstancesMap::iterator itr = m_boundInstances[diff].begin(); itr != m_boundInstances[diff].end();)
    {
        InstanceSave* instanceSave = itr->second.save;
        const MapEntry* entry = sMapStore.LookupEntry(itr->first);
        if (!entry || entry->IsRaid() != isRaid || (!instanceSave->CanReset() && method != INSTANCE_RESET_GROUP_DISBAND))
        {
            ++itr;
            continue;
        }

        if (method == INSTANCE_RESET_ALL)
        {
            // the "reset all instances" method can only reset normal maps
            if (entry->MapType == MAP_RAID || diff == DUNGEON_DIFFICULTY_HEROIC)
            {
                ++itr;
                continue;
            }
        }

        bool isEmpty = true;
        // if the map is loaded, reset it
        Map* map = sMapMgr->FindMap(instanceSave->GetMapId(), instanceSave->GetInstanceId());
        if (map && map->IsDungeon() && !(method == INSTANCE_RESET_GROUP_DISBAND && !instanceSave->CanReset()))
        {
            if (instanceSave->CanReset())
                isEmpty = ((InstanceMap*)map)->Reset(method);
            else
                isEmpty = !map->HavePlayers();
        }

        if (SendMsgTo)
        {
            if (!isEmpty)
                SendMsgTo->SendResetInstanceFailed(0, instanceSave->GetMapId());
            else if (sWorld->getBoolConfig(CONFIG_INSTANCES_RESET_ANNOUNCE))
            {
                if (Group* group = SendMsgTo->GetGroup())
                {
                    for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                        if (Player* player = groupRef->GetSource())
                            player->SendResetInstanceSuccess(instanceSave->GetMapId());
                }

                else
                    SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
            }
            else
                SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
        }

        if (isEmpty || method == INSTANCE_RESET_GROUP_DISBAND || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // do not reset the instance, just unbind if others are permanently bound to it
            if (isEmpty && instanceSave->CanReset())
            {
                if (map && this->isRaidGroup() && map->IsDungeon() && SendMsgTo)
                {
                    AreaTriggerStruct const* instanceEntrance = sObjectMgr->GetGoBackTrigger(map->GetId());

                    if (!instanceEntrance)
                        TC_LOG_DEBUG("root", "Instance entrance not found for maps %u", map->GetId());
                    else
                    {
                        WorldLocation graveyard = WorldLocation(instanceEntrance->target_mapId, instanceEntrance->target_X, instanceEntrance->target_Y, instanceEntrance->target_Z);
                        WorldSafeLocsEntry const* graveyardLocation = sObjectMgr->GetClosestGraveyard(graveyard, SendMsgTo->GetTeam(), nullptr);
                        uint32 const zoneId = sTerrainMgr.GetZoneId(PhasingHandler::GetEmptyPhaseShift(), graveyardLocation->Continent, graveyardLocation->Loc.X, graveyardLocation->Loc.Y, graveyardLocation->Loc.Z);

                        for (const MemberSlot &member : this->GetMemberSlots())
                        {
                            if (!ObjectAccessor::FindConnectedPlayer(member.guid))
                            {
                                CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_POSITION_BY_MAPID);

                                stmt->setFloat(0, graveyardLocation->Loc.X);
                                stmt->setFloat(1, graveyardLocation->Loc.Y);
                                stmt->setFloat(2, graveyardLocation->Loc.Z);
                                stmt->setFloat(3, instanceEntrance->target_Orientation);
                                stmt->setUInt32(4, graveyardLocation->Continent);
                                stmt->setUInt32(5, zoneId);
                                stmt->setUInt32(6, member.guid);
                                stmt->setUInt32(7, map->GetId());

                                CharacterDatabase.Execute(stmt);
                            }
                        }
                    }
                }

                instanceSave->DeleteFromDB();
            }
            else
            {
                CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_INSTANCE);

                stmt->setUInt32(0, instanceSave->GetInstanceId());

                CharacterDatabase.Execute(stmt);
            }


            // i don't know for sure if hash_map iterators
            m_boundInstances[diff].erase(itr);
            itr = m_boundInstances[diff].begin();
            // this unloads the instance save unless online players are bound to it
            // (eg. permanent binds or GM solo binds)
            instanceSave->RemoveGroup(this);
        }
        else
            ++itr;
    }
}

InstanceGroupBind* Group::GetBoundInstance(Player* player)
{
    uint32 mapid = player->GetMapId();
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    return GetBoundInstance(mapEntry);
}

InstanceGroupBind* Group::GetBoundInstance(Map* aMap)
{
    // Currently spawn numbering not different from map difficulty
    Difficulty difficulty = GetDifficulty(aMap->IsRaid());
    return GetBoundInstance(difficulty, aMap->GetId());
}

InstanceGroupBind* Group::GetBoundInstance(MapEntry const* mapEntry)
{
    if (!mapEntry || !mapEntry->IsDungeon())
        return nullptr;

    Difficulty difficulty = GetDifficulty(mapEntry->IsRaid());
    return GetBoundInstance(difficulty, mapEntry->ID);
}

InstanceGroupBind* Group::GetBoundInstance(Difficulty difficulty, uint32 mapId)
{
    // some instances only have one difficulty
    sDBCManager.GetDownscaledMapDifficultyData(mapId, difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapId);
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return nullptr;
}

InstanceGroupBind* Group::BindToInstance(InstanceSave* save, bool permanent, bool load)
{
    if (!save || isBGGroup() || isBFGroup())
        return nullptr;

    InstanceGroupBind& bind = m_boundInstances[save->GetDifficulty()][save->GetMapId()];
    if (!load && (!bind.save || permanent != bind.perm || save != bind.save))
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GROUP_INSTANCE);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, save->GetInstanceId());
        stmt->setBool(2, permanent);

        CharacterDatabase.Execute(stmt);
    }

    if (bind.save != save)
    {
        if (bind.save)
            bind.save->RemoveGroup(this);
        save->AddGroup(this);
    }

    bind.save = save;
    bind.perm = permanent;
    if (!load)
        TC_LOG_DEBUG("maps", "Group::BindToInstance: %s, storage id: %u is now bound to map %d, instance %d, difficulty %d",
            GetGUID().ToString().c_str(), m_dbStoreId, save->GetMapId(), save->GetInstanceId(), save->GetDifficulty());

    return &bind;
}

void Group::UnbindInstance(uint32 mapid, uint8 difficulty, bool unload)
{
    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapid);
    if (itr != m_boundInstances[difficulty].end())
    {
        if (!unload)
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_GUID);

            stmt->setUInt32(0, m_dbStoreId);
            stmt->setUInt32(1, itr->second.save->GetInstanceId());

            CharacterDatabase.Execute(stmt);
        }

        itr->second.save->RemoveGroup(this);                // save can become invalid
        m_boundInstances[difficulty].erase(itr);
    }
}

void Group::_homebindIfInstance(Player* player)
{
    if (player && !player->IsGameMaster() && sMapStore.LookupEntry(player->GetMapId())->IsDungeon())
        player->m_InstanceValid = false;
}

void Group::BroadcastGroupUpdate(void)
{
    // FG: HACK: force flags update on group leave - for values update hack
    // -- not very efficient but safe
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pp = ObjectAccessor::FindPlayer(citr->guid);
        if (pp)
        {
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_BYTES_2);
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_FACTIONTEMPLATE);
            TC_LOG_DEBUG("misc", "-- Forced group value update for '%s'", pp->GetName().c_str());
        }
    }
}

void Group::ResetMaxEnchantingLevel()
{
    m_disenchantInfo.Initialize();
    Player* member = nullptr;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        member = ObjectAccessor::FindPlayer(citr->guid);
        if (member && m_disenchantInfo.MaxDisenchantSkillLevel < member->GetSkillValue(SKILL_ENCHANTING))
        {
            m_disenchantInfo.MaxDisenchantSkillLevel = member->GetSkillValue(SKILL_ENCHANTING);
            m_disenchantInfo.DisenchanterGUID = member->GetGUID();
        }
    }
}

void Group::SetLootMethod(LootMethod method)
{
    m_lootMethod = method;
}

void Group::SetLooterGuid(ObjectGuid guid)
{
    m_looterGuid = guid;
}

void Group::SetMasterLooterGuid(ObjectGuid guid)
{
    m_masterLooterGuid = guid;
}

void Group::SetLootThreshold(ItemQualities threshold)
{
    m_lootThreshold = threshold;
}

void Group::SetLfgRoles(ObjectGuid guid, uint8 roles)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    slot->roles = roles;
    SendUpdate();
}

uint8 Group::GetLfgRoles(ObjectGuid guid)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return 0;

    return slot->roles;
}

bool Group::IsFull() const
{
    return isRaidGroup() ? (m_memberSlots.size() >= MAXRAIDSIZE) : (m_memberSlots.size() >= MAXGROUPSIZE);
}

bool Group::isLFGGroup() const
{
    return (m_groupFlags & GROUP_FLAG_LFG) != 0;
}

bool Group::isLFRGroup() const
{
    return (m_groupFlags & GROUP_FLAG_LFG) != 0 && (m_groupFlags & GROUP_FLAG_RAID) != 0;
}

bool Group::isRaidGroup() const
{
    return (m_groupFlags & GROUP_FLAG_RAID) != 0;
}

bool Group::isBGGroup() const
{
    return m_bgGroup != nullptr;
}

bool Group::isBFGroup() const
{
    return m_bfGroup != nullptr;
}

bool Group::IsCreated() const
{
    return GetMembersCount() > 0;
}

ObjectGuid Group::GetLeaderGUID() const
{
    return m_leaderGuid;
}

ObjectGuid Group::GetGUID() const
{
    return m_guid;
}

ObjectGuid::LowType Group::GetLowGUID() const
{
    return m_guid.GetCounter();
}

char const* Group::GetLeaderName() const
{
    return m_leaderName.c_str();
}

LootMethod Group::GetLootMethod() const
{
    return m_lootMethod;
}

ObjectGuid Group::GetLooterGuid() const
{
    if (GetLootMethod() == FREE_FOR_ALL)
        return ObjectGuid::Empty;
    return m_looterGuid;
}

ObjectGuid Group::GetMasterLooterGuid() const
{
    return m_masterLooterGuid;
}

ItemQualities Group::GetLootThreshold() const
{
    return m_lootThreshold;
}

bool Group::IsMember(ObjectGuid guid) const
{
    return _getMemberCSlot(guid) != m_memberSlots.end();
}

bool Group::IsLeader(ObjectGuid guid) const
{
    return (GetLeaderGUID() == guid);
}

ObjectGuid Group::GetMemberGUID(const std::string& name)
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->name == name)
            return itr->guid;
    return ObjectGuid::Empty;
}

uint8 Group::GetMemberFlags(ObjectGuid guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return 0u;
    return mslot->flags;
}

bool Group::SameSubGroup(ObjectGuid guid1, ObjectGuid guid2) const
{
    member_citerator mslot2 = _getMemberCSlot(guid2);
    if (mslot2 == m_memberSlots.end())
       return false;
    return SameSubGroup(guid1, &*mslot2);
}

bool Group::SameSubGroup(ObjectGuid guid1, MemberSlot const* slot2) const
{
    member_citerator mslot1 = _getMemberCSlot(guid1);
    if (mslot1 == m_memberSlots.end() || !slot2)
        return false;
    return (mslot1->group == slot2->group);
}

bool Group::HasFreeSlotSubGroup(uint8 subgroup) const
{
    return (m_subGroupsCounts && m_subGroupsCounts[subgroup] < MAXGROUPSIZE);
}


uint8 Group::GetMemberGroup(ObjectGuid guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
       return (MAX_RAID_SUBGROUPS+1);
    return mslot->group;
}

void Group::SetBattlegroundGroup(Battleground* bg)
{
    m_bgGroup = bg;
}

void Group::SetBattlefieldGroup(Battlefield *bg)
{
    m_bfGroup = bg;
}

void Group::SetGroupMemberFlag(ObjectGuid guid, bool apply, GroupMemberFlags flag)
{
    // Assistants, main assistants and main tanks are only available in raid groups
    if (!isRaidGroup())
       return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Do flag specific actions, e.g ensure uniqueness
    switch (flag)
    {
        case MEMBER_FLAG_MAINASSIST:
            RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);         // Remove main assist flag from current if any.
            break;
        case MEMBER_FLAG_MAINTANK:
            RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main tank flag from current if any.
            break;
        case MEMBER_FLAG_ASSISTANT:
            break;
        default:
            return;                                                      // This should never happen
    }

    // Switch the actual flag
    ToggleGroupMemberFlag(slot, flag, apply);

    // Preserve the new setting in the db
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_FLAG);

    stmt->setUInt8(0, slot->flags);
    stmt->setUInt32(1, guid.GetCounter());

    CharacterDatabase.Execute(stmt);

    // Broadcast the changes to the group
    SendUpdate();
}

Difficulty Group::GetDifficulty(bool isRaid) const
{
    return isRaid ? m_raidDifficulty : m_dungeonDifficulty;
}

Difficulty Group::GetDungeonDifficulty() const
{
    return m_dungeonDifficulty;
}

Difficulty Group::GetRaidDifficulty() const
{
    return m_raidDifficulty;
}

bool Group::isRollLootActive() const
{
    return !RollId.empty();
}

Group::Rolls::iterator Group::GetRoll(ObjectGuid Guid)
{
    Rolls::iterator iter;
    for (iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->isValid() && (*iter)->itemGUID == Guid)
            return iter;

    return RollId.end();
}

void Group::LinkMember(GroupReference* pRef)
{
    m_memberMgr.insertFirst(pRef);
}

void Group::DelinkMember(ObjectGuid guid)
{
    GroupReference* ref = m_memberMgr.getFirst();
    while (ref)
    {
        GroupReference* nextRef = ref->next();
        if (ref->GetSource()->GetGUID() == guid)
        {
            ref->unlink();
            break;
        }
        ref = nextRef;
    }
}

Group::BoundInstancesMap& Group::GetBoundInstances(Difficulty difficulty)
{
    return m_boundInstances[difficulty];
}

void Group::_initRaidSubGroupsCounter()
{
    // Sub group counters initialization
    if (!m_subGroupsCounts)
        m_subGroupsCounts = new uint8[MAX_RAID_SUBGROUPS];

    memset((void*)m_subGroupsCounts, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ++m_subGroupsCounts[itr->group];
}

Group::member_citerator Group::_getMemberCSlot(ObjectGuid Guid) const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

Group::member_witerator Group::_getMemberWSlot(ObjectGuid Guid)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

void Group::SubGroupCounterIncrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        ++m_subGroupsCounts[subgroup];
}

void Group::SubGroupCounterDecrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        --m_subGroupsCounts[subgroup];
}

void Group::RemoveUniqueGroupMemberFlag(GroupMemberFlags flag)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->flags & flag)
            itr->flags &= ~flag;
}

void Group::ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply)
{
    if (apply)
        slot->flags |= flag;
    else
        slot->flags &= ~flag;
}

void Group::SetEveryoneIsAssistant(bool apply)
{
    if (apply)
        m_groupFlags = GroupFlags(m_groupFlags | GROUP_FLAG_EVERYONE_ASSISTANT);
    else
        m_groupFlags = GroupFlags(m_groupFlags & ~GROUP_FLAG_EVERYONE_ASSISTANT);

    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ToggleGroupMemberFlag(itr, MEMBER_FLAG_ASSISTANT, apply);

    SendUpdate();
}
