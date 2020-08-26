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

#include "WardenProcCheck.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "Random.h"
#include "HmacHash.h"

#include <boost/algorithm/hex.hpp>

WardenProcCheck::WardenProcCheck() : WardenCheck(Type::Proc)
{

}

bool WardenProcCheck::LoadFromDB(Field* fields)
{
    _moduleName = ReadDatabaseField<DatabaseColumn::Data0>(fields);
    _functionName = ReadDatabaseField<DatabaseColumn::Data1>(fields);
    _address = ReadDatabaseField<DatabaseColumn::Address>(fields);
    _length = ReadDatabaseField<DatabaseColumn::Length>(fields);

    std::string expectedData = ReadDatabaseField<DatabaseColumn::Result>(fields);
    boost::algorithm::unhex(expectedData.begin(), expectedData.end(), std::back_inserter(_expectedData));

    return WardenCheck::LoadFromDB(fields);
}

bool WardenProcCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    uint32 seed = rand32();

    HmacSha1 hmac(sizeof(uint32), reinterpret_cast<uint8*>(&seed));
    hmac.UpdateData(_expectedData.data(), _expectedData.size());
    hmac.Finalize();

    auto [moduleIndex, moduleSuccess] = request.RegisterString(_moduleName);
    if (!moduleSuccess)
        return false;

    auto [functionIndex, functionSuccess] = request.RegisterString(_functionName);
    if (!functionSuccess)
    {
        request.UnregisterString(moduleIndex);
        return false;
    }

    ByteBuffer dataBuffer;
    dataBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    dataBuffer << uint32(seed);
    dataBuffer.append(hmac.GetDigest());
    dataBuffer << uint8(moduleIndex);
    dataBuffer << uint8(functionIndex);
    dataBuffer << uint32(_address);
    dataBuffer << uint8(_length);

    if (!request.CanWrite(dataBuffer.wpos()))
    {
        request.UnregisterString(moduleIndex);
        request.UnregisterString(functionIndex);
        return false;
    }

    requestBuffer.append(dataBuffer);
    return true;
}

bool WardenProcCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 scanResponseCode;
    packet >> scanResponseCode;

    bool checkFailed = scanResponseCode == 0xE9;
    checkFailed = TransformCheckResult(checkFailed);

    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Failed procedure check for %s!%s+%u.", GetID(), GetModuleName().c_str(),
            GetFunctionName().c_str(), GetAddress());

        warden->Violation(GetID());
    }

    return checkFailed;
}
