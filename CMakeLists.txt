cmake_minimum_required(VERSION 3.0)

project(nes_emu)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)


include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)
