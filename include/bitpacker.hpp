//
// Created by forager on 5/11/25.
//

#ifndef BITPACKER_H
#define BITPACKER_H

#include "bitpacker.hpp"

#include <cstddef>
#include <vector>

#include "cbor_variant.hpp"

namespace bitpacker {
    template<typename T>
    std::vector<std::byte> encode(T value) {
        return bitpacker::cvariant(value).serialize();
    }

    // template<typename T>  // Return type
    // T decode(std::vector<std::byte> &data) {
    //     return bitpacker::cvariant(data).deserialize();
    // }
}

#endif //BITPACKER_H
