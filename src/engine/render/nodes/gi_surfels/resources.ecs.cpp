#include "dbg_flags.h"
#include "gi_state.h"

#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/meta.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>
#include <engine/shaders/shaders/intersection_structs.hlsl>
#include <engine/shaders/shaders/gi_surfels/lifetime.hlsl>

ECS_COMP_GETTER(GIOnSurfels, state);

auto import_surfels_lifetime() -> gapi::BufferViewWithState*
{
  if (auto* st = get_state())
    return st->getSurfelsLifetime();
  return nullptr;
}

auto import_surfels_meta() -> gapi::BufferViewWithState*
{
  if (auto* st = get_state())
    return st->getSurfelsLifetime();
  return nullptr;
}

auto import_surfels_storage() -> gapi::BufferViewWithState*
{
  if (auto* st = get_state())
    return st->getSurfelsStorage();
  return nullptr;
}

auto import_surfels_pool() -> gapi::BufferViewWithState*
{
  if (auto* st = get_state())
    return st->getSurfelsPool();
  return nullptr;
}

auto import_nonlinear_aabbs() -> gapi::BufferViewWithState*
{
  if (auto* st = get_state())
    return st->getNonlinearAabbs();
  return nullptr;
}

auto import_rayguiding_map() -> gapi::TextureViewWithState*
{
  if (auto* st = get_state())
    return st->getRayguidingMap();
  return nullptr;
}

NODE_BEGIN(gibs_resources_import)
  IMPORT_BUF                (gibs_surfels_lifetime, import_surfels_lifetime)
  IMPORT_BUF                (gibs_surfels_storage, import_surfels_storage)
  IMPORT_BUF                (gibs_surfels_pool, import_surfels_pool)
  IMPORT_BUF                (gibs_surfels_meta, import_surfels_meta)
  IMPORT_BUF                (gibs_nonlinear_aabbs, import_nonlinear_aabbs)
  IMPORT_TEX                (gibs_rayguiding_map, import_rayguiding_map)

  NO_EXEC()
NODE_END()

NODE_BEGIN(gibs_resources)
  ORDER_ME_AFTER (atm_sync_out)

  CREATE_TEX_2D             (gibs_dbg_rt, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_dbg_alloc, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_dbg_surfels, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                             TEX_USAGE2(SRV, RT), TEX_STATE(ShaderReadWrite))
  
  MODIFY_BUF_UAV            (gibs_surfels_lifetime)

  MODIFY_BUF_UAV            (gibs_surfels_storage)

  MODIFY_BUF_UAV            (gibs_surfels_pool)

  CREATE_GPU_BUF            (gibs_surfels_allocation_locks, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(CELLS_COUNT * sizeof(uint)))

  CREATE_GPU_BUF            (gibs_surfels_spatial_storage, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(CELLS_COUNT * SPATIAL_STORAGE_CELL_PAYLOAD * sizeof(uint)))

  MODIFY_BUF_UAV            (gibs_surfels_meta)
  
  MODIFY_BUF_UAV            (gibs_nonlinear_aabbs)

  CREATE_TEX_2D             (gibs_surfels_sdf, TEX_SIZE_RELATIVE(), R32_FLOAT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D             (gibs_surfels_allocation_pos, TEX_SIZE_RELATIVE(), R8_UINT,
                             TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))

  CREATE_GPU_BUF            (gibs_surfels_ray_budget, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                              BUF_SIZE(sizeof(int)))

  MODIFY_TEX_UAV            (gibs_rayguiding_map)

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

  GIOnSurfels* state = get_state();  
  if (!state->isInitialized() || gibs_force_reset)
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

    state->markInitialized();
  }
  
  //if (!state.initialized || gibs_force_reset)
  {
    tfx::set_extern("gibsNonlinearAABBs", gibs_nonlinear_aabbs);
    tfx::activate_technique("GIBS_PrepareNonLinearAABBs", encoder);
    encoder.updateResources();

    const uint3 gc = tfx::calc_group_count("GIBS_PrepareNonLinearAABBs", uint3(CELLS_DIM,CELLS_DIM,CELLS_DIM));
    encoder.dispatch(gc.x, gc.y, gc.z);
  }

  state->markInitialized();
  gibs_force_reset = false;
}