cmake_minimum_required(VERSION 3.28)
project(nes)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
add_executable(nes src/main.cpp

        src/Cartridge.h
        src/Cartridge.cpp

        src/Console.h
        src/Console.cpp

        src/Cpu.h
        src/Cpu.cpp

        src/Emulator.h
        src/Emulator.cpp

        src/Graphics.h
        src/Graphics.cpp

        src/INes.h
        src/INes.cpp

        src/Main.cpp

        src/Mapper.h
        src/Mapper.cpp

        src/PPU.h
        src/PPU.cpp

        src/Ram.h

        src/Utils.h
        src/Utils.cpp

        src/Audio.cpp
        src/Audio.h

        src/Controller.cpp
        src/Controller.h
)

include_directories(nes "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2")
target_link_libraries(nes ${SDL2_LIBRARIES} "-L/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib -lSDL2_image")
