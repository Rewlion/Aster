cmake_minimum_required(VERSION 3.21)

file(GLOB engine_src
  "${CMAKE_SOURCE_DIR}/src/engine/*.cpp")

add_library(engine ${engine_src})

target_link_libraries(engine
  deps_imgui
  deps_imgui_node_editor
  engine_assets
  engine_console
  engine_datablock
  engine_ecs
  engine_editor
  engine_gapi
  engine_gui
  engine_input
  engine_input_drivers
  engine_qjs
  engine_scene
  engine_tfx
  engine_utils
  engine_work_cycle
  "EASTL"
  "dbgeng"
  "freetype"
  "ktx"
  "user32"
  "vulkan-1"
  "xxhash"
  )

SET(ENGINE_SELF_REG_LIB
  engine_dbg_text
  engine_font_render
  engine_gui_modules
  engine_framegraph
  engine_imgui
  engine_render_ecs_utils
  engine_world_render
  PARENT_SCOPE)

