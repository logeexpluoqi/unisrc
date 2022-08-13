include_directories(../frame)

add_library(utils
        qkey.c
)
target_link_libraries(utils 
        frame
)

