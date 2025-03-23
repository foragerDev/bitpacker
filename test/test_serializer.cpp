#include <gtest/gtest.h>
#include "serialize.hpp"
#include "cbor_variant.hpp"

using namespace bitpacker;

TEST(TestSerialization, TestMajorType)
{
    EXPECT_EQ((uint8_t)0 << 5, std::to_underlying(MajorType::UNSIGNED_INTEGER));
    EXPECT_EQ((uint8_t)1 << 5, std::to_underlying(MajorType::SIGNED_INTEGER));
    EXPECT_EQ((uint8_t)2 << 5, std::to_underlying(MajorType::BYTE_STRING));
    EXPECT_EQ((uint8_t)3 << 5, std::to_underlying(MajorType::TEXT_STRING));
    EXPECT_EQ((uint8_t)4 << 5, std::to_underlying(MajorType::ARRAY));
    EXPECT_EQ((uint8_t)5 << 5, std::to_underlying(MajorType::MAP));
    // EXPECT_EQ((uint8_t)6 << 5, std::to_underlying(MajorType::TAGGED_DATA));
    // EXPECT_EQ((uint8_t)7 << 5, std::to_underlying(MajorType::FLOATING_POINT));
}

TEST(TestSerialization, TestAddArgument)
{
    EXPECT_EQ(add_argument(MajorType::UNSIGNED_INTEGER, std::byte{2}), std::byte{2});
    EXPECT_EQ(add_argument(MajorType::SIGNED_INTEGER, std::byte{16}), std::byte{48});
    // EXPECT_EQ(add_argument(MajorType::FLOATING_POINT, std::byte{16}), std::byte{224 + 16});
}

TEST(TestSerialization, TestGetMajorType)
{
    // EXPECT_EQ(get_major_type(std::byte{0b11101010}), MajorType::FLOATING_POINT);
    EXPECT_EQ(get_major_type(std::byte{0b00010101}), MajorType::UNSIGNED_INTEGER);
    EXPECT_EQ(get_major_type(std::byte{0b01010111}), MajorType::BYTE_STRING);
}

TEST(TestSerialization, TestGetArgument)
{
    EXPECT_EQ(get_argument(std::byte{0b11101010}), std::byte{0xA});
    EXPECT_EQ(get_argument(std::byte{0b01011111}), std::byte{0x1F});
}

TEST(TestSerialization, TestSignedIntegerSerialization)
{
    std::vector<std::byte> bytes = serialize(-10);
    EXPECT_EQ(get_argument(bytes[0]), std::byte{0x09});

    bytes = serialize(-23);
    EXPECT_EQ(get_argument(bytes[0]), std::byte{0x16});
}

void compare_bytes(const std::vector<int> &expected, std::vector<std::byte> &bytes)
{
    for (int i = 0; i < expected.size(); i++)
    {
        EXPECT_EQ(std::to_integer<int>(bytes[i]), expected[i]);
    }
}

TEST(TestSerialization, TestSignedInteger1Bytes)
{
    bitpacker::cvariant data{-55};
    auto bytes = data.serialize();
    const std::vector<int> expected{0x38, 0x36};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestSignedNegative2Bytes)
{

    auto bytes = cvariant(-344).serialize();
    const std::vector<int> expected{0x39, 0x01, 0x57};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestSignedNegative4Bytes)
{
    auto bytes = cvariant(-655100).serialize();
    const std::vector<int> expected{0x3A, 0x00, 0x09, 0xFE, 0xFB};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestSignedNegative8Bytes)
{
    auto bytes = cvariant(-23428794977400).serialize();
    const std::vector<int> expected{0x3B, 0x00, 0x00, 0x15, 0x4E, 0xF1, 0x00, 0x8C, 0x77};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestUnsigned1Byte)
{
    auto bytes = cvariant(10).serialize();
    EXPECT_EQ(get_argument(bytes[0]), std::byte{0x0A});
}

TEST(TestSerialization, TestUnsigned2Byte)
{
    auto bytes = cvariant(27).serialize();
    const std::vector<int> expected{0x18, 0x1B};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestUnsigned4Byte)
{
    auto bytes = cvariant(655100).serialize();
    const std::vector<int> expected{0x1A, 0x00, 0x09, 0xFE, 0xFC};
    compare_bytes(expected, bytes);
}

TEST(TestSerialization, TestUnsigned8Byte)
{
    auto bytes = cvariant(23428794977400).serialize();
    const std::vector<int> expected{0x1B, 0x00, 0x00, 0x15, 0x4E, 0xF1, 0x00, 0x8C, 0x78};
    compare_bytes(expected, bytes);
}

// TEST(TestSerialization, TestByteString1Byte)
// {
//     auto bytes = serialize_raw("ABCD");
//     const std::vector<int> expected{0x44, 0x41, 0x42, 0x43, 0x44};
//     compare_bytes(expected, bytes);
// }

// TEST(TestSerialization, TestByteString2Byte)
// {
//     auto bytes = serialize_raw("ABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCD");
//     std::print("{:#x}", std::to_integer<int>(bytes[1]));
//     EXPECT_EQ(bytes[0], std::byte(88));
// }

TEST(TestArraySerialize, TestArray)
{
    std::vector<cvariant> data{1, 2, 3, 4, 5};
    const std::vector<int> expected{0x85, 0x01, 0x02, 0x03, 0x04, 0x05};
    auto bytes = serialize(data);
    compare_bytes(expected, bytes);
}

TEST(TestArraySerialize, TestArrayOfArray)
{
    std::vector<cvariant> data{
        std::vector<cvariant>{1, 2, 3},
        std::vector<cvariant>{4, 5, 6},
        std::vector<cvariant>{7, 8, 9}};
    auto bytes = serialize(data);
    std::vector<int> expected{0x83, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06, 0x83, 0x07, 0x08, 0x09};
    compare_bytes(expected, bytes);
}

TEST(TestArraySerialize, TestMixArray)
{
    std::vector<cvariant> data{
        1, 2, 3,
        std::vector<cvariant>{4, 5, 6},
        7, 8, 9};
    auto bytes = serialize(data);
    std::vector<int> expected{0x87, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    compare_bytes(expected, bytes);
}


TEST(TestMapSerialize, TestMap) {
    std::unordered_map<std::string, cvariant> data{
        {"name", "John Doe"},
        {"age", 25},
        {"is_student", true}
    };
    auto bytes = serialize(data);
    std::vector<int> expected{0xA3, 0x44, 0x6E, 0x61, 0x6D, 0x65, 0x64, 0x4A, 0x6F, 0x68, 0x6E, 0x20, 0x44, 0x6F, 0x65, 0x01, 0x69, 0x73, 0x5F, 0x73, 0x74, 0x75, 0x64, 0x65, 0x6E, 0x74, 0xF5};
    compare_bytes(expected, bytes);
}