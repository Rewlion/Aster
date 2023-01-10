cmake_minimum_required(VERSION 3.21)

set (QJS_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/array.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/exception.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/function.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/module_registration.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/object.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/value.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/qjs/vm.cpp"
)

add_library(engine_qjs ${QJS_SRC})

target_link_libraries(engine_qjs "quickjs")
