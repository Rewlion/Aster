cmake_minimum_required(VERSION 3.21)

project(Not_this_again VERSION 0.1
        DESCRIPTION "<=*_*=>"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DPLATFORM_WIN64)
add_definitions(#-Wno-defaulted-function-deleted
-Wno-deprecated-volatile
-Wno-deprecated-declarations)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/deps/SPIRV-Cross-moltenvk_1.1.5/include"
  "${CMAKE_SOURCE_DIR}/deps/boost-1.77/include"
  "${CMAKE_SOURCE_DIR}/deps/dxc/include"
  "${CMAKE_SOURCE_DIR}/deps/eastl-3.17.06/include"
  "${CMAKE_SOURCE_DIR}/deps/freetype-2.12.1/include"
  "${CMAKE_SOURCE_DIR}/deps/function2/include"
  "${CMAKE_SOURCE_DIR}/deps/glm-0.9.9.8/include"
  "${CMAKE_SOURCE_DIR}/deps/gtest/include"
  "${CMAKE_SOURCE_DIR}/deps/imgui/src"
  "${CMAKE_SOURCE_DIR}/deps/imgui-node-editor/src"
  "${CMAKE_SOURCE_DIR}/deps/ktx-4.0.0/include"
  "${CMAKE_SOURCE_DIR}/deps/quickjs/include"
  "${CMAKE_SOURCE_DIR}/deps/spdlog-1.9.2/include"
  "${CMAKE_SOURCE_DIR}/deps/tinygltf-2.5.0/include"
  "${CMAKE_SOURCE_DIR}/deps/vulkan-1.3.216.0/include"
  "${CMAKE_SOURCE_DIR}/deps/xxhash/include"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories("${CMAKE_SOURCE_DIR}/deps/eastl-3.17.06/win64/debug")
else()
  link_directories("${CMAKE_SOURCE_DIR}/deps/eastl-3.17.06/win64/release")
endif ()


link_directories(
  "${CMAKE_SOURCE_DIR}/deps/boost-1.77/win64"
  "${CMAKE_SOURCE_DIR}/deps/dxc"
  "${CMAKE_SOURCE_DIR}/deps/freetype-2.12.1/win64"
  "${CMAKE_SOURCE_DIR}/deps/ktx-4.0.0/win64"
  "${CMAKE_SOURCE_DIR}/deps/vulkan-1.3.216.0/win64/"
  "${CMAKE_SOURCE_DIR}/deps/quickjs/win64"
  "${CMAKE_SOURCE_DIR}/deps/xxhash/win64"
)

#workaround for dxcompiler linkage
configure_file("${CMAKE_SOURCE_DIR}/deps/dxc/win64/dxcompiler.lib" "${CMAKE_BINARY_DIR}/dxcompiler.lib" COPYONLY)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/assets")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/console")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/datablock")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/ecs")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/editor")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/gapi")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/gui")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/input")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/input/drivers")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/qjs")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/render")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/scene")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/tfx")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/utils")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine/work_cycle")

add_subdirectory("${CMAKE_SOURCE_DIR}/deps/SPIRV-Cross-moltenvk_1.1.5/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/deps/imgui/src/imgui")
add_subdirectory("${CMAKE_SOURCE_DIR}/deps/imgui-node-editor/src/imgui-node-editor")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/shaders_compiler")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/tests")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/aster")
