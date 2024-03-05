#include <engine/console/cmd.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/imgui/imgui.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/lifetime.hlsl>
#include <engine/shaders/shaders/gi_surfels/meta.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>
#include <engine/shaders/shaders/intersection_structs.hlsl>

constexpr uint64_t SLOW_ALLOC_PERIOD_MS = 150;
static uint next_slow_alloc_time_ms = 0;
static bool slow_allocation = false;
static bool transform_surfels = true;
static bool freeze_allocation = false;
static bool show_dbg_wnd = false;
static bool show_surfels = false;
static bool show_surfels_sdf = false;
static bool show_surfels_coverage = false;
static bool show_cells_coverage = false;
static bool show_cells = false;
static bool show_surfels_irradiance = false;
static bool force_reset = false;

static
void show_dbg_wnd_cb(eastl::span<string_view> args)
{
  show_dbg_wnd = !show_dbg_wnd;
}

static
void imgui_dbg_wnd()
{
  if (!show_dbg_wnd)
    return;

  const auto showtex = [](string_view tex_name, bool& toggle)
  {
    extern void show_tex(eastl::span<string_view> args);
    toggle = !toggle;
    if (toggle)
      show_tex(eastl::span<string_view>(&tex_name, 1));
    else
      show_tex({});
  };

  ImGui::Begin("gibs_dbg", nullptr, 0);
  ImGui::Checkbox("freeze surfels allocation", &freeze_allocation);
  ImGui::Checkbox("transform surfels", &transform_surfels);
  ImGui::Checkbox("slow surfels allocation", &slow_allocation);

  if (ImGui::Button("show surfels", ImVec2(200, 50)))
    showtex("gibs_dbg_surfels", show_surfels);

  {
    const char* showSurfelsOptions[] = {"base", "sdf", "surfels_coverage", "cells_coverage", "cells", "irradiance"};
    static const char* curOpt = showSurfelsOptions[0];
    bool blob = false;
    bool* values[] = {&blob, &show_surfels_sdf, &show_surfels_coverage, &show_cells_coverage, &show_cells, &show_surfels_irradiance};
    if (ImGui::BeginCombo("format", curOpt))
    {
      for (int i = 0; i < IM_ARRAYSIZE(showSurfelsOptions); ++i)
      {
        bool isSelected = curOpt == showSurfelsOptions[i];
        if (ImGui::Selectable(showSurfelsOptions[i], isSelected))
        {
          curOpt = showSurfelsOptions[i];
          isSelected = true;
        }

        *values[i] = isSelected;
      }
      ImGui::EndCombo();
    }
  }

  if (ImGui::Button("reset surfels", ImVec2(100, 25)))
    force_reset = true;

  ImGui::End();
}

CONSOLE_CMD("render.gibs", 0, 0, show_dbg_wnd_cb);
IMGUI_REG_WND(imgui_dbg_wnd);

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
                         const gapi::TextureHandle gibs_surfels_allocation_pos)
{
  encoder.fillBuffer(gibs_surfels_allocation_locks, 0, CELLS_COUNT * sizeof(uint), 0);
  encoder.fillBuffer(gibs_surfels_spatial_storage, 0, CELLS_COUNT * SPATIAL_STORAGE_CELL_PAYLOAD * sizeof(uint), 0);
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
  if (!state.initialized || force_reset)
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
  
  //if (!state.initialized || force_reset)
  {
    tfx::set_extern("gibsNonlinearAABBs", gibs_nonlinear_aabbs);
    tfx::activate_technique("GIBS_PrepareNonLinearAABBs", encoder);
    encoder.updateResources();

    const uint3 gc = tfx::calc_group_count("GIBS_PrepareNonLinearAABBs", uint3(CELLS_DIM,CELLS_DIM,CELLS_DIM));
    encoder.dispatch(gc.x, gc.y, gc.z);
  }

  state.initialized = true;
  force_reset = false;
}

NODE_BEGIN(gibs_surfels_recycle)
  ORDER_ME_BEFORE(gibs_spatial_storage_binning)

  BIND_BUF_RW_UAV_AS (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_RW_UAV_AS (gibs_surfels_pool, surfelsPool)
  BIND_BUF_RW_UAV_AS (gibs_surfels_meta, surfelsMeta)

  EXEC(gibs_surfels_recycle)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_recycle(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("GIBS_SurfelsRecycle", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_SurfelsRecycle", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}

NODE_BEGIN(gibs_spatial_storage_binning)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_spatial_storage, surfelsSpatialStorage)
  BIND_BUF_SRV_AS      (gibs_nonlinear_aabbs, gibsNonlinearAABBs)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_pool, surfelsPool)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_meta, surfelsMeta)

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

NODE_BEGIN(gibs_surfels_find_surfels_alloc_pos)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_sdf, surfelsSDF)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_spatial_storage, surfelsSpatialStorage)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_allocation_pos, surfelsAllocPos)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)

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

NODE_BEGIN(gibs_draw_surfels)
  ORDER_ME_BEFORE(gibs_binning_sync)

  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_spatial_storage, surfelsSpatialStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_meta, surfelsMeta)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(gibs_dbg_surfels)
  RP_END()

  EXEC(gibs_draw_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_draw_surfels(gapi::CmdEncoder& encoder)
{
  tfx::set_extern("showSurfelsCoverage", show_surfels_coverage ? (uint)1 : (uint)0);
  tfx::set_extern("showSurfelSDF", show_surfels_sdf ? (uint)1 : (uint)0);
  tfx::set_extern("showCellsCoverage", show_cells_coverage ? (uint)1 : (uint)0);
  tfx::set_extern("showCells", show_cells ? (uint)1 : (uint)0);
  tfx::set_extern("showSurfelsIrradiance", show_surfels_irradiance ? (uint)1:(uint)0);
  tfx::activate_technique("GIBS_DrawSurfels", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(gibs_binning_sync)
  RENAME_BUF(gibs_surfels_storage, gibs_surfels_storage_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_spatial_storage, gibs_surfels_spatial_storage_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_meta, gibs_surfels_meta_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_pool, gibs_surfels_pool_binned, BUF_STATE(UAV_RW))
  NO_EXEC()
NODE_END()

NODE_BEGIN(gibs_allocate_surfels)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RW_UAV_AS   (gibs_dbg_alloc, dbgTex)
  BIND_TEX_SRV_AS      (gbuf1, gbuffer_normal)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_pool_binned, surfelsPool)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_allocation_locks, surfelsAllocLocks)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_meta_binned, surfelsMeta)
  BIND_TEX_SRV_AS      (gibs_surfels_allocation_pos, surfelsAllocPos)

  ORDER_ME_BEFORE(gbuffer_resolve)

  EXEC(gibs_allocate_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_allocate_surfels(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  if (freeze_allocation)
    return;

  if (slow_allocation)
  {
    uint64_t curMs = Engine::Time::get_time_ms_since_start();
    if (curMs >= next_slow_alloc_time_ms)
      next_slow_alloc_time_ms = curMs + SLOW_ALLOC_PERIOD_MS;
    else
      return;
  }

  tfx::activate_technique("GIBS_AllocateSurfels", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_AllocateSurfels", uint3(render_size, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}

NODE_BEGIN(gibs_transform_surfels)
  ORDER_ME_BEFORE(gibs_sync_out)
  BIND_BUF_RW_UAV_AS(gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_SRV_AS   (gibs_surfels_lifetime, surfelsLifeTime)

  EXEC(gibs_transform_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_transform_surfels(gapi::CmdEncoder& encoder)
{
  if (!transform_surfels)
    return;

  tfx::activate_technique("GIBS_TransformSurfels", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_TransformSurfels", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}

NODE_BEGIN(gibs_surfels_irradiance)
  ORDER_ME_BEFORE(gibs_sync_out)
  BIND_BUF_RW_UAV_AS(gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_SRV_AS   (gibs_surfels_spatial_storage_binned, surfelsSpatialStorage)
  BIND_TEX_SRV_AS   (atm_sky_lut, skyLUT)

  EXEC(gibs_surfels_irradiance)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_irradiance(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("GIBS_AccSurfelsIrradiance", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_AccSurfelsIrradiance", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}

NODE_BEGIN(gibs_test_rt)
  ORDER_ME_BEFORE(gibs_sync_out)

  READ_RENDER_SIZE_AS (render_size)
  BIND_TEX_RW_UAV_AS  (gibs_dbg_rt, dbgOutput)
  
  EXEC(gibs_test_rt)
NODE_END()

NODE_EXEC()
static
void gibs_test_rt(gapi::CmdEncoder& encoder, const uint2& render_size)
{
  tfx::set_extern("renderSize", render_size);

  tfx::activate_technique("RT_TEST", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("RT_TEST", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, gc.z);
}

NODE_BEGIN(gibs_sync_out)
  NO_EXEC()

  ORDER_ME_BEFORE(gbuffer_resolve)
NODE_END()

DECLARE_INITABLE_ECS_OBJECT_COMPONENT(GIOnSurfels);