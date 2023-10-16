#include <engine/ecs/macros.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/debug/poly_render.h>
#include <engine/events.h>

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
void poly_renderer_creation_handler(const ecs::OnEntityCreated&,
                                    Engine::dbg::PolyRenderer& dbg_poly_renderer)
{
  fg::register_node("dbg_poly_renderer", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeBefore("ui");

    reg.requestRenderPass()
       .addTarget("final_target")
       .addRODepth("gbuffer_depth");

    return [](gapi::CmdEncoder& encoder)
    {
      query_dbg_poly_renderer([&encoder](auto& renderer){
        renderer.render(encoder);
      });
    };
  });
}
