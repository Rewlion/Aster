#include "components.h"

namespace Engine::ECS
{ 
  component_type_id ComponentTypeId::m_LastId = 0;
  
  ComponentMetaStorage meta_storage;

  void ComponentMetaStorage::init()
  {
    const size_t count = ComponentMetaRegistration::size();
    m_Metas.resize(count);

    ComponentMetaRegistration::enumerate([this](const ComponentMeta& meta){
      m_Metas[meta.typeId] = meta;
      m_NameToTypeId.insert(eastl::make_pair(
        str_hash(meta.typeName), meta.typeId
      ));

      loginfo("ecs: registered type {}", meta.typeName);
    });
  }

  const ComponentMeta& ComponentMetaStorage::getMeta(const string_view type) const
  {
    const auto it = m_NameToTypeId.find(str_hash(type));
    ASSERT_FMT(it != m_NameToTypeId.end(), "ecs: unknown component type `{}`", type);
    return m_Metas[it->second];
  }

  const ComponentMeta& ComponentMetaStorage::getMeta(const component_type_id type_id) const
  {
    return m_Metas[type_id];
  }

  void init_meta_storage()
  {
    meta_storage.init();
  }

  const ComponentMetaStorage& get_meta_storage()
  {
    return meta_storage;
  }

  ComponentMetaRegistration* ComponentMetaRegistration::m_List = nullptr;
  ComponentMetaRegistration::ComponentMetaRegistration(const ComponentMeta& meta)
  {
    m_Next = m_List;
    m_List = this;

    m_Id = m_Next ? m_Next->m_Id + 1 : 0;
    m_Meta = meta;
  }

  const ComponentMeta& get_component_meta(const string_view type)
  {
    return meta_storage.getMeta(type);
  }

  const ComponentMeta& get_component_meta(const component_type_id type_id)
  {
    return meta_storage.getMeta(type_id);
  }
  
  class StringTypeManager : public TypeManager
  {
    public:
      virtual void constructor(void* component) override
      {
        new (component) string("");
      }

      virtual void destructor(void* component) override
      {
        reinterpret_cast<string*>(component)->~string();
      }

      virtual void copy(void* __restrict__ to, const void* __restrict__ from) override
      {
        string& _to = *reinterpret_cast<string*>(to);
        const string& _from = *reinterpret_cast<const string*>(from);
        _to = _from;
      }
      
      virtual void move(void* __restrict__ to, void* __restrict__ from) override
      {
        string& _to = *reinterpret_cast<string*>(to);
        string& _from = *reinterpret_cast<string*>(from);
        _to = std::move(_from);
      }
  };

  DECLARE_TRIVIAL_ECS_COMPONENT(bool);
  DECLARE_TRIVIAL_ECS_COMPONENT(int);
  DECLARE_TRIVIAL_ECS_COMPONENT(int2);
  DECLARE_TRIVIAL_ECS_COMPONENT(int3);
  DECLARE_TRIVIAL_ECS_COMPONENT(int4);
  DECLARE_TRIVIAL_ECS_COMPONENT(float);
  DECLARE_TRIVIAL_ECS_COMPONENT(float2);
  DECLARE_TRIVIAL_ECS_COMPONENT(float3);
  DECLARE_TRIVIAL_ECS_COMPONENT(float4);
  DECLARE_TRIVIAL_ECS_COMPONENT(EntityId);

  DECLARE_ECS_COMPONENT(string, StringTypeManager, false, true);
}
