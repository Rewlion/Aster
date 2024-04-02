cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")

add_ecs_library(
  engine_dbg_text
  FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/debug/debug_text.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/debug/line_render.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/debug/poly_render.cpp"
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/debug/debug_text_queue.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/debug/render.ecs.cpp"
  )

set(DBG_TEXT_STUB_SRC "${CMAKE_SOURCE_DIR}/src/engine/render/debug/debug_text_stub.cpp")
add_library(engine_dbg_text_stub OBJECT ${DBG_TEXT_STUB_SRC})

add_ecs_library(
  engine_render_ecs_utils
  ECS_FILES
  "${CMAKE_SOURCE_DIR}/src/engine/render/ecs_utils.ecs.cpp"
)

add_ecs_library(
  engine_imgui
  FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_input.cpp"
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/imgui/imgui_render.ecs.cpp"
)

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

    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/dbg_wnd.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/gi_state.cpp"

  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/render/clustered_lights.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/model_render.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/show_tex.ecs.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/atmosphere.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/clustered_light_culling.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/dbg_line_renderer.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/dbg_poly_renderer.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/dbg_text.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/decals.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/frame_preparing.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/hi_z.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/opaque.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/post_process.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/present.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/reflections.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/shadow/shadow_map.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/taa.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/ui.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/virtual_terrain.ecs.cpp"

    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/dbg_draw_surfels.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/indirect_light.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/indirect_light_spatial_filter.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/resources.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/spatial_storage_binning.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/surfels_acc_irradiance.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/surfels_allocate.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/surfels_find_alloc_pos.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/surfels_recycle.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/surfels_transform.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/render/nodes/gi_surfels/sync_nodes.ecs.cpp"
)

set(FONT_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/render/font/font_render.cpp")

add_library(engine_font_render OBJECT ${FONT_SRC})
add_library(engine_framegraph OBJECT ${FRAMEGRAPH_SRC})
