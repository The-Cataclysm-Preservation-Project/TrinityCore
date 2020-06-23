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

#ifndef _WARDEN_WIN_H
#define _WARDEN_WIN_H

#include "Cryptography/ARC4.h"
#include "Cryptography/BigNumber.h"
#include "OptionalFwd.h"

#include "ByteBuffer.h"

#include "Warden.h"
#include "WardenCheck.h"

#include <vector>
#include <memory>

class TC_GAME_API WardenWin : public Warden
{
    public:
        WardenWin(WardenPlatform platform);
        ~WardenWin();

        void Init(WorldSession* session, BigNumber* K) override;

        void InitializeModule() override;
        bool HandleHashResult(ByteBuffer& packet) override;
        void RequestData() override;
        void HandleCheatChecksResult(ByteBuffer& packet) override;
        uint8 EncodeWardenCheck(WardenCheck::Type checkType) const override;

        uint32 GetCheatCheckRequestTime() const override { return _serverTicks; }

        void SubmitCheck(std::shared_ptr<WardenCheck> check) override;

        void HandleInteropCheckResult(uint64 clientBase, uint64 moduleBase, uint64 handlerBase);

    protected:
        bool IsAwaitingReply() const override { return !_sentChecks.empty(); }

    private:
        uint32 _serverTicks;

        //< Various checks that need to be sent to the client.
        std::vector<std::shared_ptr<WardenCheck>> _pendingChecks;

        //< Checks sent to the client, awaiting reply
        std::vector<std::shared_ptr<WardenCheck>> _sentChecks;

        struct ClientModuleInfo
        {
            uint64 ClientBase;
            uint64 ModuleBase;
            uint64 HandlerBase;
        };
        Optional<ClientModuleInfo> _moduleInfo;
};

#endif
