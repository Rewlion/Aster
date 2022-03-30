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
    m_TestBuffer = gapi::allocate_buffer(sizeof(float2) * 3, gapi::BF_GpuVisible | gapi::BF_BindVertex);

    float2 vertices[3] = {
      float2(0,0),
      float2(0,1),
      float2(1,0)
    };
    write_buffer(m_TestBuffer, (void*)vertices, 0, sizeof(float2) * 3);

    m_TestIndexBuffer = gapi::allocate_buffer(sizeof(gapi::index_type) * 3,
                                              gapi::BF_GpuVisible | gapi::BF_BindIndex);

    gapi::index_type indices[3] = {0,1,2};
    write_buffer(m_TestIndexBuffer, indices, 0, sizeof(gapi::index_type) * 3);

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = allocate_sampler(samplerAllocDesc);

    m_TestConstBuffer = gapi::allocate_buffer(sizeof(float4), gapi::BF_CpuVisible | gapi::BF_BindConstant);
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

    float4 color{1.0, 1.0, 0.6, 1.0};
    write_buffer(m_TestConstBuffer, &color, 0, sizeof(color), gapi::WR_DISCARD);

    bind_texture(cmdList, texture.texture, 0, 0);
    bind_sampler(cmdList, m_TestSampler, 0, 1);
    bind_const_buffer(cmdList, m_TestConstBuffer, 0, 2);

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
