#include "decal.h"

#include <engine/data/ed.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>
#include <engine/ecs/macros.h>

DecalComponent::DecalComponent(const ed::Scope* init)
  : TransformComponent(init)
{
  m_Name = init->getVariableOr<string>("name", "");
}

DecalComponent::DecalComponent(const string_view decal_name, const float3& pos, const float3& scale)
  : TransformComponent(pos, float3{0,0,0}, scale)
  , m_Name(decal_name)
{
}

ECS_EVENT_SYSTEM()
static
void default_init_decal_component(const ecs::OnEntityCreated& evt,
                                 const TransformComponent& root_tm,
                                 DecalComponent& decal)
{
  if (!decal.hasAttachment())
    decal.attachToSelfComponent(&root_tm);
}

DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(DecalComponent);