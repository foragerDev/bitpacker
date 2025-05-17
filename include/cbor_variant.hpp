#ifndef CVARIANT
#define CVARIANT

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include "serialize.hpp"

namespace bitpacker {
    class cvariant : public var_t {
        using var_t::variant;

    public:
        template<typename T>
        cvariant(T &&value) : var_t(std::forward<T>(value)) {
        }


        template<typename... Ts>
        struct overloaded : Ts... {
            using Ts::operator()...;
        };

        std::vector<std::byte> serialize() {
            std::vector<std::byte> data;
            auto serializable =
                    overloaded{
                        [&](std::integral auto number) { data = bitpacker::serialize(number); },
                        [&](std::string_view str) { data = bitpacker::serialize(str); },
                        [&](std::vector<cvariant> &array) { data = bitpacker::serialize(array); },
                        [&](float single_float) {
                            data = bitpacker::serialize(single_float);
                        },
                        [&](double double_float) {
                            data = bitpacker::serialize(double_float);
                        }
                    };
            std::visit(serializable, *this);

            return data;
        }
        // template<typename T>
        // T deserialize(std::vector<std::byte> &data) {
        //
        // }

        // static cvariant from_bytes(std::vector<std::byte> &data) {
        //     MajorType type = get_major_type(data[0]);
        //
        //     switch (type) {
        //         case MajorType::UNSIGNED_INTEGER:
        //
        //     }
        //
        // }
    };
}
#endif
