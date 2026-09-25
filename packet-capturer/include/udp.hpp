#pragma once
#include "bytes.hpp"

struct UdpHeader {
    std::uint16_t src_port = 0, dst_port = 0, length = 0;
    std::size_t header_len = 8;
};

std::optional<UdpHeader> parse_udp(Bytes d);