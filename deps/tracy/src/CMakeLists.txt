cmake_minimum_required(VERSION 3.21)

file(GLOB deps_tracy_src
  "${CMAKE_SOURCE_DIR}/deps/tracy/src/*.cpp"
)

add_library(deps_tracy ${deps_tracy_src})