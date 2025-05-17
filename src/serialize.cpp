#include "serialize.hpp"
#include <iostream>

namespace bitpacker
{
    // I believe if use want to save the raw string, he should be coverting this data to byte and then cbor will handle.
    // Convert the string into bytes
    return_type serialize(std::string_view data)
    {
        auto size = data.size();
        auto first_byte = add_argument(MajorType::TEXT_STRING, get_argument_value(size));
        const auto *str_data = reinterpret_cast<const std::byte *>(data.data());

        return to_bytes(first_byte, str_data, size, true);
    }

    return_type serialize(float singleP_float) {
        auto first_byte = add_argument(MajorType::FLOATING_POINT, std::byte(26));
        const auto *bytes = reinterpret_cast<const std::byte *>(&singleP_float);
        return to_bytes(first_byte, bytes, sizeof(float));
    }

    return_type serialize(double doubleP_float) {
        auto first_byte = add_argument(MajorType::FLOATING_POINT, std::byte(27));
        const auto *bytes = reinterpret_cast<const std::byte *>(&doubleP_float);
        return to_bytes(first_byte, bytes, sizeof(double));
    }
}
