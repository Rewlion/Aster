#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/scene/scene.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>

#include <glm/gtx/transform.hpp>

ECS_DESCRIBE_QUERY(query_dynamic_mesh,
  (const string& test_static_mesh_model,
   const float3& test_static_mesh_pos,
   const float3& test_static_mesh_rot,
   const float3& test_static_mesh_scale));

namespace
{
  void render_model(gapi::CmdEncoder& encoder,
                    const string& model,
                    const float3& pos,
                    const float3& scale,
                    const float3& rotation)
  {
    const mat4 rotTm = glm::rotate(rotation.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(rotation.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(rotation.x, float3{1.0, 0.0, 0.0});
    const mat4 scaleTm = glm::scale(scale);
    const mat4 trTm = glm::translate(pos);

    const float4x4 modelTm = trTm * scaleTm * rotTm;
    const float4x4 normalTm = glm::transpose(glm::inverse(modelTm));
    tfx::set_channel("model_tm", modelTm);
    tfx::set_channel("normal_tm", normalTm);

    Engine::ModelAsset* asset = Engine::assets_manager.getModel(model);

    for(size_t i = 0; i < asset->mesh->gpuSubmeshes.count(); ++i)
    {
      const Engine::GpuSubmesh& submesh = asset->mesh->gpuSubmeshes.get(i);
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
    render_model(encoder, obj.model, obj.pos, obj.scale, obj.rot);

  query_dynamic_mesh([&encoder](const string& model, const float3& pos, const float3& rot, const float3& scale) {
    render_model(encoder, model, pos, scale, rot);
  });
}

NODE_BEGIN(late_opaque_sync)
  RENAME_TEX_RO_DEPTH(opaque_depth, late_opaque_depth)
  NO_EXEC()
NODE_END()

NODE_BEGIN(gbuffer_resolve)
  CREATE_TEX_2D(resolve_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE3(RT,UAV,SRV), TEX_STATE(ShaderReadWrite))
  BIND_SHADER_VAR_AS(resolve_target, resolveTarget)

  READ_RENDER_SIZE_AS(render_size)

  BIND_TEX_SRV_AS(gbuf0, gbuffer_albedo)
  BIND_TEX_SRV_AS(gbuf1, gbuffer_normal)
  BIND_TEX_SRV_AS(gbuf2, gbuffer_metal_roughness)

  BIND_PREV_FRAME_TEX_SRV_AS(post_process_input, prevPostProcessInput)
  BIND_TEX_SRV_AS(motionBuf, motionBuf)

  BIND_BUF_SRV_AS(clustered_lights, clustersLightBuffer)
  BIND_BUF_SRV_AS(clusters_info, clustersInfoBuffer)
  BIND_BUF_SRV_AS(clusters_indirecion, clustersIndirectionBuffer)

  RENAME_TEX_RO_DEPTH(late_opaque_depth, gbuffer_depth)
  BIND_SHADER_VAR_AS(gbuffer_depth, gbuffer_depth)

  BIND_BUF_SRV_AS(sph_buf, atmParamsBuffer)

  BIND_TEX_SRV_AS(specular_light, specularLight)

  BIND_BUF_SRV_AS(gibs_surfels_storage_srv, surfelsStorage)
  BIND_BUF_SRV_AS(gibs_surfels_spatial_storage_srv, surfelsSpatialStorage)

  EXEC(gbuffer_resolve_exec)
NODE_END()

NODE_EXEC()
static
void gbuffer_resolve_exec(gapi::CmdEncoder& encoder,
                          const uint2& render_size)
{
  tfx::activate_scope("AtmosphereScope", encoder);
  tfx::activate_technique("ResolveGbuffer", encoder);
  encoder.updateResources();

  const auto getGroupSize = [](uint render_dim, uint tile_dim) {
    uint add = render_dim % tile_dim > 0 ? 1 : 0;
    return render_dim / tile_dim + add;
  };

  const uint3 gc = tfx::calc_group_count("ResolveGbuffer", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, 1);
}

NODE_BEGIN(post_opaque_sync)
  RENAME_TEX_RT(resolve_target, post_opaque_target)
  NO_EXEC()
NODE_END()


NODE_BEGIN(transparent_sync)
  RENAME_TEX_RT(taa_target, transparent_target)
  NO_EXEC()
NODE_END()
