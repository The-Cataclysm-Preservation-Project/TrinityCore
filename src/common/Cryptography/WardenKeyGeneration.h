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

#include "SHA1.h"

#include <cstring>

#ifndef _WARDEN_KEY_GENERATION_H
#define _WARDEN_KEY_GENERATION_H

class SHA1Randx
{
public:
    SHA1Randx(uint8* buff, uint32 size)
    {
        uint32 halfSize = size / 2;

        sh.Initialize();
        sh.UpdateData(buff, halfSize);
        sh.Finalize();

        o1 = sh.GetDigest();

        sh.Initialize();
        sh.UpdateData(buff + halfSize, size - halfSize);
        sh.Finalize();

        o2 = sh.GetDigest();
        o0.fill(0x00);

        FillUp();
    }

    void Generate(uint8* buf, uint32 sz)
    {
        for (uint32 i = 0; i < sz; ++i)
        {
            if (taken == 20)
                FillUp();

            buf[i] = o0[taken];
            taken++;
        }
    }

    template <size_t N>
    void Generate(std::array<uint8_t, N>& buffer)
    {
        Generate(buffer.data(), buffer.size());
    }

private:
    void FillUp()
    {
        sh.Initialize();
        sh.UpdateData(o1.data(), o1.size());
        sh.UpdateData(o0.data(), o0.size());
        sh.UpdateData(o2.data(), o2.size());
        sh.Finalize();

        o0 = sh.GetDigest();

        taken = 0;
    }

    SHA1Hash sh;
    uint32 taken;
    std::array<uint8, SHA_DIGEST_LENGTH> o0;
    std::array<uint8, SHA_DIGEST_LENGTH> o1;
    std::array<uint8, SHA_DIGEST_LENGTH> o2;
};

#endif
