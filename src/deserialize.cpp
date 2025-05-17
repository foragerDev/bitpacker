//
// Created by forager on 5/12/25.
//


#include "deserialize.hpp"

#include <format>
#include <iostream>
#include <ostream>

#include "encode.hpp"

namespace bitpacker {
    std::optional<uint64_t> decode_numaric(const std::vector<std::byte> &data) {
        if (data.empty()) {
            return std::nullopt;
        }
        if (const auto argument = std::to_integer<uint64_t>(bitpacker::get_argument(data[0])); argument < 24) {
            return argument;
        } else if (argument == 24) {
            if (data.size() < 2) return std::nullopt;
            return std::to_integer<uint64_t>(data[1]);
        } else if (argument == 25) {
            if (data.size() < 3) return std::nullopt;
            return std::to_integer<uint64_t>(data[1]) << 8 | std::to_integer<uint64_t>(data[2]);
        } else if (argument == 26) {
            if (data.size() < 5) return std::nullopt;

            return (std::to_integer<uint64_t>(data[1]) << 24) |
                   (std::to_integer<uint64_t>(data[2]) << 16) |
                   (std::to_integer<uint64_t>(data[3]) << 8) |
                   std::to_integer<uint64_t>(data[4]);
        } else if (argument == 27) {
            if (data.size() < 9) return std::nullopt;

            return
                    (std::to_integer<uint64_t>(data[1]) << 56) |
                    (std::to_integer<uint64_t>(data[2]) << 48) |
                    (std::to_integer<uint64_t>(data[3]) << 40) |
                    (std::to_integer<uint64_t>(data[4]) << 32) |
                    (std::to_integer<uint64_t>(data[5]) << 24) |
                    (std::to_integer<uint64_t>(data[6]) << 16) |
                    (std::to_integer<uint64_t>(data[7]) << 8) |
                    std::to_integer<uint64_t>(data[8]);
        }
        return std::nullopt;
    }

    std::optional<uint64_t> parse_unsigned_int(const std::vector<std::byte> &data) {
        return decode_numaric(data);
    }

    std::optional<int64_t> parse_int(const std::vector<std::byte> &data) {
        return decode_numaric(data).and_then([](const int64_t value) -> std::optional<int64_t> {
            return {-(value + 1)};
        });
    }
}
