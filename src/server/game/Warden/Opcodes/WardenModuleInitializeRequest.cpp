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

#include "WardenModuleInitializeRequest.h"
#include "WardenDefines.h"

void WardenModuleInitializeMPQRequest::Write(Warden* warden, ByteBuffer& buffer)
{
    ByteBuffer bodyBuffer;
    bodyBuffer << uint8(1);
    bodyBuffer << uint8(0);
    bodyBuffer << uint8(2); // 2 also allowed. Populates a different set of pointers
    Base::Write(warden, bodyBuffer);

    uint32 checksum = warden->BuildChecksum(bodyBuffer.contents(), bodyBuffer.wpos());

    buffer << uint8(WARDEN_SMSG_MODULE_INITIALIZE);
    buffer << uint16(bodyBuffer.wpos());
    buffer << uint32(checksum);
    buffer.append(bodyBuffer);
}

void WardenModuleInitializeTimingRequest::Write(Warden* warden, ByteBuffer& buffer)
{
    ByteBuffer bodyBuffer;
    bodyBuffer << uint8(1);
    bodyBuffer << uint8(1);
    Base::Write(warden, bodyBuffer);
    bodyBuffer << uint8(1); // Trailing byte, read but unused

    uint32 checksum = warden->BuildChecksum(bodyBuffer.contents(), bodyBuffer.wpos());

    buffer << uint8(WARDEN_SMSG_MODULE_INITIALIZE);
    buffer << uint16(bodyBuffer.wpos());
    buffer << uint32(checksum);
    buffer.append(bodyBuffer);
}

void WardenModuleInitializeFrameXMLRequest::Write(Warden* warden, ByteBuffer& buffer)
{
    ByteBuffer bodyBuffer;
    bodyBuffer << uint8(4);
    bodyBuffer << uint8(0);
    Base::Write(warden, bodyBuffer);
    bodyBuffer << uint8(1); // Trailing byte, read but unused

    uint32 checksum = warden->BuildChecksum(bodyBuffer.contents(), bodyBuffer.wpos());

    buffer << uint8(WARDEN_SMSG_MODULE_INITIALIZE);
    buffer << uint16(bodyBuffer.wpos());
    buffer << uint32(checksum);
    buffer.append(bodyBuffer);
}
