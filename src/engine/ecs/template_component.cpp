#include "template_component.h"

#include <EASTL/utility.h>

namespace ecs
{
  auto TemplateComponent::getMeta() const -> const TypeMeta*
  {
    const TypeMeta* meta = get_meta_storage().getMeta(m_TypeId);
    if (meta) [[likely]]
      return meta;
    else
      return nullptr;
  }

  auto TemplateComponent::getNameHash() const -> name_hash_t
  {
    return m_NameHash;
  }

  TemplateComponent::TemplateComponent()
    : m_TypeId(INVALID_COMPONENT_TYPE_ID)
    , m_NameHash(0)
  {
    as.rawValue = 0;
  }

  TemplateComponent::TemplateComponent(const name_hash_t hash)
    : m_TypeId(INVALID_COMPONENT_TYPE_ID)
    , m_NameHash(hash)
  {
    as.rawValue = 0;
  }

  TemplateComponent::TemplateComponent(TemplateComponent&& rvl)
  {
    as.rawValue = 0;
    m_TypeId = INVALID_COMPONENT_TYPE_ID;
    m_NameHash = 0;

    std::swap(as.rawValue, rvl.as.rawValue);
    std::swap(m_TypeId, rvl.m_TypeId);
    std::swap(m_NameHash, rvl.m_NameHash);
  }

  TemplateComponent::~TemplateComponent()
  {
    if (m_TypeId != INVALID_COMPONENT_TYPE_ID)
    {
      const TypeMeta* meta = getMeta();
      if (isBoxedType(meta->size))
      {
        meta->manager->destructor(as.ptr);
      }

      as.rawValue = 0;
      m_TypeId = INVALID_COMPONENT_TYPE_ID;
      m_NameHash = 0;
    }
  }

  TemplateComponent& TemplateComponent::operator=(TemplateComponent&& rvl)
  {
    this->~TemplateComponent();
    std::swap(as.rawValue, rvl.as.rawValue);
    std::swap(m_TypeId, rvl.m_TypeId);
    std::swap(m_NameHash, rvl.m_NameHash);

    return *this;
  }

  auto TemplateComponent::isBoxedType(const size_t size) -> bool
  {
    return size > sizeof(as.rawValue);
  }

  auto TemplateComponent::getData() const -> const void*
  {
    return isBoxedType(getMeta()->size) ? as.ptr : &as.rawValue;
  }

  auto TemplateComponentsMap::operator[] (const string_view name) -> TemplateComponent&
  {
    const name_hash_t id = ecs_name_hash(name);
    m_Names.insert(eastl::make_pair(id, string{name}));
    return m_Components.insert(eastl::make_pair(id, TemplateComponent{id})).first->second;
  }

  auto TemplateComponentsMap::getComponent(const name_hash_t name) const -> const TemplateComponent&
  {
    return m_Components.find(name)->second;
  }

  auto TemplateComponentsMap::getMap() const -> const eastl::vector_map<name_hash_t, TemplateComponent>&
  {
    return m_Components;
  }

  auto TemplateComponentsMap::getComponentName(const name_hash_t name) const -> string_view
  {
    return m_Names.find(name)->second;
  }
}
