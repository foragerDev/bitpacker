#include "encode.hpp"
#include <utility>
#include <algorithm>

using namespace bitpacker;

namespace bitpacker
{
    // Return MajorType from the byte
    MajorType get_major_type(std::byte data)
    {
        std::byte major_type = (data >> 5) << 5;
        return static_cast<MajorType>(major_type);
    }

    // Returns the last 5 bytes of the byte other than MajorType
    std::byte get_argument(std::byte data)
    {
        std::byte mask{0x1F};
        return data & mask;
    }

    // Calculate what will the argument type for the given size
    std::byte get_argument_value(size_t size)
    {
        if (size < 24)
        {
            return static_cast<std::byte>(size);
        }
        else if (size <= 0xFF)
        {
            return static_cast<std::byte>(24);
        }
        else if (size <= 0xFFFF)
        {
            return static_cast<std::byte>(25);
        }
        else if (size <= 0xFFFFFFFF)
        {
            return static_cast<std::byte>(26);
        }
        else
        {
            return static_cast<std::byte>(27);
        }
    }

    std::pair<std::byte, std::vector<std::byte>> get_argument_value_with_size(size_t size)
    {
        std::byte argument_value;
        std::vector<std::byte> size_bytes;
        auto append_bytes = [&](int count)
        {
            for (int i = count; i >= 0; i--)
            {
                size_bytes.push_back(static_cast<std::byte>((size >> (8 * i)) & 0xFF));
            }
        };

        if (size < 24)
        {
            argument_value = static_cast<std::byte>(size);
        }
        else if (size <= 0xFF)
        {
            argument_value = static_cast<std::byte>(24);
            size_bytes.push_back(static_cast<std::byte>(size));
        }
        else if (size <= 0xFFFF)
        {
            argument_value = static_cast<std::byte>(25);
            append_bytes(1);
        }
        else if (size <= 0xFFFFFFFF)
        {
            argument_value = static_cast<std::byte>(26);
            append_bytes(3);
        }
        else
        {
            argument_value = static_cast<std::byte>(27);
            append_bytes(7);
        }

        return {argument_value, size_bytes};
    }

    // This will convert to bytes in lower endian on Windows 11
    void save_bytes(std::vector<std::byte> &destination, const std::byte *source, size_t bytes)
    {
        for (int i = 0; i < bytes; i++)
        {
            destination.push_back(*(source + i));
        }
    }

    // Add Argument to MajorType
    std::byte add_argument(MajorType type, std::byte argument)
    {
        auto value = std::byte{std::to_underlying(type)};
        return value | argument;
    }

    std::vector<std::byte> to_bytes(std::byte first_byte, const std::byte *data, int bytes_size, bool skip_reverse)
    {
        std::vector<std::byte> bytes;
        bytes.push_back(first_byte);
        save_bytes(bytes, data, bytes_size);

        if (skip_reverse)
            return bytes;

        if constexpr (std::endian::native == std::endian::little)
        {
            std::reverse(bytes.begin() + 1, bytes.end());
        }
        return bytes;
    }

    void add_first_value(std::byte &data, std::byte value)
    {
        data |= value;
    }
}
