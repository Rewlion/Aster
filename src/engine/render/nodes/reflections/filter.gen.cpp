// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "filter.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_spatial_filter(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_spatial_filter", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    auto reflections_sfilter_feedback = reg.renameTexture("reflections_ta_history", "reflections_sfilter_feedback", gapi::TextureState::ShaderReadWrite);

    auto reflections_sfilter_tex_one = reg.createTexture("reflections_sfilter_tex_one",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite
    );


    auto reflections_sfilter_tex_two = reg.createTexture("reflections_sfilter_tex_two",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite
    );

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto reflections_ta_target = reg.readTexture("reflections_ta_target", gapi::TextureState::ShaderRead, false);
    auto reflections_variance = reg.readTexture("reflections_variance", gapi::TextureState::ShaderRead, false);
    auto late_opaque_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto gbuf1 = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead, false);
    auto gbuf2 = reg.readTexture("gbuf2", gapi::TextureState::ShaderRead, false);

    return [reflections_ta_target,reflections_sfilter_feedback,reflections_variance,late_opaque_depth,gbuf1,gbuf2,render_size,reflections_sfilter_tex_one,reflections_sfilter_tex_two](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("filterInput", reflections_ta_target.get());
      tfx::set_extern("filterOutput", reflections_sfilter_feedback.get());
      tfx::set_extern("variance", reflections_variance.get());
      tfx::set_extern("gbuffer_depth", late_opaque_depth.get());
      tfx::set_extern("gbuffer_normal", gbuf1.get());
      tfx::set_extern("gbuffer_metal_roughness", gbuf2.get());
      reflections_spatial_filter(encoder, render_size.get(), reflections_ta_target.get(), reflections_sfilter_feedback.get(), reflections_sfilter_tex_one.get(), reflections_sfilter_tex_two.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_reflections_spatial_filter_registration(
  mk_fg_node_reflections_spatial_filter,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_reflections_spatial_filter"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_reflections_sync(Event*, ComponentsAccessor&)
{
  fg::register_node("reflections_sync", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto specular_light = reg.renameTexture("reflections_sfilter_tex_one", "specular_light", gapi::TextureState::ShaderRead);
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
