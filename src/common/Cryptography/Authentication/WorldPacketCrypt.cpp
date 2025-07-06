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

#include "WorldPacketCrypt.h"
#include "CryptoHash.h"
#include "HMAC.h"
#include <cstring>

WorldPacketCrypt::WorldPacketCrypt() : PacketCrypt()
{
}

void WorldPacketCrypt::Init(SessionKey const& K)
{
    std::array<uint8, SEED_KEY_SIZE> ServerEncryptionKey = { 0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91, 0x53, 0x57 };
    std::array<uint8, SEED_KEY_SIZE> ServerDecryptionKey = { 0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43, 0x67, 0xCE };
    Init(K, ServerEncryptionKey, ServerDecryptionKey);
}

void WorldPacketCrypt::Init(SessionKey const& K, std::array<uint8, SEED_KEY_SIZE> const& serverKey, std::array<uint8, SEED_KEY_SIZE> const& clientKey)
{
    _serverEncrypt.Init(Trinity::Crypto::HMAC_SHA1::GetDigestOf(serverKey, K));
    _clientDecrypt.Init(Trinity::Crypto::HMAC_SHA1::GetDigestOf(clientKey, K));

    // Drop first 1024 bytes, as WoW uses ARC4-drop1024.
    std::array<uint8, 1024> syncBuf;
    _serverEncrypt.UpdateData(syncBuf);
    _clientDecrypt.UpdateData(syncBuf);

    _initialized = true;
}
