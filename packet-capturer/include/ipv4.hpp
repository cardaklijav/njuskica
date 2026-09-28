#pragma once
#include "bytes.hpp"

struct IPv4Header {
    std::uint8_t ihl = 0, ttl = 0, protocol = 0;
    std::uint16_t total_length = 0, frag_fields = 0;
    bool dont_fragment;
    bool more_fragments;
    std::uint16_t fragment_offset;
    std::uint32_t src = 0, dst = 0;
    std::size_t header_len = 0;
    Bytes payload;
};

IPv4Header parse_ipv4(Bytes d);