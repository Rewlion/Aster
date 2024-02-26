#include "bvh.h"
#include "mesh.h"

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

  BVH::BVH(const Engine::CpuSubmesh& mesh)
  {
    unpackTriangles(mesh);
    buildNodes();
  }

  void BVH::unpackTriangles(const Engine::CpuSubmesh& mesh)
  {
    const size_t triCount = mesh.indices.size() / 3;

    m_VerticesPos.reserve(triCount*3);
    m_TriangleCentroids.reserve(triCount);
    m_VerticesPayload.reserve(triCount*3);

    minAABB = float3(std::numeric_limits<float>::max());
    maxAABB = float3(std::numeric_limits<float>::min());

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

        minAABB = glm::min(minAABB, v.pos);
        maxAABB = glm::max(maxAABB, v.pos);

        centroid += v.pos;
      }
      centroid /= 3.0f;

      m_TriangleCentroids.push_back(centroid);
    }
  }

  void BVH::buildNodes()
  {
    const size_t triCount = m_TriangleCentroids.size();
    m_Nodes.reserve(triCount*2-1);
    m_TriIds.reserve(triCount);

    for (uint i = 0; i < triCount; ++i)
      m_TriIds.push_back(i);

    BVHNode& root = *reinterpret_cast<BVHNode*>(m_Nodes.push_back_uninitialized());
    root.minAABB = float4(minAABB, 0.0f);
    root.maxAABB = float4(maxAABB, 0.0f);
    root.triBegin = 0;
    root.triCount = triCount;
    root.leftChild = 0;

    subdivideNode(0);
  }

  void BVH::updateNodeAABB(BVHNode& node)
  {
    float4 min = float4(std::numeric_limits<float>::max());
    float4 max = float4(std::numeric_limits<float>::min());

    const uint triEnd = node.triBegin + node.triCount;
    for (uint iTri = node.triBegin;  iTri < triEnd; ++iTri)
    {
      const uint triID = m_TriIds[iTri];
      for (int i = 0; i < 3; ++i)
      {
        min = glm::min(min, m_VerticesPos[triID*3 + i]);
        max = glm::max(max,  m_VerticesPos[triID*3 + i]);
      }
    }

    node.minAABB = min;
    node.maxAABB = max;
  }

  void BVH::subdivideNode(const size_t node_id)
  {
    BVHNode& node = m_Nodes[node_id];

    const float baseScore = calc_bin_area(node.minAABB, node.maxAABB) * node.triCount;
    const auto [bestAxis, bestSplitPos, bestScore] = findSubdivisionPlane(node);

    if (bestScore >= baseScore)
      return;

    uint i = node.triBegin;
    uint j = node.triBegin + node.triCount - 1;
    while (i <= j)
    {
      uint triID = m_TriIds[i];
      float3 centroid = m_TriangleCentroids[triID];
      if (centroid[bestAxis] < bestSplitPos)
        ++i;
      else
        std::swap(m_TriIds[i], m_TriIds[j--]);
    }

    const uint leftTriCount = i - node.triBegin;
    const uint leftTriBegin = node.triBegin;
    const uint rightTriCount = node.triCount - leftTriCount;
    const uint rightTriBegin = leftTriBegin + leftTriCount;

    if (leftTriCount == 0 || leftTriCount == node.triCount)
      return;

    const uint leftChildId = m_Nodes.size();
    addNode(leftTriBegin, leftTriCount);
    addNode(rightTriBegin, rightTriCount);

    node.triBegin = 0;
    node.triCount = 0;
    node.leftChild = leftChildId;

    subdivideNode(leftChildId);
    subdivideNode(leftChildId+1);
  }

  auto BVH::findSubdivisionPlane(const BVHNode& node) const
    -> SubdivisionResult
  {
    constexpr uint BINS_COUNT = 32;
    SahBin bins[BINS_COUNT];

    uint bestAxis = 0;
    float bestSplitPos = 0.0f;
    float bestScore = std::numeric_limits<float>::max();

    for (uint axis = 0; axis < 3; ++axis)
    {
      float binMin = std::numeric_limits<float>::max();
      float binMax = std::numeric_limits<float>::min();

      const uint triEnd = node.triBegin + node.triCount;
      for (uint iTri = node.triBegin; iTri < triEnd; ++iTri)
      {
        const uint triID = m_TriIds[iTri];

        binMin = glm::min(binMin, m_TriangleCentroids[triID][axis]);
        binMax = glm::max(binMax, m_TriangleCentroids[triID][axis]);
      }

      for (uint iTri = node.triBegin; iTri < triEnd; ++iTri)
      {
        const uint triID = m_TriIds[iTri];

        const float centroid = m_TriangleCentroids[triID][axis];
        const uint iBin = std::min(BINS_COUNT-1, 
                                  (uint)(BINS_COUNT * (centroid - binMin) / (binMax - binMin)));

        const float4& v0 = m_VerticesPos[triID*3 + 0];
        const float4& v1 = m_VerticesPos[triID*3 + 1];
        const float4& v2 = m_VerticesPos[triID*3 + 2];

        bins[iBin].grow(v0,v1,v2);
      }

      float leftAreas[BINS_COUNT-1], rightAreas[BINS_COUNT-1];
      uint leftTriCounts[BINS_COUNT-1], rightTriCounts[BINS_COUNT-1];
      SahBin leftBin, rightBin;

      for (uint i = 0; i < BINS_COUNT-1; ++i)
      {
        const uint j = (BINS_COUNT-1)-i;
        leftBin.grow(bins[i]);
        rightBin.grow(bins[j]);

        leftAreas[i] = leftBin.area();
        leftTriCounts[i] = leftBin.count;
        rightAreas[i] = rightBin.area();
        rightTriCounts[i] = rightBin.count;
      }

      for (uint iPlane = 0; iPlane < BINS_COUNT-1; ++iPlane)
      {
        const uint iLeft = iPlane;
        const uint iRight = (BINS_COUNT-1)-1-iLeft;
        const float planeScore = leftAreas[iLeft] * leftTriCounts[iLeft] + rightAreas[iRight] * rightTriCounts[iRight];
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

  void BVH::addNode(const uint tri_begin, const uint tri_count)
  {
    BVHNode& node = *reinterpret_cast<BVHNode*>(m_Nodes.push_back_uninitialized());
    node.triBegin = tri_begin;
    node.triCount = tri_count;
    node.leftChild = 0;
    updateNodeAABB(node);
  }

  auto BVH::traceRay(const Utils::Ray& ray) const -> TraceResult
  {
    TraceResult res{
      .t = -1.0f,
      .uv = {0.0f, 0.0f}
    };

    if (!Utils::test_intersection(Utils::AABB{minAABB, maxAABB}, ray))
      return res;

    uint nodeIds[64];
    uint topId = 0;
    nodeIds[0] = 0;

    while (topId != -1)
    {
      const uint nodeId = nodeIds[topId--];
      const BVHNode& node = m_Nodes[nodeId];

      if (node.triCount == 0)
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

      uint triEnd = node.triBegin + node.triCount;
      for (uint iTri = node.triBegin; iTri < triEnd; ++iTri)
      {
        const uint triID = m_TriIds[iTri];
        const float3& v0 = m_VerticesPos[triID*3 + 0];
        const float3& v1 = m_VerticesPos[triID*3 + 1];
        const float3& v2 = m_VerticesPos[triID*3 + 2];

        const auto [t, uv] = Utils::calc_intersection_t(Utils::Triangle{v0,v1,v2}, ray);
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
}
