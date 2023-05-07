cmake_minimum_required(VERSION 3.21)

set(WORLD_RENDER_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/debug_text.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/gui_render.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/world_render.cpp"

  "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/frame_preparing.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/opaque.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/post_transparent.cpp"
)

set(IMGUI_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_input.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_render.cpp")

set(FRAMEGRAPH_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/frame_graph.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/manager.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/registry.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/resource_request.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/resource_storage.cpp"
  )

set(FONT_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/font/font_render.cpp")

add_library(engine_font_render ${FONT_SRC})
add_library(engine_framegraph ${FRAMEGRAPH_SRC})
add_library(engine_imgui ${IMGUI_SRC})
add_library(engine_world_render ${WORLD_RENDER_SRC})
