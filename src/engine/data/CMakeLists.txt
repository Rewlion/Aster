cmake_minimum_required(VERSION 3.21)

set(ENGINE_DATA_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/data/data.cpp"
)

add_library(engine_data ${ENGINE_DATA_SRC})
