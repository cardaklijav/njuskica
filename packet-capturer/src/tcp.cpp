#include "tcp.hpp"

inline std::optional<TcpHeader> parse_tcp(Bytes d) {
    if(d.size() < 20) return std::nullopt;
    TcpHeader h;
    h.header_len = std::size_t(d[12] >> 4) * 4;
    if(h.header_len < 20 || d.size() < h.header_len) return std::nullopt;
    h.src_port = be16(d, 0);
    h.dst_port = be16(d, 2);
    h.seq = be32(d, 4);
    h.ack = be32(d, 6);
    h.flags = d[13];
    h.window = be16(d, 14);

    return h;
}