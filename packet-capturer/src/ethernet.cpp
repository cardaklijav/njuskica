#include "ethernet.hpp"

inline std::optional<EthernetHeader> parse_ethernet(Bytes d) {
    if(d.size() < 14) return std::nullopt;
    EthernetHeader h;
    for(int i = 0; i < 6; ++i) {
        h.dst[i] = d[i];
        h.src[i] = d[6 + i];
    }
    h.ether_type = be16(d, 12);
    
    return h;
}