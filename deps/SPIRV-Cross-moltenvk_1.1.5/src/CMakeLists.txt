cmake_minimum_required(VERSION 3.21)

file(GLOB spirv_cross_src
  "${CMAKE_SOURCE_DIR}/deps/SPIRV-Cross-moltenvk_1.1.5/src/*.cpp")

add_library(spirv_cross ${spirv_cross_src})
