#include "WardenKey.h"

WardenKey::WardenKey()
{
    ID = -1;
    ClientKey.fill(0);
    ServerKey.fill(0);
    Seed.fill(0);
}

bool WardenKey::Validate(std::array<uint8_t, SHA_DIGEST_LENGTH> const& clientProof) const
{
    SHA1Hash hash;
    hash.UpdateData(ClientKey.data(), ClientKey.size());
    hash.Finalize();

    return clientProof == hash.GetDigest();
}
