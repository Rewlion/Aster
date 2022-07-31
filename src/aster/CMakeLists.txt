cmake_minimum_required(VERSION 3.21)

file(GLOB aster_src
  "${CMAKE_SOURCE_DIR}/src/aster/*.cpp")
  list(FILTER aster_src EXCLUDE REGEX ".*[.][e][c][s][.][c][p][p]")

add_executable(aster ${aster_src})
target_link_libraries(aster engine)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(aster_output_name "aster-debug")
else()
  SET(aster_output_name "aster-release")
endif ()


set_target_properties(aster PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/game/aster"
                      OUTPUT_NAME "${aster_output_name}")
