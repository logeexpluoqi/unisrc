
include_directories(../qshell)
include_directories(../frame)
include_directories(../qmath)
include_directories(../filter)
include_directories(../utils)
include_directories(../sort)
include_directories(../solver)
include_directories(../linuxbase)

add_library(demo
	# demo_ransac.cc
        demo_qsh.cpp
        demo_fsm.cpp
        demo_timeslice.cpp
        demo_msg.cpp
        demo_qmath.cpp
        demo_qkey.cpp
        demo_filter.cpp
        demo_sort.cpp
        demo_qmem.cpp
	demo_solver.cpp
        demo_mthread.cpp
        demo_udp_server.cpp
        demo_ransac.cc
)
target_link_libraries(demo
        qsh
        qmath
        utils
        filter
        sort
        ode
        linuxbase 
        -lm
)
