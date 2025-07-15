#ifndef CVARIANT
#define CVARIANT

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <concepts>

#include "deserialize.hpp"
#include "serialize.hpp"

namespace bitpacker
{
    template <typename T>
    concept PackableContainer = std::ranges::range<std::remove_cvref_t<T>> &&
                                requires(
                                    std::remove_cvref_t<T> &c,
                                    typename std::remove_cvref_t<T>::value_type const &v) {
                                    c.emplace_back(v);
                                };

    class cvariant : public var_t
    {
        using var_t::variant;

    public:
        template <typename T>
        cvariant(T &&value) : var_t(std::forward<T>(value))
        {
        }

        template <PackableContainer T>
            requires std::constructible_from<var_t, std::decay_t<typename T::value_type>>
        cvariant(T &&container) : var_t(std::vector<cvariant>(container.begin(), container.end()))
        {
        }

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
                    [&](std::string str)
                    { data = bitpacker::serialize(str); },
                    [&](std::vector<cvariant> &array)
                    { data = bitpacker::serialize(array); },
                    [&](float single_float)
                    {
                        data = bitpacker::serialize(single_float);
                    },
                    [&](double double_float)
                    {
                        data = bitpacker::serialize(double_float);
                    }};
            std::visit(serializable, *this);

            return data;
        }

        template <typename T>
        std::optional<T> get_type()
        {
            if constexpr (std::is_integral_v<T>)
            {
                if (std::is_signed_v<T> && std::holds_alternative<int64_t>(*this))
                {
                    return static_cast<int64_t>(std::get<int64_t>(*this));
                }

                if (std::holds_alternative<uint64_t>(*this))
                {
                    return static_cast<uint64_t>(std::get<uint64_t>(*this));
                }
            }

            if constexpr (std::is_same_v<T, std::string>)
            {
                if (std::holds_alternative<std::string>(*this))
                {
                    return std::get<std::string>(*this);
                }
                // if (std::holds_alternative<std::string_view>(*this)) {
                //     return std::get<std::string_view>(*this);
                // }
            }
            return {};
        }

        static std::optional<cvariant> from_bytes(const std::vector<std::byte> &data)
        {
            const MajorType type = get_major_type(data[0]);
            std::optional<cvariant> v{};
            switch (type)
            {
            case MajorType::UNSIGNED_INTEGER:
            {
                auto temp_op = parse_unsigned_int(data);
                if (temp_op.has_value())
                {
                    v = cvariant(temp_op.value());
                }

                break;
            }
            case MajorType::SIGNED_INTEGER:
            {
                auto temp_op2 = parse_int(data);
                if (temp_op2.has_value())
                {
                    v = cvariant(temp_op2.value());
                }
                // v = parse_int(data);
                break;
            }
            // case MajorType::ARRAY:
            //     break;
            case MajorType::TEXT_STRING:
            {
                auto temp_op2 = parse_string(data);
                if (temp_op2.has_value())
                {
                    v = cvariant(temp_op2.value());
                }
            }
            // case MajorType::BYTE_STRING:
            //     break;
            default:
                break;
            }

            return v;
        }

        // template<typename T>
        // T as() const {
        //     return std::get<T>valu
        // }
    };
}
#endif
