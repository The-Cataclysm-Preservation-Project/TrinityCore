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

#include "WardenModuleCheck.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "Random.h"
#include "HmacHash.h"

WardenModuleCheck::WardenModuleCheck() : WardenCheck(Type::Module)
{

}

bool WardenModuleCheck::LoadFromDB(Field* fields)
{
    _moduleName = ReadDatabaseField<DatabaseColumn::Data0>(fields);

    return WardenCheck::LoadFromDB(fields);
}

bool WardenModuleCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    uint32 seed = rand32();

    // Compute HMAC/SHA1 of the module name uppercased
    HmacSha1 hmac(sizeof(uint32), reinterpret_cast<uint8*>(&seed));
    hmac.UpdateData(_moduleName);
    hmac.Finalize();

    ByteBuffer dataBuffer;
    dataBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    dataBuffer << uint32(seed);
    dataBuffer.append(hmac.GetDigest());

    if (!request.CanWrite(dataBuffer.wpos()))
        return false;

    requestBuffer.append(dataBuffer);
    return true;

}

bool WardenModuleCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 resultCode;
    packet >> resultCode;

    bool checkFailed = resultCode == 0xE9;
    checkFailed = TransformCheckResult(checkFailed);

    if (checkFailed)
    {
        TC_LOG_DEBUG("warden", "(Check #%u) Module check failed.", GetID());

        warden->Violation(GetID());
    }

    for (auto&& handler : _handlers)
        handler(shared_from_this(), checkFailed);

    return checkFailed;
}
