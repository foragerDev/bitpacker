//
// Created by forager on 5/11/25.
//


#include <cstddef>
#include <iostream>

#include "bitpacker.hpp"
#include "cbor_variant.hpp"

int main() {
    uint64_t age = -10;

    std::vector<std::byte> bytes = bitpacker::encode(age);

    std::string name = "YOU CAN'T DO THIS OR THAT";

    std::vector<std::byte> name_pytes = bitpacker::encode(name);

    // const std::vector<int> scores = {2, 4, 5, 6, 4};

    // std::vector<std::byte> score_bytes = bitpacker::encode(scores);


    // Decode

    std::string data = bitpacker::decode<std::string>(name_pytes);
    std::cout << data << std::endl;
}

