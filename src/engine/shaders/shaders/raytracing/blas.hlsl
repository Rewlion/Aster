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

    const TriangleVertexPayload vp0 = blasVerticesPayload[primitive_id*3 + 0];
    const TriangleVertexPayload vp1 = blasVerticesPayload[primitive_id*3 + 1];
    const TriangleVertexPayload vp2 = blasVerticesPayload[primitive_id*3 + 2];

    TriangleIntersection intTriRes = calc_triangle_ray_intersection(v0,v1,v2,ray_pos,ray_dir);
    TraceResult res;
    res.t = intTriRes.t;
    res.uvw = intTriRes.uvw;
    res.primitiveID = primitive_id;

    if (intTriRes.t != TRACE_MISS)
    {
      res.payload[0] = vp0;
      res.payload[1] = vp1;
      res.payload[2] = vp2;
    }

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
