cmake_minimum_required(VERSION 3.21)

file(GLOB deps_imgui_node_editor_src
  "${CMAKE_SOURCE_DIR}/deps/imgui-node-editor/src/imgui-node-editor/*.cpp"
)

add_library(deps_imgui_node_editor ${deps_imgui_node_editor_src})