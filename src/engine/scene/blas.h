#pragma once

#include <engine/assets/bvh.h>
#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/span.h>

namespace Engine
{
  class BLAS
  {
    public:
      struct GeometryDesc
      {
        const TriangleBVH& bvh;
      };

    public:
      void resetGeometry(const eastl::span<const GeometryDesc>);
      auto traceRay(const Utils::Ray&, const uint geometry_id) const -> TraceResult;

    private:
      struct GeometryMeta
      {
        float3 minAABB, maxAABB;
        uint primitiveBegin;
        uint nodeBegin;
        uint verticesBegin;
      };

    private:
      eastl::vector<GeometryMeta> m_GeometryMeta;

      eastl::vector<BVHNode> m_Nodes;
      eastl::vector<uint> m_PrimitiveIds;

      eastl::vector<float4> m_VerticesPos;
      eastl::vector<TriangleVertexPayload> m_VerticesPayload;
  };
}