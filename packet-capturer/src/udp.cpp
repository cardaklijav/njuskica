#include "udp.hpp"

inline std::optional<UdpHeader> parse_udp(Bytes d) {
    if(d.size() < 8) return std::nullopt;
    UdpHeader h;
    h.src_port = be16(d, 0);
    h.dst_port = be16(d, 2);
    h.length = be16(d, 4);
    if (h.length < 8) return std::nullopt;

    return h;
}
