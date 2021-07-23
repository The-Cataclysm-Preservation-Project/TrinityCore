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
#include "Optional.h"
#include "World.h"
#include "Player.h"
#include "Util.h"
#include "WardenInteropCheck.h"
#include "WardenWin.h"
#include "WardenModule.h"
#include "WardenMgr.h"
#include "WardenCheck.h"
#include "WardenCheatCheckRequest.h"
#include "WardenModuleInitializeRequest.h"
#include "WardenDefines.h"
#include "Random.h"
#include <openssl/md5.h>

WardenWin::WardenWin(WardenPlatform platform) : Warden(platform), _serverTicks(0)
{
}

WardenWin::~WardenWin() { }

void WardenWin::Init(WorldSession* session, BigNumber* k)
{
    _session = session;

    // Generate Warden Key into the key.
    // Reuse existing fields for ease of use
    SHA1Randx WK(k->AsByteArray().get(), k->GetNumBytes());
    WK.Generate(_wardenKey.ClientKey);
    WK.Generate(_wardenKey.ServerKey);

    _inputCrypto.Init(_wardenKey.ClientKey.data());
    _outputCrypto.Init(_wardenKey.ServerKey.data());

    TC_LOG_DEBUG("warden", "Server side warden for client %u initializing...", session->GetAccountId());
    TC_LOG_DEBUG("warden", "Session key: %s", ByteArrayToHexStr(k->AsByteArray().get(), k->GetNumBytes()).c_str());
    TC_LOG_DEBUG("warden", "C->S Key: %s", ByteArrayToHexStr(_wardenKey.ClientKey.data(), 16).c_str());
    TC_LOG_DEBUG("warden", "S->C Key: %s", ByteArrayToHexStr(_wardenKey.ServerKey.data(), 16).c_str());
    TC_LOG_DEBUG("warden", "Loading Module...");

    _module = SelectModule();

    RequestModule();
}

void WardenWin::InitializeModule()
{
    TC_LOG_DEBUG("warden", "Initializing module function pointers");

    // Create packet structure
    ByteBuffer buffer;
    if (GetPlatform() == WardenPlatform::Win)
    {
        WardenModuleInitializeFrameXMLRequest xmlRequest;
        xmlRequest.Functions[0] = 0x0043D310;
        xmlRequest.Functions[1] = 0x0043C230;
        xmlRequest.Write(this, buffer);

        WardenModuleInitializeMPQRequest mpqRequest;
        mpqRequest.Functions[0] = 0x003A8C50;
        mpqRequest.Functions[1] = 0x003A5170;
        mpqRequest.Functions[2] = 0x003A6550;
        mpqRequest.Functions[3] = 0x003A6600;
        mpqRequest.Write(this, buffer);

        WardenModuleInitializeTimingRequest timingRequest;
        timingRequest.Functions[0] = 0x00479740;
        timingRequest.Write(this, buffer);
    }
    else if (GetPlatform() == WardenPlatform::Wn64)
    {
        WardenModuleInitializeFrameXMLRequest xmlRequest;
        xmlRequest.Functions[0] = 0x00568110;
        xmlRequest.Functions[1] = 0x00566BC0;
        xmlRequest.Write(this, buffer);

        WardenModuleInitializeMPQRequest mpqRequest;
        mpqRequest.Functions[0] = 0x0049EA10;
        mpqRequest.Functions[1] = 0x00499DF0;
        mpqRequest.Functions[2] = 0x0049B550;
        mpqRequest.Functions[3] = 0x0049B610;
        mpqRequest.Write(this, buffer);

        WardenModuleInitializeTimingRequest timingRequest;
        timingRequest.Functions[0] = 0x005B2500;
        timingRequest.Write(this, buffer);
    }
    else
        ASSERT(false, "Unhandled windows module type");

    SendPacket(std::move(buffer));
}

bool WardenWin::HandleHashResult(ByteBuffer& packet)
{
    std::array<uint8, SHA_DIGEST_LENGTH> clientDigest;
    packet.read(clientDigest.data(), clientDigest.size());

    if (!_wardenKey.Validate(clientDigest))
    {
        TC_LOG_ERROR("warden", "Handshake with %s using key %u failed.", _session->GetPlayerInfo().c_str(), _wardenKey.ID);
        return false;
    }

    _inputCrypto.Init(_wardenKey.ClientKey.data());
    _outputCrypto.Init(_wardenKey.ServerKey.data());

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
        std::shared_lock<std::shared_mutex> lock(sWardenMgr->_checkStoreLock);

        _pendingChecks = sWardenMgr->GetChecks(_session, this);
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

        if (currentCheck->TryWriteRequest(this, request, dataBuffer))
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
    outboundBuffer << uint8(_wardenKey.ClientKey[0]);

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

    for (auto&& currentCheck : _sentChecks)
    {
        WardenCheckResult checkResult = currentCheck->ProcessResponse(this, packet);
        ProcessCheckResult(currentCheck->GetID(), checkResult);
    }

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
    return _module->Checks[AsUnderlyingType(checkType)].value() ^ _wardenKey.ClientKey[0];
}

void WardenWin::SubmitCheck(std::shared_ptr<WardenCheck> check)
{
    _pendingChecks.emplace_back(check);
}
