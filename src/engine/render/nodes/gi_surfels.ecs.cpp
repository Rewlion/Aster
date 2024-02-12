#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>
#include <engine/shaders/shaders/gi_surfels/lifetime.hlsl>
#include <engine/shaders/shaders/gi_surfels/meta.hlsl>


class GIOnSurfels
{
  public:
  void init()
  {
    initialized = false;
  }

  bool initialized;
};

ECS_COMP_GETTER(GIOnSurfels, state);

NODE_BEGIN(gibs_resources)
  CREATE_TEX_2D             (gibs_dbg_alloc, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))
  
  CREATE_GPU_BUF_PERSISTENT (gibs_surfels_lifetime, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                             BUF_SIZE(SURFEL_COUNT_TOTAL * sizeof(uint)))

  CREATE_GPU_BUF_PERSISTENT (gibs_surfels_storage, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                             BUF_SIZE(SURFEL_COUNT_TOTAL * sizeof(SurfelData)))
  
  CREATE_GPU_BUF_PERSISTENT (gibs_surfels_pool, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                             BUF_SIZE(SURFEL_COUNT_TOTAL * sizeof(uint)))

  CREATE_GPU_BUF            (gibs_surfels_allocation_locks, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(CELLS_COUNT * sizeof(uint)))

  CREATE_GPU_BUF            (gibs_surfels_spatial_storage, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(CELLS_COUNT * SPATIAL_STORAGE_CELL_PAYLOAD * sizeof(uint)))

  CREATE_GPU_BUF_PERSISTENT (gibs_surfels_meta, BUF_USAGE(UAV), BUF_STATE(UAV_RW), BUF_SIZE(sizeof(SurfelsMeta)))

  CREATE_TEX_2D             (gibs_surfels_coverage, TEX_SIZE_RELATIVE(), R32_UINT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_surfels_sdf, TEX_SIZE_RELATIVE(), R32_FLOAT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_surfels_allocation_pos, TEX_SIZE_RELATIVE(), R8_UINT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  EXEC(gibs_resources_init)
NODE_END()

NODE_EXEC()
static
void gibs_resources_init(gapi::CmdEncoder& encoder,
                         const gapi::TextureHandle gibs_dbg_alloc,
                         const gapi::BufferHandler gibs_surfels_lifetime,
                         const gapi::BufferHandler gibs_surfels_storage,
                         const gapi::BufferHandler gibs_surfels_pool,
                         const gapi::BufferHandler gibs_surfels_allocation_locks,
                         const gapi::BufferHandler gibs_surfels_spatial_storage,
                         const gapi::BufferHandler gibs_surfels_meta,
                         const gapi::TextureHandle gibs_surfels_coverage,
                         const gapi::TextureHandle gibs_surfels_sdf,
                         const gapi::TextureHandle gibs_surfels_allocation_pos)
{
  encoder.fillBuffer(gibs_surfels_allocation_locks, 0, CELLS_COUNT * sizeof(uint), 0);
  encoder.fillBuffer(gibs_surfels_spatial_storage, 0, CELLS_COUNT * (SURFEL_COUNT_PER_CELL+1) * sizeof(uint), 0);
  encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_UAV_RW);

  encoder.clearColorTexture(gibs_dbg_alloc,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_surfels_coverage,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_surfels_sdf,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_surfels_allocation_pos,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  GIOnSurfels& state = get_state();
  if (!state.initialized)
  {
    {
      encoder.fillBuffer(gibs_surfels_storage, 0, SURFEL_COUNT_TOTAL * sizeof(SurfelData), 0);
      encoder.fillBuffer(gibs_surfels_lifetime, 0, SURFEL_COUNT_TOTAL * sizeof(uint), SURFEL_STATE_UNUSED);
      encoder.fillBuffer(gibs_surfels_meta, 0, sizeof(SurfelsMeta), 0);
      encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_UAV_RW);
    }
    {
      tfx::set_extern("gibsSurfelsPool", gibs_surfels_pool);
      tfx::set_extern("gibsSurfelsCount", (uint)SURFEL_COUNT_TOTAL);
      tfx::activate_technique("GIBS_InitSurfelsPool", encoder);
      encoder.updateResources();
      
      const uint3 gc = tfx::calc_group_count("GIBS_InitSurfelsPool", uint3(SURFEL_COUNT_TOTAL,1,1));
      encoder.dispatch(gc.x, gc.y, gc.z);
    }

    state.initialized = true;
  }
}

NODE_BEGIN(gibs_spatial_storage_binning)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_spatial_storage, surfelsSpatialStorage)

  EXEC(gibs_spatial_storage_binning)  
NODE_END()
 
NODE_EXEC()
static
void gibs_spatial_storage_binning(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("GIBS_SpatialStorageBinning", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_SpatialStorageBinning", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}

NODE_BEGIN(gibs_surfels_coverage)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_coverage, surfelsCoverage)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_sdf, surfelsSDF)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_spatial_storage, surfelsSpatialStorage)

  EXEC(gibs_surfels_coverage)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_coverage(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  tfx::activate_technique("GIBS_SurfelsCoverage", encoder);
  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("GIBS_SurfelsCoverage", uint3(render_size, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}

NODE_BEGIN(gibs_surfels_find_surfels_alloc_pos)
  ORDER_ME_BEFORE(gibs_binning_sync)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_SRV_AS      (gibs_surfels_sdf, surfelsSDF)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_allocation_pos, surfelsAllocPos)
  EXEC(gibs_surfels_find_surfels_alloc_pos)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_find_surfels_alloc_pos(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  tfx::activate_technique("GIBS_FindSurfelsAllocPos", encoder);
  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("GIBS_FindSurfelsAllocPos", uint3(render_size, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}

NODE_BEGIN(gibs_binning_sync)
  RENAME_BUF(gibs_surfels_storage, gibs_surfels_storage_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_spatial_storage, gibs_surfels_spatial_storage_binned, BUF_STATE(SRV))
  NO_EXEC()
NODE_END()

NODE_BEGIN(gibs_allocate_surfels)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RW_UAV_AS   (gibs_dbg_alloc, dbgTex)
  BIND_TEX_SRV_AS      (gbuf1, gbuffer_normal)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_pool, surfelsPool)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_allocation_locks, surfelsAllocLocks)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_meta, surfelsMeta)
  BIND_TEX_SRV_AS      (gibs_surfels_coverage, surfelsCoverage)
  BIND_TEX_SRV_AS      (gibs_surfels_allocation_pos, surfelsAllocPos)

  ORDER_ME_BEFORE(gbuffer_resolve)

  EXEC(gibs_allocate_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_allocate_surfels(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  tfx::activate_technique("GIBS_AllocateSurfels", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_AllocateSurfels", uint3(render_size, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}

DECLARE_INITABLE_ECS_OBJECT_COMPONENT(GIOnSurfels);