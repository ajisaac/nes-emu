#include "Console.h"


//    controller1 = Controller();
//    controller2 = Controller();

// ram starts out blank
//    ram = Ram();
//    mapper = Mapper(ram);
//    cpu = Cpu(ram);
//    ppu = Ppu(ram);
//    apu = Apu(ram);

Console::Console(Cartridge cartridge) : ram(Ram()), cpu(Cpu(&ram)) {
}

void Console::step(long i) {

}
