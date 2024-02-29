#include "blas.h"

#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>

namespace Engine
{
  void BLAS::resetGeometry(const eastl::span<const GeometryDesc> geometry_descs)
  {
    if (geometry_descs.empty())
      return;

    m_GeometryMeta.reserve(geometry_descs.size());
    eastl::vector<uint> geometryPrimitiveBegins;
    geometryPrimitiveBegins.reserve(geometry_descs.size());

    uint primitiveBegin = 0;
    uint nodeBegin = 0;
    uint verticesBegin = 0;
    for (const GeometryDesc& gd: geometry_descs)
    {
      const auto [minAABB,maxAABB] = gd.bvh.getMinMaxAABB();
      m_GeometryMeta.push_back(GeometryMeta{
        .nodeBegin = nodeBegin,
      });

      geometryPrimitiveBegins.push_back(primitiveBegin);

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
      const uint geometryPrimitiveBegin = geometryPrimitiveBegins[i];

      for (const BVHNode& node : nodes)
      {
        m_Nodes.push_back(node);
        m_Nodes.back().primitiveBegin += geometryPrimitiveBegin;
      }
      for (const uint primitiveID : primitiveIds)
        m_PrimitiveIds.push_back(geometryPrimitiveBegin + primitiveID);
      eastl::copy(verticesPos.begin(), verticesPos.end(), eastl::back_inserter(m_VerticesPos));
      eastl::copy(verticesPayload.begin(), verticesPayload.end(),  eastl::back_inserter(m_VerticesPayload));
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

  auto BLAS::buildGpuResources(gapi::CmdEncoder& encoder) const -> GpuResources
  {
    if (m_Nodes.empty())
      return {};

    const size_t geomMetaSize =     m_GeometryMeta.size()    * sizeof(m_GeometryMeta[0]);
    const size_t bvhNodesSize =     m_Nodes.size()           * sizeof(m_Nodes[0]);
    const size_t primitiveIdsSize = m_PrimitiveIds.size()    * sizeof(m_PrimitiveIds[0]);
    const size_t verticesSize =     m_VerticesPos.size()     * sizeof(m_VerticesPos[0]);
    const size_t payloadSize =      m_VerticesPayload.size() * sizeof(m_VerticesPayload[0]);

    gapi::BufferHandler gpuGeomMeta = gapi::allocate_buffer(geomMetaSize, gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible);
    gapi::BufferHandler gpuBvhNodes = gapi::allocate_buffer(bvhNodesSize, gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible);
    gapi::BufferHandler gpuPrimitiveIds = gapi::allocate_buffer(primitiveIdsSize, gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible);
    gapi::BufferHandler gpuVertices = gapi::allocate_buffer(verticesSize, gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible);
    gapi::BufferHandler gpuVerticesPayload = gapi::allocate_buffer(payloadSize, gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible);

    encoder.writeBuffer(gpuGeomMeta,        m_GeometryMeta.data(), 0,    geomMetaSize);
    encoder.writeBuffer(gpuBvhNodes,        m_Nodes.data(), 0,           bvhNodesSize);
    encoder.writeBuffer(gpuPrimitiveIds,    m_PrimitiveIds.data(), 0,    primitiveIdsSize);
    encoder.writeBuffer(gpuVertices,        m_VerticesPos.data(), 0,     verticesSize);
    encoder.writeBuffer(gpuVerticesPayload, m_VerticesPayload.data(), 0, payloadSize);

    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_SRV);

    return GpuResources {
      .geometryMeta = gpuGeomMeta,
      .bvhNodes = gpuBvhNodes,
      .primitiveIds = gpuPrimitiveIds,
      .vertices = gpuVertices,
      .verticesPayload = gpuVerticesPayload
    };
  }
}