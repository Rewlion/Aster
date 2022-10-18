cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")

add_ecs_library(
  engine_work_cycle
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/engine/work_cycle/collect_camera.ecs.cpp"
)
