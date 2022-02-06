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

#include "WardenCheck.h"
#include "Warden.h"
#include "Player.h"
#include "WardenDefines.h"
#include "WorldSession.h"
#include "EnumFlag.h"
#include "WardenMgr.h"

WardenCheck::WardenCheck(Type scanType, Field* fields) : _id(0), _scanType(scanType), _flags()
{
    _id = ReadDatabaseField<DatabaseColumn::ID>(fields);
    _flags = static_cast<WardenCheckFlags>(ReadDatabaseField<DatabaseColumn::Flags>(fields));
    _comment = ReadDatabaseField<DatabaseColumn::Comment>(fields);
}

WardenCheck::WardenCheck(Type scanType) : _id(0), _scanType(scanType), _flags()
{
    
}

bool WardenCheck::TrySelect(WorldSession* session, Warden* warden)
{
    if (!warden->CanHandle(_scanType))
        return false;

    EnumFlag<WardenCheckFlags> checkFlags(_flags);

    Player* player = session->GetPlayer();
    if (checkFlags.HasFlag(WardenCheckFlags::IsLoggedIn) != (player != nullptr && player->IsInWorld()))
        return false;

    switch (warden->GetPlatform())
    {
        case WardenPlatform::Win:
            if (!checkFlags.HasFlag(WardenCheckFlags::Win32))
                return false;
            break;
        case WardenPlatform::Wn64:
            if (!checkFlags.HasFlag(WardenCheckFlags::Win64))
                return false;
            break;
        case WardenPlatform::Mac:
            if (!checkFlags.HasFlag(WardenCheckFlags::OSX))
                return false;
            break;
        default:
            return false;
    }

    return true;
}

void WardenCheck::WritePackedValue(ByteBuffer& buffer, uint64 value)
{
    uint8* bytes = reinterpret_cast<uint8(&)[8]>(value);

    uint32 fullByteCount = 0;
    uint32 zeroByteCount = 0;

    for (uint32 byteIndex = 1; byteIndex < sizeof(uint64); ++byteIndex)
    {
        uint8 currentByte = bytes[sizeof(uint64) - byteIndex];
        if (currentByte == 0xFF)
            ++fullByteCount;
        else if (currentByte == 0x00)
            ++zeroByteCount;
    }

    size_t maskPosition = buffer.wpos();
    buffer << uint8(0);
    buffer << uint8(bytes[0]);

    // Pick a default value between 0x00 and 0xFF for each byte that is not sent
    // It makes perfect sense to optimize sending 64 bytes. /s
    uint8 defaultValue = (zeroByteCount < fullByteCount) ? 0xFF : 0x00;
    uint8 currentMask = defaultValue;

    for (uint32 byteIndex = sizeof(uint64) - 1; byteIndex > 0; --byteIndex)
    {
        currentMask >>= 1;

        uint8 currentByte = bytes[sizeof(uint64) - byteIndex];
        // Send the byte if different from the default calculated value.
        if (currentByte != defaultValue)
        {
            buffer << uint8(currentByte);
            // Set the high bit, we shift it next cycle if need be
            currentMask |= uint32(0x80);
        }
    }

    buffer.put(maskPosition, currentMask);
}

uint64 WardenCheck::ReadPackedValue(ByteBuffer& buffer)
{
    uint8 bytesMask;
    buffer >> bytesMask;
    uint8 defaultValue = (bytesMask & 1) ? 0xFF : 0x00;

    uint64 value = 0;
    uint8* valueBytes = reinterpret_cast<uint8*>(&value);

    buffer >> valueBytes[0];
    for (size_t i = 1; i < sizeof(uint64_t); ++i)
    {
        if (bytesMask & (1 << i))
            buffer >> valueBytes[i];
        else
            valueBytes[i] = defaultValue;
    }

    return value;
}

bool WardenCheck::TransformResultCode(bool checkFailed) const
{
    EnumFlag<WardenCheckFlags> checkFlags(_flags);
    if (checkFlags.HasFlag(WardenCheckFlags::Inverted))
        checkFailed = !checkFailed;

    return checkFailed;
}

WardenCheckResult WardenCheck::HandleResponse(bool checkFailed) const
{
    if (checkFailed)
        return sWardenMgr->GetCheckFailureAction(GetID());

    return WardenCheckResult::Success;
}
