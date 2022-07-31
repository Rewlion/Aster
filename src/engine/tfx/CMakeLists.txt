cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE tfx_src
  "${CMAKE_SOURCE_DIR}/src/engine/tfx/*.cpp")

add_library(engine_tfx ${tfx_src})
