#include "world_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/scene/scene.h>
#include <engine/time.h>
#include <engine/window.h>

#include <engine/shaders/shaders/frame_uniforms.hlsl>
#include <engine/shaders/shaders/mesh_uniforms.hlsl>

#include <glm/gtx/transform.hpp>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::init()
  {
    m_WindowSize = Window::get_window_size();
    m_Aspect = (float)m_WindowSize.x / (float)m_WindowSize.y;

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_ModelSampler = allocate_sampler(samplerAllocDesc);

    m_FrameUniforms = gapi::allocate_buffer(sizeof(FrameUniforms), gapi::BF_CpuVisible | gapi::BF_BindConstant);
    m_StaticMeshUniforms = gapi::allocate_buffer(sizeof(PerStaticMeshUniform), gapi::BF_CpuVisible | gapi::BF_BindConstant);

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
    renderWorld();
  }

  void WorldRender::beforeRender(const mat4& cameraVP)
  {
    m_FrameGC.nextFrame();
    m_FrameData.vp = cameraVP;
    updateFrameUniforms();
  }

  void WorldRender::updateFrameUniforms()
  {
    FrameUniforms uniforms;
    uniforms.viewProj = m_FrameData.vp;
    uniforms.secSinceStart = Time::get_sec_since_start();

    write_buffer(m_FrameUniforms, &uniforms, 0, sizeof(uniforms), gapi::WR_DISCARD);
    m_CmdEncoder.bindConstBuffer(m_FrameUniforms, DSET_PER_FRAME, 0);

    m_CmdEncoder.bindSampler(m_ModelSampler, DSET_PER_FRAME, 1);
  }

  void WorldRender::renderWorld()
  {
    gapi::transit_texture_state(gapi::get_backbuffer(), gapi::TextureState::Present, gapi::TextureState::RenderTarget);

    m_CmdEncoder.beginRenderpass(
      {
        gapi::RenderPassAttachment{gapi::get_backbuffer(), gapi::TextureState::RenderTarget, gapi::TextureState::Present}
      },
      gapi::RenderPassAttachment{m_RtDepth, gapi::TextureState::DepthWriteStencilRead, gapi::TextureState::DepthWriteStencilRead}
    );
    m_CmdEncoder.clear(gapi::CLEAR_RT | gapi::CLEAR_DEPTH);

    renderOpaque();

    m_CmdEncoder.present();
    m_CmdEncoder.flush();
  }

  void WorldRender::renderOpaque()
  {
    renderScene(RenderPassType::Main);
  }

  void WorldRender::renderScene(const RenderPassType rpType)
  {
    const auto objects = scene.queueObjects();
    for (const auto& obj: objects)
    {
      const mat4 rot = glm::rotate(obj.rot.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(obj.rot.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(obj.rot.x, float3{1.0, 0.0, 0.0});
      const mat4 scale = glm::scale(obj.scale);
      const mat4 tr = glm::translate(obj.pos);

      PerStaticMeshUniform meshUniforms;
      meshUniforms.modelTm = tr * scale * rot;

      auto meshConstBuf = m_FrameGC.allocate([](){
        return gapi::allocate_buffer(sizeof(PerStaticMeshUniform), gapi::BF_CpuVisible | gapi::BF_BindConstant);
      });

      write_buffer(meshConstBuf, &meshUniforms, 0, sizeof(meshUniforms), gapi::WR_DISCARD);
      m_CmdEncoder.bindConstBuffer(meshConstBuf, DSET_PER_MODEL, 0);

      ModelAsset* asset = assets_manager.getModel(obj.model);

      for(size_t i = 0; i < asset->mesh->submeshes.getSize(); ++i)
      {
        const Submesh& submesh = asset->mesh->submeshes.get(i);
        Material* material = asset->materials[i];
        material->setState(m_CmdEncoder, rpType);
        material->setParams(m_CmdEncoder);

        m_CmdEncoder.bindVertexBuffer(submesh.vertexBuffer);
        m_CmdEncoder.bindIndexBuffer(submesh.indexBuffer);
        m_CmdEncoder.drawIndexed(material->getTopology(), submesh.indexCount, 1, 0, 0, 0);
      }
    }
  }

}
