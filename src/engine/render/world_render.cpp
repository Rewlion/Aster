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

  void WorldRender::init()
  {
    gapi::BufferAllocationDescription ad;
    ad.size = sizeof(float2) * 3;
    ad.usage = gapi::BufferUsage::Vertex;
    m_TestBuffer = allocate_buffer(ad);

    float2 vertices[3] = {
      float2(0,0),
      float2(0,1),
      float2(1,0)
    };

    copy_to_buffer_sync((void*)vertices, 0, ad.size, m_TestBuffer);

    ad = gapi::BufferAllocationDescription{};
    ad.size = sizeof(gapi::index_type) * 3;
    ad.usage = gapi::BufferUsage::Index;
    m_TestIndexBuffer = allocate_buffer(ad);

    gapi::index_type indices[3] = {0,1,2};
    copy_to_buffer_sync(indices, 0,  ad.size, m_TestIndexBuffer);

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = allocate_sampler(samplerAllocDesc);
  }

  void WorldRender::render()
  {
    gapi::CommandList cmdList;

    begin_renderpass(cmdList, { gapi::get_backbuffer() });
    clear(cmdList, gapi::CLEAR_RT);
    bind_graphics_shaders(cmdList, {str_hash("test_vs"), str_hash("test_ps")});

    mat4 mvp = mat4{1};
    mvp = glm::translate(mvp, float3{0,0, 5});
    mvp = math::perspective(90.0, 3.0f/4.0f, 0.1, 100.0) * mvp;

    push_constants(cmdList, &mvp, sizeof(mvp), gapi::ShaderStage::Vertex);

    StaticModelAsset asset;
    if (!assets_manager.getStaticModel(str_hash("bin/assets/cube/cube.gltf"), asset))
    {
      logerror("failed to get asset");
      return;
    }

    TextureAsset texture;
    if (!assets_manager.getTexture(str_hash("bin/assets/cube/container.ktx"), texture))
    {
      logerror("failed to get asset");
      return;
    }

    bind_texture(cmdList, texture.texture, 0, 0);
    bind_sampler(cmdList, m_TestSampler, 0, 1);

    for(const auto& submesh: asset.submeshes)
    {
      bind_vertex_buffer(cmdList, submesh.vertexBuffer);
      bind_index_buffer(cmdList, submesh.indexBuffer);
      draw_indexed(cmdList, gapi::PrimitiveTopology::TriangleList, submesh.indexCount, 1, 0, 0, 0);
    }

    present(cmdList);
    submit_commands(std::move(cmdList));
  }

}
