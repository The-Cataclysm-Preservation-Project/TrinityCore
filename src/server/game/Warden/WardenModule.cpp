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

    // Ensure key size is 32 characters (16 bytes)
    if (key.length() != 2 * Module.Key.size())
    {
        TC_LOG_ERROR("server.loading", "Module %u has an invalid key (16 bytes expected, %u read from database). Ignored.", ID, key.length());
        return false;
    }

    boost::algorithm::unhex(key, Module.Key.begin());

    Module.Data = std::move(fields[3].GetBinary());

    uint32 missingCheckCount = 0;
    for (uint32 i = 0; i < AsUnderlyingType(WardenCheck::Type::MAX); ++i)
    {
        if (fields[i + 4].IsNull())
        {
            ++missingCheckCount;
            continue;
        }

        Checks[i] = fields[i + 4].GetUInt8();
    }

    if (missingCheckCount == Checks.size())
        TC_LOG_ERROR("server.loading", "Module %u has no defined check codes. Skipped.", ID);

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
