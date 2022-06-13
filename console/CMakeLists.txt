include_directories(../${CMAKE_CURRENT_SOURCE_DIR})

add_library(qsh qsh.c)
add_library(qcli qcli.c)
target_link_libraries(qsh cmd)
