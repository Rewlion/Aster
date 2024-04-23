cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")

add_ecs_library(
  engine_components
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/components/static_mesh.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/transform.ecs.cpp"
  )

