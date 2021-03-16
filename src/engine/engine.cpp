#include "engine.h"

#include <engine/assets/asset_storage.h>
#include <engine/rendering/renderer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <ctime>

Engine::Engine(const Settings& settings)
  : settings(settings)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  if (settings.window.isFullscreen)
  {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    this->wnd = glfwCreateWindow(mode->width, mode->height, "Vulkan window", monitor, nullptr);
  }
  else
  {
    this->wnd = glfwCreateWindow(settings.window.width, settings.window.height, "Vulkan window", nullptr, nullptr);
  }

  uint32_t count;
  const char** extensions = glfwGetRequiredInstanceExtensions(&count);

  vkCore = std::make_unique<Vulkan::Core>(wnd, extensions, count, vk::Extent2D{ settings.window.width, settings.window.height });
  assetStorage = std::make_unique<AssetStorage>(*vkCore);
  inputHandler = std::make_unique<InputHandler>(wnd);

  ecsContext.SetUserData(this);
  ecsContext.AddLogicSystems({
    new RenderSystem{&ecsContext,*vkCore}
  });
}

Engine::~Engine()
{
  glfwDestroyWindow(wnd);
  glfwTerminate();
}

void Engine::Start()
{
  ecsContext.RunInitializationSystems();

  clock_t t1 = clock();
  while (!glfwWindowShouldClose(wnd)) {
    const clock_t t2 = clock();
    const float dt = static_cast<float>(t2 - t1) / static_cast<float>(CLOCKS_PER_SEC);
    t1 = t2;

    ecsContext.UpdateSystems(dt);
  }
}