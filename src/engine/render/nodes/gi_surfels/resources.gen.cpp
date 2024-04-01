// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "resources.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_state_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("state", GIOnSurfels)
  },
  "query_state"};
const static query_id_t query_state_queryId = query_state_queryReg.getId();


void query_state (eastl::function<
  void(
    GIOnSurfels& state)> cb)
{
  ecs::get_registry().query(query_state_queryId, [&](ComponentsAccessor& accessor)
  {
    GIOnSurfels& state = accessor.get<GIOnSurfels>(compile_ecs_name_hash("state"));
    cb(state);
  });
}


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_resources(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_resources", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    reg.orderMeAfter("atm_sync_out");

    auto gibs_dbg_rt = reg.createTexture("gibs_dbg_rt",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto gibs_dbg_alloc = reg.createTexture("gibs_dbg_alloc",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto gibs_dbg_surfels = reg.createTexture("gibs_dbg_surfels",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_RT)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );

    auto gibs_surfels_lifetime = reg.createBuffer(
      "gibs_surfels_lifetime",
      gapi::BufferAllocationDescription{
        .size = (250 * 1024) * sizeof(uint),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      fg::PERSISTENT
    );
    auto gibs_surfels_storage = reg.createBuffer(
      "gibs_surfels_storage",
      gapi::BufferAllocationDescription{
        .size = (250 * 1024) * sizeof(SurfelData),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      fg::PERSISTENT
    );
    auto gibs_surfels_pool = reg.createBuffer(
      "gibs_surfels_pool",
      gapi::BufferAllocationDescription{
        .size = (250 * 1024) * sizeof(uint),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      fg::PERSISTENT
    );
    auto gibs_surfels_allocation_locks = reg.createBuffer(
      "gibs_surfels_allocation_locks",
      gapi::BufferAllocationDescription{
        .size = uint((6 + 1) * 21 * 21 * 21) * sizeof(uint),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      false
    );
    auto gibs_surfels_spatial_storage = reg.createBuffer(
      "gibs_surfels_spatial_storage",
      gapi::BufferAllocationDescription{
        .size = uint((6 + 1) * 21 * 21 * 21) * (1 + 127) * sizeof(uint),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      false
    );
    auto gibs_surfels_meta = reg.createBuffer(
      "gibs_surfels_meta",
      gapi::BufferAllocationDescription{
        .size = sizeof(SurfelsMeta),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      fg::PERSISTENT
    );
    auto gibs_nonlinear_aabbs = reg.createBuffer(
      "gibs_nonlinear_aabbs",
      gapi::BufferAllocationDescription{
        .size = uint(21 * 21 * 21) * sizeof(AABB),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      fg::PERSISTENT
    );

    auto gibs_surfels_sdf = reg.createTexture("gibs_surfels_sdf",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32_FLOAT,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto gibs_surfels_allocation_pos = reg.createTexture("gibs_surfels_allocation_pos",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R8_UINT,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );

    auto gibs_surfels_ray_budget = reg.createBuffer(
      "gibs_surfels_ray_budget",
      gapi::BufferAllocationDescription{
        .size = sizeof(int),
        .usage = gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible
      },
      gapi::BufferState::BF_STATE_UAV_RW,
      false
    );

    auto gibs_rayguiding_map = reg.createTexture("gibs_rayguiding_map",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(4096, 4096, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      },
      gapi::TextureState::ShaderReadWrite,
      fg::PERSISTENT
    );


    return [gibs_dbg_rt,gibs_dbg_alloc,gibs_dbg_surfels,gibs_surfels_lifetime,gibs_surfels_storage,gibs_surfels_pool,gibs_surfels_allocation_locks,gibs_surfels_spatial_storage,gibs_surfels_meta,gibs_nonlinear_aabbs,gibs_surfels_sdf,gibs_surfels_allocation_pos,gibs_surfels_ray_budget,gibs_rayguiding_map](gapi::CmdEncoder& encoder)
    {
      gibs_resources_init(encoder, gibs_dbg_rt.get(), gibs_dbg_alloc.get(), gibs_dbg_surfels.get(), gibs_surfels_lifetime.get(), gibs_surfels_storage.get(), gibs_surfels_pool.get(), gibs_surfels_allocation_locks.get(), gibs_surfels_spatial_storage.get(), gibs_surfels_meta.get(), gibs_nonlinear_aabbs.get(), gibs_surfels_sdf.get(), gibs_surfels_allocation_pos.get(), gibs_surfels_ray_budget.get(), gibs_rayguiding_map.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_resources_registration(
  mk_fg_node_gibs_resources,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_resources"
);