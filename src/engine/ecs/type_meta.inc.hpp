#pragma once

namespace ecs
{
  template<class T>
  auto CompileTypeId::from() -> component_type_id_t
  {
    return fromInternal<typename std::remove_cvref<T>::type>();
  }

  template<class T>
  auto CompileTypeId::fromInternal() -> component_type_id_t
  {
    static component_type_id_t id = component_type_id_t{m_LastId++};
    return id;
  }

  template<class T>
  void TypeMetaRegistration::enumerate(T cb)
  {
    const TypeMetaRegistration* p = m_List;
    while(p)
    {
      cb(p->m_Meta);
      p = p->m_Next;
    }
  }
}
