#ifndef BVH_HLSL
#define BVH_HLSL

struct BVHNode
{
  float4 minAABB;
  float4 maxAABB;
  uint primitiveBegin;
  uint primitiveCount;
  uint leftChild;
  uint _p;
};  

struct TriangleVertexPayload
{
  float2 uv;
  float2 normal;
};

#ifdef __preshader__ /////////////// [HLSL]  //////////////////////

#include <math.hlsl>
#include <intersections.hlsl>

struct TraceResult
{
  uint primitiveID;
  uint instanceID;
  float t;
  float2 uv;
  float4x4 objectToWorldTm;
  float4x4 worldToObjectTm;
};


template <typename PrimitiveTracer>
TraceResult trace_ray_over_bvh(
  float3 ray_pos,
  float3 ray_dir,
  uint nodes_begin,
  StructuredBuffer<BVHNode> nodes,
  StructuredBuffer<uint> primitive_ids,
  PrimitiveTracer primitive_tracer)
{
  TraceResult res;
  res.t = FLOAT_MAX;

  BVHNode rootNode = nodes[nodes_begin + 0];

  if (!test_aabb_ray_intersection(rootNode.minAABB.xyz, rootNode.maxAABB.xyz, ray_pos, ray_dir))
    return res;

  uint nodeIds[64];
  uint topId = 0;
  nodeIds[0] = nodes_begin;

  while (topId != -1)
  {
    const uint nodeId = nodeIds[topId--];
    const BVHNode node = nodes[nodeId];

    if (node.primitiveCount == 0)
    {
      uint il = nodes_begin + node.leftChild;
      uint ir = nodes_begin + node.leftChild+1;

      const BVHNode leftChild  = nodes[il];
      const BVHNode rightChild = nodes[ir];

      float tl = calc_aabb_ray_intersection_t(leftChild.minAABB.xyz, leftChild.maxAABB.xyz, ray_pos, ray_dir);
      float tr = calc_aabb_ray_intersection_t(rightChild.minAABB.xyz, rightChild.maxAABB.xyz, ray_pos, ray_dir);

      if (tr < tl)
      {
        swap(il,ir);
        swap(tl,tr);
      }

      if (tr != TRACE_MISS && tr <= res.t)
        nodeIds[++topId] = ir;

      if (tl != TRACE_MISS && tl <= res.t)
        nodeIds[++topId] = il;
      
      continue;
    }

    const uint primitiveEnd = node.primitiveBegin + node.primitiveCount;
    for (uint iPrimitive = node.primitiveBegin; iPrimitive < primitiveEnd; ++iPrimitive)
    {
      const uint primitiveID = primitive_ids[iPrimitive];
      const TraceResult tres = primitive_tracer.intersectPrimitive(ray_pos, ray_dir, primitiveID);

      if (tres.t < res.t)
        res = tres;
    }
  }

  return res;
}
#endif //__preshader__


#endif //BVH_HLSL