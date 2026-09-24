#include <cstdint>   // za uint64_t, uint16_t
#include <cstddef>   // za size_t
#include <string>    // za std::string

enum class Protocol {
    TCP,
    UDP,
    ICMP,
    OTHER
};

struct TcpFlags {
    bool syn;
    bool ack;
    bool fin;
    bool rst;
    bool psh;
    bool urg;
};

struct Packet {
    uint64_t timestamp;
    size_t length;

    std::string srcMac;
    std::string dstMac;

    std::string srcIp;
    std::string dstIp;

    Protocol protocol;

    uint16_t srcPort;
    uint16_t dstPort;

    TcpFlags tcpFlags;
};