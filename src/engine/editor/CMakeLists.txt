cmake_minimum_required(VERSION 3.21)

set(EDITOR_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/editor/editor.cpp"
)

add_library(engine_editor ${EDITOR_SRC})

