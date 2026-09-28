#include "ethernet.hpp"
#include <stdexcept>
#include <ostream>

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

std::ostream& operator<<(std::ostream& os, const EthernetHeader& h) {
    os << "Ethernet{src=";
    for(int i = 0; i < 6; ++i) {
        if(i > 0) os << ':';
        os << std::hex << int(h.src[i]);
    }
    os << ", dst=";
    for(int i = 0; i < 6; ++i) {
        if(i > 0) os << ':';
        os << std::hex << int(h.dst[i]);
    }
    return os << std::dec
              << ", ether_type=" << std::hex << h.ether_type
              << '}';
}