cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE render_src
  "${CMAKE_SOURCE_DIR}/src/engine/render/*.cpp")

add_library(engine_render ${render_src})
