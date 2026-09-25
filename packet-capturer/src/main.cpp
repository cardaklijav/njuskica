#include <iostream>
#include <pcap.h>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    std::cout << "Uhvacen paket! Duzina: " << pkthdr->len << " bajtova." << std::endl;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Koristimo ime vašeg interfejsa koje ste dobili u ip a
    const char *dev = "enp0s3"; 

    std::cout << "Otvaram mrezni interfejs: " << dev << std::endl;

    // Otvaranje mrežne kartice u Promiscuous modu
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Ne mogu otvoriti mrezni interfejs " << dev << ": " << errbuf << std::endl;
        return 1;
    }

    std::cout << "Slusam saobracaj na " << dev << "... (Pritisnite Ctrl+C za prekid)" << std::endl;

    // Uhvati 10 paketa i pozovi packet_handler za svaki
    pcap_loop(handle, 10, packet_handler, nullptr);

    pcap_close(handle);
    std::cout << "Zavrseno hvatanje paketa." << std::endl;

    return 0;
}