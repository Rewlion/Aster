cmake_minimum_required(VERSION 3.19)

project(Aster VERSION 0.1
        DESCRIPTION "<=*_*=>"
        LANGUAGES CXX)


find_package(Vulkan)

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(DEP_DIR "${CMAKE_SOURCE_DIR}/dependency")
set(GLFW_DEP_DIR "${DEP_DIR}/glfw")
set(GLM_DEP_DIR "${DEP_DIR}/glm")
set(SPIRV_DEP_DIR "${DEP_DIR}/spirv_cross")
set(CATCH2_DEP_DIR "${DEP_DIR}/Catch2")
set(YAML_CPP_DEP_DIR "${DEP_DIR}/yaml-cpp")
set(KTX_DIR "${DEP_DIR}/ktx")
set(IMGUI_DEP_DIR "${DEP_DIR}/imgui")
set(GOOGLE_BENCHMARK_DEP_DIR "${DEP_DIR}/google/benchmark")

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${DEP_DIR}"
    "${Vulkan_INCLUDE_DIRS}"
    "${CATCH2_DEP_DIR}/Catch2"
    "${KTX_DIR}/"
    "${GOOGLE_BENCHMARK_DEP_DIR}/include"
)

add_subdirectory("${CMAKE_SOURCE_DIR}/dependency/Catch2")
add_subdirectory("${CMAKE_SOURCE_DIR}/dependency/ecs")
add_subdirectory("${CMAKE_SOURCE_DIR}/dependency/imgui")
add_subdirectory("${CMAKE_SOURCE_DIR}/dependency/google/benchmark")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/engine")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/application/Aster")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/application/mesh_editor")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/application/examples")

add_subdirectory("${CMAKE_SOURCE_DIR}/tests/shader_parsing")

add_subdirectory("${CMAKE_SOURCE_DIR}/benchmarks")
