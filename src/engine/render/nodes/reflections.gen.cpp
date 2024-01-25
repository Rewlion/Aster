// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "reflections.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_resources(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_resources", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();


    auto reflections_target = reg.createTexture("reflections_target",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto reflections_acc = reg.createTexture("reflections_acc",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto reflections_target_filtered = reg.createTexture("reflections_target_filtered",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    return [reflections_target,reflections_acc,reflections_target_filtered](gapi::CmdEncoder& encoder)
    {
      reflections_resources(encoder, reflections_target.get(), reflections_acc.get(), reflections_target_filtered.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_reflections_resources_registration(
  mk_fg_node_reflections_resources,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections_resources"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto reflectionsTarget = reg.modifyTexture("reflections_target", gapi::TextureState::ShaderReadWrite);
    auto gbuffer_albedo = reg.readTexture("gbuf0", gapi::TextureState::ShaderRead, false);
    auto gbuffer_normal = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead, false);
    auto gbuffer_metal_roughness = reg.readTexture("gbuf2", gapi::TextureState::ShaderRead, false);
    auto prevPostProcessInput = reg.readTexture("post_process_input", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead, false);
    auto gbuffer_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto hi_z_buffer = reg.readTexture("hi_z_buffer", gapi::TextureState::ShaderRead, false);
    auto enviSpecular = reg.readTexture("atm_envi_specular", gapi::TextureState::ShaderRead, true);
    auto enviBRDF = reg.readTexture("atm_envi_brdf", gapi::TextureState::ShaderRead, true);

    return [reflectionsTarget,gbuffer_albedo,gbuffer_normal,gbuffer_metal_roughness,prevPostProcessInput,motionBuf,gbuffer_depth,hi_z_buffer,enviSpecular,enviBRDF,render_size](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("reflectionsTarget", reflectionsTarget.get());
      tfx::set_extern("gbuffer_albedo", gbuffer_albedo.get());
      tfx::set_extern("gbuffer_normal", gbuffer_normal.get());
      tfx::set_extern("gbuffer_metal_roughness", gbuffer_metal_roughness.get());
      tfx::set_extern("prevPostProcessInput", prevPostProcessInput.get());
      tfx::set_extern("motionBuf", motionBuf.get());
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      tfx::set_extern("hi_z_buffer", hi_z_buffer.get());
      tfx::set_extern("enviSpecular", enviSpecular.get());
      tfx::set_extern("enviBRDF", enviBRDF.get());
      reflections(encoder, render_size.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_reflections_registration(
  mk_fg_node_reflections,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_temporal_acc(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_temporal_acc", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead, false);
    auto taInput = reg.readTexture("reflections_target_filtered", gapi::TextureState::ShaderRead, false);
    auto taHistory = reg.readTexture("reflections_acc", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto taOutput = reg.modifyTexture("reflections_acc", gapi::TextureState::ShaderReadWrite);

    return [motionBuf,taInput,taHistory,taOutput,render_size](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("motionBuf", motionBuf.get());
      tfx::set_extern("taInput", taInput.get());
      tfx::set_extern("taHistory", taHistory.get());
      tfx::set_extern("taOutput", taOutput.get());
      reflections_temporal_acc(encoder, render_size.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_reflections_temporal_acc_registration(
  mk_fg_node_reflections_temporal_acc,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections_temporal_acc"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_blur(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_blur", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto blurInput = reg.readTexture("reflections_target", gapi::TextureState::ShaderRead, false);
    auto blurOutput = reg.modifyTexture("reflections_target_filtered", gapi::TextureState::ShaderReadWrite);

    return [blurInput,blurOutput,render_size](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("blurInput", blurInput.get());
      tfx::set_extern("blurOutput", blurOutput.get());
      reflections_blur(encoder, render_size.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_reflections_blur_registration(
  mk_fg_node_reflections_blur,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections_blur"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto specular_light = reg.renameTexture("reflections_acc", "specular_light", gapi::TextureState::ShaderRead);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_reflections_sync_registration(
  mk_fg_node_reflections_sync,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections_sync"
);
