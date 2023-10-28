#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/render/debug/poly_render.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/tfx/tfx.h>

ECS_EVENT_SYSTEM()
void dbg_line_renderer_tick(const Engine::OnGameTick&,
                            Engine::dbg::PolyRenderer& dbg_poly_renderer)
{
  dbg_poly_renderer.tick();
}

ECS_EVENT_SYSTEM()
void dbg_line_renderer_prepare(const Engine::OnBeforeRender& evt,
                               Engine::dbg::PolyRenderer& dbg_poly_renderer)
{
  dbg_poly_renderer.beforeRender(*evt.encoder);
}

ECS_DESCRIBE_QUERY(query_dbg_poly_renderer,
                   Engine::dbg::PolyRenderer& dbg_poly_renderer)

ECS_EVENT_SYSTEM()
static
void poly_renderer_creation_handler(const Engine::OnFrameGraphInit&)
{
  fg::register_node("dbg_poly_render", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeBefore("ui");

    auto texAllocBase = gapi::TextureAllocationDescription{
      .extent = uint3{Engine::Render::get_render_size(), 1},
      .usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_SRV,
    };

    texAllocBase.format = gapi::TextureFormat::R16G16B16A16_SFLOAT;
    auto accHndl = reg.createTexture("transparent_poly_acc",
                    texAllocBase,
                    gapi::TextureState::RenderTarget);

    texAllocBase.format = gapi::TextureFormat::R8_UNORM;
    auto revealageHndl = reg.createTexture("transparent_poly_revealage",
                          texAllocBase,
                          gapi::TextureState::RenderTarget);

    reg.requestRenderPass()
        .addTarget(accHndl, gapi::ClearColorValue{uint32_t{0}})
        .addTarget(revealageHndl, gapi::ClearColorValue{float{1.0}})
        .addRODepth("gbuffer_depth");

    return [](gapi::CmdEncoder& encoder)
    {
      query_dbg_poly_renderer([&encoder](auto& renderer){
        renderer.render(encoder);
      });
    };
  });

  fg::register_node("dbg_poly_combine", FG_FILE_DECL, [](fg::Registry& reg)
  {
    const auto accHndl = reg.readTexture("transparent_poly_acc", gapi::TextureState::ShaderRead);
    const auto revealageHndl = reg.readTexture("transparent_poly_revealage", gapi::TextureState::ShaderRead);

    reg.requestRenderPass()
       .addTarget("final_target")
       .addRODepth("gbuffer_depth");

    return [accHndl, revealageHndl](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("dbgPolyAcc", accHndl.get());
      tfx::set_extern("dbgPolyRevealage", revealageHndl.get());
      query_dbg_poly_renderer([&encoder](auto& renderer){
        renderer.combine(encoder);
      });
    };
  });
}
