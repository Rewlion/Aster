// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "dbg_poly_renderer.ecs.cpp" 

using namespace ecs;

static void dbg_line_renderer_tick_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnGameTick* casted_event = reinterpret_cast<const Engine::OnGameTick*>(event);
  Engine::dbg::PolyRenderer& dbg_poly_renderer = accessor.get<Engine::dbg::PolyRenderer>(compile_ecs_name_hash("dbg_poly_renderer"));
  dbg_line_renderer_tick(*casted_event, dbg_poly_renderer);
}


static EventSystemRegistration dbg_line_renderer_tick_registration(
  dbg_line_renderer_tick_internal,
  compile_ecs_name_hash("OnGameTick"),
  {
    DESCRIBE_QUERY_COMPONENT("dbg_poly_renderer", Engine::dbg::PolyRenderer)
  },
  "dbg_line_renderer_tick"
);


static void dbg_line_renderer_prepare_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnBeforeRender* casted_event = reinterpret_cast<const Engine::OnBeforeRender*>(event);
  Engine::dbg::PolyRenderer& dbg_poly_renderer = accessor.get<Engine::dbg::PolyRenderer>(compile_ecs_name_hash("dbg_poly_renderer"));
  dbg_line_renderer_prepare(*casted_event, dbg_poly_renderer);
}


static EventSystemRegistration dbg_line_renderer_prepare_registration(
  dbg_line_renderer_prepare_internal,
  compile_ecs_name_hash("OnBeforeRender"),
  {
    DESCRIBE_QUERY_COMPONENT("dbg_poly_renderer", Engine::dbg::PolyRenderer)
  },
  "dbg_line_renderer_prepare"
);


const static DirectQueryRegistration query_dbg_poly_renderer_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("dbg_poly_renderer", Engine::dbg::PolyRenderer)
  },
  "query_dbg_poly_renderer"};
const static query_id_t query_dbg_poly_renderer_queryId = query_dbg_poly_renderer_queryReg.getId();


void query_dbg_poly_renderer (eastl::function<
  void(
    Engine::dbg::PolyRenderer& dbg_poly_renderer)> cb)
{
  ecs::get_registry().query(query_dbg_poly_renderer_queryId, [&](ComponentsAccessor& accessor)
  {
    Engine::dbg::PolyRenderer& dbg_poly_renderer = accessor.get<Engine::dbg::PolyRenderer>(compile_ecs_name_hash("dbg_poly_renderer"));
    cb(dbg_poly_renderer);
  });
}


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_dbg_poly_render(Event*, ComponentsAccessor&)
{
  fg::register_node("dbg_poly_render", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    const uint2 __renderSize__ = reg.getRenderSize();

    reg.orderMeBefore("ui");

    auto transparent_poly_acc = reg.createTexture("transparent_poly_acc",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(__renderSize__, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      },
      gapi::TextureState::RenderTarget,
      false
    );


    auto transparent_poly_revealage = reg.createTexture("transparent_poly_revealage",
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R8_UNORM,
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
      .addTarget(transparent_poly_acc, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addTarget(transparent_poly_revealage, gapi::LoadOp::Clear, gapi::StoreOp::Store, gapi::ClearColorValue{float{1.0f}})
      .addRODepth("gbuffer_depth", gapi::LoadOp::Load);


    return [](gapi::CmdEncoder& encoder)
    {
      dbg_poly_render_exec(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_dbg_poly_render_registration(
  mk_fg_node_dbg_poly_render,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_dbg_poly_render"
);


//Engine::OnFrameGraphInit handler
static
void mk_fg_node_dbg_poly_combine(Event*, ComponentsAccessor&)
{
  fg::register_node("dbg_poly_combine", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    auto transparent_poly_acc = reg.readTexture("transparent_poly_acc", gapi::TextureState::ShaderRead, false);
    auto transparent_poly_revealage = reg.readTexture("transparent_poly_revealage", gapi::TextureState::ShaderRead, false);
    reg.requestRenderPass()
      .addTarget("final_target", gapi::LoadOp::Load, gapi::StoreOp::Store, gapi::ClearColorValue{uint32_t{0}})
      .addRODepth("gbuffer_depth", gapi::LoadOp::Load);


    return [transparent_poly_acc,transparent_poly_revealage](gapi::CmdEncoder& encoder)
    {
      dbg_poly_combine_exec(encoder, transparent_poly_acc.get(), transparent_poly_revealage.get());
    };
  });
}

static
EventSystemRegistration mk_fg_node_dbg_poly_combine_registration(
  mk_fg_node_dbg_poly_combine,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_dbg_poly_combine"
);
