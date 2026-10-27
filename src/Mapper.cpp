#include "Mapper.h"

#include <format>
#include <stdexcept>

#include "Console.h"
#include "Mapper1.h"
#include "Mapper2.h"
#include "Mapper3.h"
#include "Mapper4.h"
#include "Mapper7.h"
#include "Mapper40.h"
#include "Mapper225.h"

Mapper *NewMapper(Console *console) {
    Cartridge *cartridge = console->cartridge;
    switch (cartridge->mapper) {
        case 0:
            return new Mapper2(cartridge);
        case 1:
            return new Mapper1(cartridge);
        case 2:
            return new Mapper2(cartridge);
        case 3:
            return new Mapper3(cartridge);
        case 4:
            return new Mapper4(console, cartridge);
        case 7:
            return new Mapper7(cartridge);
        case 40:
            return new Mapper40(console, cartridge);
        case 225:
            return new Mapper225(cartridge);
    }
    throw std::runtime_error{std::format("unsupported mapper: {}", cartridge->mapper)};
}
