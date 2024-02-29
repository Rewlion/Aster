#ifndef TLAS_HLSL
#define TLAS_HLSL

struct TLASInstance
{
  float4 aabbMin_ws;
  float4 aabbMax_ws;
  float4x4 objectToWorldTm;
  float4x4 worldToObjectTm;
  uint geometryId;
  uint3 _p;
};

#ifdef __preshader__ /////////////// [HLSL]  //////////////////////

#include "blas.hlsl"
#include "bvh.hlsl"

struct TLAS
{
  StructuredBuffer<TLASInstance>          tlasInstances;
  StructuredBuffer<BVHNode>               tlasBvhNodes;
  StructuredBuffer<uint>                  tlasPrimitiveIds;
  BLAS                                    blas;

  TraceResult intersectPrimitive(float3 ray_pos, float3 ray_dir, uint primitive_id)
  {
    TraceResult res;
    res.t = TRACE_MISS;

    const TLASInstance instance = tlasInstances[primitive_id]; 
    if (!test_aabb_ray_intersection(instance.aabbMin_ws.xyz, instance.aabbMax_ws.xyz, ray_pos, ray_dir))
      return res;

    const float3 rayPos_os = mul(instance.worldToObjectTm, float4(ray_pos, 1.0)).xyz;
    const float3 rayDir_os = mul(instance.worldToObjectTm, float4(ray_dir, 0.0)).xyz;

    res = blas.traceRay(rayPos_os, rayDir_os, instance.geometryId);

    if (res.t != TRACE_MISS)
    {
      const uint axis = ray_dir.x != 0 ? 0 : ray_dir.y != 0 ? 1 : 2;
      const float t_os = res.t;
      const float3 objToWs = float3(instance.objectToWorldTm[axis][0], instance.objectToWorldTm[axis][1], instance.objectToWorldTm[axis][2]);
      const float t_ws = dot(objToWs, rayDir_os) * t_os / ray_dir[axis];
      res.t = t_ws;
    }

    res.instanceID = primitive_id;
    res.objectToWorldTm = instance.objectToWorldTm;
    res.worldToObjectTm = instance.worldToObjectTm;

    return res;
  }

  TraceResult traceRay(float3 ray_pos, float3 ray_dir)
  {
    uint nodesBegin = 0;

    return trace_ray_over_bvh(ray_pos, ray_dir, nodesBegin, tlasBvhNodes, tlasPrimitiveIds, this);
  }
};

#endif // __preshader__

#endif // TLAS_HLSL