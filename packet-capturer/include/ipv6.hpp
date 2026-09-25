#pragma once
#include "bytes.hpp"

struct IPv6Header {
    std::uint8_t traffic_class = 0;
    std::uint32_t flow_label = 0;
    std::uint16_t payload_length = 0;
    std::uint8_t next_header = 0;
    std::uint8_t hop_limit = 0;
    std::array<std::uint8_t, 16> src{};
    std::array<std::uint8_t, 16> dst{};
    std::size_t header_len = 40;
};

std::optional<IPv6Header> parse_ipv6(Bytes d);