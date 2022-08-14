#include "world_render.h"
#include "blackboard_components.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/frame_graph/blackboard.hpp>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/scene/scene.h>
#include <engine/time.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

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
    m_FrameId = (m_FrameId+1) % FRAMES_COUNT;
    m_FrameGraphs[m_FrameId] = fg::FrameGraph{};
    m_FrameData.fg = &m_FrameGraphs[m_FrameId];
    m_FrameData.blackboard = fg::Blackboard{};

    m_FrameData.cmdEncoder.reset(gapi::allocate_cmd_encoder());
    m_FrameData.camera = camera;
    m_FrameData.cmdEncoder->insertSemaphore(gapi::ackquire_backbuffer());

    tfx::set_extern("view_proj", m_FrameData.camera.viewProj);
    tfx::set_extern("camera_pos", m_FrameData.camera.pos);
    tfx::set_extern("sec_since_start", Time::get_sec_since_start());
    tfx::set_extern("model_sampler", m_ModelSampler);
    tfx::set_extern("viewport_size", float2(m_WindowSize.x, m_WindowSize.y));
    tfx::activate_scope("FrameScope", m_FrameData.cmdEncoder.get());
  }

  void WorldRender::renderWorld()
  {
    m_FrameData.blackboard.add<blackboard::Gbuffer>() = m_FrameData.fg->addCallbackPass<blackboard::Gbuffer>(
      "gbuffer_main_pass",
      [&](fg::RenderPassBuilder& builder, blackboard::Gbuffer& data){
        gapi::TextureAllocationDescription allocDesc;
        allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
        allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
        allocDesc.mipLevels = 1;
        allocDesc.arrayLayers = 1;
        allocDesc.usage = gapi::TextureUsage::DepthStencil;

        data.depth = builder.createTexture("gbuffer_depth", allocDesc);
        data.depth = builder.write(data.depth);

        allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
        allocDesc.usage = (gapi::TextureUsage)((uint32_t)gapi::TextureUsage::RenderTarget | (uint32_t)gapi::TextureUsage::Uniform);
        data.albedo = builder.createTexture("gbuffer_albedo", allocDesc);
        data.albedo = builder.write(data.albedo);

        data.normal = builder.createTexture("gbuffer_normal", allocDesc);
        data.normal = builder.write(data.normal);

        data.metalRoughness = builder.createTexture("gbuffer_metal_roughness", allocDesc);
        data.metalRoughness = builder.write(data.metalRoughness);

        allocDesc.format = gapi::TextureFormat::R32G32B32A32_S;
        data.worldPos = builder.createTexture("gbuffer_world_pos", allocDesc);
        data.worldPos = builder.write(data.worldPos);
      },
      [this](const blackboard::Gbuffer& data, const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        renderOpaque(data, resources);
      }
    );

    m_FrameData.fg->compile();
    m_FrameData.fg->execute(*m_FrameData.cmdEncoder);
    m_FrameData.cmdEncoder->flush();
    gapi::present_backbuffer();
  }

  void WorldRender::renderOpaque(const blackboard::Gbuffer& gbuffer, const fg::RenderPassResources& resources)
  {
    const gapi::TextureHandler depth = resources.getTexture(gbuffer.depth);
    const gapi::TextureHandler albedo = resources.getTexture(gbuffer.albedo);
    const gapi::TextureHandler normal = resources.getTexture(gbuffer.normal);
    const gapi::TextureHandler worldPos = resources.getTexture(gbuffer.worldPos);
    const gapi::TextureHandler metalRoughness = resources.getTexture(gbuffer.metalRoughness);
    m_FrameData.cmdEncoder->transitTextureState(depth, gapi::TextureState::Undefined, gapi::TextureState::DepthWriteStencilRead);
    m_FrameData.cmdEncoder->transitTextureState(albedo, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
    m_FrameData.cmdEncoder->transitTextureState(normal, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
    m_FrameData.cmdEncoder->transitTextureState(worldPos, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);
    m_FrameData.cmdEncoder->transitTextureState(metalRoughness, gapi::TextureState::Undefined, gapi::TextureState::RenderTarget);

    m_FrameData.cmdEncoder->beginRenderpass(
      {
        gapi::RenderPassAttachment{albedo, gapi::TextureState::RenderTarget, gapi::TextureState::RenderTarget},
        gapi::RenderPassAttachment{normal, gapi::TextureState::RenderTarget, gapi::TextureState::RenderTarget},
        gapi::RenderPassAttachment{worldPos, gapi::TextureState::RenderTarget, gapi::TextureState::RenderTarget},
        gapi::RenderPassAttachment{metalRoughness, gapi::TextureState::RenderTarget, gapi::TextureState::RenderTarget},
      },
      gapi::RenderPassAttachment{depth, gapi::TextureState::DepthWriteStencilRead, gapi::TextureState::DepthWriteStencilRead},
      (gapi::ClearState)(gapi::CLEAR_RT | gapi::CLEAR_DEPTH)
    );

    renderScene();
    m_FrameData.cmdEncoder->endRenderpass();

    m_FrameData.cmdEncoder->transitTextureState(albedo, gapi::TextureState::RenderTarget, gapi::TextureState::ShaderRead);
    m_FrameData.cmdEncoder->transitTextureState(normal, gapi::TextureState::RenderTarget, gapi::TextureState::ShaderRead);
    m_FrameData.cmdEncoder->transitTextureState(worldPos, gapi::TextureState::RenderTarget, gapi::TextureState::ShaderRead);
    m_FrameData.cmdEncoder->transitTextureState(metalRoughness, gapi::TextureState::RenderTarget, gapi::TextureState::ShaderRead);

    m_FrameData.cmdEncoder->transitTextureState(gapi::get_backbuffer(), gapi::TextureState::Present, gapi::TextureState::RenderTarget);
    m_FrameData.cmdEncoder->beginRenderpass(
      {
        gapi::RenderPassAttachment{gapi::get_backbuffer(), gapi::TextureState::RenderTarget, gapi::TextureState::Present},
      },
      {},
      gapi::ClearState::CLEAR_NONE
    );

    tfx::set_extern("gbuffer_albedo", albedo);
    tfx::set_extern("gbuffer_normal", normal);
    tfx::set_extern("gbuffer_world_pos", worldPos);
    tfx::set_extern("gbuffer_metal_roughness", metalRoughness);

    tfx::activate_scope("ProcessGbufferScope", m_FrameData.cmdEncoder.get());
    tfx::activate_technique("ProcessGbuffer", m_FrameData.cmdEncoder.get());
    m_FrameData.cmdEncoder->updateResources();
    m_FrameData.cmdEncoder->draw(4, 1, 0, 0);

    m_FrameData.cmdEncoder->endRenderpass();

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
