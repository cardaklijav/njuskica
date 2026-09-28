#include "udp.hpp"
#include <stdexcept>

UdpHeader parse_udp(Bytes d) {
    if(d.size() < 8) throw std::runtime_error("UDP header too short");
    UdpHeader h;
    h.src_port = be16(d, 0);
    h.dst_port = be16(d, 2);
    h.length = be16(d, 4);
    if(h.length < 8 || h.length > d.size()) throw std::runtime_error("UDP length is invalid");
    h.payload = d.subspan(8, h.length - 8);
    h.checksum = be16(d, 6);
    if (h.length > d.size()) throw std::runtime_error("UDP length exceeds data size");
    if (h.length < 8) throw  std::runtime_error("UDP length is less than header size");

    return h;
}
