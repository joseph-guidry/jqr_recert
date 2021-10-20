cmake_minimum_required (VERSION 3.14)

project(dsproject)

option(USE_TRIE "Use Threadpool" ON)

add_subdirectory(heap       EXCLUDE_FROM_ALL)
add_subdirectory(threadpool EXCLUDE_FROM_ALL)
add_subdirectory(tree       EXCLUDE_FROM_ALL)
add_subdirectory(wordlist   EXCLUDE_FROM_ALL)