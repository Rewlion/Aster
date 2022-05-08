#include "scene.h"

#include <engine/datablock/datablock.h>
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

  void Scene::loadScene(const DataBlock& sceneBlk)
  {
    m_SceneObjects.clear();

    for (const auto& obj: sceneBlk.getChildBlocks())
    {
      if (obj.getName() == "object")
      {
        Object newObj;
        newObj.model = obj.getText("model");
        newObj.pos = obj.getFloat3("pos");
        newObj.scale = obj.getFloat3("scale", {1, 1, 1});
        newObj.rot = to_radians(obj.getFloat3("rot", {0, 0, 0}));
        m_SceneObjects.push_back(std::move(newObj));
      }
    }
  }

  eastl::vector<Scene::Object> Scene::queueObjects() const
  {
    return m_SceneObjects;
  }
}
