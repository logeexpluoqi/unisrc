cmake_minimum_required(VERSION 3.7)
project(unisrc)
set(CMAKE_CPP_STANDARD 11)

add_subdirectory(demo)
add_subdirectory(qshell)
add_subdirectory(controller)
add_subdirectory(frame)
add_subdirectory(qmath)
add_subdirectory(solver)
add_subdirectory(utils)
add_subdirectory(filter)
add_subdirectory(sort)
add_subdirectory(cv)
add_subdirectory(ransac)
add_subdirectory(linuxbase)

add_executable(qdemo 
        qdemo.cpp
)
target_link_libraries(qdemo 
        demo
        -lpthread
)
