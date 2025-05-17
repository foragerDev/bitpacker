//
// Created by forager on 5/18/25.
//
#include <gtest/gtest.h>
#include "serialize.hpp"
#include "cbor_variant.hpp"
#include "deserialize.hpp"

using namespace bitpacker;

TEST(TestUnsignedIntegerDeserialize, Test1ByteUnsignedInt) {
    auto bytes = cvariant(10).serialize();
    ASSERT_EQ(bitpacker::parse_unsigned_int(bytes), std::optional<uint64_t>(10));
}

TEST(TestUnsignedIntegerDeserialize, Test2ByteUnsignedInt) {
    auto bytes = cvariant(27).serialize();
    ASSERT_EQ(bitpacker::parse_unsigned_int(bytes), std::optional<uint64_t>(27));
}

TEST(TestUnsignedIntegerDeserialize, Test4ByteUnsignedInt) {
    auto bytes = cvariant(655100).serialize();
    ASSERT_EQ(bitpacker::parse_unsigned_int(bytes), std::optional<uint64_t>(655100));
}

TEST(TestUnsignedIntegerDeserialize, Test8ByteUnsignedInt) {
    auto bytes = cvariant(23428794977400).serialize();
    ASSERT_EQ(bitpacker::parse_unsigned_int(bytes), std::optional<uint64_t>(23428794977400));
}

TEST(TestUnsignedIntegerDeserialize, TestWrongArgument) {
    std::vector<std::byte> bytes{std::byte(0x1F), std::byte(0x1B)};
    ASSERT_EQ(bitpacker::parse_unsigned_int(bytes), std::nullopt);
}

TEST(TestIntegerDeserialize, Test1ByteSignedInt) {
    auto bytes = cvariant(-10).serialize();
    ASSERT_EQ(bitpacker::parse_int(bytes), std::optional<int64_t>(-10));
}

TEST(TestIntegerDeserialize, Test2ByteSignedInt) {
    auto bytes = cvariant(-27).serialize();
    ASSERT_EQ(bitpacker::parse_int(bytes), std::optional<int64_t>(-27));
}

TEST(TestIntegerDeserialize, Test4ByteSignedInt) {
    auto bytes = cvariant(-655100).serialize();
    ASSERT_EQ(bitpacker::parse_int(bytes), std::optional<int64_t>(-655100));
}

TEST(TestIntegerDeserialize, Test8ByteSignedInt) {
    auto bytes = cvariant(-23428794977400).serialize();
    ASSERT_EQ(bitpacker::parse_int(bytes), std::optional<int64_t>(-23428794977400));
}

TEST(TestIntegerDeserialize, TestWrongArgument) {
    std::vector<std::byte> bytes{std::byte(0x1F), std::byte(0x1B)};
    ASSERT_EQ(bitpacker::parse_int(bytes), std::nullopt);
}