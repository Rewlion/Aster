#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

namespace ed
{
  class Scope;
}

namespace Engine
{
  class Scene
  {
    public:
      struct Object
      {
        float3 pos = float3(0,0,0);
        float3 scale = float3(1, 1, 1);
        float3 rot = float3(0, 0, 0);
        string model;
      };

    public:
      void loadScene(const ed::Scope& sceneBlk);
      eastl::vector<Object> queueObjects() const;

    private:
      eastl::vector<Object> m_SceneObjects;
  };

  extern Scene scene;
}
