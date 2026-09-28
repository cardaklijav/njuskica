#pragma once
#include "bytes.hpp"

struct TcpHeader {
    std::uint16_t src_port = 0, dst_port = 0, checksum = 0, urgent_pointer = 0;
    std::uint32_t seq = 0, ack = 0;
    std::uint8_t flags = 0;
    std::uint16_t window = 0;
    std::size_t header_len = 0;
    Bytes payload;
};

TcpHeader parse_tcp(Bytes d);