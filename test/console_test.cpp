#include "gtest/gtest.h"
#include "../headers/console.h"

TEST(consoleTest, constructorWorks) {
    console cons{};
    std::cout << sizeof(std::size_t) << std::endl;
    EXPECT_EQ (0, 0);
}
