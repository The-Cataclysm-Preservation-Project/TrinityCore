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

#ifndef WARDEN_PROC_CHECK_H_
#define WARDEN_PROC_CHECK_H_

#include "WardenCheck.h"
#include "SHA1.h"

#include <array>
#include <functional>
#include <vector> 

class Warden;

struct WardenProcCheck : public WardenCheck
{
    WardenProcCheck(Field* fields);
    WardenProcCheck(std::string const& moduleName, std::string const& functionName, uint32 offset, uint8 length, std::vector<uint8> const& expectedData);

    bool TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override;
    WardenCheckResult ProcessResponse(Warden* warden, ByteBuffer& packet) const override;

    std::string const& GetModuleName() const { return _moduleName; }
    std::string const& GetFunctionName() const { return _functionName; }
    uint32 GetOffset() const { return _offset; }
    uint8 GetLength() const { return _length; }

private:
    std::string _moduleName;
    std::string _functionName;
    std::vector<uint8> _expectedData;
    uint32 _offset;
    uint8 _length;
};

#endif // WARDEN_PROC_CHECK_H_
