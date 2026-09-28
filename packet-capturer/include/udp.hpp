#pragma once
#include "bytes.hpp"

struct UdpHeader {
    std::uint16_t src_port = 0, dst_port = 0, length = 0, checksum = 0;
    std::size_t header_len = 8;
    Bytes payload;
};

UdpHeader parse_udp(Bytes d);