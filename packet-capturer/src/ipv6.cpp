#include "ipv6.hpp"

inline std::optional<IPv6Header> parse_ipv6(Bytes d) {
    if(d.size() < 40) return std::nullopt;
    std::uint8_t b0 = d[0], b1 = d[1];
    const std::uint8_t version = d[0] >> 4;
    if(version != 6) return std::nullopt;
    IPv6Header h;
    h.payload_length = be16(d, 4);
    h.next_header = d[6];
    h.hop_limit = d[7];
    h.traffic_class = (b0 << 4) | (b1 >> 4);
    h.flow_label = ((std::uint32_t(b1) & 0x0F) << 16) | (std::uint32_t(d[2]) << 8) | std::uint32_t(d[3]);
    for(int i = 0; i < 16; ++i) {
        h.src[i] = d[i + 8];
        h.dst[i] = d[i + 24];   
    }

    return h;
}