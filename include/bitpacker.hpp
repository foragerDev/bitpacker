//
// Created by forager on 5/11/25.
//

#ifndef BITPACKER_H
#define BITPACKER_H

#include "bitpacker.hpp"

#include <cstddef>
#include <vector>

#include "cbor_variant.hpp"

namespace bitpacker
{
    template <typename T>
    std::vector<std::byte> encode(T value)
    {
        return bitpacker::cvariant(value).serialize();
    }

    // TODO: Add Error handling
    template <typename T> // Return type
    T decode(std::vector<std::byte> &data)
    {
        if (data.empty())
        {
            throw std::runtime_error("Data is empty!");
        }
        auto type = cvariant::from_bytes(data);
        return *(type->get_type<T>());
    }
}

#endif // BITPACKER_H
