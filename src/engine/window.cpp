#include "window.h"

#include <engine/settings.h>
#include <engine/assert.h>

#include <EASTL/vector.h>

#include <Windows.h>

namespace Engine::Window
{
  static eastl::vector<IWndProcHandler*> wnd_proc_handlers;

  HWND CURRENT_WINDOW_HANDLER = 0;
  int2 window_size = {0,0};

  void reg_wnd_proc_handler(IWndProcHandler* h)
  {
    const auto it = eastl::find(wnd_proc_handlers.begin(), wnd_proc_handlers.end(), h);
    if (it != wnd_proc_handlers.end())
      logerror("trying to register already registered WndProcHandler");
    else
      wnd_proc_handlers.push_back(h);
  }

  void unreg_wnd_proc_handler(IWndProcHandler* h)
  {
    wnd_proc_handlers.erase(eastl::find(wnd_proc_handlers.begin(), wnd_proc_handlers.end(), h));
  }

  void* get_hwnd()
  {
    return reinterpret_cast<void*>(&CURRENT_WINDOW_HANDLER);
  }

  int2 get_window_size()
  {
    return window_size;
  }

  static bool SplitResolutionString(const string& str, unsigned int& width, unsigned int& height)
  {
    try
    {
      const auto d = str.find('x');
      if (d != std::string::npos && str.length() > 3)
      {
        const string widthStr = str.substr(0, d);
        const string heightStr = str.substr(d+1, str.length());

        width = std::stoul(widthStr);
        height = std::stoul(heightStr);

        return true;
      }
      else
        return false;
    }
    catch(...)
    {
      return false;
    }
  }

  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    UINT engineMsg = uMsg;
    switch (uMsg)
    {
      case WM_KEYDOWN:     engineMsg = WND_MSG_KEY_DOWN;                           break;
      case WM_KEYUP:       engineMsg = WND_MSG_KEY_UP;                             break;
      case WM_LBUTTONDOWN: engineMsg = WND_MSG_BMOUSE_DOWN;  wParam = KEY_LBMOUSE; break;
      case WM_LBUTTONUP:   engineMsg = WND_MSG_BMOUSE_UP;    wParam = KEY_LBMOUSE; break;
      case WM_RBUTTONDOWN: engineMsg = WND_MSG_BMOUSE_DOWN;  wParam = KEY_RBMOUSE; break;
      case WM_RBUTTONUP:   engineMsg = WND_MSG_BMOUSE_UP;    wParam = KEY_RBMOUSE; break;
      case WM_MBUTTONDOWN: engineMsg = WND_MSG_BMOUSE_DOWN;  wParam = KEY_MBMOUSE; break;
      case WM_MBUTTONUP:   engineMsg = WND_MSG_BMOUSE_UP;    wParam = KEY_MBMOUSE; break;
      case WM_MOUSEMOVE:   engineMsg = WND_MSG_MOUSE_MOVE;                         break;
      default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    for (auto* handler: wnd_proc_handlers)
      handler->handleWndEvent(hwnd, engineMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }

  static void InitWin32Window(const unsigned int width, const unsigned int height)
  {
    const LPCSTR CLASS_NAME = "Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    RECT rect;
    rect.left = 0;
    rect.right = width;
    rect.top = 0;
    rect.bottom = height;

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    CURRENT_WINDOW_HANDLER  = CreateWindowEx(
      0,                              // Optional window styles.
      CLASS_NAME,                     // Window class
      "",    // Window text
      WS_OVERLAPPEDWINDOW,            // Window style

      // Size and position
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      rect.right - rect.left,
      rect.bottom - rect.top,

      NULL,       // Parent window
      NULL,       // Menu
      GetModuleHandle(0),       //hinstance
      NULL        // Additional application data
    );

    ShowCursor(FALSE);

    ASSERT(CURRENT_WINDOW_HANDLER != 0);
    ShowWindow(CURRENT_WINDOW_HANDLER, SW_SHOWDEFAULT);
  }

  void init_window()
  {
    DataBlock* blk = Engine::get_app_settings();
    DataBlock* graphicsBlk = blk->getChildBlock("graphics");
    const string resolution = graphicsBlk->getText("resolution");
    unsigned int width = 0;
    unsigned int height = 0;

    ASSERT(SplitResolutionString(resolution, width, height));

    InitWin32Window(width, height);

    window_size = int2{width, height};
  }

  void poll_wnd_messages()
  {
    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  bool is_active_window()
  {
    return GetForegroundWindow() == CURRENT_WINDOW_HANDLER;
  }

  void show_cursor(const bool v)
  {
    if (v)
      while(ShowCursor(TRUE) <= 0);
    else
      while(ShowCursor(FALSE) >= 0);
  }
}