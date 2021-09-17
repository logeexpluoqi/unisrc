cmake_minimum_required(VERSION 3.7)
project(unisrc)
set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(demo)
add_subdirectory(console)
add_subdirectory(controller)
add_subdirectory(kernel)
add_subdirectory(qmath)

add_executable(udemo unitest.c)

target_link_libraries(udemo demo_qsh qsh cmd)
target_link_libraries(udemo demo_timeslice timeslice)
target_link_libraries(udemo demo_msg msg)
target_link_libraries(udemo demo_fsm fsm)
target_link_libraries(udemo -lpthread)
