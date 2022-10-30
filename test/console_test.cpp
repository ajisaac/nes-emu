#include "gtest/gtest.h"
#include "Console.cpp"
#include "Cpu.cpp"


TEST(ConsoleTest, can_construct_console) {
	Console console{};
	console.run();
}