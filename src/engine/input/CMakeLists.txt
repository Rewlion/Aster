cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE input_src
  "${CMAKE_SOURCE_DIR}/src/engine/input/*.cpp")

add_library(engine_input ${input_src})
