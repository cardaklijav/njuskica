#pragma once
#include <cstdint>
#include <vector>
#include <chrono>

struct RawPacket {
    std::vector<uint8_t> data;                              // sirovi bajtovi, netaknuti
    std::chrono::system_clock::time_point timestamp;        // kad je uhvaćen
    uint32_t length;                                        // originalna dužina (može biti > data.size() ako je snaplen manji)
    uint32_t captured_length;                               // koliko je stvarno uhvaćeno
};