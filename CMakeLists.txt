cmake_minimum_required(VERSION 3.14)

project(senior_jqr C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_BINARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(ASAN_ENABLED "Enable Address Sanitization" OFF)

include(cmake/asan.cmake)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(src)

set(MAIN_SRC
    src/tp_main.c
)

add_executable(ds-app ${MAIN_SRC})
target_link_libraries(ds-app PRIVATE libtp libwordlist)