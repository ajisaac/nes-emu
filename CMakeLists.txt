cmake_minimum_required(VERSION 3.29)

project(nes_emu)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

set(BINARY ${PROJECT_NAME})

set(SOURCES
        src/APU.h
        src/Cartridge.h
        src/Console.h
        src/Controller.h
        src/Cpu.h
        src/Emulator.h
        src/Graphics.h
        src/INes.h
        src/Main.cpp
        src/Mapper.h
        src/PPU.h
        src/Ram.h

)
add_executable(${PROJECT_NAME}_run ${SOURCES})

