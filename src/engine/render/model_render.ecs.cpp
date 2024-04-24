#include "model_render.h"

#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/scene/scene.h>
#include <engine/components/static_mesh.h>

#include <glm/gtx/transform.hpp>

ECS_DESCRIBE_QUERY(query_static_mesh, const StaticMeshComponent& static_mesh);

namespace
{
  void render_model(gapi::CmdEncoder& encoder,
                    const string_view model_name,
                    const float4x4& model_tm)
  {
    const float4x4 normalTm = glm::transpose(glm::inverse(model_tm));
    tfx::set_channel("model_obj_tm", model_tm);
    tfx::set_channel("normal_obj_tm", normalTm);

    const Engine::ModelAsset* asset = Engine::assets_manager.getModel(model_name);

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
  {
    const mat4 rotTm = glm::rotate(obj.rot.z, float3{0.0, 0.0, 1.0}) *
                       glm::rotate(obj.rot.y, float3{0.0, 1.0, 0.0}) *
                       glm::rotate(obj.rot.x, float3{1.0, 0.0, 0.0});
    const mat4 scaleTm = glm::scale(obj.scale);
    const mat4 trTm = glm::translate(obj.pos);
    const float4x4 modelTm = trTm * scaleTm * rotTm;

    render_model(encoder, obj.model, modelTm);
  }

  query_static_mesh([&encoder](const StaticMeshComponent& c) {
    render_model(encoder, c.getModelName(), c.getWorldTransform());
  });
}