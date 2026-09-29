#pragma once

#include <iosfwd>
#include <variant>

#include "icmp.hpp"
#include "ipv4.hpp"
#include "ipv6.hpp"
#include "tcp.hpp"
#include "udp.hpp"

void printFormatHeader(std::ostream& os);

void formatPacket(
    std::ostream& os,
    const std::variant<IPv4Header, IPv6Header>& ipHeader,
    const std::variant<std::monostate, TcpHeader, UdpHeader, IcmpHeader>& transportHeader
);
