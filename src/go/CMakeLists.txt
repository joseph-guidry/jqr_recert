set(go_files
  server.go
)

set(cgo_deps
  libworld
)

add_cgo_executable(goodprogram go_files cgo_deps ${CMAKE_CURRENT_BINARY_DIR})