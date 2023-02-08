cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE ecs_src
  "${CMAKE_SOURCE_DIR}/src/engine/ecs/*.cpp")

add_library(engine_ecs ${ecs_src})
