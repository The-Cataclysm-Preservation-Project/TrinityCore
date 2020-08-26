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

#ifndef WARDEN_PAGE_CHECK_H_
#define WARDEN_PAGE_CHECK_H_

#include "WardenCheck.h"

#include <functional>
#include <memory>
#include <vector>

class Warden;

//< Handles PageCheckA and PageCheckB.
struct WardenPageCheck final : public WardenCheck, std::enable_shared_from_this<WardenPageCheck>
{
    WardenPageCheck(WardenCheck::Type checkType);

    bool LoadFromDB(Field* fields) override;
    bool WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override;
    bool ProcessResponse(Warden* warden, ByteBuffer& packet) const override;

    uint32 GetAddress() const { return _address; }
    uint8 GetLength() const { return _length; }


private:
    uint32 _address;
    uint8 _length;
    std::vector<uint8> _expectedData;
};

#endif // WARDEN_PAGE_CHECK_H_
