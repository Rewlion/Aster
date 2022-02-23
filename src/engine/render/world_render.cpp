#include "world_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/log.h>
#include <engine/render/gapi/gapi.h>
#include <engine/time.h>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::Init()
  {
    gapi::BufferAllocationDescription ad;
    ad.size = sizeof(float2) * 3;
    ad.usage = gapi::BufferUsage::Vertex;
    m_TestBuffer = gapi::AllocateBuffer(ad);

    float2 vertices[3] = {
      float2(0,0),
      float2(0,1),
      float2(1,0)
    };

    gapi::CopyToBufferSync((void*)vertices, 0, ad.size, m_TestBuffer);

    ad = gapi::BufferAllocationDescription{};
    ad.size = sizeof(gapi::index_type) * 3;
    ad.usage = gapi::BufferUsage::Index;
    m_TestIndexBuffer = gapi::AllocateBuffer(ad);

    gapi::index_type indices[3] = {0,1,2};
    gapi::CopyToBufferSync(indices, 0,  ad.size, m_TestIndexBuffer);
  }

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

    StaticModelAsset asset;
    if (!assets_manager.GetStaticModel(str_hash("bin/assets/cube/cube.gltf"), asset))
    {
      logerror("failed to get asset");
      return;
    }

    for(const auto& submesh: asset.submeshes)
    {
      cmdList.push_back(gapi::BindVertexBufferCmd{
       .buffer = submesh.vertexBuffer
      });

      cmdList.push_back(gapi::BindIndexBufferCmd{
        .buffer = submesh.indexBuffer
      });

      cmdList.push_back(gapi::DrawIndexedCmd{
        .indexCount = submesh.indexCount,
        .instanceCount = 1,
        .firstIndex = 0,
        .vertexOffset = 0,
        .firstInstance = 0
      });
    }

    cmdList.push_back(gapi::EndRenderPassCmd{});
    cmdList.push_back(gapi::PresentSurfaceImageCmd{});

    gapi::submitCommands(std::move(cmdList));
  }

}
