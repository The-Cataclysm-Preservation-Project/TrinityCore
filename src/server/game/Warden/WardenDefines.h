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

#ifndef WARDEN_DEFINES_H_
#define WARDEN_DEFINES_H_

#include "Common.h"
#include "EnumFlag.h"

enum WardenOpcodes : uint32
{
    // Client->Server
    WARDEN_CMSG_MODULE_MISSING                  = 0,
    WARDEN_CMSG_MODULE_OK                       = 1,
    WARDEN_CMSG_CHEAT_CHECKS_RESULT             = 2,
    WARDEN_CMSG_MEM_CHECKS_RESULT               = 3,        // only sent if MEM_CHECK bytes doesn't match
    WARDEN_CMSG_HASH_RESULT                     = 4,
    WARDEN_CMSG_MODULE_FAILED                   = 5,        // this is sent when client failed to load uploaded module due to cache fail

    // Server->Client
    WARDEN_SMSG_MODULE_USE                      = 0,
    WARDEN_SMSG_MODULE_CACHE                    = 1,
    WARDEN_SMSG_CHEAT_CHECKS_REQUEST            = 2,
    WARDEN_SMSG_MODULE_INITIALIZE               = 3,
    WARDEN_SMSG_MEM_CHECKS_REQUEST              = 4,        // byte len; while (!EOF) { byte unk(1); byte index(++); string module(can be 0); int offset; byte len; byte[] bytes_to_compare[len]; }
    WARDEN_SMSG_HASH_REQUEST                    = 5
};

enum WardenActions : uint32
{
    WARDEN_ACTION_LOG,
    WARDEN_ACTION_KICK,
    WARDEN_ACTION_BAN,
    WARDEN_ACTION_MAX
};

enum class WardenPlatform : uint8
{
    Win,
    Wn64,
    Mac
};

enum class WardenCheckFlags : uint32
{
    /// <summary>
    /// Check can only be sent if the player is logged on a character.
    /// </summary>
    IsLoggedIn        = 0x00000001,
    /// <summary>
    /// The result of the check is inverted (must differ from result).
    /// </summary>
    Inverted          = IsLoggedIn << 1,

    AmericanLocale    = Inverted   << 1,
    KoreanLocale      = AmericanLocale << 1,
    FrenchLocale      = KoreanLocale << 1,
    GermanLocale      = FrenchLocale << 1,
    ChineseLocale     = GermanLocale << 1,
    TaiwaneseLocale   = ChineseLocale << 1,
    SpanishLocale     = TaiwaneseLocale << 1,
    MexicanLocale     = SpanishLocale << 1,
    RussianLocale     = MexicanLocale << 1,
    BrazilianLocale   = RussianLocale << 1,

    Win32             = BrazilianLocale << 1,
    Win64             = Win32 << 1,
    OSX               = Win64 << 1
};

DEFINE_ENUM_FLAG(WardenCheckFlags);

#endif // WARDEN_DEFINES_H_
