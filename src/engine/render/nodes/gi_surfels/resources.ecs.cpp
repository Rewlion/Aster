#include "dbg_flags.h"
#include "gi_state.h"

#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/meta.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>
#include <engine/shaders/shaders/intersection_structs.hlsl>
#include <engine/shaders/shaders/gi_surfels/lifetime.hlsl>

ECS_COMP_GETTER(GIOnSurfels, state);

NODE_BEGIN(gibs_resources)
  ORDER_ME_AFTER (atm_sync_out)

  CREATE_TEX_2D             (gibs_dbg_rt, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_dbg_alloc, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_dbg_surfels, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, RT), TEX_STATE(ShaderReadWrite))
  
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
  
  CREATE_GPU_BUF_PERSISTENT (gibs_nonlinear_aabbs, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(CELLS_PER_CASCADE * sizeof(AABB)))

  CREATE_TEX_2D             (gibs_surfels_sdf, TEX_SIZE_RELATIVE(), R32_FLOAT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_surfels_allocation_pos, TEX_SIZE_RELATIVE(), R8_UINT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_GPU_BUF            (gibs_surfels_ray_budget, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(sizeof(int)))

  CREATE_TEX_2D_PERSISTENT  (gibs_rayguiding_map, TEX_SIZE(RAYGUIDE_TEX_DIM,RAYGUIDE_TEX_DIM,1), R32G32B32A32_S,
                               TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  EXEC(gibs_resources_init)
NODE_END()

NODE_EXEC()
static
void gibs_resources_init(gapi::CmdEncoder& encoder,
                         const gapi::TextureHandle gibs_dbg_rt,
                         const gapi::TextureHandle gibs_dbg_alloc,
                         const gapi::TextureHandle gibs_dbg_surfels,
                         const gapi::BufferHandler gibs_surfels_lifetime,
                         const gapi::BufferHandler gibs_surfels_storage,
                         const gapi::BufferHandler gibs_surfels_pool,
                         const gapi::BufferHandler gibs_surfels_allocation_locks,
                         const gapi::BufferHandler gibs_surfels_spatial_storage,
                         const gapi::BufferHandler gibs_surfels_meta,
                         const gapi::BufferHandler gibs_nonlinear_aabbs,
                         const gapi::TextureHandle gibs_surfels_sdf,
                         const gapi::TextureHandle gibs_surfels_allocation_pos,
                         const gapi::BufferHandler gibs_surfels_ray_budget,
                         const gapi::TextureHandle gibs_rayguiding_map)
{
  encoder.fillBuffer(gibs_surfels_allocation_locks, 0, CELLS_COUNT * sizeof(uint), 0);
  encoder.fillBuffer(gibs_surfels_spatial_storage, 0, CELLS_COUNT * SPATIAL_STORAGE_CELL_PAYLOAD * sizeof(uint), 0);
  encoder.fillBuffer(gibs_surfels_ray_budget, 0, sizeof(int), SURFEL_RAY_BUDGET);
  encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_UAV_RW);

  encoder.clearColorTexture(gibs_dbg_rt,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_dbg_alloc,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_dbg_surfels,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_surfels_sdf,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(gibs_surfels_allocation_pos,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  GIOnSurfels& state = get_state();
  if (!state.initialized || gibs_force_reset)
  {
    encoder.clearColorTexture(gibs_rayguiding_map,
      gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});
      
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
  
  //if (!state.initialized || gibs_force_reset)
  {
    tfx::set_extern("gibsNonlinearAABBs", gibs_nonlinear_aabbs);
    tfx::activate_technique("GIBS_PrepareNonLinearAABBs", encoder);
    encoder.updateResources();

    const uint3 gc = tfx::calc_group_count("GIBS_PrepareNonLinearAABBs", uint3(CELLS_DIM,CELLS_DIM,CELLS_DIM));
    encoder.dispatch(gc.x, gc.y, gc.z);
  }

  state.initialized = true;
  gibs_force_reset = false;
}