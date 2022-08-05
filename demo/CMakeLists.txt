
include_directories(../console)
include_directories(../frame)
include_directories(../qmath)
include_directories(../filter)
include_directories(../utils/key)
include_directories(../sort)

add_library(demo_qsh 
        demo_qsh.c)
target_link_libraries(demo_qsh 
        qsh 
        timeslice cmd)

add_library(demo_fsm 
        demo_fsm.c)
target_link_libraries(demo_fsm 
        fsm 
        qsh)

add_library(demo_timeslice 
        demo_timeslice.c)
target_link_libraries(demo_timeslice 
        qsh 
        timeslice)

add_library(demo_msg 
        demo_msg.c)
target_link_libraries(demo_msg 
        msg)

add_library(demo_qmath 
        demo_qmath.c)
target_link_libraries(demo_qmath 
        qsh 
        qcomplex 
        qmath)

add_library(demo_qkey 
        demo_qkey.c)
target_link_libraries(demo_qkey 
        qkey)

add_library(demo_filter 
        demo_filter.c)
target_link_libraries(demo_filter 
        qsh 
        -lm 
        sliding_average_filter 
        lpf_first_order 
        kalman_filter)

add_library(demo_sort 
        demo_sort.c)
target_link_libraries(demo_sort 
        qsh 
        quick_sort 
        bubble_sort)

add_library(demo_qmem 
        demo_qmem.c)
target_link_libraries(demo_qmem 
        qsh 
        qmem)
