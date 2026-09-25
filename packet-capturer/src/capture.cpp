#include "capture.hpp"
#include <stdexcept>
#include <cstring>

PacketCapture::PacketCapture(const std::string& interface_name)
    : interface_(interface_name) {}

PacketCapture::~PacketCapture() {
    if (handle_) pcap_close(handle_);
}

bool PacketCapture::open(int snaplen, int promiscuous, int timeout_ms) {
    char errbuf[PCAP_ERRBUF_SIZE];
    handle_ = pcap_open_live(interface_.c_str(), snaplen, promiscuous, timeout_ms, errbuf);
    if (!handle_) {
        throw std::runtime_error("pcap_open_live failed: " + std::string(errbuf));
    }
    return true;
}

void PacketCapture::setFilter(const std::string& bpf_filter) {
    struct bpf_program fp;
    if (pcap_compile(handle_, &fp, bpf_filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) {
        throw std::runtime_error("pcap_compile failed: " + std::string(pcap_geterr(handle_)));
    }
    if (pcap_setfilter(handle_, &fp) == -1) {
        throw std::runtime_error("pcap_setfilter failed");
    }
    pcap_freecode(&fp);
}

void PacketCapture::start(PacketCallback callback) {
    callback_ = std::move(callback);
    pcap_loop(handle_, -1, pcapHandler, reinterpret_cast<u_char*>(this));
}

void PacketCapture::stop() {
    if (handle_) pcap_breakloop(handle_);
}

void PacketCapture::pcapHandler(u_char* user, const pcap_pkthdr* header, const u_char* bytes) {
    auto* self = reinterpret_cast<PacketCapture*>(user);

    RawPacket pkt;
    pkt.data.assign(bytes, bytes + header->caplen);
    pkt.length = header->len;
    pkt.captured_length = header->caplen;
    pkt.timestamp = std::chrono::system_clock::now();  // ili konvertuj header->ts

    self->callback_(pkt);
}