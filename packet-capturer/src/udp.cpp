#include "udp.hpp"
#include <stdexcept>
#include <ostream>

UdpHeader parse_udp(Bytes d) {
    if(d.size() < 8) throw std::runtime_error("UDP header too short");
    UdpHeader h;
    h.src_port = be16(d, 0);
    h.dst_port = be16(d, 2);
    h.length = be16(d, 4);
    if(h.length < 8 || h.length > d.size()) throw std::runtime_error("UDP length is invalid");
    h.payload = d.subspan(8, h.length - 8);
    h.checksum = be16(d, 6);

    return h;
}

std::ostream& operator<<(std::ostream& os, const UdpHeader& h) {
    return os << "UDP{src_port=" << h.src_port
              << ", dst_port=" << h.dst_port
              << ", length=" << h.length
              << ", checksum=" << h.checksum
              << ", payload_size=" << h.payload.size()
              << '}';
}
