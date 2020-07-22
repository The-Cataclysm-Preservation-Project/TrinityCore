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

enum class WardenActions : uint32
{
    Log,
    Kick,
    Ban
};

enum class WardenPlatform : uint8
{
    Win,
    Wn64,
    Mac
};

enum class WardenCheckFlags : uint32
{
    IsLoggedIn        = 0x00000001, //< Check can only be sent if the player is logged on a character.
    Inverted          = 0x00000002, //< The result of the check is inverted (must differ from result).

    AmericanLocale    = 0x00000004, //< Check applies to enUS locale.
    KoreanLocale      = 0x00000008, //< Check applies to krKR locale.
    FrenchLocale      = 0x00000010, //< Check applies to frFR locale.
    GermanLocale      = 0x00000020, //< Check applies to deDE locale.
    ChineseLocale     = 0x00000040, //< Check applies to zhCN locale.
    TaiwaneseLocale   = 0x00000080, //< Check applies to zhTW locale.
    SpanishLocale     = 0x00000100, //< Check applies to esES locale.
    MexicanLocale     = 0x00000200, //< Check applies to esMX locale.
    RussianLocale     = 0x00000400, //< Check applies to ruRU locale.
    BrazilianLocale   = 0x00000800, //< Check applies to esBR locale.

    Win32             = 0x00001000, //< Check applies to 32-bits Windows clients.
    Win64             = 0x00002000, //< Check applies to 64-bits Windows clients.
    OSX               = 0x00004000  //< Check applies to OSX clients.
};

DEFINE_ENUM_FLAG(WardenCheckFlags);

#endif // WARDEN_DEFINES_H_
