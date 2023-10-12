cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/src/cmake/ecs.cmake")

add_ecs_library(
  aster_ecs
  ECS_FILES
    "${CMAKE_SOURCE_DIR}/src/aster/camera.ecs.cpp"
    "${CMAKE_SOURCE_DIR}/src/aster/ecs/gui.ecs.cpp"
)

add_executable(aster "${CMAKE_SOURCE_DIR}/src/aster/main.cpp")
target_link_libraries(aster aster_ecs "$<LINK_LIBRARY:WHOLE_ARCHIVE,engine>")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(aster_output_name "aster-debug")
else()
  SET(aster_output_name "aster-release")
endif ()


set_target_properties(aster PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/game/aster"
                      OUTPUT_NAME "${aster_output_name}")
