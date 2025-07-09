#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <string_view>
#include <string>
#include <variant>
#include <unordered_map>

#include "encode.hpp"
// #include "cbor_variant.hpp"

namespace bitpacker
{

    class cvariant;
    using var_t =
        std::variant<uint64_t, int64_t, std::string, std::vector<bitpacker::cvariant>>;
    using return_type = std::vector<std::byte>;

    return_type serialize_numeric(MajorType data_type, std::integral auto integer)
    {
        std::vector<std::byte> byte_data;
        auto d = integer < 0 ? -integer : integer;

        auto input_size = [](auto num)
        {
            int value = 0;
            if (num < 24)
            {
                value = 0;
            }
            else if (num <= 0xFF)
            {
                value = 1;
            }
            else if (num <= 0xFFFF)
            {
                value = 2;
            }
            else if (num <= 0xFFFFFFFF)
            {
                value = 4;
            }
            else
            {
                value = 8;
            }
            return value;
        };

        auto first_byte = add_argument(data_type, get_argument_value(d));

        const auto *data_ptr = reinterpret_cast<const std::byte *>(&d);

        return to_bytes(first_byte, data_ptr, input_size(d));
    }

    // Serializations function specializations
    // The value of the signed integer is argument - 1, hence our number is < 0 so
    // Reference: https://datatracker.ietf.org/doc/html/rfc8949#section-3.1-2.4
    return_type serialize(std::integral auto integer)
    {
        return integer < 0 ? serialize_numeric(MajorType::SIGNED_INTEGER, integer + 1)
                           : serialize_numeric(MajorType::UNSIGNED_INTEGER, integer);
    }
    return_type serialize(std::string_view str);
    return_type serialize(std::vector<cvariant> &arr);
    return_type serialize(std::unordered_map<std::string, cvariant> &ht);
    return_type serialize(float singleP_float);
    return_type serialize(double doubleP_float);

} // namespace bitpacker
#endif