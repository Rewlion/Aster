#include "window.h"

#include <engine/settings.h>
#include <engine/assert.h>

#include <Windows.h>

HWND CURRENT_WINDOW_HANDLER = 0;
Int2 window_size = {0,0};

void* GetHwnd()
{
  return reinterpret_cast<void*>(&CURRENT_WINDOW_HANDLER);
}

Int2 GetWindowSize()
{
  return window_size;
}

static bool SplitResolutionString(const String& str, unsigned int& width, unsigned int& height)
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

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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

  ASSERT(CURRENT_WINDOW_HANDLER != 0);
  ShowWindow(CURRENT_WINDOW_HANDLER, SW_SHOWDEFAULT);
}

void InitWindow()
{
  DataBlock* blk = ::GetAppSettings();
  DataBlock* graphicsBlk = blk->GetChildBlock("graphics");
  const String resolution = graphicsBlk->GetText("resolution");
  unsigned int width = 0;
  unsigned int height = 0;

  ASSERT(SplitResolutionString(resolution, width, height));

  InitWin32Window(width, height);

  window_size = Int2{width, height};
}

void PollWndMessages()
{
  MSG msg = { };
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
