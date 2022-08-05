add_library(cmd 
        cmd.c)
target_link_libraries(cmd 
        qlist)

add_library(fsm 
        fsm.c)
target_link_libraries(fsm qlist)

add_library(msg 
        msg.c)

add_library(timeslice 
        timeslice.c)
target_link_libraries(timeslice 
        qlist)

add_library(qlist 
        qlist.c)

add_library(qmem 
        qmem.c)

add_library(ctrl 
        ctrl.c)

