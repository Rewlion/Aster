#include "bvh.h"
#include "mesh.h"

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

    m_TrianglesPos.reserve(triCount);
    m_TriangleCentroids.reserve(triCount);
    m_VerticesPayload.reserve(triCount*3);

    minAABB = float3(std::numeric_limits<float>::max());
    maxAABB = float3(std::numeric_limits<float>::min());

    for (int i = 0; i < triCount; ++i)
    {
      const gapi::index_type* is = mesh.indices.data() + i*3;

      float3 centroid = float3(0,0,0);
      for (int j = 0; j < 3; ++j)
      {
        const StaticMeshVertex& v = mesh.vertices[is[i]];
        
        m_TrianglesPos.push_back(float4(v.pos, 0.0));

        m_VerticesPayload.push_back(TriangleVertexPayload{
          .uv = v.uv,
          .normal = v.normal
        });

        minAABB = glm::min(minAABB, v.pos);
        maxAABB = glm::min(maxAABB, v.pos);

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
  }

  void BVH::updateNodeAABB(BVHNode& node)
  {
    float4 min = float4(std::numeric_limits<float>::max());
    float4 max = float4(std::numeric_limits<float>::min());

    uint lastVertexId = node.triBegin + node.triCount * 3;
    for (uint i = node.triBegin;  i < lastVertexId; ++i)
    {
      min = glm::min(min, m_TrianglesPos[i]);
      max = glm::max(max,  m_TrianglesPos[i]);
    }

    node.minAABB = min;
    node.maxAABB = max;
  }

  void BVH::subdivideNode(const size_t node_id)
  {
    BVHNode& node = m_Nodes[node_id];

    const float baseScore = calc_bin_area(node.minAABB, node.maxAABB) * node.triCount;
    const auto [bestScore, bestSplitPos, bestAxis] = findSubdivisionPlane(node);

    if (bestScore >= baseScore)
      return;

    uint i = node.triBegin;
    uint j = node.triBegin + node.triCount - 1;
    while (i <= j)
    {
      uint triID = m_TriIds[i];
      float3 centroid = m_TriangleCentroids[triID];
      if (centroid[bestAxis] < bestSplitPos[bestAxis])
        ++i;
      else
        std::swap(m_TriIds[i], m_TriIds[--j]);
    }

    const uint leftTriCount = i - node.triBegin;
    const uint rightTriCount = node.triCount - leftTriCount;

    if (leftTriCount == 0 || leftTriCount == node.triCount)
      return;


    const uint leftChildId = m_Nodes.size();
    addNode(i, leftTriCount);
    addNode(j, rightTriCount);

    node.triCount = 0;
    node.leftChild = leftChildId;

    subdivideNode(leftChildId);
    subdivideNode(leftChildId+1);
  }

  auto BVH::findSubdivisionPlane(const BVHNode& node) const
    -> eastl::tuple<float, float3, uint>
  {
    uint bestAxis = 0;
    float3 bestSplitPos;
    float bestScore = std::numeric_limits<float>::max();

    for (uint axis = 0; axis < 3; ++axis)
    {
      uint triEnd = node.triBegin + node.triCount;
      for (uint iTri = node.triBegin; iTri < triEnd; ++iTri)
      {
        uint triID = m_TriIds[iTri];
        float3 centroid = m_TriangleCentroids[triID];
        float score = calcSAH(node, centroid, axis);

        if (score < bestScore)
        {
          bestAxis = axis;
          bestScore = score;
          bestSplitPos = centroid;
        }
      }
    }

    return {bestScore, bestSplitPos, bestAxis};
  }

  auto BVH::calcSAH(const BVHNode& node, const float3 axis_pos, const uint axis) const -> float
  {
    SahBin left, right;

    uint triEnd = node.triBegin + node.triCount;
    for (uint iTri = node.triBegin; iTri < triEnd; ++iTri)
    {
      float3 centroid = m_TriangleCentroids[iTri];
      const float4& v0 = m_TrianglesPos[iTri + 0];
      const float4& v1 = m_TrianglesPos[iTri + 1];
      const float4& v2 = m_TrianglesPos[iTri + 2];

      SahBin& bin = centroid[axis] < axis_pos[axis] ? left : right;
      bin.grow(v0,v1,v2);
    }

    float score = left.count * left.area() + right.count + right.area();
    return score > 0 ? score :  std::numeric_limits<float>::max();
  }

  void BVH::addNode(const uint tri_begin, const uint tri_count)
  {
    BVHNode& node = *reinterpret_cast<BVHNode*>(m_Nodes.push_back_uninitialized());
    node.triBegin = tri_begin;
    node.triCount = tri_count;
    node.leftChild = 0;
    updateNodeAABB(node);
  }
}
