#pragma once
#include <iosfwd>
#include "bytes.hpp"

struct IcmpHeader {
    std::uint8_t type =0, code = 0;
    std::uint16_t checksum = 0;
    std::array<std::uint8_t, 4> type_data{};
    std::size_t header_len = 8;
    std::optional<std::uint16_t> id, seq;
    Bytes payload;
};

IcmpHeader parse_icmp(Bytes d);
std::ostream& operator<<(std::ostream& os, const IcmpHeader& h);
