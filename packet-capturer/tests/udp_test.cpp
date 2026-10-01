#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "udp.hpp"

TEST(ParseUdp, ThrowsWhenInputIsShorterThanHeader) {
    const std::vector<std::uint8_t> packet(8);

    for (std::size_t size = 0; size < 8; ++size) {
        const Bytes data{packet.data(), size};
        EXPECT_THROW(parse_udp(data), std::runtime_error) << "Input size: " << size;
    }
}

TEST(ParseUdp, ThrowsWhenDeclaredLengthIsLessThanHeader) {
    for (const std::uint16_t length : {0, 7}) {
        const std::vector<std::uint8_t> packet{
            0x00, 0x01, 0x00, 0x02,
            static_cast<std::uint8_t>(length >> 8),
            static_cast<std::uint8_t>(length),
            0x00, 0x00
        };

        EXPECT_THROW(parse_udp(Bytes{packet.data(), packet.size()}),
                       std::runtime_error)
            << "Declared length: " << length;
    }
}

TEST(ParseUdp, ThrowsWhenDeclaredLengthExceedsInputSize) {
    const std::vector<std::uint8_t> packet{
        0x00, 0x01, 0x00, 0x02,
        0x00, 0x09, 0x00, 0x00
    };

    EXPECT_THROW(parse_udp(Bytes{packet.data(), packet.size()}),
                   std::runtime_error);
}

TEST(ParseUdp, ParsesHeaderWithNoPayload) {
    const std::vector<std::uint8_t> packet{
        0x12, 0x34, // source port
        0xAB, 0xCD, // destination port
        0x00, 0x08, // length
        0x56, 0x78  // checksum
    };

    const UdpHeader header = parse_udp(Bytes{packet.data(), packet.size()});

    EXPECT_EQ(header.src_port, 0x1234);
    EXPECT_EQ(header.dst_port, 0xABCD);
    EXPECT_EQ(header.length, 8);
    EXPECT_EQ(header.checksum, 0x5678);
    EXPECT_TRUE(header.payload.empty());
}

TEST(ParseUdp, ParsesPayload) {
    const std::vector<std::uint8_t> packet{
        0x00, 0x35, 0x12, 0x34, // ports
        0x00, 0x0B,             // length: 8-byte header + 3-byte payload
        0xAB, 0xCD,             // checksum
        0xDE, 0xAD, 0xBE        // payload
    };

    const UdpHeader header = parse_udp(Bytes{packet.data(), packet.size()});

    ASSERT_EQ(header.payload.size(), 3);
    EXPECT_EQ(header.payload[0], 0xDE);
    EXPECT_EQ(header.payload[1], 0xAD);
    EXPECT_EQ(header.payload[2], 0xBE);
}

TEST(ParseUdp, IgnoresBytesAfterDeclaredLength) {
    const std::vector<std::uint8_t> packet{
        0x00, 0x35, 0x12, 0x34,
        0x00, 0x09, 0xAB, 0xCD, // declared length: 9
        0xDE,                   // included payload
        0xAD, 0xBE              // bytes beyond declared length
    };

    const UdpHeader header = parse_udp(Bytes{packet.data(), packet.size()});

    ASSERT_EQ(header.payload.size(), 1);
    EXPECT_EQ(header.payload[0], 0xDE);
}

TEST(ParseUdp, FormatsHeaderWhenStreamed) {
    const std::vector<std::uint8_t> packet{
        0x12, 0x34, 0xAB, 0xCD,
        0x00, 0x08, 0x56, 0x78
    };

    const UdpHeader header = parse_udp(Bytes{packet.data(), packet.size()});
    std::ostringstream output;
    output << header;

    EXPECT_EQ(output.str(),
              "UDP\n"
              "src_port=4660\n"
              "dst_port=43981\n"
              "length=8\n"
              "checksum=22136\n"
              "payload_size=0\n");
}