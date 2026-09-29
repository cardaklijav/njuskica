#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <variant>
#include <optional>

#include "ipv4.hpp"
#include "ipv6.hpp"
#include "icmp.hpp"
#include "tcp.hpp"
#include "udp.hpp"
#include "ethernet.hpp"

template <typename T>
void printOptional(std::ostream& os, const std::optional<T>& opt) {
    if (opt.has_value()) {
        os << *opt;
    }
}

void printFormatHeader(std::ostream& os) {
    // in order: ipv4, ipv6, ethernet, tcp, udp, icmp without repetition of common fields
    os << "ihl, ttl, protocol, total_length, frag_fields, dont_fragment, more_fragments, fragment_offset, src, dst, header_len, payload"
    << "traffic_class, flow_label, total_length, next_header, hop_limit"
    << "ether_type"
    << "src_port, dst_port, checksum, urgent_pointer, seq, ack, flags, window"
    << "type, code, identifier, sequence_number";
}

void formatPacket(std::ostream& os, 
                 const std::variant<IPv4Header, IPv6Header>& ipHeader,
                 const std::variant<std::monostate, TcpHeader, UdpHeader, IcmpHeader>& transportHeader) 
{
    // ip layer (ipv4 or ipv6)
    if (std::holds_alternative<IPv4Header>(ipHeader)) {
        const auto& h = std::get<IPv4Header>(ipHeader);
        
        os << "IPv4,"
           << ((h.src >> 24) & 0xFF) << '.' << ((h.src >> 16) & 0xFF) << '.' 
           << ((h.src >> 8) & 0xFF) << '.' << (h.src & 0xFF) << ','
           << ((h.dst >> 24) & 0xFF) << '.' << ((h.dst >> 16) & 0xFF) << '.' 
           << ((h.dst >> 8) & 0xFF) << '.' << (h.dst & 0xFF) << ','
           << int(h.protocol) << ','
           << int(h.ttl) << ','
           << h.total_length << ','
           << (h.dont_fragment ? "true" : "false") << ','
           << (h.more_fragments ? "true" : "false") << ','
           << h.fragment_offset << ','
           << h.payload.size() << ','
           << ",,"; // traffic_class i flow_label are empty for IPv4
    } else {
        const auto& h = std::get<IPv6Header>(ipHeader);
        
        os << "IPv6,";
        
        os << std::hex;
        for(int i = 0; i < 16; ++i) {
            if(i > 0) os << ':';
            os << int(h.src[i]);
        }
        os << ',';

        for(int i = 0; i < 16; ++i) {
            if(i > 0) os << ':';
            os << int(h.dst[i]);
        }
        os << std::dec << ',';

        os << int(h.next_header) << ','
           << int(h.hop_limit) << ','
           << h.total_length << ','
           << ",,,," // df, mf, fragment_offset, payload_size are empty for IPv6
           << int(h.traffic_class) << ','
           << h.flow_label << ',';
    }

    // transport layer (tcp, udp, icmp)
    if (std::holds_alternative<TcpHeader>(transportHeader)) {
        const auto& tcp = std::get<TcpHeader>(transportHeader);
        os << tcp.src_port << ','
           << tcp.dst_port << ','
           << tcp.seq << ','
           << tcp.ack << ','
           << tcp.flags << ','
           << tcp.window << ','
           << tcp.checksum << ','
           << tcp.urgent_pointer << ','
           << ',' // udp_len stays empty
           << ",,,,"; // icmp specific fields
    } 
    else if (std::holds_alternative<UdpHeader>(transportHeader)) {
        const auto& udp = std::get<UdpHeader>(transportHeader);
        os << udp.src_port << ','
           << udp.dst_port << ','
           << ",,,,,," // tcp specific fields 
           << udp.checksum << ','
           << ',' // urgent_pointer
           << udp.length << ','
           << ",,,,"; // icmp specific fields
    } 
    else if (std::holds_alternative<IcmpHeader>(transportHeader)) {
        const auto& icmp = std::get<IcmpHeader>(transportHeader);
        os << ",,,,,,,," // all TCP/UDP fields are empty
           << icmp.checksum << ",,," // checksum followed by empty urgent/udp_len
           << int(icmp.type) << ','
           << int(icmp.code) << ',';

        printOptional(os, icmp.id);
        os << ',';
            
        printOptional(os, icmp.seq);
    } 
    else {
        // no transport layer
        os << ",,,,,,,,,,,,,";
    }

    os << '\n';
}