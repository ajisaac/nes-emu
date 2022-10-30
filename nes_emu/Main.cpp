#include <iostream>
#include <bitset>
#include <cstddef>
#include "src/Console.h"

std::ostream& operator<<(std::ostream& os, std::byte b) {
	return os << std::bitset<8>(std::to_integer<int>(b));
}

int main() {
	Console console{};
	console.run();
	return 0;
}