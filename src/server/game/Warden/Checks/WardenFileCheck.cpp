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

#include "WardenFileCheck.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"

#include <boost/algorithm/hex.hpp>

WardenFileCheck::WardenFileCheck() : WardenCheck(Type::MPQ)
{
    _expectedResult.fill(0);
}

bool WardenFileCheck::LoadFromDB(Field* fields)
{
    bool success = WardenCheck::LoadFromDB(fields);
    if (!success)
        return false;

    _fileName = ReadDatabaseField<DatabaseColumn::Data0>(fields);

    std::string expectedResult = ReadDatabaseField<DatabaseColumn::Result>(fields);

    // Here's hoping this if is actually not necessary...
    if (expectedResult.length() != 40)
    {
        TC_LOG_ERROR("server.loading", "Warden check %u has invalid result data (40 bytes expected)", GetID());
        return false;
    }

    auto itr = boost::algorithm::unhex(expectedResult.begin(), expectedResult.end(), _expectedResult.begin());
    if (itr != _expectedResult.end())
    {
        TC_LOG_ERROR("server.loading", "Warden check %u has invalid result data (40 bytes expected)", GetID());
        return false;
    }

    return true;
}

bool WardenFileCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    auto [stringIndex, success] = request.RegisterString(_fileName);
    if (!success || !request.CanWrite(2 * sizeof(uint8)))
        return false;

    requestBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    requestBuffer << uint8(stringIndex);
    return true;
}

bool WardenFileCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 scanCode;

    std::array<uint8, SHA_DIGEST_LENGTH> clientDigest;
    packet >> scanCode;

    bool checkFailed = scanCode != 0x00;
    if (!checkFailed)
    {
        packet.read(clientDigest.data(), clientDigest.size());

        checkFailed = clientDigest != _expectedResult;
    }
    
    checkFailed = TransformCheckResult(checkFailed);
    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) MPQ file check failed for '%s'.", GetID(), GetFileName().c_str());

        warden->Violation(GetID());
    }

    for (auto&& handler : _handlers)
        handler(shared_from_this(), checkFailed);

    return checkFailed;
}
