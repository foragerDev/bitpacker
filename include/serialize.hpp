#include <cstdint>
#include <utility>
#include <concepts>
#include <bit>
#include <type_traits>

namespace bitpacker
{
    // template <typename T>
    // concept Serializable = requires(T a) { a.serialize(); };

    enum class MajorType : std::uint8_t
    {
        UNSIGNED_INTEGER = 0 << 5,
        SIGNED_INTEGER = 1 << 5,
        BYTE_STRING = 2 << 5,
        TEXT_STRING = 3 << 5,
        ARRAY = 4 << 5,
        MAP = 5 << 5,
        TAGGED_DATA = 6 << 5,
        FLOATING_POINT = 7 << 5
    };

    template <typename T>
    concept isPositive = requires(T t) {
        t >= 0;
    };

    template <typename T>
    concept isNegative = !isPositive<T>;

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
        return std::byte{std::to_underlying(type)} | argument;
    }

    std::vector<std::byte> to_bytes(MajorType data_type, std::byte argument, const std::byte *data, int bytes_size, bool skip_reverse = 0)
    {
        std::vector<std::byte> bytes;
        std::byte first_byte = add_argument(data_type, argument);
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

    inline std::vector<std::byte> serialize_numeric(MajorType data_type, std::integral auto integer)
    {
        std::vector<std::byte> byte_data;
        auto d = abs(integer);
        const std::byte *data_ptr = reinterpret_cast<const std::byte *>(&d);
        if (d < 24)
        {
            byte_data = to_bytes(data_type, std::byte(d), data_ptr, 0);
        }
        else if (d <= 0xFF)
        {
            byte_data = to_bytes(data_type, std::byte(24), data_ptr, 1);
        }
        else if (d <= 0xFFFF)
        {
            byte_data = to_bytes(data_type, std::byte(25), data_ptr, 2);
        }
        else if (d <= 0xFFFFFFFF)
        {
            byte_data = to_bytes(data_type, std::byte(26), data_ptr, 4);
        }
        else if (d <= 0xFFFFFFFFFFFFFFFF)
        {
            byte_data = to_bytes(data_type, std::byte(27), data_ptr, 8);
        }
        // std::reverse(byte_data.begin() + 1, byte_data.end());
        return byte_data;
    }

    // Serializations function specializations
    // The value of the signed integer is argument - 1, hence our number is < 0 so
    // Reference: https://datatracker.ietf.org/doc/html/rfc8949#section-3.1-2.4
    inline std::vector<std::byte> serialize(std::integral auto integer)
    {
        return integer < 0 ? serialize_numeric(MajorType::SIGNED_INTEGER, integer + 1)
                           : serialize_numeric(MajorType::UNSIGNED_INTEGER, integer);
    }

    // Let's try with a string first
    inline std::vector<std::byte> serialize_raw(std::string_view data)
    {
        auto size = data.size();
        std::cout << data.size() << std::endl;
        auto size_byte = std::byte(size);
        const std::byte *str_data = reinterpret_cast<const std::byte *>(data.data());
        std::vector<std::byte> byte_data;

        if (size < 24)
        {
            byte_data = to_bytes(MajorType::BYTE_STRING, size_byte, str_data, size, true);
        }
        else if (size <= 0xFF)
        {
            byte_data = to_bytes(MajorType::BYTE_STRING, std::byte(24), str_data, size, true);
        }
        else if (size <= 0xFFFF)
        {
            byte_data = to_bytes(MajorType::BYTE_STRING, std::byte(25), str_data, size, true);
        }
        else if (size <= 0xFFFFFFFF)
        {
            byte_data = to_bytes(MajorType::BYTE_STRING, std::byte(26), str_data, size, true);
        }
        else if (size <= 0xFFFFFFFFFFFFFFFF)
        {
            byte_data = to_bytes(MajorType::BYTE_STRING, std::byte(27), str_data, size, true);
        }

        return byte_data;
    }
} // namespace bitpacker