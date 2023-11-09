cmake_minimum_required(VERSION 3.21)

file(GLOB deps_optick_src
  "${CMAKE_SOURCE_DIR}/deps/optick/src/*.cpp"
)

add_library(deps_optick ${deps_optick_src})