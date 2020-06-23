#include "WardenModule.h"
#include "WardenDefines.h"
#include "Containers.h"
#include "Util.h"

#include <boost/algorithm/hex.hpp>
#include <openssl/sha.h>

bool WardenModule::LoadFromDB(Field* fields)
{
    ID                 = fields[0].GetUInt32();
    Platform           = static_cast<WardenPlatform>(fields[1].GetUInt8());
    std::string key    = fields[2].GetString();
    std::string module = fields[3].GetString();
    std::string checks = fields[4].GetString();

    // Ensure key size is 32 characters (16 bytes)
    if (key.length() != 2 * Module.Key.size())
        return false;

    boost::algorithm::unhex(key, Module.Key.begin());

    // Reserver module data
    Module.Data.resize(module.length() / 2);
    boost::algorithm::unhex(module, Module.Data.begin());

    // Parse checks - needs to have a value for each supported check in sequence, empty token if none
    Tokenizer tokens(fields[4].GetString(), ';', true);
    if (tokens.size() != static_cast<uint32>(WardenCheck::Type::MAX))
        return false;

    // Setup
    for (uint32 i = 0; i < static_cast<uint32>(WardenCheck::Type::MAX); ++i)
        if (strlen(tokens[i]) != 0)
            Checks[static_cast<WardenCheck::Type>(i)] = std::stoi(tokens[i], nullptr, 16);

    // Fail if no checks
    return !Checks.empty();
}

std::array<uint8, 32> WardenModule::GetModuleChecksum() const
{
    std::array<uint8, 32> signature;

    // sha256 hash
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, Module.Data.data(), Module.Data.size());
    SHA256_Final(signature.data(), &ctx);

    return signature;
}

WardenKey const& WardenModule::SelectRandomKey() const
{
    return Trinity::Containers::SelectRandomContainerElement(KeyChains);
}
