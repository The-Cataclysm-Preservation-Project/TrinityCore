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

#include "WardenPageCheck.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "Errors.h"
#include "Random.h"
#include "HmacHash.h"

#include <boost/algorithm/hex.hpp>

WardenPageCheck::WardenPageCheck(WardenCheck::Type checkType) : WardenCheck(checkType)
{
    ASSERT(checkType == Type::PageCheckA || checkType == Type::PageCheckB);
}

bool WardenPageCheck::LoadFromDB(Field* fields)
{
    _address = ReadDatabaseField<DatabaseColumn::Address>(fields);
    _length = ReadDatabaseField<DatabaseColumn::Length>(fields);

    std::string expectedData = ReadDatabaseField<DatabaseColumn::Data0>(fields);
    boost::algorithm::unhex(expectedData, std::back_inserter(_expectedData));

    if (_expectedData.size() != _length)
        return false;

    return WardenCheck::LoadFromDB(fields);
}

bool WardenPageCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    uint32 seed = rand32();

    // Compute HMAC/SHA1 of the expected bytes
    HmacSha1 hmac(sizeof(uint32), reinterpret_cast<uint8*>(&seed));
    hmac.UpdateData(_expectedData.data(), _expectedData.size());
    hmac.Finalize();

    ByteBuffer dataBuffer;
    dataBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    dataBuffer << uint32(seed);
    dataBuffer.append(hmac.GetDigest());
    dataBuffer << uint32(_address);
    dataBuffer << uint8(_length);

    if (!request.CanWrite(dataBuffer.wpos()))
        return false;

    requestBuffer.append(dataBuffer);
    return true;
}

bool WardenPageCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 resultCode;
    packet >> resultCode;

    bool checkFailed = resultCode == 0xE9;
    checkFailed = TransformCheckResult(checkFailed);

    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Failed page check.", GetID());

        warden->Violation(GetID());
    }

    return checkFailed;
}
