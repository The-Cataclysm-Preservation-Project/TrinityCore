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

#include "WardenLuaCheck.h"
#include "Errors.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"

WardenLuaCheck::WardenLuaCheck(Type scanType, Field* fields) : WardenCheck(scanType, fields)
{
    ASSERT(scanType == Type::LuaString || scanType == Type::LuaStringAdvanced);

    switch (GetCheckType())
    {
    case Type::LuaString:
        _queriedString = ReadDatabaseField<DatabaseColumn::Data0>(fields);
        break;
    case Type::LuaStringAdvanced:
        _executableString = ReadDatabaseField<DatabaseColumn::Data0>(fields);
        _queriedString = ReadDatabaseField<DatabaseColumn::Data1>(fields);
        break;
    /* [[unlikely]] */ default: // Make CI happy...
        break;
    }

    _expectedResult = ReadDatabaseField<DatabaseColumn::Result>(fields);
}

bool WardenLuaCheck::TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    switch (GetCheckType())
    {
    case Type::LuaString:
    {
        auto [stringIndex, success] = request.RegisterString(_queriedString);
        if (!success)
            return false;

        if (!request.CanWrite(2 * sizeof(uint8)))
        {
            request.UnregisterString(stringIndex);
            return false;
        }

        requestBuffer << uint8(warden->EncodeWardenCheck(Type::LuaString));
        requestBuffer << uint8(stringIndex);
        return true;
    }
    case Type::LuaStringAdvanced:
    {
        auto [codeStringIndex, success0] = request.RegisterString(_executableString);
        if (!success0)
            return false;

        auto [queryStringIndex, success1] = request.RegisterString(_queriedString);
        if (!success1)
        {
            request.UnregisterString(codeStringIndex);
            return false;
        }

        if (!request.CanWrite(3 * sizeof(uint8)))
        {
            request.UnregisterString(codeStringIndex);
            request.UnregisterString(queryStringIndex);
            return false;
        }

        requestBuffer << uint8(warden->EncodeWardenCheck(Type::LuaStringAdvanced));
        requestBuffer << uint8(codeStringIndex);
        requestBuffer << uint8(queryStringIndex);
        return true;
    }
    /* [[unlikely]] */ default: // Make CI happy
        return true;
    }
}

WardenCheckResult WardenLuaCheck::ProcessResponse(ByteBuffer& packet) const
{
    // Same logic for both checks.
    uint8 scanError;
    packet >> scanError;

    std::string clientString;

    bool checkFailed = scanError != 0x00;
    if (!checkFailed)
    {
        uint8 stringLength;
        packet >> stringLength;

        clientString = packet.ReadString(stringLength);
        checkFailed = clientString != GetExpectedResult();
    }

    checkFailed = TransformResultCode(checkFailed);
    return HandleExtendedResponse(checkFailed, clientString);
}

WardenCheckResult WardenLuaCheck::HandleExtendedResponse(bool checkFailed, std::string const& /* clientResponse*/) const
{
    return WardenCheck::HandleResponse(checkFailed);
}
