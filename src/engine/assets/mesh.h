#pragma once

#include "vertex.h"

#include <engine/gapi/resources.h>

#include <EASTL/vector.h>

namespace Engine
{
  struct CpuSubmesh
  {
    eastl::vector<StaticMeshVertex> vertices;
    eastl::vector<gapi::index_type> indices;
  };

  struct GpuSubmesh
  {
    gapi::BufferHandler       vertexBuffer;
    gapi::BufferHandler       indexBuffer;
    gapi::index_type          indexCount = 0;
  };

  constexpr size_t MAX_SUBMESH_COUNT = 32;

  struct UnpackedStaticMesh
  {
    Utils::FixedStack<CpuSubmesh, MAX_SUBMESH_COUNT> cpuSubmeshes;
  };

  struct StaticMesh
  {
    Utils::FixedStack<GpuSubmesh, MAX_SUBMESH_COUNT> gpuSubmeshes;
  };
}
