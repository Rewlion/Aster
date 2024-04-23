#include "static_mesh.h"

#include <engine/data/ed.h>
#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/ecs/type_meta.h>

StaticMeshComponent::StaticMeshComponent(const ed::Scope* init)
  : TransformComponent(init)
{
  m_Model = init->getVariableOr<string>("model", "");
}

ECS_EVENT_SYSTEM()
static
void default_init_mesh_component(const ecs::OnEntityCreated& evt,
                                 const TransformComponent& root_tm,
                                 StaticMeshComponent& static_mesh)
{
  if (!static_mesh.hasAttachment())
    static_mesh.attachToSelfComponent(&root_tm);
}

DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(StaticMeshComponent, "StaticMeshComponent");