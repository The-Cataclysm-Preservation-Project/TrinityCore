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

#include "WardenMemoryCheck.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"

#include <boost/algorithm/hex.hpp>

WardenMemoryCheck::WardenMemoryCheck() : WardenCheck(Type::Memory)
{

}

bool WardenMemoryCheck::LoadFromDB(Field* fields)
{
    std::string result = ReadDatabaseField<DatabaseColumn::Result>(fields);
    boost::algorithm::unhex(result, std::back_inserter(_result));

    SetModuleName(ReadDatabaseField<DatabaseColumn::Data0>(fields));
    SetScanAddress(ReadDatabaseField<DatabaseColumn::Address>(fields));
    SetScanLength(ReadDatabaseField<DatabaseColumn::Length>(fields));

    // Scan size must be equal to the result size
    if (_length != _result.size())
        return false;

    return WardenCheck::LoadFromDB(fields);
}

void WardenMemoryCheck::SetModuleName(std::string const& moduleName)
{
    if (moduleName.empty())
        _moduleName = "@exe";
    else
        _moduleName = moduleName;
}

bool WardenMemoryCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    auto [stringIndex, success] = request.RegisterString(_moduleName);
    if (!success)
        return false;

    ByteBuffer buffer;
    buffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    buffer << uint8(stringIndex);
    WritePackedValue(buffer, _address);
    buffer << uint8(_length);

    if (!request.CanWrite(buffer.wpos()))
    {
        request.UnregisterString(stringIndex);
        return false;
    }

    requestBuffer.append(buffer);
    return true;
}

bool WardenMemoryCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 responseCode;
    packet >> responseCode;

    std::vector<uint8> clientResponse;
    clientResponse.resize(_result.size());
    packet.read(clientResponse.data(), _result.size());

    bool checkFailed = responseCode != 0;
    if (!checkFailed)
        checkFailed = memcmp(clientResponse.data(), _result.data(), _result.size()) != 0;

    checkFailed = TransformCheckResult(checkFailed);
    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Memory check failed.", GetID());

        warden->Violation(GetID());
    }

    return checkFailed;
}

