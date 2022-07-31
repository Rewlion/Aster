cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE scene_src
  "${CMAKE_SOURCE_DIR}/src/engine/scene/*.cpp")

add_library(engine_scene ${scene_src})
