#pragma once

namespace ecs
{
  template<class T>
  void TemplateComponent::operator=(T&& v) &
  {
    if (isBoxedType<T>())
    {
      using PtrType = typename std::remove_cvref<T>::type;
      as.ptr = new PtrType;
      *(PtrType*)as.ptr = std::move(v);
    }
    else
      std::memcpy(&as.rawValue, &v, sizeof(v));
    m_TypeId = CompileTypeId::from<T>();
  }

  template<class T>
  auto TemplateComponent::isBoxedType() -> bool
  {
    return isBoxedType(sizeof(T));
  }
}
