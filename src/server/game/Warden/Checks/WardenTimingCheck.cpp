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

#include "WardenTimingCheck.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "GameTime.h"

WardenTimingCheck::WardenTimingCheck(Field* fields) : WardenCheck(Type::Timing, fields)
{

}

WardenTimingCheck::WardenTimingCheck() : WardenCheck(Type::Timing)
{
    
}

bool WardenTimingCheck::TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    if (!request.CanWrite(sizeof(uint8)))
        return false;

    requestBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    return true;
}

WardenCheckResult WardenTimingCheck::ProcessResponse(ByteBuffer& packet) const
{
    uint8 scanResult;
    packet >> scanResult;

    uint32 newClientTicks;
    packet >> newClientTicks;

    bool checkFailed = scanResult == 0x00;
    checkFailed = TransformResultCode(checkFailed);

    return HandleExtendedResponse(checkFailed, newClientTicks);
}

WardenCheckResult WardenTimingCheck::HandleExtendedResponse(bool checkFailed, uint32 clientTicks) const
{
    return WardenCheck::HandleResponse(checkFailed);
}

