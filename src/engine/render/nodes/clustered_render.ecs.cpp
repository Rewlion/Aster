#include <engine/console/cmd.h>
#include <engine/render/clustered_render.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

ECS_EVENT_SYSTEM()
void clustered_render_prepare(const Engine::OnBeforeRender& evt,
                              Engine::Render::ClusteredRenderer& clustered_render)
{
  clustered_render.beforeRender(*evt.encoder);
}

static bool is_clustered_render_dbg_enabled = true;

NODE_BEGIN(dbg_clustered_render)
  ORDER_ME_BEFORE(ui)
  MODIFY_TEX_AS(final_target, AS(rt), TEX_STATE(RenderTarget))
  EXEC(dbg_clustered_render_exec)
NODE_END()

NODE_EXEC()
static
void dbg_clustered_render_exec(gapi::CmdEncoder& encoder,
                               const gapi::TextureHandle rt)
{
  if (is_clustered_render_dbg_enabled)
  {
    encoder.beginRenderpass(gapi::RenderTargets{gapi::RenderPassAttachment{rt}}, {});
    tfx::activate_technique("DebugClusters", encoder);
    encoder.updateResources();
    encoder.draw(4,1,0,0);
    encoder.endRenderpass();
  }
}

static
void toggle_clustered_render_dbg(eastl::span<string_view>)
{
  is_clustered_render_dbg_enabled = !is_clustered_render_dbg_enabled;
}

CONSOLE_CMD("render.debug_clusters", 0, 0, toggle_clustered_render_dbg);