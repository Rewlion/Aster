#pragma once

#include <engine/assets/bvh.h>
#include <engine/gapi/resource_wrappers.h>
#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/span.h>

#include <engine/shaders/shaders/raytracing/blas.hlsl>

namespace Engine
{
  class BLAS
  {
    public:
      struct GeometryDesc
      {
        const TriangleBVH& bvh;
      };

      struct GpuResources
      {
        gapi::BufferWrapper geometryMeta;
        gapi::BufferWrapper bvhNodes;
        gapi::BufferWrapper primitiveIds;
        gapi::BufferWrapper vertices;
        gapi::BufferWrapper verticesPayload;
      };

    public:
      void resetGeometry(const eastl::span<const GeometryDesc>);
      auto traceRay(const Utils::Ray&, const uint geometry_id) const -> TraceResult;
      auto buildGpuResources(gapi::CmdEncoder&) const -> GpuResources;

    private:
      eastl::vector<GeometryMeta> m_GeometryMeta;

      eastl::vector<BVHNode> m_Nodes;
      eastl::vector<uint> m_PrimitiveIds;

      eastl::vector<float4> m_VerticesPos;
      eastl::vector<TriangleVertexPayload> m_VerticesPayload;
  };
}