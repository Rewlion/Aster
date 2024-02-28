#include "tlas.h"

#include "blas.h"

#include <engine/render/debug/render.h>


namespace Engine
{
  TLAS::TLAS(const BLAS& blas) 
    : m_BLAS(blas)
  {
  }

  void TLAS::rebuild(eastl::vector<Instance>&& instances)
  {
    m_Instances = std::move(instances);
    m_PrimitiveCentroids.reserve(m_Instances.size());

    for (const Instance& instance: m_Instances)
    {
      const float3 centroid = (instance.aabbMin_ws + instance.aabbMin_ws) * 0.5f;
      m_PrimitiveCentroids.push_back(centroid);
    }

    buildNodes();
  }

  auto TLAS::calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3>
  {
    return {m_Instances[primitive_id].aabbMin_ws, m_Instances[primitive_id].aabbMax_ws};
  }

  auto TLAS::intersectRayWithPrimitive(const Utils::Ray& ray, const uint primitive_id) const -> TraceResult
  {
    TraceResult res;

    const Instance& instance = m_Instances[primitive_id]; 
    if (!Utils::test_intersection(Utils::AABB{instance.aabbMin_ws, instance.aabbMax_ws}, ray))
      return res;

    const Utils::Ray& ray_os = instance.worldToObjectTm * ray;

    res = m_BLAS.traceRay(ray_os, instance.geometryId);

    if (res.t != TRACE_MISS)
    {
      const uint axis = ray.dir.x != 0 ? 0 : ray.dir.y != 0 ? 1 : 2;
      const float t_os = res.t;
      const float3 objToWs = float3{instance.objectToWorldTm[0][axis], instance.objectToWorldTm[1][axis], instance.objectToWorldTm[2][axis]};
      const float t_ws = glm::dot(objToWs, ray_os.dir) * t_os / ray.dir[axis];
      res.t = t_ws;
    }

    res.primitiveId = primitive_id;
    res.objectToWorldTm = instance.objectToWorldTm;
    res.worldToObjectTm = instance.worldToObjectTm;

    return res;
  }

  void TLAS::growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const
  {
    bin.grow(m_Instances[primitive_id].aabbMin_ws, m_Instances[primitive_id].aabbMax_ws);
  }

  void TLAS::dbgDrawInstances() const
  {
    for (const auto& instance: m_Instances)
    {
      const float3 center = (instance.aabbMin_ws + instance.aabbMax_ws) * 0.5f;
      const float3 extent = (instance.aabbMax_ws - instance.aabbMin_ws);
      dbg::draw_aabb(center, extent, float4{0.0f, 1.0f, 0.0f, 0.4f}, 0.0f);
    }
  }

  void TLAS::dbgDrawNodes(const bool leafs_only) const
  {
    eastl::vector<uint> nodes;
    eastl::vector<uint> depths;

    nodes.push_back(0);
    depths.push_back(0);

    float4 clrs [5] = {
      float4{1,0,0,0.3f},
      float4{0,1,0,0.3f},
      float4{0,0,1,0.3f},
      float4{1,0,1,0.3f},
      float4{1,1,0,0.3f},
    };

    while(!nodes.empty())
    {
      float depth = depths.back();
      depths.pop_back();

      uint nID = nodes.back();
      nodes.pop_back();

      const BVHNode& node = m_Nodes[nID];

      const float3 center = (node.minAABB + node.maxAABB) * 0.5f;
      const float3 extent = (node.maxAABB - node.minAABB);

      if (!leafs_only || (leafs_only && node.primitiveCount > 0))
        dbg::draw_aabb(center, extent,clrs[nID%5], 0.0f);

      if (node.primitiveCount == 0)
      {
        depths.push_back(depth+1);
        nodes.push_back(node.leftChild);
        depths.push_back(depth+1);
        nodes.push_back(node.leftChild+1);
      }
    }
  }
}
