#pragma once

#include "blas.h"
#include "object.h"
#include "tlas.h"

#include <engine/tfx/tfx.h>
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
      struct GpuRTAccelerationStructure
      {
        TLAS::GpuResources tlas;
        BLAS::GpuResources blas;
      };
    public:
      void loadScene(const ed::Scope& sceneBlk);
      eastl::vector<SceneObject> queueObjects() const;

      auto getBindlessMaterialTextures() const -> const tfx::TextureArray& { return m_BindlessMaterialTextures; }
      auto getTLAS() const -> const TLAS& { return m_TLAS; }
      auto getRTAS() const -> const GpuRTAccelerationStructure& { return m_GpuRTAS; }

    private:
      void rebuildBindlessMaterialParams();
      void rebuildBLAS();
      void rebuildTLAS();
      void rebuildRTAS();

    private:
      eastl::vector<SceneObject> m_SceneObjects;
      eastl::vector_map<string, uint> m_ModelToBindlessParams;
      tfx::TextureArray m_BindlessMaterialTextures;
      eastl::vector_map<string, uint> m_MeshToGeometryId;
      BLAS m_BLAS;
      TLAS m_TLAS{m_BLAS};
      GpuRTAccelerationStructure m_GpuRTAS;
  };

  extern Scene scene;
}
