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
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"

WardenLuaCheck::WardenLuaCheck(Type scanType) : WardenCheck(scanType)
{
    ASSERT(scanType == Type::LuaString || scanType == Type::LuaStringAdvanced);
}

bool WardenLuaCheck::LoadFromDB(Field* fields)
{
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

    return WardenCheck::LoadFromDB(fields);
}

bool WardenLuaCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    switch (GetCheckType())
    {
        case Type::LuaString:
        {
            auto [stringIndex, success] = request.RegisterString(_queriedString);
            if (!success || !request.CanWrite(2 * sizeof(uint8)))
                return false;

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

bool WardenLuaCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
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

    checkFailed = TransformCheckResult(checkFailed);
    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Lua check failed.", GetID());

        warden->Violation(GetID());
    }

    for (auto&& handler : _handlers)
        handler(checkFailed, clientString);

    return checkFailed;
}
