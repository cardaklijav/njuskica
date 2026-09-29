#include "icmp.hpp"
#include <stdexcept>
#include <ostream>

IcmpHeader parse_icmp(Bytes d) {
    IcmpHeader h;
    if(d.size() < h.header_len) throw std::runtime_error("ICMP header is too short for header length");
    h.type = d[0];
    h.code = d[1];
    h.checksum = be16(d, 2);
    for(int i = 0; i < 4; ++i) {
        h.type_data[i] = d[i + 4];
    }
    if(h.type == 8 || h.type == 0 || h.type == 128 || h.type == 129) {
        h.id = be16(d, 4);
        h.seq = be16(d, 6);
    }
    h.payload = d.subspan(h.header_len);

    return h;
}

std::ostream& operator<<(std::ostream& os, const IcmpHeader& h) {
    return os << "ICMP" << '\n' <<"type=" << int(h.type) << '\n'
    << "code=" << int(h.code) << '\n'
    << "checksum=" << h.checksum << '\n'
    << "identifier=" << h.id.value_or(0) << '\n'
    << "sequence_number=" << h.seq.value_or(0) << '\n';
}