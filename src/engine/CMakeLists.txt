cmake_minimum_required(VERSION 3.21)

file(GLOB engine_src
  "${CMAKE_SOURCE_DIR}/src/engine/*.cpp")

add_library(engine ${engine_src})

target_link_libraries(engine
  engine_assets
  engine_datablock
  engine_ecs
  engine_font_render
  engine_framegraph
  engine_gapi
  engine_input
  engine_scene
  engine_scheme
  engine_tfx
  engine_utils
  engine_work_cycle
  engine_world_render
  "EASTL"
  "dbgeng"
  "freetype"
  "ktx"
  "user32"
  "vulkan-1"
  )
