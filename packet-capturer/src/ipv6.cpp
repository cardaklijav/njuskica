#include "ipv6.hpp"
#include <stdexcept>

IPv6Header parse_ipv6(Bytes d) {
    if(d.size() < 40) throw std::runtime_error("IPv6 header is too short");
    std::uint8_t b0 = d[0], b1 = d[1];
    const std::uint8_t version = d[0] >> 4;
    if(version != 6) throw std::runtime_error("Not an IPv6 packet");
    IPv6Header h;
    h.total_length = be16(d, 4);
    if(h.total_length > d.size() - 40) throw std::runtime_error("IPv6 payload length is too long");
    h.paylaod = d.subspan(40, h.total_length);
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