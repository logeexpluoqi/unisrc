include_directories(../frame)

add_library(qsh 
        qsh.c)
target_link_libraries(qsh 
        frame)
