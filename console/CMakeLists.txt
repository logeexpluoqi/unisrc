include_directories(../${CMAKE_CURRENT_SOURCE_DIR})

add_library(qsh qsh.c)
target_link_libraries(qsh cmd)
