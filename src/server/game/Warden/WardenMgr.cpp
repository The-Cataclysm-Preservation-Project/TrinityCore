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

#include "Common.h"
#include "Containers.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Util.h"
#include "DatabaseEnv.h"
#include "WardenMgr.h"
#include "Warden.h"
#include "WardenKey.h"
#include "WardenCheck.h"
#include "WardenDriverCheck.h"
#include "WardenFileCheck.h"
#include "WardenLuaCheck.h"
#include "WardenMemoryCheck.h"
#include "WardenModuleCheck.h"
#include "WardenPageCheck.h"
#include "WardenProcCheck.h"
#include "WardenTimingCheck.h"
#include "WardenDefines.h"
#include "World.h"

WardenMgr::WardenMgr() { }

WardenMgr::~WardenMgr()
{
    _checkStore.clear();
}

void WardenMgr::LoadWardenKeys()
{
    uint32 oldMSTime = getMSTime();

    // Check if Warden is enabled by config before loading anything
    if (!sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
    {
        TC_LOG_INFO("warden", ">> Warden disabled, loading keys skipped.");
        return;
    }

    QueryResult result = WorldDatabase.Query("SELECT `id`, `platform`, `seed`, `clientKey`, `serverKey` FROM `warden_keys`");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Warden keys. DB table `warden_keys` is empty!");
        return;
    }

    uint32 count = 0;
    do {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();
        std::string platform = fields[1].GetString();

        auto loadIntoContainer = [&count](Field* fields, std::vector<WardenKey>& store)
        {
            auto&& record = store.emplace_back();

            auto loadIntoVector = [](std::string const& hexString, std::array<uint8, 16>& container) {
                if (hexString.length() > container.size() * 2)
                    return;

                HexStrToByteArray(hexString, container.data());
            };

            record.ID = fields[0].GetUInt32();
            loadIntoVector(fields[2].GetString(), record.Seed);
            loadIntoVector(fields[3].GetString(), record.ClientKey);
            loadIntoVector(fields[4].GetString(), record.ServerKey);

            ++count;
        };

        if (platform == "Win")
            loadIntoContainer(fields, _keyStore[WardenPlatform::Win]);
        // else if (platform == "Wn64")
        //     loadIntoContainer(fields, KeyStore[WardenPlatform::Wn64]);
        // else if (platform == "OSX")
        //     loadIntoContainer(fields, KeyStore[WardenPlatform::Mac]);
        else
        {
            TC_LOG_INFO("server.loading", ">> Warden Key %u targets non-handled platform '%s'. Skipped!", id, platform.c_str());
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u warden keys in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void WardenMgr::LoadWardenChecks()
{
    uint32 oldMSTime = getMSTime();

    // Check if Warden is enabled by config before loading anything
    if (!sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
    {
        TC_LOG_INFO("warden", ">> Warden disabled, loading checks skipped.");
        return;
    }

    //! The order of fields retrieve must match WardenCheck::DatabaseColumn
    //                                               0    1     2      3      4      5      6       7        8
    QueryResult result = WorldDatabase.Query("SELECT id, type, flags, data0, data1, address, length, result, comment FROM warden_checks WHERE enabled = 1 ORDER BY id ASC");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Warden checks. All checks are disabled.");
        return;
    }

    uint32 count = 0;
    uint16 id = 0;
    do
    {
        Field* fields = result->Fetch();

        WardenCheck::Type checkType = static_cast<WardenCheck::Type>(fields[1].GetUInt8());

        std::shared_ptr<WardenCheck> wardenCheck;
        switch (checkType)
        {
            case WardenCheck::Type::Timing:
                wardenCheck = std::make_shared<WardenTimingCheck>();
                break;
            case WardenCheck::Type::PageCheckA:
            case WardenCheck::Type::PageCheckB:
                wardenCheck = std::make_shared<WardenPageCheck>(checkType);
                break;
            case WardenCheck::Type::Driver:
                wardenCheck = std::make_shared<WardenDriverCheck>();
                break;
            case WardenCheck::Type::Memory:
                wardenCheck = std::make_shared<WardenMemoryCheck>();
                break;
            case WardenCheck::Type::Module:
                wardenCheck = std::make_shared<WardenModuleCheck>();
                break;
            case WardenCheck::Type::MPQ:
                wardenCheck = std::make_shared<WardenFileCheck>();
                break;
            case WardenCheck::Type::LuaString:
            case WardenCheck::Type::LuaStringAdvanced:
                wardenCheck = std::make_shared<WardenLuaCheck>(checkType);
                break;
            case WardenCheck::Type::Proc:
                wardenCheck = std::make_shared<WardenProcCheck>();
                break;
            default:
                TC_LOG_ERROR("server.loading", "Unhandled check type (ID: %u), skipped.", id);
                break;
        }

        if (!wardenCheck)
            continue;

        if (!wardenCheck->LoadFromDB(fields))
            continue;

        auto [itr, success] = _checkStore.emplace(wardenCheck->GetID(), wardenCheck);
        if (success)
            ++count;

    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u warden checks in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void WardenMgr::LoadWardenOverrides()
{
    uint32 oldMSTime = getMSTime();

    // Check if Warden is enabled by config before loading anything
    if (!sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
    {
        TC_LOG_INFO("warden", ">> Warden disabled, loading check overrides skipped.");
        return;
    }

    //                                                      0        1
    QueryResult result = CharacterDatabase.Query("SELECT wardenId, action FROM warden_action");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Warden action overrides. DB table `warden_action` is empty!");
        return;
    }

    uint32 count = 0;

    boost::unique_lock<boost::shared_mutex> lock(sWardenMgr->_checkStoreLock);

    do
    {
        Field* fields = result->Fetch();

        uint16 checkId = fields[0].GetUInt16();
        uint8  action  = fields[1].GetUInt8();

        if (action >= WARDEN_ACTION_MAX)
            TC_LOG_ERROR("server.loading", "Warden check override action out of range (ID: %u)", checkId);
        else
        {
            std::shared_ptr<WardenCheck> check = GetWardenCheck(checkId);
            if (!check)
                TC_LOG_ERROR("server.loading", "Warden check action override for non-existing check (ID: %u), skipped", checkId);
            else
            {
                _actionOverrides.emplace(checkId, WardenActions(action));
                ++count;
            }
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u warden action overrides in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

WardenActions WardenMgr::GetCheckFailureAction(uint32 failedCheck) const
{
    auto itr = _actionOverrides.find(failedCheck);
    if (itr == _actionOverrides.end())
        return WardenActions(sWorld->getIntConfig(CONFIG_WARDEN_CLIENT_FAIL_ACTION));

    return itr->second;
}

WardenMgr* WardenMgr::instance()
{
    static WardenMgr instance;
    return &instance;
}

const WardenKey* WardenMgr::SelectWardenKey(WardenPlatform platform) const
{
    auto itr = _keyStore.find(platform);
    if (itr == _keyStore.end())
        return nullptr;

    if (itr->second.empty())
        return nullptr;

    const WardenKey& element = Trinity::Containers::SelectRandomContainerElement(itr->second);
    return &element;
}

std::shared_ptr<WardenCheck> WardenMgr::GetWardenCheck(uint16 id)
{
    auto itr = _checkStore.find(id);
    if (itr != _checkStore.end())
        return itr->second;

    return nullptr;
}

std::vector<std::shared_ptr<WardenCheck>> WardenMgr::GetChecks(WorldSession* session, WardenPlatform platform) const
{
    std::vector<std::shared_ptr<WardenCheck>> container;

    for (auto [checkID, check] : _checkStore)
    {
        if (!check->TrySelect(session, platform))
            continue;

        container.push_back(check);
    }

    return container;
}
