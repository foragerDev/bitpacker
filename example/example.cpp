//
// Created by forager on 5/11/25.
//

#include <cstddef>
#include <iostream>
#include <list>
#include <queue>

#include "bitpacker.hpp"
#include "cbor_variant.hpp"

int main()
{
    uint64_t age = -10;

    std::vector<std::byte> bytes = bitpacker::encode(age);

    std::string name = "YOU CAN'T DO THIS OR THAT";

    std::vector<std::byte> name_pytes = bitpacker::encode(name);
    const std::vector<int> scores = {2, 4, 5, 6, 4};

    std::vector<std::byte> score_bytes = bitpacker::encode(std::vector<int>{1, 2, 3, 4, 5});

    const std::list<int> lists{1, 2, 3, 4, 5, 5};
    std::vector<std::byte> lists_bytes = bitpacker::encode(lists);

    const std::deque<int> que{1, 2, 3, 4, 56};
    std::vector<std::byte> que_bytes = bitpacker::encode(que);

    std::string data = bitpacker::decode<std::string>(name_pytes);
    std::cout << data << std::endl;
}
