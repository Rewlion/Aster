cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE gapi_src
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/*.cpp")

add_library(engine_gapi ${gapi_src})
