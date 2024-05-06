cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")

add_ecs_library(
  engine_components
  FILES
    "${CMAKE_SOURCE_DIR}/src/engine/components/atmosphere.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/sun.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/terrain.cpp"
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/components/camera.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/decal.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/point_light.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/static_mesh.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/engine/components/transform.ecs.cpp"
  )

