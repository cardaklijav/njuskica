#include "ipv4.hpp"

inline std::optional<IPv4Header> parse_ipv4(Bytes d) {
    if (d.size() < 20) return std::nullopt;
    const std::uint8_t version = d[0] >> 4;
    IPv4Header h;
    h.ihl = d[0] & 0x0F;
    if (version != 4 || h.ihl < 5) return std::nullopt;
    h.header_len = std::size_t(h.ihl) * 4;
    if( d.size() < h.header_len) return std::nullopt;
    h.total_length = be16(d, 2);
    if(h.total_length < h.header_len) return std::nullopt;
    h.frag_fields = be16(d, 6);
    h.ttl = d[8];
    h.protocol = d[9];
    h.src = be32(d, 12);
    h.dst = be32(d, 16);

    return h;
}