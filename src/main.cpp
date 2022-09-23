#include <cstddef>
#include <iostream>
#include <bitset>
#include "../headers/console.h"


std::ostream &operator<<(std::ostream &os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int main(int argc, char *argv[]) {
    console console{};
    console.run();
    return 0;
}