#include "capture.hpp"

#include <csignal>
#include <iostream>

namespace {
PacketCapture* active_capture = nullptr;

void handleSignal(int) {
    if (active_capture != nullptr) {
        active_capture->stop();
    }
}

void printPacket(const RawPacket& packet) {
    std::cout << "Uhvacen paket: " << packet.length
              << " bajtova (" << packet.captured_length << " uhvaceno)\n";
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
        capture.start(printPacket);

        active_capture = nullptr;
        std::cout << "Hvatanje je zavrseno.\n";
    } catch (const std::exception& error) {
        std::cerr << "Greska: " << error.what() << '\n';
        return 1;
    }

    return 0;
}