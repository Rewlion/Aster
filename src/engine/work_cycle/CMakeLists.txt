cmake_minimum_required(VERSION 3.21)

file(GLOB work_cycle_src
"${CMAKE_SOURCE_DIR}/src/engine/work_cycle/*.cpp")
list(FILTER work_cycle_src EXCLUDE REGEX ".*[.][e][c][s][.][c][p][p]")

add_library(engine_work_cycle ${work_cycle_src})
