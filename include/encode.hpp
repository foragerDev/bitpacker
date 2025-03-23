#ifndef ENCODE_HPP
#define ENCODE_HPP

#include <cstdint>
#include <cstddef>
#include <vector>

namespace bitpacker
{
    enum class MajorType : std::uint8_t
    {
        UNSIGNED_INTEGER = 0x00,
        SIGNED_INTEGER = 0x20,
        BYTE_STRING = 0x40,
        TEXT_STRING = 0x60,
        ARRAY = 0x80,
        MAP = 0xa0,
        TAGGED_DATA = 0xc6,
        FLOATING_POINT = 0xfb
    };

    MajorType get_major_type(std::byte data);
    std::byte get_argument(std::byte data);
    std::byte get_argument_value(size_t size);
    void save_bytes(std::vector<std::byte> &destination, const std::byte *source, size_t bytes);
    std::vector<std::byte> to_bytes(std::byte first_byte, const std::byte *data, int bytes_size, bool skip_reverse = 0);
    std::byte add_argument(MajorType type, std::byte argument);
    void add_first_value(std::byte &data, std::byte value);
}; // namespace bitpacker
#endif // ENCODE_HPP