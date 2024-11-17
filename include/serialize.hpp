#include <cstdint>
#include <utility>
#include <concepts>
#include <type_traits>

namespace bitpacker {
    // template <typename T>
    // concept Serializable = requires(T a) { a.serialize(); };

    enum class MajorType : std::uint8_t {
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
        std::is_unsigned(t);
    };

    template <typename T>
    concept isNegative = !isPositive<T>;

    // Return MajorType from the byte
    MajorType get_major_type(std::byte data) {
        std::byte major_type = (data >> 5) << 5;
        return static_cast<MajorType>(major_type);
    }

    // Returns the last 5 bytes of the byte other than MajorType
    std::byte get_argument(std::byte data) {
        std::byte mask{ 0x1F };
        return data & mask;
    }
    // This will convert to bytes in lower endian on Windows 11
    void save_bytes(std::vector<std::byte>& destination, std::byte* source, size_t bytes) {
        for (int i = 0; i < bytes; i++) {
            destination.push_back(*(source + i));
        }
    }
    // Add Argument to MajorType
    std::byte add_argument(MajorType type, std::byte argument) {
        return std::byte{ std::to_underlying(type) } | argument;
    }

    void add_first_value(std::byte& data, std::byte value) {
        data |= value;
    }


    inline std::vector<std::byte> serialize_numeric(MajorType data_type, std::integral auto integer) {
        std::vector<std::byte> byte_data;
        auto d = abs(integer);
        // std::byte data{d};
        if (d < 24) {
            std::byte first_byte = add_argument(data_type, std::byte{ d });
            byte_data.push_back(first_byte);
        }
        else if (d <= 0xFF) {
            std::byte first_byte = add_argument(data_type, std::byte{ 24 });
            byte_data.push_back(first_byte);
            save_bytes(byte_data, reinterpret_cast<std::byte*>(&d), 1);
        }
        else if (d <= 0xFFFF) {
            std::byte first_byte = add_argument(data_type, std::byte{ 25 });
            byte_data.push_back(first_byte);
            save_bytes(byte_data, reinterpret_cast<std::byte*>(&d), 2);
        }
        else if (d <= 0xFFFFFFFF) {
            std::byte first_byte = add_argument(data_type, std::byte{ 26 });
            byte_data.push_back(first_byte);
            save_bytes(byte_data, reinterpret_cast<std::byte*>(&d), 4);
        }
        else if (d <= 0xFFFFFFFFFFFFFFFF) {
            std::byte first_byte = add_argument(data_type, std::byte{ 27 });
            byte_data.push_back(first_byte);
            save_bytes(byte_data, reinterpret_cast<std::byte*>(&d), 8);
        }
        std::reverse(byte_data.begin() + 1, byte_data.end());
        return byte_data;
    }

    // Serializations function specializations
    // The value of the signed integer is argument - 1, hence our number is < 0 so 
    // Reference: https://datatracker.ietf.org/doc/html/rfc8949#section-3.1-2.4
    inline std::vector<std::byte> serialize(isNegative auto integer) {
        return serialize_numeric(MajorType::SIGNED_INTEGER, integer + 1);
    }

    inline std::vector<std::byte> serialize(isPositive auto integer) {
        return serialize_numeric(MajorType::UNSIGNED_INTEGER, integer);
    }


} // namespace bitpacker