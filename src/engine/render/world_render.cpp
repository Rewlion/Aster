#include "world_render.h"

#include <engine/render/gapi/gapi.h>
#include <engine/algorithm/hash.h>

#include <engine/time.h>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::Render()
  {
    gapi::CommandList cmdList;

    gapi::BeginRenderPassCmd beginPass;
    beginPass.renderTargets.Push({
      .texture = gapi::getCurrentSurfaceRT(),
      .loadOp = gapi::TextureLoadOp::Clear,
      .storeOp = gapi::TextureStoreOp::Store
    });
    cmdList.push_back(beginPass);

    gapi::GraphicsPipelineDescription pipeline;
    pipeline.shaderNames.Push(str_hash("test_vs"));
    pipeline.shaderNames.Push(str_hash("test_ps"));
    pipeline.topology = gapi::PrimitiveTopology::TriangleList;
    pipeline.blendState.attachmentsCount = 1;

    cmdList.push_back(gapi::BindGraphicsPipelineCmd{
      .description = pipeline
    });

    static float4 color = {1.0,0, 0, 255};
    color.g += Engine::Time::GetDt();
    color.g = color.g > 1 ? 0 : color.g;

    cmdList.push_back(gapi::PushConstantsCmd{
      .data = &color,
      .size = sizeof(color),
      .stage = gapi::ShaderStage::Fragment
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
