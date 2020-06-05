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

#ifndef _WardenMgr_H
#define _WardenMgr_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <shared_mutex>

#include "Cryptography/SHA1.h"

#include "WardenFwd.h"
#include "WardenCheck.h"

class TC_GAME_API WardenMgr
{
private:
    WardenMgr();
    ~WardenMgr();

public:
    static WardenMgr* instance();

    //< Returns all modules that can be used for a given platform.
    std::vector<std::shared_ptr<WardenModule>> FindModules(WardenPlatform platform) const;

    //< Returns the check associated with the provided id, or nullptr if none exist.
    std::shared_ptr<WardenCheck> GetWardenCheck(uint16 Id);

    void LoadWardenChecks();
    void LoadWardenKeys();
    void LoadWardenModules();
    void LoadWardenOverrides();

    std::shared_mutex _checkStoreLock;

    //< Returns all checks for the given platform and session.
    std::vector<std::shared_ptr<WardenCheck>> GetChecks(WorldSession* session, Warden* warden) const;

    WardenCheckResult GetCheckFailureAction(uint32 failedChecks) const;

private:
    //< Stores all checks handled by all supported warden platforms.
    std::unordered_map<uint32, std::shared_ptr<WardenCheck>> _checkStore;

    //< Stores check-specific action overrides.
    std::unordered_map<uint32, WardenCheckResult> _actionOverrides;

    //< Stores all known modules.
    std::vector<std::shared_ptr<WardenModule>> _modules;
};

#define sWardenMgr WardenMgr::instance()

#endif
