#pragma once

#include "blas.h"
#include "object.h"
#include "tlas.h"

#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace ed
{
  class Scope;
}

namespace Engine
{
  class Scene
  {
    public:
      void loadScene(const ed::Scope& sceneBlk);
      eastl::vector<SceneObject> queueObjects() const;

      auto getTLAS() const -> const TLAS& { return m_TLAS; }

    private:
      void rebuildBLAS();
      void rebuildTLAS();

    private:
      eastl::vector<SceneObject> m_SceneObjects;
      eastl::vector_map<string, uint> m_MeshToGeometryId;
      BLAS m_BLAS;
      TLAS m_TLAS{m_BLAS};
  };

  extern Scene scene;
}
