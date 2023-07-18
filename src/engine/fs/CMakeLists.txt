cmake_minimum_required(VERSION 3.21)


set(FS_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/fs/vfs.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/fs/file_watch.cpp"
)

add_library(engine_fs ${FS_SRC})
