if (ASAN_ENABLED)
  list(APPEND CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
  list(APPEND CMAKE_C_FLAGS "-fsanitize=address")
endif()