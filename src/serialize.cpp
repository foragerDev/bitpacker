#include "serialize.hpp"
#include <iostream>

namespace bitpacker
{
    // I believe if use want to save the raw string, he should be coverting this data to byte and then cbor will handle.
    // Convert the string into bytes
    return_type serialize(std::string_view data)
    {
        auto size = data.size();
        auto first_byte = add_argument(MajorType::BYTE_STRING, get_argument_value(size));
        const std::byte *str_data = reinterpret_cast<const std::byte *>(data.data());

        return to_bytes(first_byte, str_data, size, true);
    }
}
