#include "capture.hpp"

#include <csignal>
#include <iostream>

// #include "bytes.hpp"
// #include "ethernet.hpp"
// #include "ipv4.hpp"
// #include "ipv6.hpp"

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
            // try {
            //     Bytes d{packet.data.data(), packet.data.size()};
            //     EthernetHeader eth = parse_ethernet(d);
            //     if(eth.ether_type == 0x0800) {
            //         std::cout << parse_ipv4(eth.payload);
            //     }
            //     if(eth.ether_type == 0x86DD) {
            //         std::cout << parse_ipv6(eth.payload);
            //     }
            // } catch (const std::exception& error) {
            //     std::cerr << "Greska pri parsiranju paketa: " << error.what() << '\n';
            // }
        });

        active_capture = nullptr;
        std::cout << "Hvatanje je zavrseno.\n";
    } catch (const std::exception& error) {
        std::cerr << "Greska: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
