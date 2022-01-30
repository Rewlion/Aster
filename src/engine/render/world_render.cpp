#include "world_render.h"

#include <engine/render/gapi/gapi.h>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::Render()
  {
    gapi::CommandList cmdList;

    gapi::BeginRenderPassCmd beginPass;
    beginPass.renderTargets[0].texture = gapi::getCurrentSurfaceRT();
    beginPass.renderTargets[0].loadOp = gapi::TextureLoadOp::Clear;
    cmdList.push_back(beginPass);

    cmdList.push_back(gapi::EndRenderPassCmd{});
    cmdList.push_back(gapi::PresentSurfaceImageCmd{});

    gapi::submitCommands(std::move(cmdList));
  }

}
