#include "template_component.h"

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

  TemplateComponent::TemplateComponent(const name_hash_t hash)
    : m_NameHash(hash)
  {
  }

  void TemplateComponent::destroy()
  {
    const TypeMeta* meta = getMeta();
    if (isBoxedType(meta->size))
    {
      meta->manager->destructor(as.ptr);
    }
    as.rawValue = 0;
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
    return m_Components.insert({id, TemplateComponent{id}}).first->second;
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

  TemplateComponentsMap::~TemplateComponentsMap()
  {
    for (auto& [_, c]: m_Components)
    {
      c.destroy();
    }
  }
}
