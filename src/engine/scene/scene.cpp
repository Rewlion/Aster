#include "scene.h"

#include "object_utils.h"

#include <engine/assets/assets_manager.h>
#include <engine/data/ed.h>
#include <engine/math.h>

#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>

namespace Engine
{
  Scene scene;

  static float3 to_radians(float3 angles)
  {
    angles.x = math::radians(angles.x);
    angles.y = math::radians(angles.y);
    angles.z = math::radians(angles.z);

    return angles;
  }

  void Scene::loadScene(const ed::Scope& scene)
  {
    m_SceneObjects.clear();

    for (const auto& obj: scene.getScopes())
    {
      if (obj.getName() == "object")
      {
        SceneObject newObj;
        newObj.model = obj.getVariable<string_view>("model");
        newObj.pos = obj.getVariable<float3>("pos");
        newObj.scale = obj.getVariableOr<float3>("scale", {1, 1, 1});
        newObj.rot = to_radians(obj.getVariableOr<float3>("rot", {0, 0, 0}));
        m_SceneObjects.push_back(std::move(newObj));
      }
    }

    rebuildBLAS();
    rebuildTLAS();
  }

  eastl::vector<SceneObject> Scene::queueObjects() const
  {
    return m_SceneObjects;
  }

  void Scene::rebuildBLAS()
  {
    m_MeshToGeometryId.clear();

    eastl::vector_set<string> modelNames;
    for (const SceneObject& obj: m_SceneObjects)
      modelNames.insert(obj.model);

    eastl::vector<BLAS::GeometryDesc> geometryDesc;

    uint geometryId = 0; 
    for (const string& modelName: modelNames)
    {
      ModelAsset* asset = assets_manager.getModel(modelName);
      if (m_MeshToGeometryId.find(asset->mesh->name) != m_MeshToGeometryId.end())
        continue;

      m_MeshToGeometryId.insert(eastl::make_pair(asset->mesh->name, geometryId++));

      for (const TriangleBVH& bvh: asset->mesh->submeshesBVH)
      {
        geometryDesc.push_back(BLAS::GeometryDesc{
          .bvh = bvh
        });
      }
    }

    m_BLAS.resetGeometry(geometryDesc);
  }
  
  void Scene::rebuildTLAS()
  {
    eastl::vector<TLAS::Instance> instances;

    for (const SceneObject& obj: m_SceneObjects)
    {
      ModelAsset* asset = assets_manager.getModel(obj.model);
      uint geometryID = m_MeshToGeometryId[asset->mesh->name];

      const float4x4 objectToWorldTm = calc_object_to_world_tm(obj);
      const float4x4 worldToObjectTm = glm::inverse(objectToWorldTm);

      for (const TriangleBVH& bvh : asset->mesh->submeshesBVH)
      {
        const auto [min,max] = bvh.getMinMaxAABB();
        const auto [min_ws, max_ws] = objectToWorldTm * Utils::AABB{min,max};

        instances.push_back(TLAS::Instance{
          .aabbMin_ws = float4(min_ws, 0.0f),
          .aabbMax_ws = float4(max_ws, 0.0f),
          .objectToWorldTm = objectToWorldTm,
          .worldToObjectTm = worldToObjectTm,
          .geometryId = geometryID
        });
      }
    }

    m_TLAS.rebuild(std::move(instances));
  }
}
