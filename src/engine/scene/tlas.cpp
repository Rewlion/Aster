#include "tlas.h"

#include "blas.h"

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

    res = m_BLAS.traceRay(ray, instance.geometryId);
    res.primitiveId = primitive_id;
    res.objectToWorldTm = instance.objectToWorldTm;
    res.worldToObjectTm = instance.worldToObjectTm;

    return res;
  }

  void TLAS::growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const
  {
    bin.grow(m_Instances[primitive_id].aabbMin_ws, m_Instances[primitive_id].aabbMax_ws);
  }
}
