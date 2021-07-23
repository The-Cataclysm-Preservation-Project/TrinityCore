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

#ifndef WARDEN_FILE_CHECK_H_
#define WARDEN_FILE_CHECK_H_

#include "WardenCheck.h"
#include "SHA1.h"

#include <array>
#include <functional>
#include <memory>
#include <vector>

class Warden;

struct WardenFileCheck final : public WardenCheck
{
    WardenFileCheck(Field* fields);
    
    bool TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override;
    WardenCheckResult ProcessResponse(Warden* warden, ByteBuffer& packet) const override;

    bool TrySelect(WorldSession* session, Warden* warden) override;

    std::array<uint8, SHA_DIGEST_LENGTH> const& GetExpectedResult() const { return _expectedResult; }
    std::string const& GetFileName() const { return _fileName; }

private:
    std::string _fileName;
    std::array<uint8, SHA_DIGEST_LENGTH> _expectedResult;
};

#endif // WARDEN_FILE_CHECK_H_
