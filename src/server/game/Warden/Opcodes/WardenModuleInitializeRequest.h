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

#ifndef WARDEN_MODULE_INITIALIZE_REQUEST_H_
#define WARDEN_MODULE_INITIALIZE_REQUEST_H_

#include "Common.h"
#include "ByteBuffer.h"
#include "Warden.h"

#include <array>

template <size_t N>
struct WardenModuleInitializeRequest
{
    std::string Module;
    std::array<uint32, N> Functions;

    virtual void Write(Warden* warden, ByteBuffer& buffer)
    {
         buffer << Module;
         for (uint32 function : Functions)
             buffer << uint32(function);
    }
};

struct WardenModuleInitializeFrameXMLRequest final : WardenModuleInitializeRequest<2>
{
    using Base = WardenModuleInitializeRequest<2>;

    void Write(Warden* warden, ByteBuffer& buffer) override;
};

struct WardenModuleInitializeTimingRequest final : WardenModuleInitializeRequest<1>
{
    using Base = WardenModuleInitializeRequest<1>;

    void Write(Warden* warden, ByteBuffer& buffer) override;
};

struct WardenModuleInitializeMPQRequest final : WardenModuleInitializeRequest<4>
{
    using Base = WardenModuleInitializeRequest<4>;

    void Write(Warden* warden, ByteBuffer& buffer) override;
};

#endif // WARDEN_MODULE_INITIALIZE_REQUEST_H_
