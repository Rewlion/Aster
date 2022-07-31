cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE shaders_compiler_src
  "${CMAKE_SOURCE_DIR}/src/shaders_compiler/*.cpp")

add_executable(shaders_compiler ${shaders_compiler_src})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(shc_output_name "shaders_compiler-debug")
else()
  SET(shc_output_name "shaders_compiler-release")
endif ()

target_link_libraries(shaders_compiler
  engine
  spirv_cross
  "dxcompiler"
)

set_target_properties(shaders_compiler PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tools"
  OUTPUT_NAME "${shc_output_name}")
