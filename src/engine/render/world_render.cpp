#include "world_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/scene/scene.h>
#include <engine/time.h>

#include <glm/gtx/transform.hpp>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::init()
  {
    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = allocate_sampler(samplerAllocDesc);

    m_TestConstBuffer = gapi::allocate_buffer(sizeof(float4), gapi::BF_CpuVisible | gapi::BF_BindConstant);
  }

  void WorldRender::render()
  {
    beforeRender();
    renderStaticSceneOpaque();
  }

  void WorldRender::beforeRender()
  {
  }

  void WorldRender::renderStaticSceneOpaque()
  {
    gapi::CommandList cmdList;

    m_CmdEncoder.beginRenderpass({ gapi::get_backbuffer()});
    m_CmdEncoder.clear(gapi::CLEAR_RT);

    const auto objects = scene.queueObjects();
    for (const auto& obj: objects)
    {
      mat4 mvp = mat4{1};
      mvp = glm::translate(mvp, obj.pos);
      mvp = math::perspective(90.0, 3.0f/4.0f, 0.1, 100.0) * mvp;

      m_CmdEncoder.pushConstants(&mvp, sizeof(mvp), gapi::ShaderStage::Vertex);

      StaticModelAsset* asset = assets_manager.getStaticModel(obj.model);

      float4 color{1.0, 1.0, 0.6, 1.0};
      write_buffer(m_TestConstBuffer, &color, 0, sizeof(color), gapi::WR_DISCARD);

      m_CmdEncoder.bindSampler(m_TestSampler, 0, 1);
      m_CmdEncoder.bindConstBuffer(m_TestConstBuffer, 0, 2);

      for(const auto& submesh: asset->submeshes)
      {
        submesh.material->setState(m_CmdEncoder, Engine::RenderPassType::Main);
        submesh.material->setParams(m_CmdEncoder);

        m_CmdEncoder.bindVertexBuffer(submesh.vertexBuffer);
        m_CmdEncoder.bindIndexBuffer(submesh.indexBuffer);
        m_CmdEncoder.drawIndexed(submesh.material->getTopology(), submesh.indexCount, 1, 0, 0, 0);
      }
    }

    m_CmdEncoder.present();
    m_CmdEncoder.flush();
  }

}
