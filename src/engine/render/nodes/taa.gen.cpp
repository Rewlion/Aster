// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "taa.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_TAA(Event*, ComponentsAccessor&)
{
  fg::register_node("TAA", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    auto taaCurrentFrame = reg.readTexture("post_opaque_target", gapi::TextureState::ShaderRead, false);
    auto taaPrevFrame = reg.readTexture("taa_target", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead, false);
    auto gbuffer_depth = reg.readTexture("gbuffer_depth", gapi::TextureState::DepthReadStencilRead, false);

    auto taa_target = reg.createTexture("taa_target",
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

    reg.requestRenderPass()
      .addTarget(taa_target, gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [taaCurrentFrame,taaPrevFrame,motionBuf,gbuffer_depth](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("taaCurrentFrame", taaCurrentFrame.get());
      tfx::set_extern("taaPrevFrame", taaPrevFrame.get());
      tfx::set_extern("motionBuf", motionBuf.get());
      tfx::set_extern("gbuffer_depth", gbuffer_depth.get());
      taa_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_TAA_registration(
  mk_fg_node_TAA,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_TAA"
);