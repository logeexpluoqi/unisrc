add_library(linuxbase 
    mthread.c
    udp_server.c)

target_link_libraries(linuxbase
    -lpthread
)