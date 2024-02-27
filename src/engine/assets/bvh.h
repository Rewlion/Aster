#pragma once

#include <engine/types.h>
#include <engine/utils/collision.h>

#include <EASTL/span.h>
#include <EASTL/tuple.h>
#include <EASTL/vector.h>
#include <function2/function2.hpp>

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
    uint primitiveId         = 0;
    float t                  = - 1.0f;
    float2 uv                = float2{0.0f, 0.0f};
    float4x4 objectToWorldTm = float4x4{1.0f};
    float4x4 worldToObjectTm = float4x4{1.0f};
  };

  struct SahBin
  {
    float4 min = float4(0,0,0,0), max = float4(0,0,0,0);
    uint count = 0;

    void grow(const float4& v0, const float4& v1, const float4& v2);
    void grow(const SahBin& b);
    void grow(const float4& min, const float4& max);
    float area() const;
  };

  class BVH
  {
    constexpr static uint BINS_COUNT = 32;
    public:
      BVH() = default;
      BVH(BVH&&) = default;
      BVH& operator=(BVH&&) = default;

      auto traceRay(const Utils::Ray&) const -> TraceResult;

      auto getMinMaxAABB() const -> eastl::tuple<float3, float3>
      {
        return !m_Nodes.empty() ?
          eastl::tuple<float3, float3>{m_Nodes[0].minAABB, m_Nodes[0].maxAABB} :
          eastl::tuple<float3, float3>{float3{0.0f}, float3{0.0f}};
      }
      auto getPrimitiveIds() const -> const eastl::vector<uint>& { return m_PrimitiveIds; }
      auto getNodes() const -> const eastl::vector<BVHNode>&     { return m_Nodes; }

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

      auto getVerticesPos() const -> const eastl::vector<float4>& { return m_VerticesPos; }
      auto getVerticesPayload() const -> const eastl::vector<TriangleVertexPayload>& { return m_VerticesPayload; }

    private:
      void unpackTriangles(const Engine::CpuSubmesh& mesh);

      auto intersectRayWithPrimitive(const Utils::Ray&, const uint primitive_id) const -> TraceResult override;

      auto calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3> override;
      void growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const override;

    private:
      eastl::vector<float4> m_VerticesPos;
      eastl::vector<TriangleVertexPayload> m_VerticesPayload;
  };

  struct TriangleBVHView
  {
    const eastl::span<const BVHNode> nodes;
    const eastl::span<const uint> primitiveIds;
    const eastl::span<const float4> verticesPos;
    const eastl::span<const TriangleVertexPayload> verticesPayload;

    auto traceRay(const Utils::Ray&) const -> TraceResult;
  };
}
