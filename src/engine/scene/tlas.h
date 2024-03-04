#pragma once

#include <engine/assets/bvh.h>
#include <engine/gapi/resource_wrappers.h>
#include <engine/types.h>

#include <EASTL/vector.h>

#include <engine/shaders/shaders/raytracing/tlas.hlsl>

namespace Engine
{
  class BLAS;

  class TLAS : public BVH
  {
    public:
      struct GpuResources
      {
        gapi::BufferWrapper instances;
        gapi::BufferWrapper bvhNodes;
        gapi::BufferWrapper primitiveIds;
        gapi::BufferWrapper instanceToBindlessPackId;
      };

      struct MaterialTextures
      {
        gapi::TextureHandle albedo;
        gapi::TextureHandle normalMap;
        gapi::TextureHandle metalRoughness;
      };

    public:
      TLAS(const BLAS& blas);
      void rebuild(eastl::vector<TLASInstance>&&, eastl::vector<uint> instance_to_bindless_pack_Id);
      auto buildGpuResources(gapi::CmdEncoder&) const -> GpuResources;

      void dbgDrawInstances() const;
      void dbgDrawNodes(const bool leafs_only) const;

    protected:
      auto calcPrimitiveMinMax(const uint primitive_id) -> eastl::tuple<float3, float3> override;
      auto intersectRayWithPrimitive(const Utils::Ray&, const uint primitive_id) const -> TraceResult override;
      void growSahBinWithPrimitive(SahBin& bin, const uint primitive_id) const override;

    private:
      eastl::vector<TLASInstance> m_Instances;
      eastl::vector<uint> m_InstanceToBindlessPackId;
      const BLAS& m_BLAS;
  };
}
