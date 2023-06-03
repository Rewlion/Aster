cmake_minimum_required(VERSION 3.21)


set(VFS_SRC
  "${CMAKE_SOURCE_DIR}/src/engine/vfs/vfs.cpp"
)

add_library(engine_vfs ${VFS_SRC})
