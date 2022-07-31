cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE utils_src
  "${CMAKE_SOURCE_DIR}/src/engine/utils/*.cpp")

add_library(engine_utils ${utils_src})
