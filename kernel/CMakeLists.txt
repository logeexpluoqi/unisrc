add_library(cmd cmd.c)
add_library(fsm fsm.c)
add_library(msg msg.c)
add_library(timeslice timeslice.c)

add_subdirectory(service)

target_link_libraries(cmd list)
target_link_libraries(fsm list)
target_link_libraries(timeslice list)
