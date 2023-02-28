cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE console_src
  "${CMAKE_SOURCE_DIR}/src/engine/console/*.cpp")

add_library(engine_console ${console_src})
