#include "cbor_variant.hpp"

namespace bitpacker
{

    return_type serialize(std::vector<cvariant> &data)
    {
        std::vector<std::byte> byte_data = get_byte_buffer(MajorType::ARRAY, data);

        for (auto &item : data)
        {
            auto d = item.serialize();
            byte_data.insert(byte_data.end(), d.begin(), d.end());
        }

        return byte_data;
    }

    return_type serialize(std::unordered_map<std::string, cvariant> &data)
    {
        std::vector<std::byte> byte_data = get_byte_buffer(MajorType::MAP, data);

        for (auto &[fst, snd] : data)
        {
            auto key = serialize(fst);
            auto value = snd.serialize();
            byte_data.insert(byte_data.end(), key.begin(), key.end());
            byte_data.insert(byte_data.end(), value.begin(), value.end());
        }

        return byte_data;
    }
}
