//
// Created by forager on 5/11/25.
//


#include <cstddef>

#include "bitpacker.hpp"
#include "cbor_variant.hpp"

int main() {
    int age = 10;

    std::vector<std::byte> bytes = bitpacker::encode(age);

    std::string name = "John";

    std::vector<std::byte> name_pytes = bitpacker::encode(age);

    const std::vector<int> scores = {2, 4, 5, 6, 4};

    std::vector<std::byte> score_bytes = bitpacker::encode(scores);


    // Decode

    int data = bitpacker::decode(bytes);
}

