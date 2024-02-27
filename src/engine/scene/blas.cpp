#include "blas.h"

#include <engine/assert.h>

namespace Engine
{
  void BLAS::resetGeometry(const eastl::span<const GeometryDesc> geometry_descs)
  {
    if (geometry_descs.empty())
      return;

    m_GeometryMeta.reserve(geometry_descs.size());

    uint primitiveBegin = 0;
    uint nodeBegin = 0;
    uint verticesBegin = 0;
    for (const GeometryDesc& gd: geometry_descs)
    {
      const auto [minAABB,maxAABB] = gd.bvh.getMinMaxAABB();
      m_GeometryMeta.push_back(GeometryMeta{
        .minAABB = minAABB,
        .maxAABB = maxAABB,
        .primitiveBegin = primitiveBegin,
        .nodeBegin = nodeBegin,
        .verticesBegin = verticesBegin
      });

      const eastl::vector<uint>& primitiveIds = gd.bvh.getPrimitiveIds();
      const eastl::vector<float4>& verticesPos = gd.bvh.getVerticesPos();
      const eastl::vector<BVHNode>& nodes = gd.bvh.getNodes();

      primitiveBegin += primitiveIds.size();
      nodeBegin += nodes.size();
      verticesBegin += verticesPos.size();
    }

    m_Nodes.reserve(nodeBegin);
    m_PrimitiveIds.reserve(primitiveBegin);
    m_VerticesPos.reserve(verticesBegin);
    m_VerticesPayload.reserve(verticesBegin);

    const size_t gdCount = geometry_descs.size();
    for (size_t i = 0; i < gdCount; ++i)
    {
      const GeometryDesc& gd = geometry_descs[i];

      const eastl::vector<uint>& primitiveIds = gd.bvh.getPrimitiveIds();
      const eastl::vector<float4>& verticesPos = gd.bvh.getVerticesPos();
      const eastl::vector<TriangleVertexPayload>& verticesPayload = gd.bvh.getVerticesPayload();
      const eastl::vector<BVHNode>& nodes = gd.bvh.getNodes();

      const GeometryMeta& meta = m_GeometryMeta[i];

      for (const BVHNode& node : nodes)
      {
        m_Nodes.push_back(node);
        m_Nodes.back().primitiveBegin += meta.primitiveBegin;
        m_Nodes.back().leftChild += meta.nodeBegin;
      }
      for (const uint primitiveID : primitiveIds)
        m_PrimitiveIds.push_back(meta.primitiveBegin + primitiveID);
      eastl::copy(verticesPos.begin(), verticesPos.end(), m_VerticesPos.end());
      eastl::copy(verticesPayload.begin(), verticesPayload.end(), m_VerticesPayload.end());
    }
  }

  auto BLAS::traceRay(const Utils::Ray& ray, const uint geometry_id) const -> TraceResult
  {
    ASSERT(geometry_id < m_GeometryMeta.size());
    const GeometryMeta& meta = m_GeometryMeta[geometry_id];
    TriangleBVHView bvh = {
      .nodes = eastl::span<const BVHNode>{m_Nodes.data() + meta.nodeBegin, m_Nodes.data() + m_Nodes.size()},
      .primitiveIds = m_PrimitiveIds,
      .verticesPos = m_VerticesPos,
      .verticesPayload = m_VerticesPayload
    };

    return bvh.traceRay(ray);
  }
}