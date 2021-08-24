cmake_minimum_required(VERSION 3.13)

project(dsproject)

option(WITH_USE_THREADPOOL "Use Threadpool" ON)

# add_subdirectory(list)
# add_subdirectory(graph)

add_subdirectory(tree)


add_subdirectory(heap EXCLUDE_FROM_ALL)
add_subdirectory(threadpool EXCLUDE_FROM_ALL)



set(DS_SRC
    tp_main.c
    # main.c
)

set(WL_SRC
    word_list.c
)

set(WL_HDRS 
    "${dsproject_SOURCE_DIR}/include/word_list.h"
)

add_library(libwordlist STATIC ${WL_SRC} ${WL_HDRS})
target_link_libraries(libwordlist PRIVATE libtree)
target_include_directories(libwordlist PUBLIC "../include")

add_executable(ds_testapp ${DS_SRC})
target_link_libraries(ds_testapp PRIVATE libtp libwordlist)