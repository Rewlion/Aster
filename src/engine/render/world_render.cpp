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
    m_TestBuffer = AllocateBuffer(ad);

    float2 vertices[3] = {
      float2(0,0),
      float2(0,1),
      float2(1,0)
    };

    CopyToBufferSync((void*)vertices, 0, ad.size, m_TestBuffer);

    ad = gapi::BufferAllocationDescription{};
    ad.size = sizeof(gapi::index_type) * 3;
    ad.usage = gapi::BufferUsage::Index;
    m_TestIndexBuffer = gapi::AllocateBuffer(ad);

    gapi::index_type indices[3] = {0,1,2};
    CopyToBufferSync(indices, 0,  ad.size, m_TestIndexBuffer);

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = AllocateSampler(samplerAllocDesc);
  }

  void WorldRender::Render()
  {
    gapi::CommandList cmdList;

    BeginRenderPass(cmdList, { gapi::getCurrentSurfaceRT() });
    Clear(cmdList, gapi::CLEAR_RT);
    BindGraphicsPipeline(cmdList, {str_hash("test_vs"), str_hash("test_ps")}, gapi::PrimitiveTopology::TriangleList, {}, {.attachmentsCount=1});

    mat4 mvp = mat4{1};
    mvp = glm::translate(mvp, float3{0,0, 5});
    mvp = math::Perspective(90.0, 3.0f/4.0f, 0.1, 100.0) * mvp;

    PushConstants(cmdList, &mvp, sizeof(mvp), gapi::ShaderStage::Vertex);

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

    BindTexture(cmdList, texture.texture, 0, 0);
    BindSampler(cmdList, m_TestSampler, 0, 1);

    for(const auto& submesh: asset.submeshes)
    {
      BindVertexBuffer(cmdList, submesh.vertexBuffer);
      BindIndexBuffer(cmdList, submesh.indexBuffer);
      DrawIndexed(cmdList, submesh.indexCount, 1, 0, 0, 0);
    }

    PresentSurfaceImage(cmdList);
    submitCommands(std::move(cmdList));
  }

}
