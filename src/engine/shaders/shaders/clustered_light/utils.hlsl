#ifndef CLUSTERED_UTILS_HEADER_GUARD
#define CLUSTERED_UTILS_HEADER_GUARD

#define CLUSTERS_DIM_X 16
#define CLUSTERS_DIM_Y 8
#define CLUSTERS_DIM_Z 24
#define CLUSTERS_NUM (CLUSTERS_DIM_X*CLUSTERS_DIM_Y*CLUSTERS_DIM_Z)

#define CLUSTERS_THREADS_X 4
#define CLUSTERS_THREADS_Y 4
#define CLUSTERS_THREADS_Z 4
#define THREADS_NUM (CLUSTERS_THREADS_X*CLUSTERS_THREADS_Y*CLUSTERS_THREADS_Z)

#define CLUSTER_MAX_ITEMS 128

//todo:generated with std140 instead of std430 for RWStructuredBuffer...
struct Cluster
{
  float4 minPoint;
  float4 maxPoint;
};

struct PointLight
{
  float3 pos;
  float3 color;
  float attenuationRadius;
};

struct Light
{
  float3 params1;
  uint type;
  float4 params2;
};

struct ClusterIndirection
{
  uint lightIndex;
  uint _decalIndex;
  uint _probeIndex;
  uint _p;
};

struct ClusterInfo
{
  uint indirectionOffset;
  uint lightsCount;
  uint _decalsCount;
  uint _probesCount;
};

#define POINT_LIGHT_TYPE 0

#ifdef __preshader__
  #define INLINE_FUNCTION() 
  #define INOUT_PARAM(type) inout type
  #define IN_PARAM(type) in type
#else
  #define INLINE_FUNCTION() inline
  #define INOUT_PARAM(type) type &
  #define IN_PARAM(type) const type &
#endif

INLINE_FUNCTION()
void pack_point_light(IN_PARAM(PointLight) src, INOUT_PARAM(Light) dst)
{
  dst.type = POINT_LIGHT_TYPE;
  dst.params1.x = src.attenuationRadius;
  dst.params1.y = src.pos.x;
  dst.params1.z = src.pos.y;
  dst.params2.x = src.pos.z;
  dst.params2.y = src.color.x;
  dst.params2.z = src.color.y;
  dst.params2.w = src.color.z;
}

INLINE_FUNCTION()
void unpack_point_light(IN_PARAM(Light) src, INOUT_PARAM(PointLight) dst)
{
  dst.attenuationRadius = src.params1.x;
  dst.pos.x = src.params1.y;
  dst.pos.y = src.params1.z;
  dst.pos.z = src.params2.x;
  dst.color.x = src.params2.y;
  dst.color.y = src.params2.z;
  dst.color.z = src.params2.w;
}

INLINE_FUNCTION()
uint unpack_light_type(IN_PARAM(Light) src)
{
  return src.type;
}


#endif