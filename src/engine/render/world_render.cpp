#include "world_render.h"
#include "gui_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/font/font_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/render/frame_graph/v2/frame_graph.h>
#include <engine/scene/scene.h>
#include <engine/settings.h>
#include <engine/time.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::initFrameGraph()
  {
    fg2::register_node("backbuffer_acquiring", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      return [](gapi::CmdEncoder& encoder)
      {
        gapi::acquire_backbuffer();
      };
    });

    fg2::register_node("frame_preparing", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      reg.orderMeAfter("backbuffer_acquiring");
      auto cameraData = reg.createBlob<Engine::CameraData>("camera_data");
      auto wndSize = reg.createBlob<int2>("window_size");

      reg.importTextureProducer("backbuffer", []()
      {
        return fg2::TextureImport{gapi::get_backbuffer(), gapi::TextureState::Present};
      });

      return [cameraData, wndSize, this](gapi::CmdEncoder& encoder)
      {
        cameraData.get() = this->m_FrameData.camera;
        wndSize.get() = this->m_WindowSize;

        this->m_DbgTextQueue.tick();

        tfx::set_extern("view_proj", cameraData->viewProj);
        tfx::set_extern("camera_pos", cameraData->pos);
        tfx::set_extern("sec_since_start", Time::get_sec_since_start());
        tfx::set_extern("model_sampler", this->m_ModelSampler);
        tfx::set_extern("viewport_size", float2(wndSize->x, wndSize->y));
        tfx::activate_scope("FrameScope", encoder);

        this->m_ImGuiRender->beforeRender(encoder);
      };
    });

    fg2::register_node("gbuffer_main_pass", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      reg.orderMeAfter("frame_preparing");

      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
      allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
      allocDesc.mipLevels = 1;
      allocDesc.arrayLayers = 1;
      allocDesc.usage = gapi::TEX_USAGE_DEPTH_STENCIL;

      auto gbufDepth = reg.createTexture("gbuffer_depth", allocDesc, gapi::TextureState::DepthWriteStencilRead);

      allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
      allocDesc.usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_UNIFORM;
      auto gbuf0 = reg.createTexture("gbuf0", allocDesc, gapi::TextureState::RenderTarget);

      allocDesc.format = gapi::TextureFormat::R16G16B16A16_UNORM;
      auto gbuf1 = reg.createTexture("gbuf1", allocDesc, gapi::TextureState::RenderTarget);
      auto gbuf2 = reg.createTexture("gbuf2", allocDesc, gapi::TextureState::RenderTarget);

      allocDesc.format = gapi::TextureFormat::R32G32B32A32_S;
      auto gbuf3 = reg.createTexture("gbuf3", allocDesc, gapi::TextureState::RenderTarget);

      reg.requestRenderPass()
         .addTarget(gbuf0,  gapi::LoadOp::Clear, gapi::StoreOp::Store)
         .addTarget(gbuf1,  gapi::LoadOp::Clear, gapi::StoreOp::Store)
         .addTarget(gbuf2,  gapi::LoadOp::Clear, gapi::StoreOp::Store)
         .addTarget(gbuf3,  gapi::LoadOp::Clear, gapi::StoreOp::Store)
         .addDepth(gbufDepth, gapi::LoadOp::Clear, gapi::StoreOp::Store,
                              gapi::LoadOp::DontCare, gapi::StoreOp::DontCare);
      return [this](gapi::CmdEncoder& encoder)
      {
        renderOpaque(encoder);
      };
    });

    fg2::register_node("gbuffer_resolve", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
      allocDesc.extent = int3{m_WindowSize.x, m_WindowSize.y, 1};
      allocDesc.mipLevels = 1;
      allocDesc.arrayLayers = 1;
      allocDesc.usage = gapi::TEX_USAGE_RT|gapi::TEX_USAGE_TRANSFER_SRC;

      auto finalFrame = reg.createTexture("final_frame", allocDesc, gapi::TextureState::RenderTarget);
      auto gbuf0 = reg.readTexture("gbuf0", gapi::TextureState::ShaderRead);
      auto gbuf1 = reg.readTexture("gbuf1", gapi::TextureState::ShaderRead);
      auto gbuf2 = reg.readTexture("gbuf2", gapi::TextureState::ShaderRead);
      auto gbuf3 = reg.readTexture("gbuf3", gapi::TextureState::ShaderRead);

      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);

      return [this,gbuf0,gbuf1,gbuf2,gbuf3](gapi::CmdEncoder& encoder)
      {
        resolveGbuffer(encoder, gbuf0.get(), gbuf1.get(), gbuf2.get(), gbuf3.get());
      };
    });

    fg2::register_node("ui", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      auto finalFrame = reg.modifyTexture("final_frame", gapi::TextureState::RenderTarget);
      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);
      return [this](gapi::CmdEncoder& encoder)
      {
        m_GuiRender->render(encoder);
        m_ImGuiRender->render(encoder);
      };
    });

    fg2::register_node("dbg_text", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      reg.orderMeAfter("ui");
      auto finalFrame = reg.modifyTexture("final_frame", gapi::TextureState::RenderTarget);
      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);
      return [this](gapi::CmdEncoder& encoder)
      {
        renderDbgText(encoder);
      };
    });

    fg2::register_node("present", FG_FILE_DECL, [this](fg2::Registry& reg)
    {
      auto finalFrame = reg.readTexture("final_frame", gapi::TextureState::TransferSrc);
      auto backbuffer = reg.modifyTexture("backbuffer", gapi::TextureState::TransferDst);
      return [finalFrame, backbuffer](gapi::CmdEncoder& encoder)
      {
        const auto region = gapi::TextureSubresourceLayers{
          .aspects = gapi::ASPECT_COLOR,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1
        };

        const uint3 finalFrameExtent = finalFrame.describe().extent;

        const auto blit = gapi::TextureBlit{
          .srcSubresource = region,
          .srcOffsets = {int3{0,0,0}, finalFrameExtent},
          .dstSubresource = region,
          .dstOffsets = {int3{0,0,0}, finalFrameExtent},
        };

        encoder.blitTexture(finalFrame.get(), backbuffer.get(), 1, &blit, gapi::ImageFilter::Nearest);
        encoder.transitTextureState(backbuffer.get(), gapi::TextureState::TransferDst, gapi::TextureState::Present);
      };
    });

    fg2::set_closing_node("present");
  }

  void WorldRender::init()
  {
    fg2::init();
    initFrameGraph();

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
    m_FrameData.camera = cameraVP;
    fg2::exec_new_frame();
    gapi::present_backbuffer_and_finalize_frame();
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
