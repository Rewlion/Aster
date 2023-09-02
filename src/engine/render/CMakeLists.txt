cmake_minimum_required(VERSION 3.21)

set(DBG_TEXT_SRC
"${CMAKE_SOURCE_DIR}/src/engine/render/debug_text.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/debug_text_queue.cpp"
)

set(DBG_TEXT_STUB_SRC "${CMAKE_SOURCE_DIR}/src/engine/render/debug_text_stub.cpp")

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")
add_ecs_library(
  engine_render_ecs_utils
  ECS_FILES
  "${CMAKE_SOURCE_DIR}/src/engine/render/ecs_utils.ecs.cpp"
)

set(IMGUI_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_input.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_render.cpp")

set(FRAMEGRAPH_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/dbg_visualization.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/frame_graph.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/manager.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/registry.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/resource_request.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/render/frame_graph/resource_storage.cpp"
  )

add_ecs_library(
  engine_world_render
  FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/gui_render.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/terrain_render.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/virtual_texture/page_table.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/virtual_texture/virtual_texture.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/virtual_texture/vtile_lru.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/world_render.cpp"

    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/opaque.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/post_transparent.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/post_process.cpp"

    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes_cmd/show_tex.cpp"
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/decals.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/frame_preparing.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes_ecs/atmosphere.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes_ecs/virtual_terrain.ecs.cpp"
)

set(FONT_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/font/font_render.cpp")

add_library(engine_dbg_text OBJECT ${DBG_TEXT_SRC})
add_library(engine_dbg_text_stub OBJECT ${DBG_TEXT_STUB_SRC})
add_library(engine_font_render OBJECT ${FONT_SRC})
add_library(engine_framegraph OBJECT ${FRAMEGRAPH_SRC})
add_library(engine_imgui OBJECT ${IMGUI_SRC})
