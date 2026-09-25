#pragma once
#include "bytes.hpp"

struct EthernetHeader {
    std::array<std::uint8_t, 6> src{};
    std::array<std::uint8_t, 6> dst{};
    std::uint16_t ether_type = 0;
    std::size_t header_len = 14;
};

std::optional<EthernetHeader> parse_ethernet (Bytes d);