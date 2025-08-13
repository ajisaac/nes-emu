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
        src/Graphics.h
        src/Graphics.cpp
        src/UI/Main.cpp
        src/Mapper.h
        src/Mapper.cpp
        src/PPU.h
        src/PPU.cpp
        src/Ram.h
        src/Ram.cpp
        src/UI/Audio.h
        src/UI/Audio.cpp
        src/UI/Emulator.h
        src/UI/Emulator.cpp
        src/UI/Font.h
        src/UI/Font.cpp
        src/UI/GameView.h
        src/UI/GameView.cpp
        src/UI/MenuView.h
        src/UI/MenuView.cpp

)
add_executable(${PROJECT_NAME}_run ${SOURCES})

target_link_libraries(${PROJECT_NAME}_run PRIVATE ${SDL3_LIBRARIES})