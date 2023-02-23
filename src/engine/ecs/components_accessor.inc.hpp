#pragma once

namespace ecs
{
  template<class T>
  auto ComponentsAccessor::get(const name_hash_t name) -> T&
  {
    const auto it = m_NameToCompMap.find(name);
    ASSERT_FMT(it != m_NameToCompMap.end(), "unknown component {}", name);
    std::byte* compData = m_EntityData + m_Offsets[it->second];
    return *reinterpret_cast<T*>(compData);
  }

  template<class T>
  auto ComponentsAccessor::get(const name_hash_t name, T&& defValue) -> T&
  {
    const auto it = m_NameToCompMap.find(name);
    if (it != m_NameToCompMap.end()) [[likely]]
    {
      std::byte* compData = m_EntityData + m_Offsets[it->second];
      return *reinterpret_cast<T*>(compData);
    }
    else
      return std::move(defValue);
  }
}
