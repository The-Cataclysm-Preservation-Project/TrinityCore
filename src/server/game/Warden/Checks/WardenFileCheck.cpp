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

#include "WardenFileCheck.h"
#include "WardenDefines.h"
#include "EnumFlag.h"
#include "Log.h"
#include "Warden.h"
#include "WardenCheatCheckRequest.h"
#include "WorldSession.h"

#include <boost/algorithm/hex.hpp>

WardenFileCheck::WardenFileCheck(Field* fields) : WardenCheck(Type::MPQ, fields)
{
    _expectedResult.fill(0);

    _fileName = ReadDatabaseField<DatabaseColumn::Data0>(fields);

    std::string expectedResult = ReadDatabaseField<DatabaseColumn::Result>(fields);

    auto itr = boost::algorithm::unhex(expectedResult.begin(), expectedResult.end(), _expectedResult.begin());
    if (itr != _expectedResult.end())
        throw std::invalid_argument(Trinity::StringFormat("Warden check %u has invalid result data (40 bytes expected)", GetID()));
}

bool WardenFileCheck::WriteWardenCheckRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer)
{
    auto [stringIndex, success] = request.RegisterString(_fileName);
    if (!success || !request.CanWrite(2 * sizeof(uint8)))
        return false;

    requestBuffer << uint8(warden->EncodeWardenCheck(GetCheckType()));
    requestBuffer << uint8(stringIndex);
    return true;
}

WardenCheckResult WardenFileCheck::ProcessResponse(Warden* warden, ByteBuffer& packet) const
{
    uint8 scanCode;

    std::array<uint8, SHA_DIGEST_LENGTH> clientDigest;
    packet >> scanCode;

    bool checkFailed = scanCode != 0x00;
    if (!checkFailed)
    {
        packet.read(clientDigest.data(), clientDigest.size());

        checkFailed = clientDigest != _expectedResult;
    }
    
    checkFailed = TransformResultCode(checkFailed);
    return HandleResponse(checkFailed);
}

bool WardenFileCheck::TrySelect(WorldSession* session, Warden* warden)
{
    if (!WardenCheck::TrySelect(session, warden))
        return false;
    
    EnumFlag<WardenCheckFlags> checkFlags(GetFlags());
    
    switch (session->GetSessionDbLocaleIndex())
    {
        case LOCALE_enUS:
            if (!checkFlags.HasFlag(WardenCheckFlags::AmericanLocale))
                return false;
            break;
        case LOCALE_koKR:
            if (!checkFlags.HasFlag(WardenCheckFlags::KoreanLocale))
                return false;
            break;
        case LOCALE_frFR:
            if (!checkFlags.HasFlag(WardenCheckFlags::FrenchLocale))
                return false;
            break;
        case LOCALE_deDE:
            if (!checkFlags.HasFlag(WardenCheckFlags::GermanLocale))
                return false;
            break;
        case LOCALE_zhCN:
            if (!checkFlags.HasFlag(WardenCheckFlags::ChineseLocale))
                return false;
            break;
        case LOCALE_zhTW:
            if (!checkFlags.HasFlag(WardenCheckFlags::TaiwaneseLocale))
                return false;
            break;
        case LOCALE_esES:
            if (!checkFlags.HasFlag(WardenCheckFlags::SpanishLocale))
                return false;
            break;
        case LOCALE_esMX:
            if (!checkFlags.HasFlag(WardenCheckFlags::MexicanLocale))
                return false;
            break;
        case LOCALE_ruRU:
            if (!checkFlags.HasFlag(WardenCheckFlags::RussianLocale))
                return false;
            break;
        case LOCALE_ptBR:
            if (!checkFlags.HasFlag(WardenCheckFlags::BrazilianLocale))
                return false;
            break;
        default: // Should never happen
            return false;
    }
    
    return true;
}
