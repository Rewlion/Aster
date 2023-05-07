#include "nodes.h"

#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/world_render.h>
#include <engine/scene/scene.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>

namespace Engine::Render
{
  fg::NodeHandle mk_gbuffer_main_pass_node()
  {
    return fg::register_node("gbuffer_main_pass", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.orderMeAfter("frame_preparing");

      const int2 wndSize = Window::get_window_size();

      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::D24_UNORM_S8_UINT;
      allocDesc.extent = int3{wndSize.x, wndSize.y, 1};
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
      return [](gapi::CmdEncoder& encoder)
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
      };
    });
  }

  fg::NodeHandle mk_gbuffer_resolve_node()
  {
    return fg::register_node("gbuffer_resolve", FG_FILE_DECL, [](fg::Registry& reg)
    {
      const int2 wndSize = Window::get_window_size();

      gapi::TextureAllocationDescription allocDesc;
      allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
      allocDesc.extent = int3{wndSize.x, wndSize.y, 1};
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

      return [gbuf0,gbuf1,gbuf2,gbuf3](gapi::CmdEncoder& encoder)
      {
        tfx::set_extern("gbuffer_albedo", gbuf0.get());
        tfx::set_extern("gbuffer_normal", gbuf1.get());
        tfx::set_extern("gbuffer_world_pos", gbuf2.get());
        tfx::set_extern("gbuffer_metal_roughness", gbuf3.get());

        tfx::activate_technique("ResolveGbuffer", encoder);
        encoder.updateResources();
        encoder.draw(4, 1, 0, 0);
      };
    });
  }

  
}
