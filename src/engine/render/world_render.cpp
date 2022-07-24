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
  }

  void WorldRender::render(const CameraData& cameraVP)
  {
    beforeRender(cameraVP);
    renderWorld();
  }

  void WorldRender::beforeRender(const CameraData& camera)
  {
    m_FrameGC.nextFrame();
    m_FrameData.cmdEncoder.reset(gapi::allocate_cmd_encoder());
    m_FrameData.camera = camera;
    m_FrameData.depth = m_FrameGC.allocate([this](){
      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
      allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
      allocDesc.mipLevels = 1;
      allocDesc.arrayLayers = 1;
      allocDesc.usage = gapi::TextureUsage::DepthStencil;
      return gapi::allocate_texture(allocDesc);
    });
    m_FrameData.cmdEncoder->transitTextureState(m_FrameData.depth, gapi::TextureState::Undefined, gapi::TextureState::DepthWriteStencilRead);
    m_FrameData.cmdEncoder->insertSemaphore(gapi::ackquire_backbuffer());

    tfx::set_extern("view_proj", m_FrameData.camera.viewProj);
    tfx::set_extern("camera_pos", m_FrameData.camera.pos);
    tfx::set_extern("sec_since_start", Time::get_sec_since_start());
    tfx::activate_scope("FrameScope", m_FrameData.cmdEncoder.get());
    //m_CmdEncoder.bindSampler(m_ModelSampler, DSET_PER_FRAME, 1);
  }

  void WorldRender::renderWorld()
  {
    m_FrameData.cmdEncoder->transitTextureState(gapi::get_backbuffer(), gapi::TextureState::Present, gapi::TextureState::RenderTarget);

    m_FrameData.cmdEncoder->beginRenderpass(
      {
        gapi::RenderPassAttachment{gapi::get_backbuffer(), gapi::TextureState::RenderTarget, gapi::TextureState::Present}
      },
      gapi::RenderPassAttachment{m_FrameData.depth, gapi::TextureState::DepthWriteStencilRead, gapi::TextureState::DepthWriteStencilRead},
      (gapi::ClearState)(gapi::CLEAR_RT | gapi::CLEAR_DEPTH)
    );

    renderOpaque();

    m_FrameData.cmdEncoder->endRenderpass();
    m_FrameData.cmdEncoder->flush();
    gapi::present_backbuffer();
  }

  void WorldRender::renderOpaque()
  {
    renderScene();
  }

  void WorldRender::renderScene()
  {
    const auto objects = scene.queueObjects();
    for (const auto& obj: objects)
    {
      const mat4 rot = glm::rotate(obj.rot.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(obj.rot.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(obj.rot.x, float3{1.0, 0.0, 0.0});
      const mat4 scale = glm::scale(obj.scale);
      const mat4 tr = glm::translate(obj.pos);

      const float4x4 modelTm = tr * scale * rot;
      const float4x4 normalTm = glm::transpose(glm::inverse(modelTm));
      tfx::set_channel("model_tm", modelTm);
      tfx::set_channel("normal_tm", normalTm);

      ModelAsset* asset = assets_manager.getModel(obj.model);

      for(size_t i = 0; i < asset->mesh->submeshes.getSize(); ++i)
      {
        const Submesh& submesh = asset->mesh->submeshes.get(i);
        const tfx::Material& material = asset->materials[i];

        tfx::activate_technique(material.technique, m_FrameData.cmdEncoder.get());

        for (const auto& m: material.params)
          tfx::set_channel(m.name, m.value);

        tfx::activate_scope("StaticModelScope", m_FrameData.cmdEncoder.get());
        m_FrameData.cmdEncoder->updateResources();

         m_FrameData.cmdEncoder->bindVertexBuffer(submesh.vertexBuffer);
         m_FrameData.cmdEncoder->bindIndexBuffer(submesh.indexBuffer);
         m_FrameData.cmdEncoder->drawIndexed(submesh.indexCount, 1, 0, 0, 0);
      }
    }
  }

}
