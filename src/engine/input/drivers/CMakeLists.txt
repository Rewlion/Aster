cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE ms_input_driver_src
  "${CMAKE_SOURCE_DIR}/src/engine/input/drivers/windows/*.cpp")

add_library(engine_input_drivers ${ms_input_driver_src})
