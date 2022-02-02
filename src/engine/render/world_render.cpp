#include "world_render.h"

#include <engine/render/gapi/gapi.h>
#include <engine/algorithm/hash.h>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::Render()
  {
    gapi::CommandList cmdList;

    gapi::BeginRenderPassCmd beginPass;
    beginPass.renderTargets[0].texture = gapi::getCurrentSurfaceRT();
    beginPass.renderTargets[0].loadOp = gapi::TextureLoadOp::Clear;
    beginPass.renderTargets[0].storeOp = gapi::TextureStoreOp::Store;
    cmdList.push_back(beginPass);

    gapi::GraphicsPipelineDescription pipeline;
    pipeline.shadersNames[0] = str_hash("test_vs");
    pipeline.shadersNames[1] = str_hash("test_ps");
    pipeline.shadersCount = 2;
    pipeline.topology = gapi::PrimitiveTopology::TriangleList;
    pipeline.blendState.attachmentsCount = 1;

    cmdList.push_back(gapi::BindGraphicsPipelineCmd{
      .description = pipeline
    });

    cmdList.push_back(gapi::DrawCmd{
      .vertexCount = 3,
      .instanceCount = 1
    });

    cmdList.push_back(gapi::EndRenderPassCmd{});
    cmdList.push_back(gapi::PresentSurfaceImageCmd{});

    gapi::submitCommands(std::move(cmdList));
  }

}
