#include "world_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/gapi/gapi.h>
#include <engine/time.h>

#include <glm/gtx/transform.hpp>

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

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = gapi::AllocateSampler(samplerAllocDesc);
  }

  void WorldRender::Render()
  {
    gapi::CommandList cmdList;

    gapi::BeginRenderPassCmd beginPass;
    beginPass.renderTargets.Push({gapi::getCurrentSurfaceRT()});
    cmdList.push_back(beginPass);

    cmdList.push_back(gapi::ClearCmd{gapi::CLEAR_RT});

    gapi::GraphicsPipelineDescription pipeline;
    pipeline.shaderNames.Push(str_hash("test_vs"));
    pipeline.shaderNames.Push(str_hash("test_ps"));
    pipeline.topology = gapi::PrimitiveTopology::TriangleList;
    pipeline.blendState.attachmentsCount = 1;

    cmdList.push_back(gapi::BindGraphicsPipelineCmd{
      .description = pipeline
    });

    mat4 mvp = mat4{1};
    mvp = glm::translate(mvp, float3{0,0, 5});
    mvp = math::Perspective(90, 3.0f/4.0f, 0.1, 100) * mvp;

    cmdList.push_back(gapi::PushConstantsCmd{
      .data = &mvp,
      .size = sizeof(mvp),
      .stage = gapi::ShaderStage::Vertex
    });

    StaticModelAsset asset;
    if (!assets_manager.GetStaticModel(str_hash("bin/assets/cube/cube.gltf"), asset))
    {
      logerror("failed to get asset");
      return;
    }

    TextureAsset texture;
    if (!assets_manager.GetTexture(str_hash("bin/assets/cube/container.ktx"), texture))
    {
      logerror("failed to get asset");
      return;
    }

    cmdList.push_back(gapi::BindTextureCmd{
      .texture = texture.texture,
      .argument = 0,
      .binding = 0
    });

    cmdList.push_back(gapi::BindSamplerCmd{
      .sampler = m_TestSampler,
      .argument = 0,
      .binding = 1
    });

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

    cmdList.push_back(gapi::PresentSurfaceImageCmd{});

    gapi::submitCommands(std::move(cmdList));
  }

}
