#include "tcp.hpp"
#include <stdexcept>
#include <ostream>

TcpHeader parse_tcp(Bytes d) {
    if(d.size() < 20) throw std::runtime_error("TCP header too short");
    TcpHeader h;
    h.header_len = std::size_t(d[12] >> 4) * 4;
    if(h.header_len < 20 || d.size() < h.header_len) throw std::runtime_error("TCP header too short");
    h.src_port = be16(d, 0);
    h.dst_port = be16(d, 2);
    h.seq = be32(d, 4);
    h.ack = be32(d, 8);
    h.flags = d[13];
    h.window = be16(d, 14);
    h.checksum = be16(d, 16);
    h.urgent_pointer = be16(d, 18);
    h.payload = d.subspan(h.header_len);

    return h;
}

std::ostream& operator<<(std::ostream& os, const TcpHeader& h) {
    return os << "TCP" << '\n' << "src_port=" << h.src_port << '\n'
              << "dst_port=" << h.dst_port << '\n'
              << "seq=" << h.seq << '\n' 
              << "ack=" << h.ack << '\n'
              << "flags=" << int(h.flags) << '\n'
              << "window=" << h.window
              << '\n';
}