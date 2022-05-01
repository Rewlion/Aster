#include "scene.h"

#include <engine/datablock/datablock.h>

namespace Engine
{
  Scene scene;

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
        m_SceneObjects.push_back(std::move(newObj));
      }
    }
  }

  eastl::vector<Scene::Object> Scene::queueObjects() const
  {
    return m_SceneObjects;
  }
}
