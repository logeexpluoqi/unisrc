cmake_minimum_required(VERSION 3.7)
project(unisrc)
set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(demo)
add_subdirectory(console)
add_subdirectory(controller)
add_subdirectory(kernel)

add_executable(unitest unitest.c)

target_link_libraries(unitest qsh)
target_link_libraries(unitest timeslice)
target_link_libraries(unitest cmd)
target_link_libraries(unitest fsm)
target_link_libraries(unitest msg)
target_link_libraries(unitest demo_timeslice)
target_link_libraries(unitest demo_fsm)
target_link_libraries(unitest demo_qsh)
target_link_libraries(unitest -lpthread)
