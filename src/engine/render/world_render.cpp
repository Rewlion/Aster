#include "world_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/scene/scene.h>
#include <engine/time.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::init()
  {
    m_WindowSize = Window::get_window_size();
    m_Aspect = (float)m_WindowSize.x / (float)m_WindowSize.y;

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_TestSampler = allocate_sampler(samplerAllocDesc);

    m_TestConstBuffer = gapi::allocate_buffer(sizeof(float4), gapi::BF_CpuVisible | gapi::BF_BindConstant);

    gapi::TextureAllocationDescription allocDesc;
    allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
    allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
    allocDesc.mipLevels = 1;
    allocDesc.arrayLayers = 1;
    allocDesc.usage = gapi::TextureUsage::DepthStencil;
    m_RtDepth = gapi::allocate_texture(allocDesc);
    gapi::transit_texture_state(m_RtDepth, gapi::TextureState::Undefined, gapi::TextureState::DepthWriteStencilRead);
  }

  void WorldRender::render(const mat4& cameraVP)
  {
    beforeRender(cameraVP);
    renderStaticSceneOpaque();
  }

  void WorldRender::beforeRender(const mat4& cameraVP)
  {
    m_FrameData.vp = cameraVP;
  }

  void WorldRender::renderStaticSceneOpaque()
  {
    gapi::transit_texture_state(gapi::get_backbuffer(), gapi::TextureState::Present, gapi::TextureState::RenderTarget);

    m_CmdEncoder.beginRenderpass(
      {
        gapi::RenderPassAttachment{gapi::get_backbuffer(), gapi::TextureState::RenderTarget, gapi::TextureState::Present}
      },
      gapi::RenderPassAttachment{m_RtDepth, gapi::TextureState::DepthWriteStencilRead, gapi::TextureState::DepthWriteStencilRead}
    );
    m_CmdEncoder.clear(gapi::CLEAR_RT | gapi::CLEAR_DEPTH);

    const auto objects = scene.queueObjects();
    for (const auto& obj: objects)
    {
      const mat4 rot = glm::rotate(obj.rot.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(obj.rot.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(obj.rot.x, float3{1.0, 0.0, 0.0});
      const mat4 scale = glm::scale(obj.scale);
      const mat4 tr = glm::translate(obj.pos);
      const mat4 mTr = m_FrameData.vp * tr * scale * rot;

      m_CmdEncoder.pushConstants(&mTr, sizeof(mTr), gapi::ShaderStage::Vertex);

      ModelAsset* asset = assets_manager.getModel(obj.model);

      float4 color{1.0, 1.0, 0.6, 1.0};
      write_buffer(m_TestConstBuffer, &color, 0, sizeof(color), gapi::WR_DISCARD);

      m_CmdEncoder.bindSampler(m_TestSampler, 0, 1);
      m_CmdEncoder.bindConstBuffer(m_TestConstBuffer, 0, 2);

      for(size_t i = 0; i < asset->mesh->submeshes.getSize(); ++i)
      {
        const Submesh& submesh = asset->mesh->submeshes.get(i);
        Material* material = asset->materials[i];
        material->setState(m_CmdEncoder, Engine::RenderPassType::Main);
        material->setParams(m_CmdEncoder);

        m_CmdEncoder.bindVertexBuffer(submesh.vertexBuffer);
        m_CmdEncoder.bindIndexBuffer(submesh.indexBuffer);
        m_CmdEncoder.drawIndexed(material->getTopology(), submesh.indexCount, 1, 0, 0, 0);
      }
    }

    m_CmdEncoder.present();
    m_CmdEncoder.flush();
  }

}
