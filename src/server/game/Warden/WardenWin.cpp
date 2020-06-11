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

#include "Cryptography/HmacHash.h"
#include "Cryptography/WardenKeyGeneration.h"
#include "Common.h"
#include "Containers.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "ByteBuffer.h"
#include "Database/DatabaseEnv.h"
#include "GameTime.h"
#include "World.h"
#include "Player.h"
#include "Util.h"
#include "WardenInteropCheck.h"
#include "WardenWin.h"
#include "WardenModuleWin.h"
#include "WardenMgr.h"
#include "WardenCheck.h"
#include "WardenCheatCheckRequest.h"
#include "WardenModuleInitializeRequest.h"
#include "WardenKey.h"
#include "WardenDefines.h"
#include "Random.h"
#include <openssl/md5.h>

WardenWin::WardenWin() : Warden(WardenPlatform::Win), _serverTicks(0) {}

WardenWin::~WardenWin() { }

void WardenWin::Init(WorldSession* session, BigNumber* k)
{
    _session = session;

    // Generate Warden Key
    SHA1Randx WK(k->AsByteArray().get(), k->GetNumBytes());
    WK.Generate(_inputKey);
    WK.Generate(_outputKey);

    _inputCrypto.Init(_inputKey.data());
    _outputCrypto.Init(_outputKey.data());

    TC_LOG_DEBUG("warden", "Server side warden for client %u initializing...", session->GetAccountId());
    TC_LOG_DEBUG("warden", "C->S Key: %s", ByteArrayToHexStr(_inputKey.data(), 16).c_str());
    TC_LOG_DEBUG("warden", "S->C Key: %s", ByteArrayToHexStr(_outputKey.data(), 16).c_str());
    TC_LOG_DEBUG("warden", "Loading Module...");

    _module = GetModuleForClient();

    TC_LOG_DEBUG("warden", "Module Key: %s", ByteArrayToHexStr(_module->Key.data(), 16).c_str());
    TC_LOG_DEBUG("warden", "Module ID: %s", ByteArrayToHexStr(_module->Id.data(), 16).c_str());
    RequestModule();

    // Immediately request allocation bases for complex scans
    auto interopCheck = std::make_shared<WardenInteropCheck>();
    interopCheck->RegisterResponseHandler(std::bind(&WardenWin::HandleInteropCheckResult, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    SubmitCheck(interopCheck);
}

void WardenWin::HandleInteropCheckResult(uint64 clientBase, uint64 moduleBase, uint64 handlerBase)
{
    _moduleInfo.emplace();
    _moduleInfo->ClientBase = clientBase;
    _moduleInfo->ModuleBase = moduleBase;
    _moduleInfo->HandlerBase = handlerBase;
}

ClientWardenModule* WardenWin::GetModuleForClient()
{
    ClientWardenModule *mod = new ClientWardenModule();

    uint32 length = sizeof(Module.Module);

    // data assign
    mod->CompressedSize = length;
    mod->CompressedData = new uint8[length];
    memcpy(mod->CompressedData, Module.Module, length);
    mod->Key = Module.ModuleKey;

    // sha256 hash
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, mod->CompressedData, length);
    SHA256_Final((uint8*)&mod->Id, &ctx);

    return mod;
}

void WardenWin::InitializeModule()
{
    TC_LOG_DEBUG("warden", "Initializing module function pointers");

    // Create packet structure
    ByteBuffer buffer;

    WardenModuleInitializeMPQRequest mpqRequest;
    mpqRequest.Functions[0] = 0x003A8C50;
    mpqRequest.Functions[1] = 0x003A5170;
    mpqRequest.Functions[2] = 0x003A6550;
    mpqRequest.Functions[3] = 0x003A6600;
    mpqRequest.Write(this, buffer);

    WardenModuleInitializeFrameXMLRequest xmlRequest;
    xmlRequest.Functions[0] = 0x0043D310;
    xmlRequest.Functions[1] = 0x0043C230;
    xmlRequest.Write(this, buffer);

    WardenModuleInitializeTimingRequest timingRequest;
    timingRequest.Functions[0] = 0x00479740;
    timingRequest.Write(this, buffer);

    SendPacket(std::move(buffer));
}

void WardenWin::HandleModuleOK()
{
    // Module successfully loaded. We now pick pregenerated key pairs to reinitialize the cryptography for both
    // incoming and outgoing packets.

    _wardenKey = sWardenMgr->SelectWardenKey(_platform);
    if (_wardenKey == nullptr)
        return;

    TC_LOG_INFO("warden", "Hash request issued to %s with keychain %u", _session->GetPlayerInfo().c_str(), _wardenKey->ID);

    // Create packet structure
    ByteBuffer buffer(_wardenKey->Seed.size() + sizeof(uint8));
    buffer << uint8(WARDEN_SMSG_HASH_REQUEST);
    buffer.append(_wardenKey->Seed.data(), _wardenKey->Seed.size());

    SendPacket(std::move(buffer));
}

bool WardenWin::HandleHashResult(ByteBuffer& packet)
{
    std::array<uint8, SHA_DIGEST_LENGTH> clientDigest;
    packet.read(clientDigest.data(), clientDigest.size());

    if (!_wardenKey->Validate(clientDigest))
    {
        TC_LOG_ERROR("warden", "Handshake with %s using key %u failed.", _session->GetPlayerInfo().c_str(), _wardenKey->ID);
        return false;
    }

    TC_LOG_INFO("warden", "(Key #%u) Hash success with %s.", _wardenKey->ID, _session->GetPlayerInfo().c_str());

    _inputKey = _wardenKey->ClientKey;
    _outputKey = _wardenKey->ServerKey;

    _inputCrypto.Init(_inputKey.data());
    _outputCrypto.Init(_outputKey.data());

    _initialized = true;

    _checkTimer = 0;
    _previousTimestamp = GameTime::GetGameTimeMS();
    return true;
}

void WardenWin::RequestData()
{
    TC_LOG_DEBUG("warden", "Request data");

    // No more checks to do, re-request a new pile.
    if (_pendingChecks.empty())
    {
        boost::shared_lock<boost::shared_mutex> lock(sWardenMgr->_checkStoreLock);

        _pendingChecks = sWardenMgr->GetChecks(_session, WardenPlatform::Win);
    }

    _serverTicks = GameTime::GetGameTimeMS();

    // Make sure no checks remain from the last query.
    _sentChecks.clear();

    WardenCheatChecksRequest request;
    ByteBuffer dataBuffer;

    // Build check request
    for (uint32 i = 0; i < sWorld->getIntConfig(CONFIG_WARDEN_NUM_CHECKS) && !_pendingChecks.empty(); ++i)
    {
        auto itr = Trinity::Containers::SelectRandomContainerIterator(_pendingChecks);
        auto currentCheck = *itr;

        if (currentCheck->WriteWardenCheckRequest(this, request, dataBuffer))
        {
            _pendingChecks.erase(itr);
            _sentChecks.emplace_back(currentCheck);
        }
    }

    ByteBuffer outboundBuffer(3uLL + request.GetSize());
    outboundBuffer << uint8(WARDEN_SMSG_CHEAT_CHECKS_REQUEST);
    for (std::string const& pooledString : request)
    {
        outboundBuffer << uint8(pooledString.length());
        outboundBuffer.WriteString(pooledString);
    }
    outboundBuffer << uint8(0); // Mark the end of the string block
    outboundBuffer.append(dataBuffer);
    outboundBuffer << uint8(_inputKey[0]);

    SendPacket(std::move(outboundBuffer));

    if (sLog->ShouldLog("warden", LOG_LEVEL_DEBUG))
    {
        std::stringstream stream;
        stream << "Sent checks { ";
        for (auto&& currentCheck : _sentChecks)
            stream << currentCheck->GetID() << " ";
        stream << "} to " << _session->GetPlayerInfo();

        TC_LOG_DEBUG("warden", "%s", stream.str().c_str());
    }
}

void WardenWin::HandleCheatChecksResult(ByteBuffer& packet)
{
    _clientResponseTimer = 0;

    uint16 length;
    uint32 checksum;
    packet >> length >> checksum;

    if (!IsValidCheckSum(checksum, packet.contents() + packet.rpos(), length))
    {
        packet.read_skip(length);
        TC_LOG_WARN("warden", "Player %s failed response checksum.", _session->GetPlayerInfo().c_str());
        _session->KickPlayer();
        return;
    }

    std::vector<uint16> failedChecks;

    for (auto&& currentCheck : _sentChecks)
        currentCheck->ProcessResponse(this, packet);

    // Set hold off timer, minimum timer should at least be 1 second
    uint32 holdOff = sWorld->getIntConfig(CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF);
    if (holdOff < 1)
        holdOff = 1; // Minimum one second

    holdOff *= IN_MILLISECONDS;

    _sentChecks.clear();
    _checkTimer = holdOff;
}

uint8 WardenWin::EncodeWardenCheck(WardenCheck::Type checkType) const
{
    if (checkType >= WardenCheck::Type::MAX)
        return -1;

    return Module.CheckTypeTable[static_cast<uint32>(checkType)] ^ _inputKey[0];
}

void WardenWin::SubmitCheck(std::shared_ptr<WardenCheck> check)
{
    _pendingChecks.emplace_back(check);
}
