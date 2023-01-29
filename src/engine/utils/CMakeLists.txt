cmake_minimum_required(VERSION 3.21)

set(utils_src
  "${CMAKE_SOURCE_DIR}/src/engine/utils/fs.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/utils/string.cpp"
  )

add_library(engine_utils ${utils_src})
