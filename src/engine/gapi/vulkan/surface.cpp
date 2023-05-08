#ifdef PLATFORM_WIN64
  #include <engine/window.h>
#endif
#include <engine/gapi/vulkan/vulkan.h>
#include <engine/gapi/vulkan/result.h>

#include <Windows.h>

namespace gapi::vulkan
{
  vk::UniqueSurfaceKHR createPlatformSurface(vk::Instance& instance)
  {
    vk::UniqueSurfaceKHR surface;
  #ifdef PLATFORM_WIN64
    {
      HINSTANCE hInstance = GetModuleHandle(0);
      HWND* hwnd = (HWND*)Engine::Window::get_hwnd();

      const auto surfaceCreateInfo = vk::Win32SurfaceCreateInfoKHR()
        .setHwnd(*hwnd)
        .setHinstance(hInstance);

      auto res = instance.createWin32SurfaceKHRUnique(surfaceCreateInfo);
      VK_CHECK_RES(res);
      surface = std::move(res.value);
    }
  #else
    static_assert(!"unsupported platform for vulkan api");
  #endif

    return surface;
  }
}