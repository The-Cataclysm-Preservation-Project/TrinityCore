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

#ifndef WARDENMEMORYCHECK_H_
#define WARDENMEMORYCHECK_H_

#include "WardenCheck.h"

#include <functional>
#include <memory>
#include <vector>

class Warden;

struct WardenMemoryCheck : public WardenCheck
{
    WardenMemoryCheck(Field* fields);
    WardenMemoryCheck(std::string const& moduleName, uint64 offset, uint64 length);

    bool TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override final;
    WardenCheckResult ProcessResponse(Warden* warden, ByteBuffer& packet) const override final;

    std::string const& GetModuleName() const { return _moduleName; }
    uint64 GetScanOffset() const { return _address; }
    uint32 GetScanLength() const { return _length; }
    std::vector<uint8> const& GetExpectedData() const { return _result; }

    void SetModuleName(std::string const& moduleName);
    void SetScanAddress(uint64 address) { _address = address; }
    void SetScanLength(uint8 length) { _length = length; }

protected:
    virtual WardenCheckResult HandleExtendedResponse(bool checkFailed, std::vector<uint8> const& clientResponse) const;

private:
    std::string _moduleName;    //< Name of the module being scanned.
    uint64 _address = 0;        //< Relative address of the scan.
    uint8 _length = 0;          //< Number of bytes to scan.
    std::vector<uint8> _result; //< The expected bytes.

};

#endif // WARDENMEMORYCHECK_H_
