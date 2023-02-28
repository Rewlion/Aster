cmake_minimum_required(VERSION 3.21)

file(GLOB deps_imgui_src
  "${CMAKE_SOURCE_DIR}/deps/imgui/src/imgui/*.cpp"
)

add_library(deps_imgui ${deps_imgui_src})