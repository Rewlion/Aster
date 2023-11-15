cmake_minimum_required(VERSION 3.21)

SET(gapi_src 
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/backend.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/cache/pipelines_storage.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/cache/renderpass_storage.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/cache/shaders_storage.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/cmd_encoder.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/descriptors_set_manager.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/device.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/surface.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/swapchain.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/vma.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/vulkan.cpp"

  "${CMAKE_SOURCE_DIR}/src/engine/gapi/gapi.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/resource.cpp"
  "${CMAKE_SOURCE_DIR}/src/engine/gapi/resource_wrappers.cpp"
)

if (USE_TRACY)
  list(APPEND gapi_src "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/tracy.cpp")
else()
  list(APPEND gapi_src "${CMAKE_SOURCE_DIR}/src/engine/gapi/vulkan/tracy_stub.cpp")
endif()

add_library(engine_gapi ${gapi_src})
