#add_ecs_library
#it's basicly the same as add_library
#but also it generates final ecs sources for every ecs file
#usage:
# add_ecs_library(name FILES <file3.cpp> ECS_FILES <file1.ecs.cpp> <file2.ecs.cpp>)

function(add_ecs_library name)
  set(options "__no_options")
  set(oneValueArgs "__no_one_val_args")
  set(multiValueArgs ECS_FILES FILES)
  
  cmake_parse_arguments(PARSE_ARGV 1 ARG ${options} ${oneValueArgs} "${multiValueArgs}")
  if (NOT ARG_ECS_FILES)
    message(FATAL_ERROR "ECS_FILES is not passed")
  endif()
  
  #find invalid ecs file arguments
  #valid ecs file ends with .ecs.cpp
  set(invalid_ecs_src ${ARG_ECS_FILES})
  list(FILTER invalid_ecs_src EXCLUDE REGEX ".*[.][e][c][s][.][c][p][p]")
  
  if (invalid_ecs_src)
    message(FATAL_ERROR "ECS files has to be with extension `.ecs.cpp`.Invalid files: ${invalid_ecs_src}")
  endif()

  foreach(ecs_file IN LISTS ARG_ECS_FILES)
    if (NOT EXISTS ${ecs_file})
      message(FATAL_ERROR "${ecs_file} does not exist")
    endif()
  endforeach()

  #generate final ecs files
  #ecs.cpp => gen.cpp
  #this gen.cpp will include ecs.cpp itself
  #so this is the only file requires compilation
  set(generator_path "${CMAKE_SOURCE_DIR}/src/engine/ecs/codegen/ecs_generator.py")
  get_property(includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  set(includes_arg)
  foreach(inc IN LISTS includes)
    list(APPEND includes_arg "-I" ${inc})
  endforeach()

  set(defines_arg)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND defines_arg "-DDEBUG")
  endif()

  set(ecs_gen_list)
  foreach(ecs_file IN LISTS ARG_ECS_FILES)
    #save generated file name for further target
    string(REPLACE ".ecs.cpp" ".gen.cpp" gen_file ${ecs_file})
    list(APPEND ecs_gen_list ${gen_file})
    
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" src_arg "${ecs_file}")
    add_custom_command(
      OUTPUT ${gen_file}
      DEPENDS ${ecs_file}
      COMMAND python ${generator_path} ${defines_arg} ${includes_arg} ${src_arg}
      COMMENT "[ECS] generate ${ecs_file}"
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/"
    )
  endforeach()

  add_library(${name} OBJECT ${ecs_gen_list} ${ARG_FILES})
endfunction()
