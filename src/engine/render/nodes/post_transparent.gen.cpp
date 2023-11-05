// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "post_transparent.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_ui(Event*, ComponentsAccessor&)
{
  fg::register_node("ui", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.requestRenderPass()
      .addTarget("final_target", gapi::LoadOp::Load, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [](gapi::CmdEncoder& encoder)
    {
      ui_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_ui_registration(
  mk_fg_node_ui,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_ui"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_dbg_text(Event*, ComponentsAccessor&)
{
  fg::register_node("dbg_text", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeAfter("ui");
    reg.requestRenderPass()
      .addTarget("final_target", gapi::LoadOp::Load, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
    ;


    return [](gapi::CmdEncoder& encoder)
    {
      dbg_text_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_dbg_text_registration(
  mk_fg_node_dbg_text,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_dbg_text"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_TAA(Event*, ComponentsAccessor&)
{
  fg::register_node("TAA", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    auto taaCurrentFrame = reg.readTexture("final_target", gapi::TextureState::ShaderRead, false);
    auto taaPrevFrame = reg.readTexture("final_antialiased_target", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead, false);
    auto gbuffer_depth = reg.readTexture("gbuffer_depth", gapi::TextureState::DepthReadStencilRead, false);

    auto final_antialiased_target = reg.createTexture("final_antialiased_target",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)((gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV) | gapi::TextureUsage::TEX_USAGE_TRANSFER_SRC)
      },
      gapi::TextureState::RenderTarget,
      false
    );

    reg.requestRenderPass()
      .addTarget(final_antialiased_target, gapi::LoadOp::DontCare, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
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


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_present(Event*, ComponentsAccessor&)
{
  fg::register_node("present", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto final_antialiased_target = reg.readTexture("final_antialiased_target", gapi::TextureState::TransferSrc, false);
    auto backbuffer = reg.modifyTexture("backbuffer", gapi::TextureState::TransferDst);

    return [final_antialiased_target,backbuffer](gapi::CmdEncoder& encoder)
    {
      present_exec(encoder, final_antialiased_target.get(), backbuffer.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_present_registration(
  mk_fg_node_present,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_present"
);
