#include "construct_sphere/sphere_app.h"
#include <engine/rendering/vulkan/core.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <iostream>


int main()
{
  const int wndHeight = 1080;
  const int wndWidth = 1024;

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow* wnd = glfwCreateWindow(wndWidth, wndHeight, "Vulkan window", nullptr, nullptr);

  uint32_t count;
  const char** extensions = glfwGetRequiredInstanceExtensions(&count);

  std::unique_ptr<Vulkan::Core> vkCore = std::make_unique<Vulkan::Core>(wnd, extensions, count, vk::Extent2D{ wndWidth, wndHeight });

  ApplicationBase* app = new SphereConstructDemo{ wnd, vkCore.get(), wndWidth, wndHeight };
  app->Start();
}