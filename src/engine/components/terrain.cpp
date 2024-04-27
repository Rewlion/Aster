#include "terrain.h"

#include <engine/data/ed.h>
#include <engine/ecs/type_meta.h>

VTerrainComponent::VTerrainComponent(const ed::Scope* init)
{
  m_TerrainName = init->getVariableOr<string>("name", "");
  m_HeightmapMinMaxBorder = init->getVariableOr("heightmap_min_max_border", float2{0,1});
  m_HeightmapMaxHeight = init->getVariableOr("heightmap_max_height", 30.0f);
  m_PatchSideBits = init->getVariableOr("patch_side_bits", 5);
  m_WorldSize = init->getVariableOr("world_size_km", 1) * 1024;
  m_DetailSize = init->getVariableOr("detail_size", 256);
}

DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(VTerrainComponent);