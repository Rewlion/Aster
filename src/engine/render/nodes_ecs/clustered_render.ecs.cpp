#include <engine/console/cmd.h>
#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/render/clustered_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/tfx/tfx.h>

ECS_EVENT_SYSTEM()
void clustered_render_prepare(const Engine::OnBeforeRender& evt,
                              Engine::Render::ClusteredRenderer& clustered_render)
{
  clustered_render.beforeRender(*evt.encoder);
}

static bool is_clustered_render_dbg_enabled = true;

ECS_EVENT_SYSTEM()
static
void clustered_render_creation_handler(const ecs::OnEntityCreated&,
                                    Engine::Render::ClusteredRenderer& clustered_render)
{
  fg::register_node("dbg_clustered_render", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeBefore("ui");

    auto rt = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

    return [rt](gapi::CmdEncoder& encoder)
    {
      if (is_clustered_render_dbg_enabled)
      {
        encoder.beginRenderpass(gapi::RenderTargets{gapi::RenderPassAttachment{rt.get()}}, {});
        tfx::activate_technique("DebugClusters", encoder);
        encoder.updateResources();
        encoder.draw(4,1,0,0);
        encoder.endRenderpass();
      }
    };
  });
}

static
void toggle_clustered_render_dbg(eastl::span<string_view>)
{
  is_clustered_render_dbg_enabled = !is_clustered_render_dbg_enabled;
}

CONSOLE_CMD("render.debug_clusters", 0, 0, toggle_clustered_render_dbg);