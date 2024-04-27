#include "gi_state.h"

#include <engine/ecs/type_meta.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/meta.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>
#include <engine/shaders/shaders/intersection_structs.hlsl>

void GIOnSurfels::init()
{
  m_Initialized = false;
  
  m_SurfelsLifetime = {
    gapi::allocate_buffer(SURFEL_COUNT_TOTAL * sizeof(uint), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "gibs_surfels_lifetime"),
    gapi::BufferState::BF_STATE_UAV_RW
  };
  
  m_SurfelsStorage = {
    gapi::allocate_buffer(SURFEL_COUNT_TOTAL * sizeof(SurfelData), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "gibs_surfels_storage"),
    gapi::BufferState::BF_STATE_UAV_RW
  };

  m_SurfelsPool = {
    gapi::allocate_buffer(SURFEL_COUNT_TOTAL * sizeof(uint), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "gibs_surfels_storage"),
    gapi::BufferState::BF_STATE_UAV_RW
  };

  m_SurfelsMeta = {
    gapi::allocate_buffer(sizeof(SurfelsMeta), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "gibs_surfels_meta"),
    gapi::BufferState::BF_STATE_UAV_RW
  };

  m_NonlinearAabbs = {
    gapi::allocate_buffer(CELLS_PER_CASCADE * sizeof(AABB), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "gibs_surfels_meta"),
    gapi::BufferState::BF_STATE_UAV_RW
  };

  m_RayguidingMap = {
    gapi::allocate_texture(gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(RAYGUIDE_TEX_DIM,RAYGUIDE_TEX_DIM,1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV),
        .name = "gibs_rayguiding_map"
      }),
    gapi::TextureState::Undefined
  };
}

DECLARE_INITABLE_ECS_OBJECT_COMPONENT(GIOnSurfels);
