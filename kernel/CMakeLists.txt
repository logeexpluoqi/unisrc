add_library(cmd cmd.c)
add_library(fsm fsm.c)
add_library(msg msg.c)
add_library(timeslice timeslice.c)

target_link_libraries(list)
