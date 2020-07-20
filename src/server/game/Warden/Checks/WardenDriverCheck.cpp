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

#include "WardenDriverCheck.h"
#include "Log.h"
#include "Random.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "WorldSession.h"

#include "Cryptography/HmacHash.h"

#include <boost/algorithm/hex.hpp>

WardenDriverCheck::WardenDriverCheck() : WardenCheck(Type::Driver)
{

}

bool WardenDriverCheck::LoadFromDB(Field* fields)
{
    _driverPath = ReadDatabaseField<DatabaseColumn::Data0>(fields);

    std::string expectedData = ReadDatabaseField<DatabaseColumn::Result>(fields);
    boost::algorithm::unhex(expectedData.begin(), expectedData.end(), std::back_inserter(_expectedData));

    return WardenCheck::LoadFromDB(fields);
}

bool WardenDriverCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    auto [stringIndex, success] = request.RegisterString(_driverPath);
    if (!success)
        return false;

    uint32 seed = rand32();
    HmacSha1 hmac(4, reinterpret_cast<uint8*>(&seed));
    hmac.UpdateData(_expectedData.data(), _expectedData.size());
    hmac.Finalize();

    ByteBuffer dataBuffer;
    dataBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    dataBuffer << uint32(seed);
    dataBuffer.append(hmac.GetDigest().data(), hmac.GetDigest().size());
    dataBuffer << uint8(stringIndex);

    if (!request.CanWrite(dataBuffer.wpos()))
    {
        request.UnregisterString(stringIndex);
        return false;
    }

    requestBuffer.append(dataBuffer);
    return true;
}

bool WardenDriverCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 scanResult;
    packet >> scanResult;

    bool checkFailed = scanResult == 0xE9;
    checkFailed = TransformCheckResult(checkFailed);

    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Driver check failed.", GetID());

        warden->Violation(GetID());
    }

    for (HandlerType const& handler : _handlers)
        handler(shared_from_this(), checkFailed);

    return checkFailed;
}
