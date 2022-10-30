#include "gtest/gtest.h"
#include <Cpu.h>

TEST(CpuTest, can_construct_cpu) {
	Ram ram{};
	Cpu cpu{&ram};
}