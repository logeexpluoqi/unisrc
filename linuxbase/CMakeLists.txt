add_library(linuxbase 
    mthread.c)

target_link_libraries(linuxbase
    -lpthread
)