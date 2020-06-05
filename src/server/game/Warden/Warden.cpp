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

#include "Warden.h"
#include "WardenMgr.h"
#include "WardenDefines.h"
#include "AccountMgr.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <openssl/sha.h>

Warden::Warden(WardenPlatform platform) : _session(nullptr),
    _wardenKey(nullptr),
    _platform(platform),
    _inputCrypto(16),
    _outputCrypto(16),
    _checkTimer(10000/*10 sec*/),
    _clientResponseTimer(0),
    _previousTimestamp(0),
    _module(nullptr),
    _initialized(false)
{
    _inputKey.fill(0);
    _outputKey.fill(0);
}

Warden::~Warden()
{
    delete[] _module->CompressedData;
    delete _module;

    _module = nullptr;
    _initialized = false;
}

void Warden::HandleModuleMissing()
{
    TC_LOG_DEBUG("warden", "Send module to client");

    // Create packet structure

    int32 sizeLeft = _module->CompressedSize;
    uint32 pos = 0;
    while (sizeLeft > 0)
    {
        size_t burstSize = std::min<size_t>(sizeLeft, 500);

        WardenCommand transferCommand;
        transferCommand.Command = WARDEN_SMSG_MODULE_CACHE;
        transferCommand.Buffer << uint16(burstSize);
        transferCommand.Buffer.append(&_module->CompressedData[pos], burstSize);

        SendCommand(transferCommand);

        sizeLeft -= burstSize;
        pos += burstSize;
    }
}

void Warden::RequestModule()
{
    // Ask Maiev to use the module identified by the provided ID and Key.
    // Maiev can reply either WARDEN_CMSG_MODULE_OK if the module is found in WowCache.WDB
    // Otherwise, Maiev replies WARDEN_CMSG_MODULE_MISSING.
    TC_LOG_DEBUG("warden", "Sending WARDEN_SMSG_MODULE_USE");

    WardenCommand request;
    request.Command = WARDEN_SMSG_MODULE_USE;
    request.Buffer.append(_module->Id.data(), _module->Id.size());
    request.Buffer.append(_module->Key.data(), _module->Key.size());
    request.Buffer << uint32(_module->CompressedSize);

    SendCommand(request);
}

void Warden::SendCommand(WardenCommand const& command)
{
    // Flatten
    ByteBuffer buffer(1 + command.Buffer.size());
    buffer << uint8(command.Command);
    buffer.append(command.Buffer);

    SendPacket(std::move(buffer));
}

void Warden::SendPacket(ByteBuffer&& buffer)
{
    // Encrypt data
    EncryptData(buffer.contents(), buffer.size());

    // Send out
    WorldPacket packet(SMSG_WARDEN_DATA, sizeof(uint32) + buffer.size());
    packet << uint32(buffer.size());
    packet.append(buffer);
    _session->SendPacket(&packet);
}

void Warden::Update()
{
    if (_initialized)
    {
        uint32 currentTimestamp = GameTime::GetGameTimeMS();
        uint32 diff = currentTimestamp - _previousTimestamp;
        _previousTimestamp = currentTimestamp;

        if (IsAwaitingReply())
        {
            uint32 maxClientResponseDelay = sWorld->getIntConfig(CONFIG_WARDEN_CLIENT_RESPONSE_DELAY);

            if (maxClientResponseDelay > 0)
            {
                // Kick player if client response delays more than set in config
                if (_clientResponseTimer > maxClientResponseDelay * IN_MILLISECONDS)
                {
                    TC_LOG_WARN("warden", "%s (latency: %u, IP: %s) exceeded Warden module response delay for more than %s - disconnecting client",
                        _session->GetPlayerInfo().c_str(), _session->GetLatency(), _session->GetRemoteAddress().c_str(), secsToTimeString(maxClientResponseDelay, true).c_str());
                    _session->KickPlayer();
                }
                else
                    _clientResponseTimer += diff;
            }
        }
        else
        {
            if (diff >= _checkTimer)
            {
                RequestData();
            }
            else
                _checkTimer -= diff;
        }
    }
}

void Warden::DecryptData(uint8* buffer, uint32 length)
{
    _inputCrypto.UpdateData(length, buffer);
}

void Warden::EncryptData(uint8* buffer, uint32 length)
{
    _outputCrypto.UpdateData(length, buffer);
}

bool Warden::IsValidCheckSum(uint32 checksum, const uint8* data, const uint16 length)
{
    uint32 newChecksum = BuildChecksum(data, length);

    if (checksum != newChecksum)
    {
        TC_LOG_DEBUG("warden", "CHECKSUM IS NOT VALID");
        return false;
    }
    else
    {
        TC_LOG_DEBUG("warden", "CHECKSUM IS VALID");
        return true;
    }
}

uint32 Warden::BuildChecksum(const uint8* data, uint32 length)
{
    union {
        uint8 bytes[20];
        uint32 ints[5];
    } hash;

    SHA1(data, length, hash.bytes);
    uint32 checkSum = 0;
    for (uint8 i = 0; i < 5; ++i)
        checkSum = checkSum ^ hash.ints[i];

    return checkSum;
}

void Warden::ProcessIncoming(ByteBuffer& buffer)
{
    DecryptData(buffer.contents(), buffer.size());

    uint8 opcode;
    buffer >> opcode;

    switch (opcode)
    {
        case WARDEN_CMSG_MODULE_MISSING:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_MODULE_MISSING");
            HandleModuleMissing();
            break;
        case WARDEN_CMSG_MODULE_OK:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_MODULE_OK");
            HandleModuleOK();
            break;
        case WARDEN_CMSG_CHEAT_CHECKS_RESULT:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_CHEAT_CHECKS_RESULT");
            HandleCheatChecksResult(buffer);
            break;
        case WARDEN_CMSG_MEM_CHECKS_RESULT:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_MEM_CHECKS_RESULT");
            break;
        case WARDEN_CMSG_HASH_RESULT:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_HASH_RESULT");
            if (HandleHashResult(buffer))
                InitializeModule();
            break;
        case WARDEN_CMSG_MODULE_FAILED:
            TC_LOG_DEBUG("warden", "Received WARDEN_CMSG_MODULE_FAILED");
            break;
        default:
            TC_LOG_DEBUG("warden", "Received unknown warden opcode 0x%02X of size %u.", opcode, uint32(buffer.size() - 1));
            break;
    }
}

void Warden::Violation(uint32 checkID)
{
    uint32 accountId = GetSession()->GetAccountId();
    ObjectGuid characterGuid = ObjectGuid::Empty;
    if (Player* player = GetSession()->GetPlayer())
        characterGuid = player->GetGUID();

    WardenActions action = sWardenMgr->GetCheckFailureAction(checkID);
    switch (action)
    {
        case WARDEN_ACTION_BAN:
        {
            uint32 banDuration = sWorld->getIntConfig(CONFIG_WARDEN_CLIENT_BAN_DURATION);
            std::string accountName;
            AccountMgr::GetName(accountId, accountName);

            sWorld->BanAccount(BAN_ACCOUNT, accountName, banDuration, "Anticheat Violation", "Warden");
            break;
        }
        case WARDEN_ACTION_KICK:
            // Kick the player but log the violation as well.
            _session->KickPlayer();
            [[fallthrough]];
        case WARDEN_ACTION_LOG:
        {
            LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_WARDEN_FLAG);
            stmt->setUInt32(0, accountId);
            stmt->setUInt32(1, GameTime::GetGameTime());
            stmt->setUInt32(2, checkID);
            LoginDatabase.Execute(stmt);
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleWardenDataOpcode(WorldPacket& recvData)
{
    if (!_warden || recvData.empty())
        return;

    uint32 dataSize;
    recvData >> dataSize;
    if (dataSize == 0 || dataSize != recvData.size() - sizeof(uint32))
        return;

    ByteBuffer buffer(dataSize);
    buffer.resize(dataSize);
    buffer.wpos(0);
    recvData.read(buffer.contents(), buffer.size());

    _warden->ProcessIncoming(buffer);
}
