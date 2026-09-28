#include "ipv4.hpp"
#include <stdexcept>

inline IPv4Header parse_ipv4(Bytes d) {
    if (d.size() < 20) throw std::runtime_error("IPv4 header too short");
    const std::uint8_t version = d[0] >> 4;
    IPv4Header h;
    h.ihl = d[0] & 0x0F;
    if (version != 4 || h.ihl < 5) throw std::runtime_error("Invalid IPv4 header or version");
    h.header_len = std::size_t(h.ihl) * 4;
    if(d.size() < h.header_len) throw  std::runtime_error("IPv4 header too short for IHL");
    h.total_length = be16(d, 2);
    if(h.total_length > d.size()) throw std::runtime_error("IPv4 total length exceeds data size");
    if(h.total_length < h.header_len) throw std::runtime_error("IPv4 total length less than header length");
    h.frag_fields = be16(d, 6);
    h.dont_fragment = (h.frag_fields & 0x4000) != 0;
    h.more_fragments = (h.frag_fields & 0x2000) != 0;
    h.fragment_offset = h.frag_fields & 0x1FFF;
    h.ttl = d[8];
    h.protocol = d[9];
    h.src = be32(d, 12);
    h.dst = be32(d, 16);
    h.payload = d.subspan(h.header_len, h.total_length - h.header_len);
    return h;
}