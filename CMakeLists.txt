cmake_minimum_required(VERSION 3.7)
project(unisrc)
set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(demo)

add_executable(unitest unitest.c)

target_link_libraries(demo)