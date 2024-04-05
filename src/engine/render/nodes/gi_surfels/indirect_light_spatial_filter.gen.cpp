// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "indirect_light_spatial_filter.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_indirect_light_sfilter(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_indirect_light_sfilter", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();


    auto gibs_indirect_light_sfilter_tex_one = reg.createTexture("gibs_indirect_light_sfilter_tex_one",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__ / uint(2), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );


    auto gibs_indirect_light_sfilter_tex_two = reg.createTexture("gibs_indirect_light_sfilter_tex_two",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R32G32B32A32_S,
        .extent =          uint3(__renderSize__ / uint(2), 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_UAV | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::ShaderReadWrite,
      false
    );

    fg::dsl::AccessDecorator render_size{__renderSize__};
    auto gibs_indirect_light_sample = reg.readTexture("gibs_indirect_light_sample", gapi::TextureState::ShaderRead, false);
    auto late_opaque_depth = reg.readTexture("late_opaque_depth", gapi::TextureState::DepthReadStencilRead, false);
    auto gbuf1 = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead, false);

    return [late_opaque_depth,gbuf1,render_size,gibs_indirect_light_sfilter_tex_one,gibs_indirect_light_sfilter_tex_two,gibs_indirect_light_sample](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("gbuffer_depth", late_opaque_depth.get());
      tfx::set_extern("gbuffer_normal", gbuf1.get());
      gibs_indirect_light_sfilter(encoder, render_size.get(), gibs_indirect_light_sfilter_tex_one.get(), gibs_indirect_light_sfilter_tex_two.get(), gibs_indirect_light_sample.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_indirect_light_sfilter_registration(
  mk_fg_node_gibs_indirect_light_sfilter,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_indirect_light_sfilter"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_indirect_light_end(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_indirect_light_end", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto gibs_indirect_light_srv = reg.renameTexture("gibs_indirect_light_sfilter_tex_one", "gibs_indirect_light_srv", gapi::TextureState::ShaderRead);
    return [](gapi::CmdEncoder&){};
  });
}

static
EventSystemRegistration mk_fg_node_gibs_indirect_light_end_registration(
  mk_fg_node_gibs_indirect_light_end,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_indirect_light_end"
);
