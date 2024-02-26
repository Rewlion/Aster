#pragma once

#include <engine/types.h>
#include <engine/utils/collision.h>

#include <EASTL/vector.h>
#include <EASTL/tuple.h>

namespace Utils
{
  struct Ray;
}

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

  struct TraceResult
  {
    float t;
    float2 uv;
  };

  class BVH
  {
    public:
      BVH() = default;
      BVH(BVH&&) = default;
      BVH(const Engine::CpuSubmesh& mesh);

      BVH& operator=(BVH&&) = default;

      auto getMinMaxAABB() const -> eastl::tuple<float3, float3>
      {
        return {minAABB, maxAABB};
      }

      auto traceRay(const Utils::Ray&) const -> TraceResult;

    private:
      void unpackTriangles(const Engine::CpuSubmesh& mesh);
      void buildNodes();
      void updateNodeAABB(BVHNode& node);
      void subdivideNode(const size_t node_id);
      struct SubdivisionResult { uint axis; float pos; float score; };
      auto findSubdivisionPlane(const BVHNode& node) const -> SubdivisionResult;
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
