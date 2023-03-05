#include "imgui.h"

namespace Engine::Render
{
  ImGuiGlobalWindowRegistration* ImGuiGlobalWindowRegistration::m_List = nullptr;
  ImGuiGlobalWindowRegistration::ImGuiGlobalWindowRegistration(const char* name,
                                                               DrawCb cb,
                                                               ImGuiWindowFlags flags)
  {
    m_Next = m_List;
    m_List = this;

    m_Name = name;
    m_Cb = cb;
    m_Flags = flags;
  }

  void ImGuiGlobalWindowRegistration::drawAllWindows()
  {
    const ImGuiGlobalWindowRegistration* p = m_List;
    while(p)
    {
      ImGui::Begin(p->m_Name, nullptr, p->m_Flags);
      p->m_Cb();
      ImGui::End();

      p = p->m_Next;
    }
  }
}
