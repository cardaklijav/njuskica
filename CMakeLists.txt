cmake_minimum_required(VERSION 3.10)
project(Njuskica CXX)

# Postavljanje C++17 standarda
set(CMAKE_CXX_STANDARD 17)

# Pronalaženje libpcap biblioteke za hvatanje paketa
find_library(PCAP_LIB pcap)

# 1. Putanja do vaših .hpp zaglavlja (u packet-capturer/include)
include_directories(packet-capturer/include)

# 2. Popis svih .cpp datoteka koje čine vaš glavni program
add_executable(njuskica_capturer
    packet-capturer/src/main.cpp
    packet-capturer/src/capture.cpp
    packet-capturer/src/ethernet.cpp
    packet-capturer/src/ipv4.cpp
    packet-capturer/src/tcp.cpp
    packet-capturer/src/udp.cpp
)

# 3. Povezivanje izvršnog programa s libpcap bibliotekom
target_link_libraries(njuskica_capturer ${PCAP_LIB})

# ==============================================================================
# Opcionalno: Dodavanje jedinica za testiranje (iz packet-capturer/tests)
# ==============================================================================

# Ethernet test
add_executable(ethernet_test
    packet-capturer/tests/ethernet_test.cpp
    packet-capturer/src/ethernet.cpp
)
target_link_libraries(ethernet_test ${PCAP_LIB})

# IPv4 test
add_executable(ipv4_test
    packet-capturer/tests/ipv4_test.cpp
    packet-capturer/src/ipv4.cpp
)
target_link_libraries(ipv4_test ${PCAP_LIB})

# TCP test
add_executable(tcp_test
    packet-capturer/tests/tcp_test.cpp
    packet-capturer/src/tcp.cpp
)
target_link_libraries(tcp_test ${PCAP_LIB})
