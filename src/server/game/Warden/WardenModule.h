#ifndef WARDEN_MODULE_H_
#define WARDEN_MODULE_H_

#include "Define.h"
#include "Optional.h"
#include "Util.h"
#include "WardenFwd.h"
#include "WardenCheck.h"
#include "WardenKey.h"

#include <array>
#include <unordered_map>
#include <vector>

struct WardenModule
{
    //< ID of the module in database
    uint32 ID;
    //< Platform for which this module is eligible.
    WardenPlatform Platform;

    struct {
        //< The key used by the client to process `Data` through an RC4 cipher.
        std::array<uint8, 16> Key;
        //< The RC4'd module data.
        std::vector<uint8> Data;
    } Module;

    //< Various key chains used by the server when handshaking with the client.
    std::vector<WardenKey> KeyChains;

    //< A map associating each supported warden check with its encoded network value.
    std::array<Optional<uint8_t>, AsUnderlyingType(WardenCheck::Type::MAX)> Checks;

    //< Tries to load the module from database.
    bool LoadFromDB(Field* fields);

    //< Computes a SHA-256 digest of the module's RC4'd data.
    std::array<uint8, 32> GetModuleChecksum() const;

    //< Returns a randomly selected key to use for handshaking the module.
    WardenKey const& SelectRandomKey() const;
};

#endif // WARDEN_MODULE_H_
