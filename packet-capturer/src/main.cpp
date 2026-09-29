#include "capture.hpp"

#include <csignal>
#include <iostream>

#include "bytes.hpp"
#include "ethernet.hpp"
#include "icmp.hpp"
#include "ipv4.hpp"
#include "ipv6.hpp"
#include "tcp.hpp"
#include "udp.hpp"

namespace {
PacketCapture* active_capture = nullptr;

void handleSignal(int) {
    if (active_capture != nullptr) {
        active_capture->stop();
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Upotreba: " << argv[0]
                  << " <interfejs> [bpf-filter]\n";
        return 1;
    }

    try {
        PacketCapture capture(argv[1]);
        capture.open();

        if (argc == 3) {
            capture.setFilter(argv[2]);
        }

        active_capture = &capture;
        std::signal(SIGINT, handleSignal);

        std::cout << "Slusam na interfejsu " << argv[1]
                  << "... Pritisnite Ctrl+C za prekid.\n";
        capture.start([](const RawPacket& packet) {
            std::cout << packet << '\n';
            try {
                Bytes d{packet.data.data(), packet.data.size()};
                EthernetHeader eth = parse_ethernet(d);
                std::cout << '\n' << "Ethernet: " << eth << '\n';
                if(eth.ether_type == 0x0800) {
                    const IPv4Header ip = parse_ipv4(eth.payload);
                    std::cout << ip << '\n';
                    if (ip.more_fragments || ip.fragment_offset != 0) {
                        return;
                    }
                    if(ip.protocol == 6) {
                        std::cout << '\n' << parse_tcp(ip.payload) << '\n';
                    }
                    if(ip.protocol == 17) {
                        std::cout << '\n' << parse_udp(ip.payload) << '\n';
                    }
                    if(ip.protocol == 1) {
                        std::cout << '\n' << parse_icmp(ip.payload) << '\n';
                    }
                }
                if(eth.ether_type == 0x86DD) {
                    const IPv6Header ip = parse_ipv6(eth.payload);
                    std::cout << ip << '\n';
                    // assuming there is nothing in between IPv6 and TCP/UDP, which is not always true
                    if(ip.next_header == 6) {
                        std::cout << parse_tcp(ip.payload) << '\n';
                    }
                    if(ip.next_header == 17) {
                        std::cout << parse_udp(ip.payload) << '\n';
                    }
                    if(ip.next_header == 58) {
                        std::cout << parse_icmp(ip.payload) << '\n';
                    }
                }
            } catch (const std::exception& error) {
                std::cerr << "Greska pri parsiranju paketa: " << error.what() << '\n';
            }
        });

        active_capture = nullptr;
        std::cout << "Hvatanje je zavrseno.\n";
    } catch (const std::exception& error) {
        std::cerr << "Greska: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
