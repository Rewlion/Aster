cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE render_src
  "${CMAKE_SOURCE_DIR}/src/engine/render/*.cpp")

set(WORLD_RENDER_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/world_render.cpp")

set(FRAMEGRAPH_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/blackboard.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/frame_graph.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/render_pass.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/render_pass_builder.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/render_pass_resources.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/resources.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/topological_sorter.cpp")

set(FONT_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/font/font_render.cpp")

add_library(engine_font_render ${FONT_SRC})
add_library(engine_framegraph ${FRAMEGRAPH_SRC})
add_library(engine_world_render ${WORLD_RENDER_SRC})
