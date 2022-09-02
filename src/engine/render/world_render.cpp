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

    gapi::Semaphore* sem = gapi::ackquire_backbuffer();
    m_FrameData.cmdEncoder->insertSemaphore(sem);

    blackboard::Frame& fdata = m_FrameData.blackboard.add<blackboard::Frame>();
    fdata.backbuffer = m_FrameData.fg->importTexture("backbuffer", gapi::get_backbuffer(), gapi::TextureState::Present);

    tfx::set_extern("view_proj", m_FrameData.camera.viewProj);
    tfx::set_extern("camera_pos", m_FrameData.camera.pos);
    tfx::set_extern("sec_since_start", Time::get_sec_since_start());
    tfx::set_extern("model_sampler", m_ModelSampler);
    tfx::set_extern("viewport_size", float2(m_WindowSize.x, m_WindowSize.y));
    tfx::activate_scope("FrameScope", *m_FrameData.cmdEncoder);
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
        allocDesc.usage = gapi::TEX_USAGE_DEPTH_STENCIL;

        data.depth = builder.createTexture("gbuffer_depth", allocDesc);
        data.depth = builder.write(data.depth, gapi::TextureState::DepthWriteStencilRead);

        allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
        allocDesc.usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_UNIFORM;
        data.albedo = builder.createTexture("gbuffer_albedo", allocDesc);
        data.albedo = builder.write(data.albedo, gapi::TextureState::RenderTarget);

        allocDesc.format = gapi::TextureFormat::R16G16B16A16_UNORM;
        data.normal = builder.createTexture("gbuffer_normal", allocDesc);
        data.normal = builder.write(data.normal,  gapi::TextureState::RenderTarget);

        data.metalRoughness = builder.createTexture("gbuffer_metal_roughness", allocDesc);
        data.metalRoughness = builder.write(data.metalRoughness, gapi::TextureState::RenderTarget);

        allocDesc.format = gapi::TextureFormat::R32G32B32A32_S;
        data.worldPos = builder.createTexture("gbuffer_world_pos", allocDesc);
        data.worldPos = builder.write(data.worldPos, gapi::TextureState::RenderTarget);

        builder.addRenderTarget(data.albedo, gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget(data.normal, gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget(data.worldPos, gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget(data.metalRoughness, gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.setDepthStencil(data.depth, gapi::LoadOp::Clear, gapi::StoreOp::Store,
                                            gapi::LoadOp::DontCare, gapi::StoreOp::DontCare);
      },
      [this](const blackboard::Gbuffer& data, const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        renderOpaque(encoder);
      }
    );

    m_FrameData.blackboard.add<blackboard::FinalFrame>() = m_FrameData.fg->addCallbackPass<blackboard::FinalFrame>(
      "gbuffer_resolve",
      [&](fg::RenderPassBuilder& builder, blackboard::FinalFrame& final_frame){
        gapi::TextureAllocationDescription allocDesc;
        allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
        allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
        allocDesc.mipLevels = 1;
        allocDesc.arrayLayers = 1;
        allocDesc.usage = gapi::TEX_USAGE_RT|gapi::TEX_USAGE_TRANSFER_SRC;
        final_frame.finalRt = builder.createTexture("final_frame", allocDesc);
        final_frame.finalRt = builder.write(final_frame.finalRt, gapi::TextureState::RenderTarget);

        blackboard::Gbuffer& gbuffer = m_FrameData.blackboard.get<blackboard::Gbuffer>();
        builder.read(gbuffer.albedo, gapi::TextureState::ShaderRead);
        builder.read(gbuffer.normal,  gapi::TextureState::ShaderRead);
        builder.read(gbuffer.metalRoughness, gapi::TextureState::ShaderRead);
        builder.read(gbuffer.worldPos, gapi::TextureState::ShaderRead);

        builder.addRenderTarget(final_frame.finalRt, gapi::LoadOp::Load, gapi::StoreOp::Store);
      },
      [this](const blackboard::FinalFrame& final_frame, const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        blackboard::Gbuffer& gbuffer = m_FrameData.blackboard.get<blackboard::Gbuffer>();

        const gapi::TextureHandler albedo = resources.getTexture(gbuffer.albedo);
        const gapi::TextureHandler normal = resources.getTexture(gbuffer.normal);
        const gapi::TextureHandler worldPos = resources.getTexture(gbuffer.worldPos);
        const gapi::TextureHandler metalRoughness = resources.getTexture(gbuffer.metalRoughness);

        resolveGbuffer(encoder, albedo, normal, worldPos, metalRoughness);
      }
    );

    m_FrameData.fg->addCallbackPass(
      "present",
      [&](fg::RenderPassBuilder& builder) {
        auto& finalFrame = m_FrameData.blackboard.get<blackboard::FinalFrame>();
        auto& frame = m_FrameData.blackboard.get<blackboard::Frame>();
        builder.read(finalFrame.finalRt, gapi::TextureState::TransferSrc);
        frame.backbuffer = builder.write(frame.backbuffer, gapi::TextureState::TransferDst);
      },
      [&](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        const auto& finalFrame = m_FrameData.blackboard.get<blackboard::FinalFrame>();
        const auto& frame = m_FrameData.blackboard.get<blackboard::Frame>();
        gapi::TextureHandler rt = resources.getTexture(finalFrame.finalRt);
        gapi::TextureHandler bb = resources.getTexture(frame.backbuffer);

        const auto region = gapi::TextureSubresourceLayers{
          .aspects = gapi::ASPECT_COLOR,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1
        };

        const auto blit = gapi::TextureBlit{
          .srcSubresource = region,
          .srcOffsets = {int3{0,0,0}, int3{m_WindowSize.x, m_WindowSize.y ,1}},
          .dstSubresource = region,
          .dstOffsets = {int3{0,0,0}, int3{m_WindowSize.x, m_WindowSize.y ,1}},
        };

        encoder.blitTexture(rt, bb, 1, &blit, gapi::ImageFilter::Nearest);
        encoder.transitTextureState(bb, gapi::TextureState::TransferDst, gapi::TextureState::Present);
      }
    );

    m_FrameData.fg->compile();
    m_FrameData.fg->execute(*m_FrameData.cmdEncoder);

    gapi::Semaphore* sem = m_FrameData.cmdEncoder->signalSemaphore();
    m_FrameData.cmdEncoder->flush();
    m_FrameData.cmdEncoder->present(sem);
    gapi::next_frame();
  }

  void WorldRender::renderOpaque(gapi::CmdEncoder& encoder)
  {
    renderScene(encoder);
  }

  void WorldRender::resolveGbuffer(gapi::CmdEncoder& encoder,
                                   const gapi::TextureHandler albedo, const gapi::TextureHandler normal,
                                   const gapi::TextureHandler worldPos, const gapi::TextureHandler metalRoughness)
  {
    tfx::set_extern("gbuffer_albedo", albedo);
    tfx::set_extern("gbuffer_normal", normal);
    tfx::set_extern("gbuffer_world_pos", worldPos);
    tfx::set_extern("gbuffer_metal_roughness", metalRoughness);

    tfx::activate_technique("ResolveGbuffer", encoder);
    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);
  }

  void WorldRender::renderScene(gapi::CmdEncoder& encoder)
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

        tfx::activate_technique(material.technique, encoder);

        for (const auto& m: material.params)
          tfx::set_channel(m.name, m.value);

        tfx::activate_scope("StaticModelScope", encoder);
        encoder.updateResources();

        encoder.bindVertexBuffer(submesh.vertexBuffer);
        encoder.bindIndexBuffer(submesh.indexBuffer);

        encoder.drawIndexed(submesh.indexCount, 1, 0, 0, 0);
      }
    }
  }

}
