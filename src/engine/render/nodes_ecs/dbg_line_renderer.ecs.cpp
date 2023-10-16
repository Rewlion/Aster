#include <engine/ecs/macros.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/debug/line_render.h>
#include <engine/events.h>

ECS_EVENT_SYSTEM()
void dbg_line_renderer_tick(const Engine::OnGameTick&,
                            Engine::dbg::LineRenderer& dbg_line_renderer)
{
  dbg_line_renderer.tick();
}

ECS_EVENT_SYSTEM()
void dbg_line_renderer_prepare(const Engine::OnBeforeRender& evt,
                               Engine::dbg::LineRenderer& dbg_line_renderer)
{
  dbg_line_renderer.beforeRender(*evt.encoder);
}

ECS_DESCRIBE_QUERY(query_dbg_line_renderer, Engine::dbg::LineRenderer& dbg_line_renderer)

ECS_EVENT_SYSTEM()
static
void line_renderer_creation_handler(const ecs::OnEntityCreated&,
                                      Engine::dbg::LineRenderer& dbg_line_renderer)
{
  fg::register_node("dbg_line_renderer", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeBefore("ui");

    reg.requestRenderPass()
       .addTarget("final_target")
       .addTarget("motionBuf")
       .addRODepth("gbuffer_depth");

    return [](gapi::CmdEncoder& encoder)
    {
      query_dbg_line_renderer([&encoder](auto& renderer){
        renderer.render(encoder);
      });
    };
  });
}