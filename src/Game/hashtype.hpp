#pragma once

#include <cstdint>

using hash_t = int32_t;
using uhash_t = uint32_t;

constexpr hash_t hash_from_u32(uhash_t value) noexcept
{
    return static_cast<hash_t>(value);
}

namespace rage
{
    using atHashString = hash_t;
    using atFinalHashString = hash_t;
}