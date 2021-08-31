add_library(qsh qsh.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kernel)

target_link_libraries(qsh cmd)
