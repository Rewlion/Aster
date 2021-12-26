#include "window.h"

#include <engine/settings.h>
#include <engine/assert.h>

#include <Windows.h>

HWND CURRENT_WINDOW_HANDLER = 0;

static bool split_resolution_string(const String& str, unsigned int& width, unsigned int& height)
{
  try
  {
    const auto d = str.find('x');
    if (d != std::string::npos && str.length() > 3)
    {
      const String widthStr = str.substr(0, d);
      const String heightStr = str.substr(d+1, str.length());

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

static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void init_win32_window(const unsigned int width, const unsigned int height)
{
  const LPCSTR CLASS_NAME = "Sample Window Class";

  WNDCLASS wc = { };
  wc.lpfnWndProc = window_proc;
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

  ASSERT(CURRENT_WINDOW_HANDLER != 0);
  ShowWindow(CURRENT_WINDOW_HANDLER, SW_SHOWDEFAULT);
}

void init_window()
{
  DataBlock* blk = ::get_app_settings();
  DataBlock* graphicsBlk = blk->get_child_block("graphics");
  const String resolution = graphicsBlk->get_text("resolution");
  unsigned int width = 0;
  unsigned int height = 0;

  ASSERT(split_resolution_string(resolution, width, height));

  init_win32_window(width, height);
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
