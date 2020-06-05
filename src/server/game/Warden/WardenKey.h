#ifndef WARDEN_KEY_H_
#define WARDEN_KEY_H_

#include "Common.h"
#include "SHA1.h"

#include <array>

struct WardenKey
{
    uint32 ID;
    std::array<uint8_t, 16> Seed;
    std::array<uint8_t, 16> ClientKey;
    std::array<uint8_t, 16> ServerKey;

    bool Validate(std::array<uint8_t, SHA_DIGEST_LENGTH> const& clientProof) const;
};

#endif // WARDEN_KEY_H_
