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

#include "WardenCheatCheckRequest.h"

bool WardenCheatChecksRequest::CanWrite(uint32 size)
{
    // Client buffer is 512 bytes. We limit to 509 because:
    // 1 byte is needed for the command itself.
    // 1 byte is used for the encoding mask byte of each check requested.
    // 1 byte for the string block marker
    // See WardenCheatChecksRequest::Finalize().
    if (_projectedSize + size > MaximumSize - 3)
        return false;

    _projectedSize += size;
    return true;
}

void WardenCheatChecksRequest::UnregisterString(uint32 stringIndex)
{
    // String indices are 1-based and 0 means no string
    if (stringIndex == 0)
        return;

    stringIndex -= 1;

    auto itr = _stringPool.begin();
    if (_stringPool.size() < stringIndex)
        return;

    std::advance(itr, stringIndex);
    _projectedSize -= (*itr).length() + 1;
    _stringPool.erase(itr);
}

std::pair<uint32, bool> WardenCheatChecksRequest::RegisterString(std::string const& string)
{
    // The string is empty, return it as offset 0
    if (string.size() == 0)
        return std::make_pair(0, true);

    // Max 255 strings (std::numeric_limits<uint8>::max())
    if (_stringPool.size() >= 0xFF)
        return std::make_pair(0, false);

    // If injecting the string will exceed buffer size
    if (_projectedSize + string.length() + 1 > MaximumSize)
        return std::make_pair(0, false);

    // Pool the string
    auto [itr, success] = _stringPool.insert(string);
    if (success) // Adjust projected buffer size if first insertion
        _projectedSize += string.length() + 1;

    return std::make_pair(std::distance(_stringPool.begin(), itr) + 1, true);
}
