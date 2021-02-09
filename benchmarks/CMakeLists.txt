cmake_minimum_required(VERSION 3.19)

file(GLOB_RECURSE BENCHMARKS_SOURCES "${CMAKE_SOURCE_DIR}/benchmarks/*.cpp" "${CMAKE_SOURCE_DIR}/benchmarks/*.cc" "${CMAKE_SOURCE_DIR}/benchmarks/*.h")

add_executable(BENCHMARKS_EXE ${BENCHMARKS_SOURCES})
target_link_libraries(BENCHMARKS_EXE ENGINE_LIB benchmark::benchmark)

set_target_properties(BENCHMARKS_EXE PROPERTIES 
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED YES
                        CXX_EXTENSIONS NO
                        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
                        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug")
