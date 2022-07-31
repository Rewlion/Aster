cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE datablock_src
  "${CMAKE_SOURCE_DIR}/src/engine/datablock/*.cpp")

add_library(engine_datablock ${datablock_src})

