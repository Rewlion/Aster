#include "world_render.h"
#include "gui_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/blackboard.hpp>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/scene/scene.h>
#include <engine/settings.h>
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

    m_FontRender = std::make_unique<FontRender>();
    m_GuiRender = std::make_unique<GuiRender>(*m_FontRender.get());
    m_ImGuiRender = std::make_unique<ImGuiRender>();
    m_ImGuiRender->init();

    const DataBlock* settings = Engine::get_app_settings();
    const string fontFolder = settings->getText("fonts_folder");
    if (fontFolder != "")
    {
      const string font = fontFolder + "/arial.ttf";
      m_FontRender->init(font);
    }
    else
      logerror("world_render: missing font folder inside settings");

    
  }

  void WorldRender::render(const CameraData& cameraVP)
  {
    beforeRender(cameraVP);
    renderWorld();
  }

  void WorldRender::beforeRender(const CameraData& camera)
  {
    m_DbgTextQueue.tick();

    m_FrameId = (m_FrameId+1) % FRAMES_COUNT;
    m_FrameGraphs[m_FrameId] = fg::FrameGraph{};
    m_FrameData.fg = &m_FrameGraphs[m_FrameId];

    m_FrameData.cmdEncoder.reset(gapi::allocate_cmd_encoder());
    m_FrameData.camera = camera;

    gapi::Semaphore* sem = gapi::ackquire_backbuffer();
    m_FrameData.cmdEncoder->insertSemaphore(sem);

    m_FrameData.fg->importTexture("backbuffer", gapi::get_backbuffer(), gapi::TextureState::Present);

    tfx::set_extern("view_proj", m_FrameData.camera.viewProj);
    tfx::set_extern("camera_pos", m_FrameData.camera.pos);
    tfx::set_extern("sec_since_start", Time::get_sec_since_start());
    tfx::set_extern("model_sampler", m_ModelSampler);
    tfx::set_extern("viewport_size", float2(m_WindowSize.x, m_WindowSize.y));
    tfx::activate_scope("FrameScope", *m_FrameData.cmdEncoder);

    m_ImGuiRender->beforeRender(*m_FrameData.cmdEncoder);
  }

  void WorldRender::renderWorld()
  {
    m_FrameData.fg->addCallbackPass(
      "gbuffer_main_pass",
      [&](fg::RenderPassBuilder& builder){
        gapi::TextureAllocationDescription allocDesc;
        allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
        allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
        allocDesc.mipLevels = 1;
        allocDesc.arrayLayers = 1;
        allocDesc.usage = gapi::TEX_USAGE_DEPTH_STENCIL;

        builder.createTexture("gbuffer_depth", allocDesc);
        builder.write("gbuffer_depth", gapi::TextureState::DepthWriteStencilRead);

        allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
        allocDesc.usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_UNIFORM;
        builder.createTexture("gbuffer_albedo", allocDesc);
        builder.write("gbuffer_albedo", gapi::TextureState::RenderTarget);

        allocDesc.format = gapi::TextureFormat::R16G16B16A16_UNORM;
        builder.createTexture("gbuffer_normal", allocDesc);
        builder.write("gbuffer_normal", gapi::TextureState::RenderTarget);

        builder.createTexture("gbuffer_metal_roughness", allocDesc);
        builder.write("gbuffer_metal_roughness", gapi::TextureState::RenderTarget);

        allocDesc.format = gapi::TextureFormat::R32G32B32A32_S;
        builder.createTexture("gbuffer_world_pos", allocDesc);
        builder.write("gbuffer_world_pos", gapi::TextureState::RenderTarget);

        builder.addRenderTarget("gbuffer_albedo", gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget("gbuffer_normal", gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget("gbuffer_world_pos", gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.addRenderTarget("gbuffer_metal_roughness", gapi::LoadOp::Clear, gapi::StoreOp::Store);
        builder.setDepthStencil("gbuffer_depth", gapi::LoadOp::Clear, gapi::StoreOp::Store,
                                                 gapi::LoadOp::DontCare, gapi::StoreOp::DontCare);
      },
      [this](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        renderOpaque(encoder);
      }
    );

    m_FrameData.fg->addCallbackPass(
      "gbuffer_resolve",
      [&](fg::RenderPassBuilder& builder){
        gapi::TextureAllocationDescription allocDesc;
        allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
        allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
        allocDesc.mipLevels = 1;
        allocDesc.arrayLayers = 1;
        allocDesc.usage = gapi::TEX_USAGE_RT|gapi::TEX_USAGE_TRANSFER_SRC;
        
        builder.createTexture("final_frame", allocDesc);
        builder.write("final_frame", gapi::TextureState::RenderTarget);

        builder.read("gbuffer_albedo", gapi::TextureState::ShaderRead);
        builder.read("gbuffer_normal",  gapi::TextureState::ShaderRead);
        builder.read("gbuffer_metal_roughness", gapi::TextureState::ShaderRead);
        builder.read("gbuffer_world_pos", gapi::TextureState::ShaderRead);

        builder.addRenderTarget("final_frame", gapi::LoadOp::Load, gapi::StoreOp::Store);
      },
      [this](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        const auto albedo = resources.getTexture("gbuffer_albedo");
        const auto normal = resources.getTexture("gbuffer_normal");
        const auto worldPos = resources.getTexture("gbuffer_world_pos");
        const auto metalRoughness = resources.getTexture("gbuffer_metal_roughness");

        resolveGbuffer(encoder, albedo, normal, worldPos, metalRoughness);
      }
    );

    m_FrameData.fg->addCallbackPass(
      "ui",
      [&](fg::RenderPassBuilder& builder) {
        builder.write("final_frame", gapi::TextureState::RenderTarget);
        builder.addRenderTarget("final_frame", gapi::LoadOp::Load, gapi::StoreOp::Store);
      },
      [&](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        m_GuiRender->render(encoder);
        m_ImGuiRender->render(encoder);
      }
    );

    m_FrameData.fg->addCallbackPass(
      "dbg_text",
      [&](fg::RenderPassBuilder& builder) {
        builder.write("final_frame", gapi::TextureState::RenderTarget);
        builder.addRenderTarget("final_frame", gapi::LoadOp::Load, gapi::StoreOp::Store);
      },
      [&](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        renderDbgText(encoder);
      }
    );

    m_FrameData.fg->addCallbackPass(
      "present",
      [&](fg::RenderPassBuilder& builder) {
        builder.read("final_frame", gapi::TextureState::TransferSrc);
        builder.write("backbuffer", gapi::TextureState::TransferDst);
      },
      [&](const fg::RenderPassResources& resources, gapi::CmdEncoder& encoder) {
        const auto rt = resources.getTexture("final_frame");
        const auto bb = resources.getTexture("backbuffer");

        const auto region = gapi::TextureSubresourceLayers{
          .aspects = gapi::ASPECT_COLOR,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1
        };

        const auto blit = gapi::TextureBlit{
          .srcSubresource = region,
          .srcOffsets = {int3{0,0,0}, rt.desc.extent},
          .dstSubresource = region,
          .dstOffsets = {int3{0,0,0}, rt.desc.extent},
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
                                   const gapi::TextureHandle albedo, const gapi::TextureHandle normal,
                                   const gapi::TextureHandle worldPos, const gapi::TextureHandle metalRoughness)
  {
    tfx::set_extern("gbuffer_albedo", albedo);
    tfx::set_extern("gbuffer_normal", normal);
    tfx::set_extern("gbuffer_world_pos", worldPos);
    tfx::set_extern("gbuffer_metal_roughness", metalRoughness);

    tfx::activate_technique("ResolveGbuffer", encoder);
    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);

    m_FontRender->render("test_string", float2(200,50), 60.0, float4(1,1,1, 0.4f), encoder);
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

  void WorldRender::renderDbgText(gapi::CmdEncoder& encoder)
  {
    const float textSize = m_WindowSize.y * 0.03;
    float i = 0;
    for (const auto& e: m_DbgTextQueue)
    {
      const float2 pos = {5.0f, textSize * i};
      m_FontRender->render(e.text, pos, textSize, e.color, encoder);
      ++i;
    }
  }
}
