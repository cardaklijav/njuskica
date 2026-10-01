#include <cstddef>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "bytes.hpp"
#include "icmp.hpp"

TEST(ParseIcmp, ThrowsWhenInputIsShorterThanHeader) {
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    const std::vector<std::uint8_t> packet(header_len);

    for (std::size_t size = 0; size < header_len; ++size) {
        const Bytes data{packet.data(), size};
        EXPECT_THROW(parse_icmp(data), std::runtime_error)
            << "Input size: " << size;
    }
}

TEST(ParseIcmp, ParsesMinimumLengthHeader) {
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    const std::vector<std::uint8_t> packet(header_len);
    const Bytes data{packet.data(), packet.size()};

    const IcmpHeader header = parse_icmp(data);

    EXPECT_EQ(header.type, 0);
    EXPECT_EQ(header.code, 0);
    EXPECT_EQ(header.checksum, 0);
    EXPECT_TRUE(header.payload.empty());
}

TEST(ParseIcmp, ParsesIdentifierAndSequenceForEchoTypes) {
    constexpr std::uint8_t echo_types[] = {0, 8, 128, 129};
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    for (const auto type : echo_types) {
        std::vector<std::uint8_t> packet(header_len);
        packet[0] = type;
        packet[1] = 0;
        packet[2] = 0x8f;
        packet[3] = 0x53;
        packet[4] = 0x12;
        packet[5] = 0x34;
        packet[6] = 0x56;
        packet[7] = 0x78;

        const Bytes data{packet.data(), packet.size()};
        const IcmpHeader header = parse_icmp(data);

        EXPECT_EQ(header.type, type);
        EXPECT_EQ(header.checksum, 0x8f53);
        ASSERT_TRUE(header.id.has_value());
        EXPECT_EQ(*header.id, 0x1234);
        ASSERT_TRUE(header.seq.has_value());
        EXPECT_EQ(*header.seq, 0x5678);
    }
}

TEST(ParseIcmp, ParsesTypeDataWithoutEchoIdentifierOrSequence) {
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    std::vector<std::uint8_t> packet(header_len);
    packet[0] = 3; // Destination Unreachable
    packet[4] = 0x12;
    packet[5] = 0x34;
    packet[6] = 0x56;
    packet[7] = 0x78;

    const Bytes data{packet.data(), packet.size()};
    const IcmpHeader header = parse_icmp(data);

    EXPECT_EQ(header.type, 3);
    EXPECT_FALSE(header.id.has_value());
    EXPECT_FALSE(header.seq.has_value());
    EXPECT_EQ(header.type_data[0], 0x12);
    EXPECT_EQ(header.type_data[1], 0x34);
    EXPECT_EQ(header.type_data[2], 0x56);
    EXPECT_EQ(header.type_data[3], 0x78);
}

TEST(ParseIcmp, ExtractsPayloadAfterHeader) {
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    std::vector<std::uint8_t> packet(header_len + 3);
    packet[0] = 8;
    packet[header_len] = 0xde;
    packet[header_len + 1] = 0xad;
    packet[header_len + 2] = 0xbe;

    const Bytes data{packet.data(), packet.size()};
    const IcmpHeader header = parse_icmp(data);

    ASSERT_EQ(header.payload.size(), 3);
    EXPECT_EQ(header.payload[0], 0xde);
    EXPECT_EQ(header.payload[1], 0xad);
    EXPECT_EQ(header.payload[2], 0xbe);
}

TEST(ParseIcmp, FormatsHeaderWhenStreamed) {
    const auto header_len = IcmpHeader{}.header_len;
    ASSERT_GE(header_len, std::size_t{8});

    std::vector<std::uint8_t> packet(header_len);
    packet[0] = 8;
    packet[2] = 0x8f;
    packet[3] = 0x53;
    packet[4] = 0x12;
    packet[5] = 0x34;
    packet[6] = 0x56;
    packet[7] = 0x78;

    const Bytes data{packet.data(), packet.size()};
    const IcmpHeader header = parse_icmp(data);

    std::ostringstream output;
    output << header;

    EXPECT_EQ(output.str(),
              "ICMP\n"
              "type=8\n"
              "code=0\n"
              "checksum=36691\n"
              "identifier=4660\n"
              "sequence_number=22136\n");
}