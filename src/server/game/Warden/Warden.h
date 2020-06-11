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

#ifndef _WARDEN_BASE_H
#define _WARDEN_BASE_H

#include <array>
#include <map>

#include "Cryptography/ARC4.h"
#include "Cryptography/BigNumber.h"
#include "Cryptography/SHA1.h"

#include "ByteBuffer.h"

#include "WardenFwd.h"
#include "WardenCheck.h"

#pragma pack(push, 1)
struct WardenCommand
{
    uint8 Command;
    ByteBuffer Buffer;
};

struct WardenHashRequest
{
    uint8 Command;
    uint8 Seed[16];
};
#pragma pack(pop)

struct ClientWardenModule
{
    std::array<uint8, 32> Id;
    std::array<uint8, 16> Key;
    uint32 CompressedSize;
    uint8* CompressedData;
};

class TC_GAME_API Warden
{
    friend class WardenWin;
    // friend class WardenMac;

    protected:
        Warden(WardenPlatform platform);

    public:
        virtual ~Warden();

        //< Initializes Warden with the given session key.
        virtual void Init(WorldSession* session, BigNumber* k) = 0;

        //< Returns a description of an acceptable module for the current session.
        virtual ClientWardenModule* GetModuleForClient() = 0;

        //< Initializes the module loaded by the client.
        virtual void InitializeModule() = 0;

        //< Requests checks from the client.
        virtual void RequestData() = 0;

        //< Encodes a warden check in order to be sent to the client.
        virtual uint8 EncodeWardenCheck(WardenCheck::Type checkType) const = 0;

        //< Main update loop.
        void Update();

        //< Processes any incoming packet and dispatches it to the appropriate function.
        void ProcessIncoming(ByteBuffer& buffer);

        //< Ensures the checksum sent by the client module in CMSG_WARDEN_CHEAT_CHECKS_RESPONSE is valid.
        static bool IsValidCheckSum(uint32 checksum, const uint8 *data, const uint16 length);

        //< Calculates a checksum for data received via CMSG_WARDEN_CHEAT_CHECKS_RESPONSE.
        static uint32 BuildChecksum(const uint8 *data, uint32 length);

        WorldSession* GetSession() const { return _session; }

        //< Returns the time at which the last WARDEN_SMSG_CHEAT_CHECKD_REQUEST was issued.
        virtual uint32 GetCheatCheckRequestTime() const = 0;

        void Violation(uint32 checkID);

        virtual void SubmitCheck(std::shared_ptr<WardenCheck> check) = 0;

        WardenPlatform GetPlatform() const { return _platform; }

    private:
        //< Handles WARDEN_CMSG_MODULE_MISSING.
        void HandleModuleMissing();

        //< Sends WARDEN_SMSG_MODULE_USE to the client, requesting a specific module to be loaded from cache.
        void RequestModule();

        //< Decrypts data sent by the client.
        void DecryptData(uint8* buffer, uint32 length);

        //< Encrypts data sent to the client.
        void EncryptData(uint8* buffer, uint32 length);

    protected:
        //< Handles WARDEN_CMSG_MODULE_OK.
        virtual void HandleModuleOK() = 0;
        //< Handles WARDEN_CMSG_HASH_RESULT.
        virtual bool HandleHashResult(ByteBuffer& packet) = 0;
        //< Handles WARDEN_CMSG_CHEAT_CHECK_RESULTS.
        virtual void HandleCheatChecksResult(ByteBuffer& packet) = 0;

        //< Returns true if checks have been sent to the client and we're waiting for a reply.
        virtual bool IsAwaitingReply() const = 0;

        void SendPacket(ByteBuffer&& buffer);

    private:
        WorldSession* _session;
        WardenKey const* _wardenKey;
        WardenPlatform _platform;

        std::array<uint8, 16> _inputKey;
        std::array<uint8, 16> _outputKey;

        ARC4 _inputCrypto;
        ARC4 _outputCrypto;

        uint32 _checkTimer;                          //< Timer for sending check requests
        uint32 _clientResponseTimer;                 //< Timer for client response delay

        uint32 _previousTimestamp;
        ClientWardenModule* _module;
        bool _initialized;
};

#endif
