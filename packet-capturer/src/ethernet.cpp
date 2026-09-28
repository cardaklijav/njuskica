#include "ethernet.hpp"

inline std::optional<EthernetHeader> parse_ethernet(Bytes d) {
    if(d.size() < 14) return std::nullopt;
    EthernetHeader h;
    for(int i = 0; i < 6; ++i) {
        h.dst[i] = d[i];
        h.src[i] = d[6 + i];
    }
    if(h.ether_type == 0x8100) {
        if(d.size() < 18) return std::nullopt;
        h.ether_type = be16(d, 16);
        h.header_len = 18;
    }
    h.ether_type = be16(d, 12);
    
    return h;
}