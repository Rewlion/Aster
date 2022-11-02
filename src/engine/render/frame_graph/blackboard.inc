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
  T& Blackboard::add(T&& value, const std::string_view name)
  {
    const ComponentTypeId typeId = TypeIdHelper{}.getId<T>();
    const ComponentId id = getComponentId(name);

    const auto typeIt = m_Components.find(typeId);
    if (typeIt == m_Components.end())
    {
      const auto newMapit = m_Components.insert({
        typeId,
        eastl::hash_map<ComponentId, std::any>{}
      });
      const auto compIt = newMapit.first->second.insert({
        id,
        std::forward<T>(value)
      });

      return std::any_cast<T&>(compIt.first->second);
    }
    else
    {
      const auto compIt = typeIt->second.find(id);
      ASSERT_FMT(compIt == typeIt->second.end(), " can't add new component [{}:{}] to the blackboard: it already exist", typeid(T).name(), name);

      const auto it = typeIt->second.insert({
        id,
        std::forward<T>(value)
      });
      return std::any_cast<T&>(it.first->second);
    }
  }

  template<class T>
  T& Blackboard::get(const std::string_view name)
  {
    const ComponentTypeId typeId = TypeIdHelper{}.getId<T>();
    const auto typeIt = m_Components.find(typeId);
    ASSERT_FMT(typeIt != m_Components.end(), "can't get a blackboard component [{}:{}]: it does not exist", typeid(T).name(), name);

    const ComponentId id = getComponentId(name);
    const auto it = typeIt->second.find(id);
    ASSERT_FMT(it != typeIt->second.end(), "can't get a blackboard component [{}:{}]: it does not exist", typeid(T).name(), name);

    return std::any_cast<T&>(it->second);
  }
}
