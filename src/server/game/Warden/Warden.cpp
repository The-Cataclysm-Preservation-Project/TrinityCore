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
#include "WardenModule.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Containers.h"
#include <openssl/sha.h>

uint32 BuildChecksum(const uint8* data, uint32 length)
{
    std::array<uint8, 20> digest;

    SHA1(data, length, digest.data());
    uint32 checkSum = 0;
    for (uint8 i = 0; i < 5; ++i)
        checkSum = checkSum ^ reinterpret_cast<uint32_t*>(digest.data())[i];

    return checkSum;
}

bool IsValidCheckSum(uint32 checksum, const uint8* data, const uint16 length)
{
    return checksum == BuildChecksum(data, length);
}

Warden::Warden(WardenPlatform platform) : _session(nullptr),
    _module(nullptr),
    _platform(platform),
    _inputCrypto(16),
    _outputCrypto(16),
    _checkTimer(10000/*10 sec*/),
    _clientResponseTimer(0),
    _previousTimestamp(0),
    _initialized(false)
{
    _modules = sWardenMgr->FindModules(_platform);
    _moduleWeights.resize(_modules.size(), 1.0);
}

Warden::~Warden()
{
    _module = nullptr;
    _initialized = false;
}

std::shared_ptr<WardenModule> Warden::SelectModule() const
{
    return *Trinity::Containers::SelectRandomWeightedContainerElement(_modules, _moduleWeights);
}

void Warden::HandleModuleMissing()
{
    TC_LOG_DEBUG("warden", "[S->C] WARDEN_SMSG_MODULE_CACHE");

    // Create packet structure
    int32 sizeLeft = _module->Module.Data.size();
    uint32 pos = 0;

    while (sizeLeft > 0)
    {
        size_t burstSize = std::min<size_t>(sizeLeft, 500);

        ByteBuffer buffer(burstSize + sizeof(uint8) + sizeof(uint16));
        buffer << uint8(WARDEN_SMSG_MODULE_CACHE);
        buffer << uint16(burstSize);
        buffer.append(&_module->Module.Data[pos], burstSize);

        SendPacket(std::move(buffer));

        sizeLeft -= burstSize;
        pos += burstSize;
    }
}

void Warden::RequestModule()
{
    // Ask Maiev to use the module identified by the provided ID and Key.
    // Maiev can reply WARDEN_CMSG_MODULE_OK if the module is found in WowCache.WDB
    // Otherwise, Maiev replies WARDEN_CMSG_MODULE_MISSING.
    TC_LOG_DEBUG("warden", "[S->C] WARDEN_SMSG_MODULE_USE");

    ByteBuffer buffer;
    buffer << uint8(WARDEN_SMSG_MODULE_USE);

    auto id = _module->GetModuleChecksum();

    buffer.append(id.data(), id.size());
    buffer.append(_module->Module.Key.data(), _module->Module.Key.size());
    buffer << uint32(_module->Module.Data.size());

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

void Warden::HandleModuleOK()
{
    TC_LOG_INFO("warden", "[S->C] WARDEN_SMSG_HASH_REQUEST (%s, K: %u)", _session->GetPlayerInfo().c_str(), _wardenKey.ID);

    // Select a proper key
    _wardenKey = _module->SelectRandomKey();

    // Create packet structure
    ByteBuffer buffer(_wardenKey.Seed.size() + sizeof(uint8));
    buffer << uint8(WARDEN_SMSG_HASH_REQUEST);
    buffer.append(_wardenKey.Seed.data(), _wardenKey.Seed.size());

    SendPacket(std::move(buffer));
}

void Warden::HandleModuleFailed()
{
    // C++20 ranges please
    auto itr = std::find(_modules.begin(), _modules.end(), _module);
    if (itr == _modules.end())
        return;

    auto index = std::distance(_modules.begin(), itr);

    _modules.erase(itr);

    auto weightItr = _moduleWeights.begin();
    std::advance(weightItr, index);
    _moduleWeights.erase(weightItr);

    _module = SelectModule();

    RequestModule();
}

void Warden::ProcessIncoming(ByteBuffer& buffer)
{
    DecryptData(buffer.contents(), buffer.size());

    uint8 opcode;
    buffer >> opcode;

    switch (opcode)
    {
        case WARDEN_CMSG_MODULE_MISSING:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_MODULE_MISSING");
            HandleModuleMissing();
            break;
        case WARDEN_CMSG_MODULE_OK:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_MODULE_OK");
            HandleModuleOK();
            break;
        case WARDEN_CMSG_CHEAT_CHECKS_RESULT:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_CHEAT_CHECKS_RESULT");
            HandleCheatChecksResult(buffer);
            break;
        case WARDEN_CMSG_MEM_CHECKS_RESULT:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_MEM_CHECKS_RESULT");
            break;
        case WARDEN_CMSG_HASH_RESULT:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_HASH_RESULT");
            if (HandleHashResult(buffer))
                InitializeModule();
            break;
        case WARDEN_CMSG_MODULE_FAILED:
            TC_LOG_DEBUG("warden", "[C->S] WARDEN_CMSG_MODULE_FAILED");
            HandleModuleFailed();
            break;
        default:
            TC_LOG_DEBUG("warden", "[C->S] Unknown warden opcode 0x%02X of size %u.", opcode, uint32(buffer.size() - 1));
            break;
    }
}

void Warden::ProcessCheckResult(uint32 checkID, WardenCheckResult action)
{
    uint32 accountId = GetSession()->GetAccountId();
    ObjectGuid characterGuid = ObjectGuid::Empty;
    if (Player* player = GetSession()->GetPlayer())
        characterGuid = player->GetGUID();

    switch (action)
    {
        case WardenCheckResult::FailedBan:
        {
            uint32 banDuration = sWorld->getIntConfig(CONFIG_WARDEN_CLIENT_BAN_DURATION);
            std::string accountName;
            AccountMgr::GetName(accountId, accountName);
            sWorld->BanAccount(BAN_ACCOUNT, accountName, banDuration, "Anticheat Violation", "Warden");
            break;
        }
        case WardenCheckResult::FailedKick:
            _session->KickPlayer();
            break;
        case WardenCheckResult::Success:
            return;
        default:
            break;
    }

    // Log the violation in all situations.
    LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_WARDEN_FLAG);
    stmt->setUInt32(0, accountId);
    stmt->setUInt32(1, GameTime::GetGameTime());
    stmt->setUInt32(2, checkID);
    LoginDatabase.Execute(stmt);
}

bool Warden::CanHandle(WardenCheck::Type checkType) const
{
    return !!_module->Checks[AsUnderlyingType(checkType)];
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
