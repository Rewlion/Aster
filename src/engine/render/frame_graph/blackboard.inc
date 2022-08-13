#pragma once

namespace fg
{
  template<class T>
  size_t Blackboard::TypeIdHelper::getId()
  {
    static size_t id = lastId++;
    return id;
  }

  template<class T>
  T& Blackboard::add()
  {
    const size_t id = TypeIdHelper{}.getId<T>();
    ASSERT_FMT(m_Components.find(id) == m_Components.end(), "blackboard component already exist");

    m_Components.insert({
      id,
      T{}
    });

    return std::any_cast<T&>(m_Components.find(id)->second);
  }

  template<class T>
  T& Blackboard::get()
  {
    const size_t id = TypeIdHelper{}.getId<T>();
    const auto it = m_Components.find(id);
    ASSERT_FMT(it != m_Components.end(), "blackboard component does not exist");
    return std::any_cast<T&>(it->second);
  }
}
