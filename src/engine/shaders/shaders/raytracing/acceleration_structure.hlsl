#ifndef RT_ACCELERATION_STRUCTURE_HLSL
#define RT_ACCELERATION_STRUCTURE_HLSL

#include "tlas.hlsl"

struct RTAccelerationStructure
{
  TLAS tlas;

  void init(
    StructuredBuffer<TLASInstance>          tlasInstances,
    StructuredBuffer<BVHNode>               tlasBvhNodes,
    StructuredBuffer<uint>                  tlasPrimitiveIds,

    StructuredBuffer<GeometryMeta>          blasGeometryMeta,
    StructuredBuffer<BVHNode>               blasBvhNodes,
    StructuredBuffer<uint>                  blasPrimitiveIds,
    StructuredBuffer<float4>                blasVertices,
    StructuredBuffer<TriangleVertexPayload> blasVerticesPayload
  )
  {
    BLAS blas;
    blas.blasGeometryMeta = blasGeometryMeta;
    blas.blasBvhNodes = blasBvhNodes;
    blas.blasPrimitiveIds = blasPrimitiveIds;
    blas.blasVertices = blasVertices;
    blas.blasVerticesPayload = blasVerticesPayload;

    tlas.tlasInstances = tlasInstances;
    tlas.tlasBvhNodes = tlasBvhNodes;
    tlas.tlasPrimitiveIds = tlasPrimitiveIds;
    tlas.blas = blas;
  }

  TraceResult traceRay(float3 ray_pos, float3 ray_dir)
  {
    return tlas.traceRay(ray_pos, ray_dir);
  }
};

#endif