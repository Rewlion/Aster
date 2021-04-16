cmake_minimum_required(VERSION 3.19)

project(cpp-reflect VERSION 0.1
        DESCRIPTION "<=*_*=>"
        LANGUAGES CXX)

if (MSVC) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

set(PATH_TO_LLVM "" CACHE PATH "Path to installed LLVM or LLVM source tree")
set(PATH_TO_BOOST "" CACHE PATH "Path to installed LLVM or LLVM source tree")

if (NOT PATH_TO_LLVM)
  message(FATAL_ERROR " 
  The cmake is supposed to be called with PATH_TO_LLVM pointing to
 a precompiled version of LLVM or to to the source code of LLVM
 Examples:
 cmake -G \"${CMAKE_GENERATOR}\" -DPATH_TO_LLVM=/opt/llvm-3.9.0 ${CMAKE_SOURCE_DIR}
 cmake -G \"${CMAKE_GENERATOR}\" -DPATH_TO_LLVM=/opt/llvm/source ${CMAKE_SOURCE_DIR}
")
endif()

if (NOT PATH_TO_BOOST)
  message(FATAL_ERROR " 
  The cmake is supposed to be called with PATH_TO_BOOST pointing to BOOST headers
 Examples:
 cmake -G \"${CMAKE_GENERATOR}\" -PATH_TO_BOOST=/opt/boost ${CMAKE_SOURCE_DIR}
")
endif()

set (llvm_search_paths
  ${PATH_TO_LLVM}
  ${PATH_TO_LLVM}/lib/cmake
  ${PATH_TO_LLVM}/lib/cmake/llvm
  ${PATH_TO_LLVM}/lib/cmake/clang
  ${PATH_TO_LLVM}/share/clang/cmake/
  ${PATH_TO_LLVM}/share/llvm/cmake/
)

find_package(LLVM REQUIRED CONFIG
             PATHS ${llvm_search_paths}
             NO_DEFAULT_PATH)

find_package(Clang REQUIRED CONFIG
             PATHS ${llvm_search_paths}
             NO_DEFAULT_PATH)

file(GLOB_RECURSE cpp_reflect_src "./*.cpp" "./*.h" "./.*hpp")

add_executable(cpp_reflect
   ${cpp_reflect_src}
  )

set_target_properties(cpp_reflect PROPERTIES 
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED YES
                        CXX_EXTENSIONS NO)

target_include_directories(cpp_reflect PUBLIC ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS} ${PATH_TO_BOOST})
target_link_directories(cpp_reflect PUBLIC ${PATH_TO_BOOST}/stage/lib)

target_link_libraries(cpp_reflect LLVMSupport 
                           clangTooling 
                           clangBasic 
                           clangASTMatchers)
