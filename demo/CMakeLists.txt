add_library(demo_qsh demo_qsh.c)
add_library(demo_fsm demo_fsm.c)
add_library(demo_timeslice demo_timeslice.c)

target_link_libraries(demo_timeslice timeslice)
target_link_libraries(demo_qsh timeslice)
target_link_libraries(demo_qsh cmd)
target_link_libraries(demo_fsm fsm)
