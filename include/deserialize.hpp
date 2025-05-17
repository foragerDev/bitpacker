//
// Created by forager on 5/12/25.
//

#ifndef DESERIALIZE_H
#define DESERIALIZE_H
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>


namespace bitpacker {
    std::optional<uint64_t> parse_unsigned_int(const std::vector<std::byte> &data);

    std::optional<int64_t> parse_int(const std::vector<std::byte> &data);

    std::optional<std::string_view> parse_string(std::vector<std::byte> &data);

    template<typename T>
    std::optional<std::vector<T> > parse_array(std::vector<std::byte> &data) { return {}; };

    template<typename K, typename V>
    std::optional<std::unordered_map<K, V> > parse_map(std::vector<std::byte> &data) { return {}; };
}
#endif //DESERIALIZE_H
