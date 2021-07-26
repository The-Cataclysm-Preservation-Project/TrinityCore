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

#ifndef WARDEN_LUA_CHECK_H_
#define WARDEN_LUA_CHECK_H_

#include "WardenCheck.h"
#include "WardenFwd.h"

#include <string>

class Warden;

struct WardenLuaCheck : public WardenCheck
{
    WardenLuaCheck(Type scanType, Field* fields);

    bool TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override final;
    WardenCheckResult ProcessResponse(ByteBuffer& packet) const override final;

    std::string const& GetExpectedResult() const { return _expectedResult; }

    void SetExecutableString(std::string const& executableString) { _executableString = executableString; }
    void SetQueriedString(std::string const& queriedString) { _queriedString = queriedString; }

protected:
    virtual WardenCheckResult HandleExtendedResponse(bool checkFailed, std::string const& clientResponse) const;

private:
    std::string _executableString;
    std::string _queriedString;
    std::string _expectedResult;
};

#endif // WARDEN_LUA_CHECK_H_
