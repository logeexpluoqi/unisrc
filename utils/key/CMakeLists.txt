include_directories(../../${CMAKE_CURRENT_SOURCE_DIR})

add_library(qkey qkey.c)

target_link_libraries(qkey qlist)
