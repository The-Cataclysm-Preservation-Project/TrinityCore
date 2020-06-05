#include "WardenKey.h"

bool WardenKey::Validate(std::array<uint8_t, SHA_DIGEST_LENGTH> const& clientProof) const
{
    SHA1Hash hash;
    hash.UpdateData(ClientKey.data(), ClientKey.size());
    hash.Finalize();

    return clientProof == hash.GetDigest();
}
