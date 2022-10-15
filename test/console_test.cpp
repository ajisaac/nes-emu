#include "gtest/gtest.h"
#include "../headers/Console.h"

TEST(consoleTest, constructorWorks) {
    Console cons{};
}

TEST(consoleTest, testing) {
    std::cout << sizeof(std::size_t) << std::endl;
    std::cout << sizeof(long long int) << std::endl;
    size_t s{25};
    std::cout << s << std::endl;
}
