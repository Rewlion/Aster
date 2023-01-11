cmake_minimum_required(VERSION 3.21)

SET(GUI_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/gui/element_tree_builder.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/gui.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/react_state.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/react_states_storage.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/scene_tree_builder.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/timer.cpp"
)

SET(GUI_MODULES_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/gui/ui_module.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gui/log_module.cpp")

add_library(engine_gui ${GUI_SRC})
add_library(engine_gui_modules OBJECT ${GUI_MODULES_SRC})
