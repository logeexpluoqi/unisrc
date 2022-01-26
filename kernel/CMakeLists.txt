add_library(cmd cmd.c)
add_library(fsm fsm.c)
add_library(msg msg.c)
add_library(timeslice timeslice.c)
add_library(qlist qlist.c)

target_link_libraries(cmd qlist)
target_link_libraries(fsm qlist)
target_link_libraries(timeslice qlist)
