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
    uint primitiveBegin;
    uint primitiveCount;
    uint leftChild;
  };

  struct TraceResult
  {
    float t;
    float2 uv;
  };

  struct SahBin;

  class BVH
  {
    constexpr static uint BINS_COUNT = 32;
    public:
      BVH() = default;
      BVH(BVH&&) = default;
      BVH& operator=(BVH&&) = default;

      auto getMinMaxAABB() const -> eastl::tuple<float3, float3>
      {
        return {m_MinAABB, m_MaxAABB};
      }

      auto traceRay(const Utils::Ray&) const -> TraceResult;

    protected:
      virtual
      auto intersectRayWithPrimitive(const Utils::Ray&, const uint primitive_id) const -> TraceResult = 0;

      void buildNodes();

      virtual
      auto calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3> = 0;
      void updateNodeAABB(BVHNode& node);

      virtual
      void growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const = 0;

      struct SubdivisionResult { uint axis; float pos; float score; };
      auto findSubdivisionPlane(const BVHNode& node) const -> SubdivisionResult;
      void subdivideNode(const size_t node_id);
      
      void addNode(const uint primitive_begin, const uint primitive_count);

    protected:
      float3 m_MinAABB, m_MaxAABB;

      eastl::vector<float3> m_PrimitiveCentroids;
      eastl::vector<BVHNode> m_Nodes;
      eastl::vector<uint> m_PrimitiveIds;
  };

  class TriangleBVH : public BVH
  {
    public:
      TriangleBVH() = default;
      TriangleBVH(TriangleBVH&&) = default;
      TriangleBVH(const Engine::CpuSubmesh& mesh);
      TriangleBVH& operator=(TriangleBVH&&) = default;

    private:
      void unpackTriangles(const Engine::CpuSubmesh& mesh);

      auto intersectRayWithPrimitive(const Utils::Ray&, const uint primitive_id) const -> TraceResult override;

      auto calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3> override;
      void growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const override;

    private:
      eastl::vector<float4> m_VerticesPos;
      eastl::vector<TriangleVertexPayload> m_VerticesPayload;
  };
}
