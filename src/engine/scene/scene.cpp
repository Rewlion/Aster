#include "scene.h"

#include <engine/data/ed.h>
#include <engine/math.h>

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
        Object newObj;
        newObj.model = obj.getVariable<string_view>("model");
        newObj.pos = obj.getVariable<float3>("pos");
        newObj.scale = obj.getVariableOr<float3>("scale", {1, 1, 1});
        newObj.rot = to_radians(obj.getVariableOr<float3>("rot", {0, 0, 0}));
        m_SceneObjects.push_back(std::move(newObj));
      }
    }
  }

  eastl::vector<Scene::Object> Scene::queueObjects() const
  {
    return m_SceneObjects;
  }
}
