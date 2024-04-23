#include "model_render.h"

#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/scene/scene.h>
#include <engine/components/static_mesh.h>

#include <glm/gtx/transform.hpp>

ECS_DESCRIBE_QUERY(query_dynamic_mesh,
  (const string& test_static_mesh_model,
   const float3& test_static_mesh_pos,
   const float3& test_static_mesh_rot,
   const float3& test_static_mesh_scale));

ECS_DESCRIBE_QUERY(query_static_mesh, const StaticMeshComponent& static_mesh);

namespace
{
  //deprecated TODO:remove
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
    tfx::set_channel("model_obj_tm", modelTm);
    tfx::set_channel("normal_obj_tm", normalTm);

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

  void render_static_mesh(gapi::CmdEncoder& encoder,
                          const StaticMeshComponent& mesh)
  {
    const float4x4 modelTm = mesh.getWorldTransform();
    const float4x4 normalTm = glm::transpose(glm::inverse(modelTm));
    tfx::set_channel("model_obj_tm", modelTm);
    tfx::set_channel("normal_obj_tm", normalTm);

    Engine::ModelAsset* asset = Engine::assets_manager.getModel(string{mesh.getModelName()});

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

void render_models(gapi::CmdEncoder& encoder, const float4x4& view_proj, const ModelPass pass)
{
  tfx::set_extern("depthPrepass", pass == ModelPass::Depth ? (uint)1: uint(0));
  tfx::set_extern("viewProj_obj_tm", view_proj);

  const auto objects = Engine::scene.queueObjects();
  for (const auto& obj: objects)
    render_model(encoder, obj.model, obj.pos, obj.scale, obj.rot);

  query_dynamic_mesh([&encoder](const string& model, const float3& pos, const float3& rot, const float3& scale) {
    render_model(encoder, model, pos, scale, rot);
  });

  query_static_mesh([&encoder](const StaticMeshComponent& c) {
    render_static_mesh(encoder, c);
  });
}