#include "ethernet.hpp"
#include <stdexcept>

EthernetHeader parse_ethernet(Bytes d) {
    if(d.size() < 14) throw std::runtime_error("Ethernet header too short");
    EthernetHeader h;
    for(int i = 0; i < 6; ++i) {
        h.dst[i] = d[i];
        h.src[i] = d[6 + i];
    }
    if(h.ether_type == 0x8100) {
        if(d.size() < 18) throw std::runtime_error("Ethernet header too short");
        h.ether_type = be16(d, 16);
        h.header_len = 18;
    }
    h.ether_type = be16(d, 12);
    h.payload = d.subspan(h.header_len);
    
    return h;
}