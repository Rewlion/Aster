cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE assets_src "${CMAKE_SOURCE_DIR}/src/engine/assets/*.cpp")


add_library(engine_assets ${assets_src})
target_link_libraries(engine_assets
  ${KTX_LIB})
