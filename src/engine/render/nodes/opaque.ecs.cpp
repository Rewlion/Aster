#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/scene/scene.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>

NODE_BEGIN(gbuffer_main_pass)
  ORDER_ME_AFTER(frame_preparing)

  CREATE_TEX_2D(opaque_depth, TEX_SIZE_RELATIVE(), D24_UNORM_S8_UINT,
                TEX_USAGE2(DEPTH_STENCIL, SRV), TEX_STATE(DepthWriteStencilWrite))

  CREATE_TEX_2D(gbuf0, TEX_SIZE_RELATIVE(), R8G8B8A8_UNORM,
                TEX_USAGE2(RT, SRV), TEX_STATE(RenderTarget))

  CREATE_TEX_2D(gbuf1, TEX_SIZE_RELATIVE(), R16G16B16A16_UNORM,
                TEX_USAGE2(RT, SRV), TEX_STATE(RenderTarget))

  CREATE_TEX_2D(gbuf2, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                TEX_USAGE2(RT, SRV), TEX_STATE(RenderTarget))

  CREATE_TEX_2D(motionBuf, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                TEX_USAGE2(RT, SRV), TEX_STATE(RenderTarget))

  RP_BEGIN()
    TARGET_CLEARED(gbuf0, UCLEAR(0)),
    TARGET_CLEARED(gbuf1, UCLEAR(0)),
    TARGET_CLEARED(gbuf2, UCLEAR(0)),
    TARGET_CLEARED(motionBuf, UCLEAR(0)),
    DEPTH_RW_CLEARED(opaque_depth)
  RP_END()

  EXEC(gbuffer_main_pass_exec)
NODE_END()

NODE_EXEC()
static
void gbuffer_main_pass_exec(gapi::CmdEncoder& encoder)
{
  const auto objects = Engine::scene.queueObjects();
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

    Engine::ModelAsset* asset = Engine::assets_manager.getModel(obj.model);

    for(size_t i = 0; i < asset->mesh->submeshes.getSize(); ++i)
    {
      const Engine::Submesh& submesh = asset->mesh->submeshes.get(i);
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

NODE_BEGIN(late_opaque_sync)
  RENAME_TEX(opaque_depth, late_opaque_depth, TEX_STATE(DepthReadStencilRead))
  NO_EXEC()
NODE_END()

NODE_BEGIN(gbuffer_resolve)
  CREATE_TEX_2D(resolve_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))
  READ_TEX(gbuf0, TEX_STATE(ShaderRead))
  READ_TEX(gbuf1, TEX_STATE(ShaderRead))
  READ_TEX(gbuf2, TEX_STATE(ShaderRead))
  RENAME_TEX(late_opaque_depth, gbuffer_depth, TEX_STATE(DepthReadStencilRead))
  READ_BUF(sph_buf, BUF_STATE(SRV))
  READ_TEX_OPTIONAL(atm_envi_specular, TEX_STATE(ShaderRead))
  READ_TEX_OPTIONAL(atm_envi_brdf, TEX_STATE(ShaderRead))

  RP_BEGIN()
    TARGET_CLEARED(resolve_target, UCLEAR(0))
  RP_END()

  EXEC(gbuffer_resolve_exec)
NODE_END()

NODE_EXEC()
static
void gbuffer_resolve_exec(gapi::CmdEncoder& encoder,
                          const gapi::TextureHandle gbuf0,
                          const gapi::TextureHandle gbuf1,
                          const gapi::TextureHandle gbuf2,
                          const gapi::TextureHandle gbuffer_depth,
                          const gapi::BufferHandler sph_buf,
                          const gapi::TextureHandle atm_envi_specular,
                          const gapi::TextureHandle atm_envi_brdf)
{
  tfx::set_extern("gbuffer_albedo", gbuf0);
  tfx::set_extern("gbuffer_normal", gbuf1);
  tfx::set_extern("gbuffer_metal_roughness", gbuf2);
  tfx::set_extern("gbuffer_depth", gbuffer_depth);
  tfx::set_extern("atmParamsBuffer", sph_buf);
  tfx::set_extern("enviSpecular", atm_envi_specular);
  tfx::set_extern("enviBRDF", atm_envi_brdf);

  tfx::activate_technique("ResolveGbuffer", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(transparent_sync)
  RENAME_TEX(resolve_target, transparent_target, TEX_STATE(RenderTarget))
  NO_EXEC()
NODE_END()
