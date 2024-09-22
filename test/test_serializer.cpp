#include <gtest/gtest.h>
#include "serialize.hpp"

using namespace bitpacker;

TEST(TestSerialization, TestMajorType) {
    EXPECT_EQ((uint8_t)0 << 5, std::to_underlying(MajorType::UNSIGNED_INTEGER));
    EXPECT_EQ((uint8_t)1 << 5, std::to_underlying(MajorType::SIGNED_INTEGER));
    EXPECT_EQ((uint8_t)2 << 5, std::to_underlying(MajorType::BYTE_STRING));
    EXPECT_EQ((uint8_t)3 << 5, std::to_underlying(MajorType::TEXT_STRING));
    EXPECT_EQ((uint8_t)4 << 5, std::to_underlying(MajorType::ARRAY));
    EXPECT_EQ((uint8_t)5 << 5, std::to_underlying(MajorType::MAP));
    EXPECT_EQ((uint8_t)6 << 5, std::to_underlying(MajorType::TAGGED_DATA));
    EXPECT_EQ((uint8_t)7 << 5, std::to_underlying(MajorType::FLOATING_POINT));
}

TEST(TestSerialization, TestAddArgument) {
    EXPECT_EQ(add_argument(MajorType::UNSIGNED_INTEGER, std::byte{ 2 }), std::byte{ 2 });
    EXPECT_EQ(add_argument(MajorType::SIGNED_INTEGER, std::byte{ 16 }), std::byte{ 48 });
    EXPECT_EQ(add_argument(MajorType::FLOATING_POINT, std::byte{ 16 }), std::byte{ 224 + 16 });
}

TEST(TestSerialization, TestGetMajorType) {
    EXPECT_EQ(get_major_type(std::byte{ 0b11101010 }), MajorType::FLOATING_POINT);
    EXPECT_EQ(get_major_type(std::byte{ 0b00010101 }), MajorType::UNSIGNED_INTEGER);
    EXPECT_EQ(get_major_type(std::byte{ 0b01010111 }), MajorType::BYTE_STRING);
}

TEST(TestSerialization, TestGetArgument) {
    EXPECT_EQ(get_argument(std::byte{ 0b11101010 }), std::byte{ 0xA });
    EXPECT_EQ(get_argument(std::byte{ 0b01011111 }), std::byte{ 0x1F });
}


TEST(TestSerialization, TestSignedIntegerSerialization) {
    std::vector<std::byte> bytes = serialize(10);

    EXPECT_EQ(get_major_type(bytes[0]), MajorType::SIGNED_INTEGER);
    EXPECT_EQ(get_argument(bytes[0]), std::byte {0x20});
}