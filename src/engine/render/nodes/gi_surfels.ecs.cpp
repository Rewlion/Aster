#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>
#include <engine/shaders/shaders/gi_surfels/surfel.hlsl>


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
  CREATE_TEX_2D (gibs_dbg_alloc, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                 TEX_USAGE2(SRV, UAV), TEX_STATE(ShaderReadWrite))
  
  CREATE_GPU_BUF (gibs_surfels_storage, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                   BUF_SIZE(SURFEL_COUNT_TOTAL * sizeof(SurfelData)))
  
  CREATE_GPU_BUF (gibs_surfels_pool, BUF_USAGE(UAV), BUF_STATE(UAV_RW),
                   BUF_SIZE(SURFEL_COUNT_TOTAL * sizeof(uint)))

  EXEC(gibs_resources_init)
NODE_END()

NODE_EXEC()
static
void gibs_resources_init(gapi::CmdEncoder& encoder,
                         const gapi::TextureHandle gibs_dbg_alloc)
{
  encoder.clearColorTexture(gibs_dbg_alloc,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  GIOnSurfels& state = get_state();
  if (!state.initialized)
  {
    state.initialized = true;
  }
}

NODE_BEGIN(gibs_allocate_surfels)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RW_UAV_AS   (gibs_dbg_alloc, dbgTex)
  BIND_TEX_SRV_AS      (gbuf1, gbuffer_normal)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  
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