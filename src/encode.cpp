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
            return std::byte(size);
        }
        else if (size <= 0xFF)
        {
            return std::byte(24);
        }
        else if (size <= 0xFFFF)
        {
            return std::byte(25);
        }
        else if (size <= 0xFFFFFFFF)
        {
            return std::byte(26);
        }
        else
        {
            return std::byte(27);
        }
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
