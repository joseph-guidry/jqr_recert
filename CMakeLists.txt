cmake_minimum_required(VERSION 3.14)

project(cmake_cgo)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_BINARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(ASAN_ENABLED OFF)

include(cmake/go.cmake)
# include(cmake/asan.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(src/c)
add_subdirectory(src/go)