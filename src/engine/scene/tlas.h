#pragma once

#include <engine/assets/bvh.h>
#include <engine/types.h>

#include <EASTL/vector.h>

namespace Engine
{
  class BLAS;

  class TLAS : public BVH
  {
    public:
      struct Instance
      {
        float4 aabbMin_ws;
        float4 aabbMax_ws;
        float4x4 objectToWorldTm;
        float4x4 worldToObjectTm;
        uint geometryId;
      };

    public:
      TLAS(const BLAS& blas);
      void rebuild(eastl::vector<Instance>&&);

    protected:
      auto calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3> override;
      auto intersectRayWithPrimitive(const Utils::Ray&, const uint primitive_id) const -> TraceResult override;
      void growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const override;

    private:
      eastl::vector<Instance> m_Instances;
      const BLAS& m_BLAS;
  };
}
