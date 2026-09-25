#pragma once
#include "packet.hpp"
#include <functional>
#include <string>
#include <pcap.h>

using PacketCallback = std::function<void(const RawPacket&)>;

class PacketCapture {
public:
    explicit PacketCapture(const std::string& interface_name);
    ~PacketCapture();

    bool open(int snaplen = 65536, int promiscuous = 1, int timeout_ms = 1000);
    void setFilter(const std::string& bpf_filter);      // opciono, BPF filter
    void start(PacketCallback callback);                // blokira, zove callback po paketu
    void stop();

    static std::vector<std::string> listInterfaces();   // pcap_findalldevs wrapper

private:
    std::string interface_;
    pcap_t* handle_ = nullptr;
    PacketCallback callback_;

    static void pcapHandler(u_char* user, const pcap_pkthdr* header, const u_char* bytes);
};