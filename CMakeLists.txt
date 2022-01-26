cmake_minimum_required(VERSION 3.7)
project(unisrc)
set(CMAKE_C_STANDARD 11)

add_subdirectory(demo)
add_subdirectory(console)
add_subdirectory(controller)
add_subdirectory(kernel)
add_subdirectory(qmath)
add_subdirectory(solver)

add_executable(udemo unitest.c)

target_link_libraries(udemo demo_qsh)
target_link_libraries(udemo demo_timeslice)
target_link_libraries(udemo demo_msg)
target_link_libraries(udemo demo_fsm)
target_link_libraries(udemo -lpthread)
target_link_libraries(udemo demo_qmath)
