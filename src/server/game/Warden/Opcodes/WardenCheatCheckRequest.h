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

#ifndef WARDEN_CHEAT_CHECK_REQUEST_H_
#define WARDEN_CHEAT_CHECK_REQUEST_H_

#include "Common.h"
#include "ByteBuffer.h"

#include <unordered_set>

// Simple structure that encapsulates the string pooling logic of WARDEN_SMSG_CHEAT_CHECKS_REQUEST
// and maximum packet size.
struct WardenCheatChecksRequest
{
    constexpr const static uint32 MaximumSize = 512;
    using PoolType = std::unordered_set<std::string>;

    std::pair<uint32, bool> RegisterString(std::string const& string);
    void UnregisterString(uint32 stringIndex);

    uint32 GetSize() const { return _projectedSize; }

    bool CanWrite(uint32 size);

    PoolType::const_iterator begin() const { return _stringPool.begin(); }
    PoolType::const_iterator end() const { return _stringPool.end(); }

private:
    PoolType _stringPool;
    uint32 _projectedSize = 0;
};

#endif
