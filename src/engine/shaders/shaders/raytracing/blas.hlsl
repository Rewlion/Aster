#ifndef BLAS_HLSL
#define BLAS_HLSL

struct GeometryMeta
{
  uint nodeBegin;
};

#ifdef __preshader__ /////////////// [HLSL]  //////////////////////

#include "bvh.hlsl"

struct BLAS
{
  StructuredBuffer<GeometryMeta>          blasGeometryMeta;
  StructuredBuffer<BVHNode>               blasBvhNodes;
  StructuredBuffer<uint>                  blasPrimitiveIds;
  StructuredBuffer<float4>                blasVertices;
  StructuredBuffer<TriangleVertexPayload> blasVerticesPayload;

  TraceResult intersectPrimitive(float3 ray_pos, float3 ray_dir, uint primitive_id)
  {
    const float3 v0 = blasVertices[primitive_id*3 + 0].xyz;
    const float3 v1 = blasVertices[primitive_id*3 + 1].xyz;
    const float3 v2 = blasVertices[primitive_id*3 + 2].xyz;

    TriangleIntersection intTriRes = calc_triangle_ray_intersection(v0,v1,v2,ray_pos,ray_dir);
    TraceResult res;
    res.t = intTriRes.t;
    res.uv = intTriRes.uvw.xy;
    res.primitiveID = primitive_id;

    return res;
  }

  TraceResult traceRay(float3 ray_pos, float3 ray_dir, uint primitive_id)
  {
    const GeometryMeta meta = blasGeometryMeta[primitive_id];
    const uint nodeBegin = meta.nodeBegin;

    return trace_ray_over_bvh(ray_pos, ray_dir, nodeBegin, blasBvhNodes, blasPrimitiveIds, this);
  };
};

#endif //__preshader__

#endif //BLAS_HLSL
