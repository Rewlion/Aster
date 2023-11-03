#include <engine/render/debug/poly_render.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/dsl.h>
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

NODE_BEGIN(dbg_poly_render)
  ORDER_ME_BEFORE(ui)

  CREATE_TEX_2D(transparent_poly_acc, TEX_SIZE_RELATIVE(), R16G16B16A16_SFLOAT, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))
  CREATE_TEX_2D(transparent_poly_revealage, TEX_SIZE_RELATIVE(), R8_UNORM, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))

  RP_BEGIN()
    TARGET_CLEARED(transparent_poly_acc, UCLEAR(0)),
    TARGET_CLEARED(transparent_poly_revealage, FCLEAR(1.0f)),
    DEPTH_RO(gbuffer_depth)
  RP_END()

  EXEC(dbg_poly_render_exec)
NODE_END()

NODE_EXEC()
static
void dbg_poly_render_exec(gapi::CmdEncoder& encoder)
{
  query_dbg_poly_renderer([&encoder](auto& renderer){
    renderer.render(encoder);
  });
}

NODE_BEGIN(dbg_poly_combine)
  READ_TEX_SRV(transparent_poly_acc)
  READ_TEX_SRV(transparent_poly_revealage)

  RP_BEGIN()
    TARGET(final_target),
    DEPTH_RO(gbuffer_depth)
  RP_END()

  EXEC(dbg_poly_combine_exec)
NODE_END()

NODE_EXEC()
static
void dbg_poly_combine_exec(gapi::CmdEncoder& encoder,
                           const gapi::TextureHandle transparent_poly_acc,
                           const gapi::TextureHandle transparent_poly_revealage)
{
  tfx::set_extern("dbgPolyAcc", transparent_poly_acc);
  tfx::set_extern("dbgPolyRevealage", transparent_poly_revealage);
  query_dbg_poly_renderer([&encoder](auto& renderer){
    renderer.combine(encoder);
  });
}
