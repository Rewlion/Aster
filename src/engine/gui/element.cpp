#include "element.h"
#include "behavior.h"

namespace Engine::gui
{
  ElemBehavior::ElemBehavior(IBehavior* bhv, Element* elem)
    : m_Bhv(bhv)
    , m_Elem(elem)
  {
    m_Bhv->onInit(*m_Elem);
  }

  ElemBehavior::ElemBehavior(ElemBehavior&& rvl)
  {
    this->~ElemBehavior();
    std::swap(m_Bhv, rvl.m_Bhv);
    std::swap(m_Elem, rvl.m_Elem);
  }

  ElemBehavior::~ElemBehavior()
  {
    if (m_Bhv)
    {
      m_Bhv->onDestroy();
      m_Bhv = nullptr;
      m_Elem = nullptr;
    }
  }

}