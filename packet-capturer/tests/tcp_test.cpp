#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "tcp.hpp"

TEST(ParseTcp, ThrowsWhenInputIsShorterThanMinimumHeader) {
    const std::vector<std::uint8_t> packet(20);

    for (std::size_t size = 0; size < 20; ++size) {
        const Bytes data{packet.data(), size};
        EXPECT_THROW(parse_tcp(data), std::runtime_error)
            << "Input size: " << size;
    }
}

TEST(ParseTcp, ThrowsWhenDataOffsetIsLessThanFiveWords) {
    std::vector<std::uint8_t> packet(20);
    packet[12] = 0x40; // Data offset 4 means 16 bytes.

    EXPECT_THROW(parse_tcp(Bytes{packet.data(), packet.size()}),
                   std::runtime_error);
}

TEST(ParseTcp, ThrowsWhenDataOffsetExceedsInputSize) {
    std::vector<std::uint8_t> packet(20);
    packet[12] = 0x60; // Data offset 6 means a 24-byte header.

    EXPECT_THROW(parse_tcp(Bytes{packet.data(), packet.size()}),
                   std::runtime_error);
}

TEST(ParseTcp, ParsesMinimumHeaderFieldsAndPayload) {
    const std::vector<std::uint8_t> packet{
        0x12, 0x34,             // source port
        0xab, 0xcd,             // destination port
        0x01, 0x02, 0x03, 0x04, // sequence number
        0xa1, 0xb2, 0xc3, 0xd4, // acknowledgment number
        0x50, 0x12,             // data offset, flags
        0x56, 0x78,             // window
        0x9a, 0xbc,             // checksum
        0xde, 0xf0,             // urgent pointer
        0xde, 0xad, 0xbe        // payload
    };

    const TcpHeader header = parse_tcp(Bytes{packet.data(), packet.size()});

    EXPECT_EQ(header.src_port, 0x1234);
    EXPECT_EQ(header.dst_port, 0xabcd);
    EXPECT_EQ(header.seq, 0x01020304u);
    EXPECT_EQ(header.ack, 0xa1b2c3d4u);
    EXPECT_EQ(header.header_len, 20);
    EXPECT_EQ(header.flags, 0x12);
    EXPECT_EQ(header.window, 0x5678);
    EXPECT_EQ(header.checksum, 0x9abc);
    EXPECT_EQ(header.urgent_pointer, 0xdef0);

    ASSERT_EQ(header.payload.size(), 3);
    EXPECT_EQ(header.payload[0], 0xde);
    EXPECT_EQ(header.payload[1], 0xad);
    EXPECT_EQ(header.payload[2], 0xbe);
}

TEST(ParseTcp, SkipsOptionsBeforePayload) {
    std::vector<std::uint8_t> packet(26);
    packet[0] = 0x00;
    packet[1] = 0x50; // Source port 80.
    packet[12] = 0x60; // Data offset 6 means a 24-byte header.
    packet[20] = 0x01; // Example option bytes.
    packet[21] = 0x01;
    packet[24] = 0xca;
    packet[25] = 0xfe;

    const TcpHeader header = parse_tcp(Bytes{packet.data(), packet.size()});

    EXPECT_EQ(header.header_len, 24);
    ASSERT_EQ(header.payload.size(), 2);
    EXPECT_EQ(header.payload[0], 0xca);
    EXPECT_EQ(header.payload[1], 0xfe);
}

TEST(ParseTcp, FormatsHeaderWhenStreamed) {
    std::vector<std::uint8_t> packet(20);
    packet[0] = 0x12;
    packet[1] = 0x34;
    packet[2] = 0xab;
    packet[3] = 0xcd;
    packet[4] = 0x01;
    packet[5] = 0x02;
    packet[6] = 0x03;
    packet[7] = 0x04;
    packet[8] = 0xa1;
    packet[9] = 0xb2;
    packet[10] = 0xc3;
    packet[11] = 0xd4;
    // packet[12] = 0x50;
    packet[13] = 0x12;
    packet[14] = 0x56;
    packet[15] = 0x78;

    const TcpHeader header = parse_tcp(Bytes{packet.data(), packet.size()});
    std::ostringstream output;
    output << header;

    EXPECT_EQ(output.str(),
              "TCP\n"
              "src_port=4660\n"
              "dst_port=43981\n"
              "seq=16909060\n"
              "ack=2712847316\n"
              "flags=18\n"
              "window=22136\n");
}