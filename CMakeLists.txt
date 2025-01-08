cmake_minimum_required(VERSION 3.29)

project(nes_emu)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set(BINARY ${PROJECT_NAME})

set(SOURCES
        src/APU.h
        src/Cartridge.h
        src/Console.h
        src/Controller.h
        src/Cpu.h
        src/Graphics.h
        src/UI/Main.cpp
        src/Mapper.h
        src/PPU.h
        src/Ram.h
        src/UI/Audio.h
        src/UI/Emulator.h
        src/UI/Font.h
        src/UI/GameView.h
        src/UI/MenuView.h

)
add_executable(${PROJECT_NAME}_run ${SOURCES})

target_link_libraries(${PROJECT_NAME}_run PRIVATE ${SDL2_LIBRARIES})