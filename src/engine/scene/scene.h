#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

class DataBlock;

namespace Engine
{
  class Scene
  {
    public:
      struct Object
      {
        float3 pos = float3(0,0,0);
        string model;
      };

    public:
      void loadScene(const DataBlock& sceneBlk);
      eastl::vector<Object> queueObjects() const;

    private:
      eastl::vector<Object> m_SceneObjects;
  };

  extern Scene scene;
}
