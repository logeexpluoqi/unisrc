
include_directories(../${CMAKE_CURRENT_SOURCE_DIR})

add_library(demo_qsh demo_qsh.c)
add_library(demo_fsm demo_fsm.c)
add_library(demo_timeslice demo_timeslice.c)
add_library(demo_msg demo_msg.c)
add_library(demo_qmath demo_qmath.c)
add_library(demo_qkey demo_qkey.c)
add_library(demo_filter demo_filter.c)

target_link_libraries(demo_qsh qsh cmd)
target_link_libraries(demo_timeslice qsh timeslice)
target_link_libraries(demo_qmath qsh qcomplex qmath)
target_link_libraries(demo_msg msg)
target_link_libraries(demo_fsm fsm qsh)

target_link_libraries(demo_filter sliding_average_filter qsh m)
target_link_libraries(demo_filter lpf_first_order)
target_link_libraries(demo_qkey qkey)