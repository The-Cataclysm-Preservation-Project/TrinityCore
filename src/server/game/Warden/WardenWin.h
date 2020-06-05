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

#include "ByteBuffer.h"

#include "Warden.h"
#include "WardenCheck.h"

#include <vector>
#include <memory>

#pragma pack(push, 1)

struct WardenInitModuleRequest
{
    uint8 Command1;
    uint16 Size1;
    uint32 CheckSumm1;
    uint8 Unk1;
    uint8 Unk2;
    uint8 Type;
    uint8 String_library1;
    uint32 Function1[4];

    uint8 Command2;
    uint16 Size2;
    uint32 CheckSumm2;
    uint8 Unk3;
    uint8 Unk4;
    uint8 String_library2;
    uint32 Function2[2];
    uint8 Function2_set;

    uint8 Command3;
    uint16 Size3;
    uint32 CheckSumm3;
    uint8 Unk5;
    uint8 Unk6;
    uint8 String_library3;
    uint32 Function3;
    uint8 Function3_set;
};

#pragma pack(pop)

class TC_GAME_API WardenWin : public Warden
{
    public:
        WardenWin();
        ~WardenWin();

        void Init(WorldSession* session, BigNumber* K) override;
        ClientWardenModule* GetModuleForClient() override;
        void InitializeModule() override;
        void HandleModuleOK() override;
        bool HandleHashResult(ByteBuffer& packet) override;
        void RequestData() override;
        void HandleCheatChecksResult(ByteBuffer& packet) override;
        uint8 EncodeWardenCheck(WardenCheck::Type checkType) const override;

        uint32 GetCheatCheckRequestTime() const override { return _serverTicks; }

        void SubmitCheck(std::shared_ptr<WardenCheck> check) override;

    protected:
        bool IsAwaitingReply() const override { return !_sentChecks.empty(); }

    private:
        uint32 _serverTicks;

        //< Various checks that need to be sent to the client.
        std::vector<std::shared_ptr<WardenCheck>> _pendingChecks;

        //< Checks sent to the client, awaiting reply
        std::vector<std::shared_ptr<WardenCheck>> _sentChecks;
};

#endif
