#include "type_meta.h"
#include "eid.h"
#include "hash.h"

#include <engine/algorithm/hash.h>

namespace ecs
{ 
  component_type_id_t CompileTypeId::m_LastId{0};
  
  TypeMetaStorage meta_storage;

  void TypeMetaStorage::init()
  {
    const size_t count = TypeMetaRegistration::size();
    m_Metas.resize(count);

    TypeMetaRegistration::enumerate([this](const TypeMeta& meta){
      m_Metas[meta.typeId] = meta;
      m_NameToTypeId.insert(eastl::make_pair(
        ecs_name_hash(meta.typeName), meta.typeId
      ));

      loginfo("ecs: registered type {}", meta.typeName);
    });
  }

  auto TypeMetaStorage::getMeta(const component_type_id_t type_id) const -> const TypeMeta* 
  {
    if ((size_t)type_id < m_Metas.size()) [[likely]]
      return &m_Metas[type_id];
    else
      return nullptr;
  }

  auto TypeMetaStorage::getMeta(const string_view type_name) const -> const TypeMeta*
  {
    const auto it = m_NameToTypeId.find(ecs_name_hash(type_name));
    return it != m_NameToTypeId.end() ? &m_Metas[it->second] : nullptr;
  }

  void init_meta_storage()
  {
    meta_storage.init();
  }

  auto get_meta_storage() -> const TypeMetaStorage&
  {
    return meta_storage;
  }

  TypeMetaRegistration* TypeMetaRegistration::m_List = nullptr;
  TypeMetaRegistration::TypeMetaRegistration(const TypeMeta& meta)
  {
    m_Next = m_List;
    m_List = this;

    m_Id = m_Next ? m_Next->m_Id + 1 : 0;
    m_Meta = meta;
  }

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

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(string);
}
