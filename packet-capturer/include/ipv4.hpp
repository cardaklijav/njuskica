#pragma once
#include "bytes.hpp"

struct IPv4Header {
    std::uint8_t ihl = 0, ttl = 0, protocol = 0;
    std::uint16_t total_length = 0, frag_fields = 0;
    std::uint32_t src = 0, dst = 0;
    std::size_t header_len = 0;
};

std::optional<IPv4Header> parse_ipv4(Bytes d);