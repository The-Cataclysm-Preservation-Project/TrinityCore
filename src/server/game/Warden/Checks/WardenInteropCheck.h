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

#ifndef WARDEN_INTEROP_CHECK_H_
#define WARDEN_INTEROP_CHECK_H_

#include "WardenCheck.h"

#include <functional>
#include <memory>
#include <vector>

class Warden;

struct WardenInteropCheck final : public WardenCheck, std::enable_shared_from_this<WardenInteropCheck>
{
    WardenInteropCheck();

    bool LoadFromDB(Field* fields) override;
    bool WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) override;
    bool ProcessResponse(Warden* warden, ByteBuffer& packet) const override;

    using HandlerType = std::function<void(std::shared_ptr<const WardenInteropCheck>, uint64, uint64, uint64)>;

    void RegisterResponseHandler(HandlerType&& handler)
    {
        _handlers.emplace_back(std::move(handler));
    }

private:
    std::vector<HandlerType> _handlers;
};

#endif // WARDEN_INTEROP_CHECK_H_
