#pragma once
#include <optional>
#include <vector>
#include <cstdint>
#include <array>

using Bytes = std::vector<const std::uint8_t>;

inline std::uint16_t be16(Bytes d, std::size_t off) {
    return static_cast<std::uint16_t>((d[off] << 8) | d[off + 1]);
}

inline std::uint32_t be32(Bytes d, std::size_t off) {
    return (std::uint32_t(d[off]) << 24) | (std::uint32_t(d[off + 1]) << 16) | (std::uint32_t(d[off] + 2)) << 8 | (std::uint32_t(d[off + 3]));
}
