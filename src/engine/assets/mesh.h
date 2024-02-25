#pragma once

#include "vertex.h"
#include "bvh.h"

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
  
  template<class T>
  using SubmeshStack = Utils::FixedStack<T, MAX_SUBMESH_COUNT>;

  struct UnpackedStaticMesh
  {
    SubmeshStack<CpuSubmesh> cpuSubmeshes;
  };

  struct StaticMesh
  {
    SubmeshStack<GpuSubmesh> gpuSubmeshes;
    SubmeshStack<BVH> submeshesBVH;
  };
}
