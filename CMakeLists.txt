cmake_minimum_required(VERSION 3.10)
project(Njuskica CXX)

# Postavljanje C++17 standarda
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Pronalaženje libpcap biblioteke za hvatanje paketa
find_library(PCAP_LIB pcap)

# 1. Putanja do vaših .hpp zaglavlja (u packet-capturer/include)
include_directories(packet-capturer/include)

enable_testing()
find_package(GTest REQUIRED)

# 2. Popis svih .cpp datoteka koje čine vaš glavni program
add_executable(njuskica_capturer
    packet-capturer/src/main.cpp
    packet-capturer/src/packet.cpp
    packet-capturer/src/capture.cpp
    packet-capturer/src/ethernet.cpp
    packet-capturer/src/ipv4.cpp
    packet-capturer/src/ipv6.cpp
    packet-capturer/src/tcp.cpp
    packet-capturer/src/udp.cpp
    packet-capturer/src/icmp.cpp
    packet-capturer/src/format_testing.cpp
)

# 3. Povezivanje izvršnog programa s libpcap bibliotekom
target_link_libraries(njuskica_capturer ${PCAP_LIB})

add_executable(icmp_test
    packet-capturer/tests/icmp_test.cpp
    packet-capturer/src/icmp.cpp
)
target_link_libraries(icmp_test GTest::gtest GTest::gtest_main)
add_test(NAME icmp_test COMMAND icmp_test)

add_executable(udp_test
    packet-capturer/tests/udp_test.cpp
    packet-capturer/src/udp.cpp
)
target_link_libraries(udp_test PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME udp_test COMMAND udp_test)

add_executable(tcp_test
    packet-capturer/tests/tcp_test.cpp
    packet-capturer/src/tcp.cpp
)
target_link_libraries(tcp_test PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME tcp_test COMMAND tcp_test)

# ==============================================================================
# Opcionalno: Dodavanje jedinica za testiranje (iz packet-capturer/tests)
# ==============================================================================

# # Ethernet test
# add_executable(ethernet_test
#     packet-capturer/tests/ethernet_test.cpp
#     packet-capturer/src/ethernet.cpp
# )
# target_link_libraries(ethernet_test ${PCAP_LIB})

# # IPv4 test
# add_executable(ipv4_test
#     packet-capturer/tests/ipv4_test.cpp
#     packet-capturer/src/ipv4.cpp
# )
# target_link_libraries(ipv4_test ${PCAP_LIB})

# # TCP test
# add_executable(tcp_test
#     packet-capturer/tests/tcp_test.cpp
#     packet-capturer/src/tcp.cpp
# )
# target_link_libraries(tcp_test ${PCAP_LIB})