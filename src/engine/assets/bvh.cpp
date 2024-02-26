#include "bvh.h"
#include "mesh.h"

#include <engine/assert.h>
#include <engine/utils/collision.h>

#include <limits>

namespace Engine
{
  float calc_bin_area(const float4& min, const float4& max)
  {
    float3 e{ glm::abs(max-min) };
    return e.x * e.y + e.y * e.z + e.z * e.x;
  }

  struct SahBin
  {
    float4 min = float4(0,0,0,0), max = float4(0,0,0,0);
    uint count = 0;

    void grow(const float4& v0, const float4& v1, const float4& v2)
    {
      min = glm::min(glm::min(min, v0), glm::min(v1, v2));
      max = glm::max(glm::max(max, v0), glm::max(v1, v2));
      count += 1;
    }

    void grow(const SahBin& b)
    {
      min = glm::min(min, b.min);
      max = glm::max(max, b.max);
      count += b.count;
    }

    float area() const
    {
      return calc_bin_area(min, max);
    }
  };

  void BVH::buildNodes()
  {
    ASSERT(m_MinAABB != m_MaxAABB);
    ASSERT(!m_PrimitiveCentroids.empty());

    const size_t primitiveCount = m_PrimitiveCentroids.size();
    m_Nodes.reserve(primitiveCount*2-1);
    m_PrimitiveIds.reserve(primitiveCount);

    for (uint i = 0; i < primitiveCount; ++i)
      m_PrimitiveIds.push_back(i);

    BVHNode& root = *reinterpret_cast<BVHNode*>(m_Nodes.push_back_uninitialized());
    root.minAABB = float4(m_MinAABB, 0.0f);
    root.maxAABB = float4(m_MaxAABB, 0.0f);
    root.primitiveBegin = 0;
    root.primitiveCount = primitiveCount;
    root.leftChild = 0;

    subdivideNode(0);
  }

  void BVH::updateNodeAABB(BVHNode& node)
  {
    float4 min = float4(std::numeric_limits<float>::max());
    float4 max = float4(std::numeric_limits<float>::min());

    const uint primitiveEnd = node.primitiveBegin + node.primitiveCount;
    for (uint iPrimitive = node.primitiveBegin;  iPrimitive < primitiveEnd; ++iPrimitive)
    {
      const uint primitiveID = m_PrimitiveIds[iPrimitive];
      const auto [pMin, pMax] = calcPrimitiveMinMax(primitiveID);
      min = glm::min(min, float4{pMin, 0.0f});
      max = glm::max(max, float4{pMax, 0.0f});
    }

    node.minAABB = min;
    node.maxAABB = max;
  }

  void BVH::subdivideNode(const size_t node_id)
  {
    BVHNode& node = m_Nodes[node_id];

    const float baseScore = calc_bin_area(node.minAABB, node.maxAABB) * node.primitiveCount;
    const auto [bestAxis, bestSplitPos, bestScore] = findSubdivisionPlane(node);

    if (bestScore >= baseScore)
      return;

    uint i = node.primitiveBegin;
    uint j = node.primitiveBegin + node.primitiveCount - 1;
    while (i <= j)
    {
      uint primitiveID = m_PrimitiveIds[i];
      float3 centroid = m_PrimitiveCentroids[primitiveID];
      if (centroid[bestAxis] < bestSplitPos)
        ++i;
      else
        std::swap(m_PrimitiveIds[i], m_PrimitiveIds[j--]);
    }

    const uint leftPrimitivesCount = i - node.primitiveBegin;
    const uint leftPrimitiveBegin = node.primitiveBegin;
    const uint rightPrimitivesCount = node.primitiveCount - leftPrimitivesCount;
    const uint rightPrimitiveBegin = leftPrimitiveBegin + leftPrimitivesCount;

    if (leftPrimitivesCount == 0 || leftPrimitivesCount == node.primitiveCount)
      return;

    const uint leftChildId = m_Nodes.size();
    addNode(leftPrimitiveBegin, leftPrimitivesCount);
    addNode(rightPrimitiveBegin, rightPrimitivesCount);

    node.primitiveBegin = 0;
    node.primitiveCount = 0;
    node.leftChild = leftChildId;

    subdivideNode(leftChildId);
    subdivideNode(leftChildId+1);
  }

  auto BVH::findSubdivisionPlane(const BVHNode& node) const
    -> SubdivisionResult
  {
    SahBin bins[BINS_COUNT];

    uint bestAxis = 0;
    float bestSplitPos = 0.0f;
    float bestScore = std::numeric_limits<float>::max();

    for (uint axis = 0; axis < 3; ++axis)
    {
      float binMin = std::numeric_limits<float>::max();
      float binMax = std::numeric_limits<float>::min();

      const uint primitiveEnd = node.primitiveBegin + node.primitiveCount;
      for (uint iPrimitive = node.primitiveBegin; iPrimitive < primitiveEnd; ++iPrimitive)
      {
        const uint primitiveID = m_PrimitiveIds[iPrimitive];

        binMin = glm::min(binMin, m_PrimitiveCentroids[primitiveID][axis]);
        binMax = glm::max(binMax, m_PrimitiveCentroids[primitiveID][axis]);
      }

      for (uint iPrimitive = node.primitiveBegin; iPrimitive < primitiveEnd; ++iPrimitive)
      {
        const uint primitiveID = m_PrimitiveIds[iPrimitive];

        const float centroid = m_PrimitiveCentroids[primitiveID][axis];
        const uint iBin = std::min(BINS_COUNT-1, 
                                  (uint)(BINS_COUNT * (centroid - binMin) / (binMax - binMin)));

        growSahBinWithPrimitive(bins[iBin], primitiveID);
      }

      float leftAreas[BINS_COUNT-1], rightAreas[BINS_COUNT-1];
      uint leftPrimitivesCounts[BINS_COUNT-1], rightPrimitivesCounts[BINS_COUNT-1];
      SahBin leftBin, rightBin;

      for (uint i = 0; i < BINS_COUNT-1; ++i)
      {
        const uint j = (BINS_COUNT-1)-i;
        leftBin.grow(bins[i]);
        rightBin.grow(bins[j]);

        leftAreas[i] = leftBin.area();
        leftPrimitivesCounts[i] = leftBin.count;
        rightAreas[i] = rightBin.area();
        rightPrimitivesCounts[i] = rightBin.count;
      }

      for (uint iPlane = 0; iPlane < BINS_COUNT-1; ++iPlane)
      {
        const uint iLeft = iPlane;
        const uint iRight = (BINS_COUNT-1)-1-iLeft;
        const float planeScore = leftAreas[iLeft] * leftPrimitivesCounts[iLeft] + rightAreas[iRight] * rightPrimitivesCounts[iRight];
        if (planeScore < bestScore)
        {
          bestScore = planeScore;
          bestAxis = axis;
          bestSplitPos = binMin + (binMax - binMin) / (float)BINS_COUNT * (float)(iPlane+1);
        }
      }
    }
    return {.axis = bestAxis, .pos = bestSplitPos, .score = bestScore};
  }

  void BVH::addNode(const uint primitive_begin, const uint primitive_count)
  {
    BVHNode& node = *reinterpret_cast<BVHNode*>(m_Nodes.push_back_uninitialized());
    node.primitiveBegin = primitive_begin;
    node.primitiveCount = primitive_count;
    node.leftChild = 0;
    updateNodeAABB(node);
  }

  auto BVH::traceRay(const Utils::Ray& ray) const -> TraceResult
  {
    TraceResult res{
      .t = -1.0f,
      .uv = {0.0f, 0.0f}
    };

    if (!Utils::test_intersection(Utils::AABB{m_MinAABB, m_MaxAABB}, ray))
      return res;

    uint nodeIds[64];
    uint topId = 0;
    nodeIds[0] = 0;

    while (topId != -1)
    {
      const uint nodeId = nodeIds[topId--];
      const BVHNode& node = m_Nodes[nodeId];

      if (node.primitiveCount == 0)
      {
        uint il = node.leftChild;
        uint ir = node.leftChild+1;

        const BVHNode& leftChild  = m_Nodes[il];
        const BVHNode& rightChild = m_Nodes[ir];

        float tl  = Utils::calc_intersection_t(Utils::AABB{leftChild.minAABB, leftChild.maxAABB}, ray);
        float tr = Utils::calc_intersection_t(Utils::AABB{rightChild.minAABB, rightChild.maxAABB}, ray);

        if (tr >= 0 && tr < tl)
        {
         std::swap(il,ir);
         std::swap(tl,tr);
        }

        if (tr >= 0)
          nodeIds[++topId] = ir;

        if (tl >= 0)
          nodeIds[++topId] = il;
        
        continue;
      }

      uint primitiveEnd = node.primitiveBegin + node.primitiveCount;
      for (uint iPrimitive = node.primitiveBegin; iPrimitive < primitiveEnd; ++iPrimitive)
      {
        const uint primitiveID = m_PrimitiveIds[iPrimitive];
        const auto [t, uv] = intersectRayWithPrimitive(ray, primitiveID);

        if (t >= 0)
        {
          res.t = t;
          res.uv = uv;
          
          return res;
        }
      }
    }

    return res;
  }

  TriangleBVH::TriangleBVH(const Engine::CpuSubmesh& mesh)
  {
    unpackTriangles(mesh);
    buildNodes();
  }
  
  auto TriangleBVH::intersectRayWithPrimitive(const Utils::Ray& ray, const uint primitive_id) const -> TraceResult
  {
    const float3& v0 = m_VerticesPos[primitive_id*3 + 0];
    const float3& v1 = m_VerticesPos[primitive_id*3 + 1];
    const float3& v2 = m_VerticesPos[primitive_id*3 + 2];

    const auto [t, uv] = Utils::calc_intersection_t(Utils::Triangle{v0,v1,v2}, ray);
    return TraceResult{
      .t = t,
      .uv = uv
    };
  }

  auto TriangleBVH::calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3>
  {
    float3 min = float3(std::numeric_limits<float>::max());
    float3 max = float3(std::numeric_limits<float>::min());

    for (int i = 0; i < 3; ++i)
    {
      min = glm::min(min, float3{m_VerticesPos[primitive_id*3 + i]});
      max = glm::max(max, float3{m_VerticesPos[primitive_id*3 + i]});
    }

    return {min, max};
  }

  void TriangleBVH::unpackTriangles(const Engine::CpuSubmesh& mesh)
  {
    const size_t triCount = mesh.indices.size() / 3;

    m_VerticesPos.reserve(triCount*3);
    m_PrimitiveCentroids.reserve(triCount);
    m_VerticesPayload.reserve(triCount*3);

    m_MinAABB = float3(std::numeric_limits<float>::max());
    m_MaxAABB = float3(std::numeric_limits<float>::min());

    for (int iTri = 0; iTri < triCount; ++iTri)
    {
      const gapi::index_type* is = mesh.indices.data() + iTri*3;

      float3 centroid = float3(0,0,0);
      for (int i = 0; i < 3; ++i)
      {
        const StaticMeshVertex& v = mesh.vertices[is[i]];
        
        m_VerticesPos.push_back(float4(v.pos, 0.0));

        m_VerticesPayload.push_back(TriangleVertexPayload{
          .uv = v.uv,
          .normal = v.normal
        });

        m_MinAABB = glm::min(m_MinAABB, v.pos);
        m_MaxAABB = glm::max(m_MaxAABB, v.pos);

        centroid += v.pos;
      }
      centroid /= 3.0f;

      m_PrimitiveCentroids.push_back(centroid);
    }
  }

  void TriangleBVH::growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const
  {
    const float4& v0 = m_VerticesPos[primitive_id*3 + 0];
    const float4& v1 = m_VerticesPos[primitive_id*3 + 1];
    const float4& v2 = m_VerticesPos[primitive_id*3 + 2];

    bin.grow(v0,v1,v2);
  }
}
