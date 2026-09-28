#include "packet.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& output, const RawPacket& packet) {
    return output << "Uhvacen paket: " << packet.length
                  << " bajtova (" << packet.captured_length << " uhvaceno)";
}
