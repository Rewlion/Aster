#include "nodes.h"

#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/ecs_utils.h>
#include <engine/tfx/tfx.h>

namespace Engine::Render
{
  fg::node_id_t mk_post_process_node()
  {
    return fg::register_node("post_process", FG_FILE_DECL, [](fg::Registry& reg)
    {
      auto ppInput = reg.readTexture("transparent_target", gapi::TextureState::ShaderRead);
      auto renderSize = Engine::Render::get_render_size();

      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
      allocDesc.extent = int3{renderSize.x, renderSize.y, 1};
      allocDesc.mipLevels = 1;
      allocDesc.arrayLayers = 1;
      allocDesc.usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_TRANSFER_SRC;

      auto rt = reg.createTexture("final_target", allocDesc, gapi::TextureState::RenderTarget);

      reg.requestRenderPass()
         .addTarget(rt, gapi::LoadOp::Load, gapi::StoreOp::Store);

      return [ppInput](gapi::CmdEncoder& encoder)
      {
        tfx::set_extern("post_process_input", ppInput.get());
        tfx::activate_scope("PostProcessScope", encoder);
        tfx::activate_technique("PostProcess", encoder);

        encoder.updateResources();
        encoder.draw(4, 1, 0, 0);
      };
    });
  }
}