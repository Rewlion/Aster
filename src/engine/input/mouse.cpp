#include "mouse.h"

#include <engine/window.h>
#include <engine/log.h>

#include <Windows.h>
#include <iostream>

namespace Engine::Input
{
  bool Mouse::getAnalogStatus(const string_hash action, float2& delta) const
  {
    if (m_ActiveSet.has_value())
    {
      const auto it = m_Mappings.find(*m_ActiveSet);
      if (it != m_Mappings.end())
        for (const auto& am: it->second.analogs)
          if (am.action == action && am.device == 0)
          {
            delta = am.absoluteMouse ? m_CurrentPosition : m_Delta;
            return true;
          }
    }

    delta = {0.0f, 0.0f};
    return false;
  }

  void Mouse::processAnalogInput()
  {
    HWND hwnd = *(HWND*)Window::get_hwnd();
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hwnd, &point);

    static bool isFirstInit = true;
    const int2 wndSize = Window::get_window_size();

    if (isFirstInit)
    {
      m_Delta = float2(0,0);
      isFirstInit = false;
    }
    if (point.x < 0 || point.y < 0 || point.x > wndSize.x || point.y > wndSize.y)
      m_Delta = float2(0,0);
    else
      m_Delta = float2(point.x, point.y) - m_CurrentPosition;

    m_CurrentPosition = float2(point.x, point.y);
  }
}
