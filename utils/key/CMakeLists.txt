include_directories(../../frame)

add_library(qkey 
        qkey.c)
target_link_libraries(qkey 
        frame)

