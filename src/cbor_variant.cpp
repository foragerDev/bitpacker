#include "cbor_variant.hpp"

namespace bitpacker
{
    return_type serialize(std::vector<cvariant> & data)
    {
        std::vector<std::byte> byte_data;
        auto size = data.size();
        auto first_byte = add_argument(MajorType::ARRAY, std::byte(size));

        for (auto &item : data)
        {
            auto d = item.serialize();
            byte_data.insert(byte_data.end(), d.begin(), d.end());
        }
        
        auto data_ptr = reinterpret_cast<const std::byte *>(byte_data.data());
        return to_bytes(first_byte, data_ptr, byte_data.size(), true);
    }

    return_type serialize(std::unordered_map<std::string, cvariant> & data)
    {
        std::vector<std::byte> byte_data;
        auto size = data.size();
        auto first_byte = add_argument(MajorType::MAP, std::byte(size));

        for (auto &item : data)
        {
            auto key = serialize(item.first);
            auto value = item.second.serialize();
            byte_data.insert(byte_data.end(), key.begin(), key.end());
            byte_data.insert(byte_data.end(), value.begin(), value.end());
        }

        auto data_ptr = reinterpret_cast<const std::byte *>(byte_data.data());
        return to_bytes(first_byte, data_ptr, byte_data.size(), true);
    }
}
