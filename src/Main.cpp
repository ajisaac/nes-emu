#include <cstddef>
#include <iostream>
#include <bitset>
#include "../headers/Console.h"


std::ostream &operator<<(std::ostream &os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int main(int argc, char *argv[]) {
    Console console{};
    console.run();
    return 0;
}