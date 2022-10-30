#pragma once
#include <bitset>
#include "Ram.h"
#include "Cpu.h"

class Console
{

private:
	std::bitset<16> address_bus{ 0 }; // our 16 bit bus
	std::bitset<8> data_bus{ 0 }; // 8-bit data bus
	std::bitset<8> control_bus{ 0 }; // 8-bit control bus
	Ram ram{};
	Cpu cpu{ &ram };

public:
	Console();

	~Console();

	void run();
};

