cmake_minimum_required(VERSION 3.29)

project(nes_emu)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

set(BINARY ${PROJECT_NAME})

set(SOURCES
        src/APU.h
        src/APU.cpp
        src/Cartridge.h
        src/Cartridge.cpp
        src/Console.h
        src/Console.cpp
        src/Controller.h
        src/Controller.cpp
        src/Cpu.h
        src/Cpu.cpp
        src/Main.cpp
        src/Graphics.h
        src/Graphics.cpp
        src/Mapper.h
        src/Mapper.cpp
        src/Palette.h
        src/PPU.h
        src/PPU.cpp
        src/Ram.h
        src/Ram.cpp
        src/Filter.cpp
        src/Filter.h
        src/ines.cpp
        src/ines.h
)
add_executable(${PROJECT_NAME}_run ${SOURCES})

target_link_libraries(${PROJECT_NAME}_run PRIVATE ${SDL3_LIBRARIES})
