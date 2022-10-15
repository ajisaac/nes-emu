#include "../headers/Console.h"
#include <bitset>
#include <cstddef>
#include <iostream>

std::ostream &operator<<(std::ostream &os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int main() {
    Console console{};
    console.run();
    std::cout << "Hi " << std::endl;
    return 0;
}
