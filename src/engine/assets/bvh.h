#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/tuple.h>

namespace Engine
{
  struct CpuSubmesh;

  struct TriangleVertexPayload
  {
    float2 uv;
    float2 normal;
  };
  
  struct BVHNode
  {
    float4 minAABB;
    float4 maxAABB;
    uint triBegin;
    uint triCount;
    uint leftChild;
  };

  class BVH
  {
    public:
      BVH() = default;
      BVH(BVH&&) = default;
      BVH(const Engine::CpuSubmesh& mesh);
  
    private:
      void unpackTriangles(const Engine::CpuSubmesh& mesh);
      void buildNodes();
      void updateNodeAABB(BVHNode& node);
      void subdivideNode(const size_t node_id);
      auto findSubdivisionPlane(const BVHNode& node) const -> eastl::tuple<float, float, uint>;
      void addNode(const uint tri_begin, const uint tri_count);
    private:
      float3 minAABB, maxAABB;

      eastl::vector<float4> m_VerticesPos;
      eastl::vector<float3> m_TriangleCentroids;
      eastl::vector<TriangleVertexPayload> m_VerticesPayload;
      
      eastl::vector<BVHNode> m_Nodes;
      eastl::vector<uint> m_TriIds;
  };
}
