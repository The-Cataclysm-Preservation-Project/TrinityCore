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
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "GameTime.h"

WardenTimingCheck::WardenTimingCheck() : WardenCheck(Type::Timing)
{

}

bool WardenTimingCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    if (!request.CanWrite(sizeof(uint8)))
        return false;

    requestBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    return true;
}

bool WardenTimingCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 scanResult;
    packet >> scanResult;

    uint32 newClientTicks;
    packet >> newClientTicks;

    bool checkFailed = scanResult == 0x00;
    checkFailed = TransformCheckResult(checkFailed);

    if (checkFailed)
    {
        warden->Violation(GetID());

        uint32 ticksNow = GameTime::GetGameTimeMS();
        uint32 ourTicks = newClientTicks + (ticksNow - warden->GetCheatCheckRequestTime());

        TC_LOG_DEBUG("warden", "(Check #%u) Failed timing check", GetID());
        TC_LOG_DEBUG("warden", "ServerTicks %u", ticksNow);                            // Now
        TC_LOG_DEBUG("warden", "RequestTicks %u", warden->GetCheatCheckRequestTime()); // At request
        TC_LOG_DEBUG("warden", "Ticks %u", newClientTicks);                            // At response
        TC_LOG_DEBUG("warden", "Ticks diff %u", ourTicks - newClientTicks);
    }

    return checkFailed;
}

