cmake_minimum_required(VERSION 3.13)

project(ds_wordlist_proj C)

set(LIB_SRC
    "wordlist.c"
)

set(LIB_HDRS 
    "${dsproject_SOURCE_DIR}/include/word_list.h"
)

add_library(libwordlist STATIC ${LIB_SRC} ${LIB_HDRS})
target_include_directories(libwordlist PUBLIC "../include")
target_link_libraries(libwordlist PRIVATE libtree)

source_group(
    TREE "${dsproject_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${LIB_HDRS}
)

