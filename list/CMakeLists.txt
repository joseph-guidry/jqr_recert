cmake_minimum_required(VERSION 3.13)

project(ds_list_proj)

set(LIB_SRC
    "list.c"
)

set(LIB_HDRS 
    "${dsproject_SOURCE_DIR}/include/list.h"
)

add_library(liblist STATIC ${LIB_SRC} ${LIB_HDRS})
target_include_directories(liblist PUBLIC "../include")

source_group(
    TREE "${dsproject_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${LIB_HDRS}
)