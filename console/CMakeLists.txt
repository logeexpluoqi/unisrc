add_library(qsh qsh.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kernal)

target_link_libraries(cmd)