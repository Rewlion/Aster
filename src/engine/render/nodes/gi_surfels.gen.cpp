// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "gi_surfels.ecs.cpp" 

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
        .size = uint((6 + 1) * 21 * 21 * 21) * (1 + 30) * sizeof(uint),
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


    return [gibs_dbg_alloc,gibs_dbg_surfels,gibs_surfels_lifetime,gibs_surfels_storage,gibs_surfels_pool,gibs_surfels_allocation_locks,gibs_surfels_spatial_storage,gibs_surfels_meta,gibs_nonlinear_aabbs,gibs_surfels_sdf,gibs_surfels_allocation_pos](gapi::CmdEncoder& encoder)
    {
      gibs_resources_init(encoder, gibs_dbg_alloc.get(), gibs_dbg_surfels.get(), gibs_surfels_lifetime.get(), gibs_surfels_storage.get(), gibs_surfels_pool.get(), gibs_surfels_allocation_locks.get(), gibs_surfels_spatial_storage.get(), gibs_surfels_meta.get(), gibs_nonlinear_aabbs.get(), gibs_surfels_sdf.get(), gibs_surfels_allocation_pos.get());
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


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_spatial_storage_binning(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_spatial_storage_binning", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto surfelsLifeTime = reg.modifyBuffer("gibs_surfels_lifetime", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsStorage = reg.readBuffer("gibs_surfels_storage", gapi::BufferState::BF_STATE_SRV, false);
    auto surfelsSpatialStorage = reg.modifyBuffer("gibs_surfels_spatial_storage", gapi::BufferState::BF_STATE_UAV_RW);
    auto gibsNonlinearAABBs = reg.readBuffer("gibs_nonlinear_aabbs", gapi::BufferState::BF_STATE_SRV, false);

    return [surfelsLifeTime,surfelsStorage,surfelsSpatialStorage,gibsNonlinearAABBs](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("surfelsLifeTime", surfelsLifeTime.get());
      tfx::set_extern("surfelsStorage", surfelsStorage.get());
      tfx::set_extern("surfelsSpatialStorage", surfelsSpatialStorage.get());
      tfx::set_extern("gibsNonlinearAABBs", gibsNonlinearAABBs.get());
      gibs_spatial_storage_binning(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_spatial_storage_binning_registration(
  mk_fg_node_gibs_spatial_storage_binning,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_spatial_storage_binning"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_surfels_find_surfels_alloc_pos(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_surfels_find_surfels_alloc_pos", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto gbuffer_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto surfelsSDF = reg.modifyTexture("gibs_surfels_sdf", gapi::TextureState::ShaderReadWrite);
    auto surfelsStorage = reg.readBuffer("gibs_surfels_storage", gapi::BufferState::BF_STATE_SRV, false);
    auto surfelsSpatialStorage = reg.readBuffer("gibs_surfels_spatial_storage", gapi::BufferState::BF_STATE_SRV, false);
    auto surfelsAllocPos = reg.modifyTexture("gibs_surfels_allocation_pos", gapi::TextureState::ShaderReadWrite);

    return [gbuffer_depth,surfelsSDF,surfelsStorage,surfelsSpatialStorage,surfelsAllocPos,render_size](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      tfx::set_extern("surfelsSDF", surfelsSDF.get());
      tfx::set_extern("surfelsStorage", surfelsStorage.get());
      tfx::set_extern("surfelsSpatialStorage", surfelsSpatialStorage.get());
      tfx::set_extern("surfelsAllocPos", surfelsAllocPos.get());
      gibs_surfels_find_surfels_alloc_pos(encoder, render_size.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_surfels_find_surfels_alloc_pos_registration(
  mk_fg_node_gibs_surfels_find_surfels_alloc_pos,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_surfels_find_surfels_alloc_pos"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_draw_surfels(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_draw_surfels", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("gibs_binning_sync");
    auto gbuffer_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto surfelsStorage = reg.readBuffer("gibs_surfels_storage", gapi::BufferState::BF_STATE_SRV, false);
    auto surfelsSpatialStorage = reg.readBuffer("gibs_surfels_spatial_storage", gapi::BufferState::BF_STATE_SRV, false);
    reg.requestRenderPass()
      .addTarget("gibs_dbg_surfels", gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [gbuffer_depth,surfelsStorage,surfelsSpatialStorage](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      tfx::set_extern("surfelsStorage", surfelsStorage.get());
      tfx::set_extern("surfelsSpatialStorage", surfelsSpatialStorage.get());
      gibs_draw_surfels(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_draw_surfels_registration(
  mk_fg_node_gibs_draw_surfels,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_draw_surfels"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_binning_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_binning_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto gibs_surfels_storage_binned = reg.renameBuffer("gibs_surfels_storage", "gibs_surfels_storage_binned", gapi::BufferState::BF_STATE_SRV);
    auto gibs_surfels_spatial_storage_binned = reg.renameBuffer("gibs_surfels_spatial_storage", "gibs_surfels_spatial_storage_binned", gapi::BufferState::BF_STATE_SRV);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_gibs_binning_sync_registration(
  mk_fg_node_gibs_binning_sync,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_binning_sync"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_allocate_surfels(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_allocate_surfels", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto dbgTex = reg.modifyTexture("gibs_dbg_alloc", gapi::TextureState::ShaderReadWrite);
    auto gbuffer_normal = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead, false);
    auto gbuffer_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto surfelsLifeTime = reg.modifyBuffer("gibs_surfels_lifetime", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsStorage = reg.modifyBuffer("gibs_surfels_storage_binned", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsPool = reg.modifyBuffer("gibs_surfels_pool", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsAllocLocks = reg.modifyBuffer("gibs_surfels_allocation_locks", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsMeta = reg.modifyBuffer("gibs_surfels_meta", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsAllocPos = reg.readTexture("gibs_surfels_allocation_pos", gapi::TextureState::ShaderRead, false);
    reg.orderMeBefore("gbuffer_resolve");

    return [dbgTex,gbuffer_normal,gbuffer_depth,surfelsLifeTime,surfelsStorage,surfelsPool,surfelsAllocLocks,surfelsMeta,surfelsAllocPos,render_size](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("dbgTex", dbgTex.get());
      tfx::set_extern("gbuffer_normal", gbuffer_normal.get());
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      tfx::set_extern("surfelsLifeTime", surfelsLifeTime.get());
      tfx::set_extern("surfelsStorage", surfelsStorage.get());
      tfx::set_extern("surfelsPool", surfelsPool.get());
      tfx::set_extern("surfelsAllocLocks", surfelsAllocLocks.get());
      tfx::set_extern("surfelsMeta", surfelsMeta.get());
      tfx::set_extern("surfelsAllocPos", surfelsAllocPos.get());
      gibs_allocate_surfels(encoder, render_size.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_allocate_surfels_registration(
  mk_fg_node_gibs_allocate_surfels,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_allocate_surfels"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_transform_surfels(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_transform_surfels", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("gibs_sync_out");
    auto surfelsStorage = reg.modifyBuffer("gibs_surfels_storage_binned", gapi::BufferState::BF_STATE_UAV_RW);
    auto surfelsLifeTime = reg.readBuffer("gibs_surfels_lifetime", gapi::BufferState::BF_STATE_SRV, false);

    return [surfelsStorage,surfelsLifeTime](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("surfelsStorage", surfelsStorage.get());
      tfx::set_extern("surfelsLifeTime", surfelsLifeTime.get());
      gibs_transform_surfels(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_transform_surfels_registration(
  mk_fg_node_gibs_transform_surfels,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_transform_surfels"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_sync_out(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_sync_out", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("gbuffer_resolve");
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_gibs_sync_out_registration(
  mk_fg_node_gibs_sync_out,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_sync_out"
);
