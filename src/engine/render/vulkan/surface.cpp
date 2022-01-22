#ifdef PLATFORM_WIN64
  #define VK_USE_PLATFORM_WIN32_KHR
  #include <engine/window.h>
#endif
#include <vulkan/vulkan.hpp>

#include <Windows.h>

namespace gapi::vulkan
{
  vk::UniqueSurfaceKHR createPlatformSurface(vk::Instance& instance)
  {
    vk::UniqueSurfaceKHR surface;
  #ifdef PLATFORM_WIN64
    {
      HINSTANCE hInstance = GetModuleHandle(0);
      HWND* hwnd = (HWND*)get_hwnd();

      const auto surfaceCreateInfo = vk::Win32SurfaceCreateInfoKHR()
        .setHwnd(*hwnd)
        .setHinstance(hInstance);

      surface = instance.createWin32SurfaceKHRUnique(surfaceCreateInfo);
    }
  #else
    static_assert(!"unsupported platform for vulkan api");
  #endif

    return surface;
  }
}