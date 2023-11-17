#include <engine/render/frame_graph/dsl.h>
#include <engine/render/debug/line_render.h>

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

NODE_BEGIN(dbg_line_renderer)
  RP_BEGIN()
    TARGET(post_opaque_target)
  RP_END()

  EXEC(dbg_line_renderer_exec)
NODE_END()

NODE_EXEC()
static
void dbg_line_renderer_exec(gapi::CmdEncoder& encoder)
{
  query_dbg_line_renderer([&encoder](auto& renderer){
    renderer.render(encoder);
  });
}
