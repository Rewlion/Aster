// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "opaque.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gbuffer_main_pass(Event*, ComponentsAccessor&)
{
  fg::register_node("gbuffer_main_pass", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    reg.orderMeAfter("frame_preparing");

    auto opaque_depth = reg.createTexture("opaque_depth",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::D24_UNORM_S8_UINT,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_DEPTH_STENCIL | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::DepthWriteStencilWrite,
      false
    );


    auto gbuf0 = reg.createTexture("gbuf0",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::RenderTarget,
      false
    );


    auto gbuf1 = reg.createTexture("gbuf1",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_UNORM,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::RenderTarget,
      false
    );


    auto gbuf2 = reg.createTexture("gbuf2",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::RenderTarget,
      false
    );


    auto motionBuf = reg.createTexture("motionBuf",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::RenderTarget,
      false
    );

    auto camera_data = reg.readBlob<Engine::CameraData>("camera_data");
    reg.requestRenderPass()
      .addTarget(gbuf0, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addTarget(gbuf1, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addTarget(gbuf2, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addTarget(motionBuf, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addRWDepth(opaque_depth, gapi::LoadOp::Clear, gapi::StoreOp::Store);


    return [camera_data](gapi::CmdEncoder& encoder)
    {
      gbuffer_main_pass_exec(encoder, camera_data.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gbuffer_main_pass_registration(
  mk_fg_node_gbuffer_main_pass,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gbuffer_main_pass"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_late_opaque_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("late_opaque_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto late_opaque_depth = reg.renameTexture("opaque_depth", "late_opaque_depth", gapi::TextureState::DepthReadStencilRead);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_late_opaque_sync_registration(
  mk_fg_node_late_opaque_sync,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_late_opaque_sync"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gbuffer_resolve(Event*, ComponentsAccessor&)
{
  fg::register_node("gbuffer_resolve", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();


    auto resolve_target = reg.createTexture("resolve_target",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)((gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_UAV) | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto gbuf0 = reg.readTexture("gbuf0", gapi::TextureState::ShaderRead, false);
    auto gbuf1 = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead, false);
    auto gbuf2 = reg.readTexture("gbuf2", gapi::TextureState::ShaderRead, false);
    auto post_process_input = reg.readTexture("post_process_input", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead, false);
    auto clustered_lights = reg.readBuffer("clustered_lights", gapi::BufferState::BF_STATE_SRV, false);
    auto clusters_info = reg.readBuffer("clusters_info", gapi::BufferState::BF_STATE_SRV, false);
    auto clusters_indirecion = reg.readBuffer("clusters_indirecion", gapi::BufferState::BF_STATE_SRV, false);
    auto gbuffer_depth = reg.renameTexture("late_opaque_depth", "gbuffer_depth", gapi::TextureState::DepthReadStencilRead);
    auto sph_buf = reg.readBuffer("sph_buf", gapi::BufferState::BF_STATE_SRV, false);
    auto gibs_indirect_light_srv = reg.readTexture("gibs_indirect_light_srv", gapi::TextureState::ShaderRead, false);
    auto specular_light = reg.readTexture("specular_light", gapi::TextureState::ShaderRead, false);
    auto shadow_map_viewProj = reg.readBlob<float4x4>("shadow_map_viewProj");
    auto shadow_map = reg.readTexture("shadow_map", gapi::TextureState::DepthReadStencilRead, false);

    return [resolve_target,gbuf0,gbuf1,gbuf2,post_process_input,motionBuf,clustered_lights,clusters_info,clusters_indirecion,gbuffer_depth,sph_buf,gibs_indirect_light_srv,specular_light,shadow_map,render_size,shadow_map_viewProj](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("resolveTarget", resolve_target.get());
      tfx::set_extern("gbuffer_albedo", gbuf0.get());
      tfx::set_extern("gbuffer_normal", gbuf1.get());
      tfx::set_extern("gbuffer_metal_roughness", gbuf2.get());
      tfx::set_extern("prevPostProcessInput", post_process_input.get());
      tfx::set_extern("motionBuf", motionBuf.get());
      tfx::set_extern("clustersLightBuffer", clustered_lights.get());
      tfx::set_extern("clustersInfoBuffer", clusters_info.get());
      tfx::set_extern("clustersIndirectionBuffer", clusters_indirecion.get());
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      tfx::set_extern("atmParamsBuffer", sph_buf.get());
      tfx::set_extern("indirectLight", gibs_indirect_light_srv.get());
      tfx::set_extern("specularLight", specular_light.get());
      tfx::set_extern("shadowMap", shadow_map.get());
      gbuffer_resolve_exec(encoder, render_size.get(), shadow_map_viewProj.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gbuffer_resolve_registration(
  mk_fg_node_gbuffer_resolve,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gbuffer_resolve"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_post_opaque_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("post_opaque_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto post_opaque_target = reg.renameTexture("resolve_target", "post_opaque_target", gapi::TextureState::RenderTarget);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_post_opaque_sync_registration(
  mk_fg_node_post_opaque_sync,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_post_opaque_sync"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_transparent_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("transparent_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto transparent_target = reg.renameTexture("taa_target", "transparent_target", gapi::TextureState::RenderTarget);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_transparent_sync_registration(
  mk_fg_node_transparent_sync,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_transparent_sync"
);
