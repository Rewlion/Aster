#ifndef SPATIAL_STORAGE_HLSL
#define SPATIAL_STORAGE_HLSL

#include "spatial_hash.hlsl"
#include "surfel.hlsl"
#include "../color_ramp.hlsl"

uint getCellAddress(SpatialInfo si)
{
  uint cascadeBegin = si.cascade * CELLS_PER_CASCADE * SPATIAL_STORAGE_CELL_PAYLOAD;
  uint cellId = calcSpatial1DIndex(si.id);
  uint cellBegin = cascadeBegin + cellId * SPATIAL_STORAGE_CELL_PAYLOAD;
  return cellBegin;
}

struct SpatialStorage
{
  RWStructuredBuffer<uint> surfelsSpatialStorage;
  float zFar;
  //

  void insertSurfelInCell(uint id, float3 camera_to_wpos)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint vacantIdPos;
    InterlockedAdd(surfelsSpatialStorage[counter], 1, vacantIdPos);
    if (vacantIdPos < SURFEL_COUNT_PER_CELL)
    {
      surfelsSpatialStorage[idBegin+vacantIdPos] = id;
    }
    else
    {
      uint prev;
      InterlockedMin(surfelsSpatialStorage[counter], SURFEL_COUNT_PER_CELL, prev);
    }
  }
};

struct ROSpatialStorage
{
  StructuredBuffer<uint> surfelsSpatialStorage;
  float zFar;
  //

  uint calcCoverageForPos(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

    uint coverage = 0;

    for (uint i = idBegin; i < idEnd; ++i)
    {
      uint surfelId = surfelsSpatialStorage[i];
      SurfelData surfel = surfels_storage[surfelId];

      float3 dr = wpos - surfel.pos;
      coverage += dot(dr,dr) <= (surfel.radius * surfel.radius) ? 1 : 0;
    }

    return coverage;
  }

  #define MAX_FLOAT 3.402823466e+38

  bool checkFreeSpaceForNewSurfel(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

    float sdf = MAX_FLOAT;
    for (uint i = idBegin; (i < idEnd) && (sdf > 0.0); ++i)
    {
      uint surfelId = surfelsSpatialStorage[i];
      SurfelData surfel = surfels_storage[surfelId];

      float3 dr = wpos - surfel.pos;
      float dist = clamp(length(dr) - surfel.radius, 0.0, MAX_FLOAT);
      sdf = min(sdf, dist);
    }

    float newSurfelRadius = calcSurfelRadius(spatialInfo.cellSize);

    return newSurfelRadius <= sdf;
  }

  float4 drawSurfels(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

    float3 color = float3(0,0,0);
    uint N = 0;

    for (uint i = idBegin; i < idEnd; ++i)
    {
      uint surfelId = surfelsSpatialStorage[i];
      SurfelData surfel = surfels_storage[surfelId];

      float3 dr = wpos - surfel.pos;
      float dist = length(dr) - surfel.radius;
      
      if (dist <= 0)
      {
        ++N;
        //float t = (float) (calcSpatialHash(abs(surfel.pos), 1.0) % CELLS_PER_CASCADE) / float(CELLS_PER_CASCADE);
        float t = (float)(surfelId) / 15.0;
        t = frac(t);
        if (dist/surfel.radius > -0.1)
          color += float3(0.4,0.4,0.4);
        else
          color += rgbColorRamp(t);
      }
    }

    color = color / max(N,1);
    float a = N > 0 ? 1 : 0;

    return float4(color, a);
  }
};

#endif