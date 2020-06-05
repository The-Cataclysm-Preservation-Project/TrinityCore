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

#include "WardenInteropCheck.h"
#include "Errors.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"

WardenInteropCheck::WardenInteropCheck() : WardenCheck(Type::WardenInterop)
{

}

bool WardenInteropCheck::LoadFromDB(Field* /* fields */)
{
    ASSERT(false, "Warden interop checks are not meant to be loaded from database.");

    return false;
}

bool WardenInteropCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    if (!request.CanWrite(sizeof(uint8)))
        return false;

    requestBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    return true;
}

bool WardenInteropCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint64 clientBase = ReadPackedValue(packet);
    uint64 moduleBase = ReadPackedValue(packet);
    uint64 moduleAddress = ReadPackedValue(packet);

    for (auto&& handler : _handlers)
        handler(shared_from_this(), clientBase, moduleBase, moduleAddress);

    return true;
}
