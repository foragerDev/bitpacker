#ifndef CVARIANT
#define CVARIANT

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include "serialize.hpp"

namespace bitpacker
{
    class cvariant : public var_t
    {
    public:
        template<typename T>
        cvariant(T &&value) : var_t(std::forward<T>(value)) {}

        template <typename... Ts>
        struct overloaded : Ts...
        {
            using Ts::operator()...;
        };
        std::vector<std::byte> serialize()
        {
            std::vector<std::byte> data;
            auto serializable =
                overloaded{
                    [&](std::integral auto number)
                    { data = bitpacker::serialize(number); },
                    [&](std::string_view str)
                    { data = bitpacker::serialize(str); },
                    [&](std::vector<cvariant> &array)
                    { data = bitpacker::serialize(array); }};
            std::visit(serializable, *this);

            return data;
        }
    };
}
#endif