cmake_minimum_required (VERSION 2.8)

project(dsproject)

option(USE_TRIE "Use Threadpool" ON)

# add_subdirectory(list)
# add_subdirectory(graph)
# add_subdirectory(go)


add_subdirectory(heap       EXCLUDE_FROM_ALL)
add_subdirectory(threadpool EXCLUDE_FROM_ALL)
add_subdirectory(tree       EXCLUDE_FROM_ALL)
add_subdirectory(wordlist   EXCLUDE_FROM_ALL)
add_subdirectory(test_cgo)

# set(TARGET go-test-app)

# set(DS_SRC
#     # tp_main.c
#     main.c
# )
