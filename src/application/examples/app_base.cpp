#include "app_base.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <ctime>

ApplicationBase::ApplicationBase(GLFWwindow* wnd, Vulkan::Core* vkCore, int wndWidth, int wndHeight)
  : m_Wnd(wnd)
  , m_VkCore(vkCore)
  , m_WndWidth(wndWidth)
  , m_WndHeight(wndHeight)
  , m_GUI(wnd, *vkCore)
{

}

void ApplicationBase::Start()
{
  std::clock_t t1 = std::clock();

  while (!glfwWindowShouldClose(m_Wnd))
  {
    const std::clock_t t2 = std::clock();
    const float dt = static_cast<float>(t2 - t1) / static_cast<float>(CLOCKS_PER_SEC);
    t1 = t2;

    glfwPollEvents();

    Dt(dt);
  }
}