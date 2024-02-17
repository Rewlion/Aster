#ifndef SPATIAL_STORAGE_HLSL
#define SPATIAL_STORAGE_HLSL

#include "spatial_hash.hlsl"
#include "surfel.hlsl"
#include "../color_ramp.hlsl"
#include "../intersections.hlsl"
#include "../intersection_structs.hlsl"

uint getCellAddress(int3 id, int cascade)
{
  uint cascadeBegin = cascade * CELLS_PER_CASCADE * SPATIAL_STORAGE_CELL_PAYLOAD;
  uint cellId = calcSpatial1DIndex(id);
  uint cellBegin = cascadeBegin + cellId * SPATIAL_STORAGE_CELL_PAYLOAD;
  return cellBegin;
}

uint getCellAddress(SpatialInfo s)
{
  return getCellAddress(s.id, s.cascade);
}

struct SpatialStorage
{
  RWStructuredBuffer<uint> surfelsSpatialStorage;
  float zFar;
  //

  void insertSurfelInCell(uint surfel_id, int3 cell_id, int cascade)
  {
    uint baseAddr = getCellAddress(cell_id, cascade);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint vacantIdPos;
    InterlockedAdd(surfelsSpatialStorage[counter], 1, vacantIdPos);
    if (vacantIdPos < SURFEL_COUNT_PER_CELL)
    {
      surfelsSpatialStorage[idBegin+vacantIdPos] = surfel_id;
    }
    else
    {
      uint prev;
      InterlockedMin(surfelsSpatialStorage[counter], SURFEL_COUNT_PER_CELL, prev);
    }
  }

  void insertSurfelInLinearGrid(SurfelData surfel, uint surfel_id, float3 camera_wpos)
  {
    float3 e = surfel.radius.xxx;
    float3 cameraToMin = surfel.pos - e - camera_wpos;
    float3 cameraToMax = surfel.pos + e - camera_wpos;

    SpatialInfo minP = calcLinearInfo(cameraToMin);
    SpatialInfo maxP = calcLinearInfo(cameraToMax);

    int3 minId = clampSpatialId(minP.id);
    int3 maxId = clampSpatialId(maxP.id);

    for (int x = minId.x; x <= maxId.x; ++x)
      for (int y = minId.y; y <= maxId.y; ++y)
        for (int z = minId.z; z <= maxId.z; ++z)
        {
          int3 cellId = int3(x,y,z);
          GridAABB aabb = calcLinearAABB(cellId, camera_wpos);

          bool isSurfelInsideCell = test_aabb_sphere_intersection(aabb.minWS, aabb.maxWS, surfel.pos, surfel.radius);
          if (isSurfelInsideCell)
            insertSurfelInCell(surfel_id, cellId, CASCADE_ZERO);
        }
  }

  void insertSurfelInNonLinearGrid(SurfelData surfel, uint surfel_id, PointInCascade surfel_center_in_cascade, StructuredBuffer<AABB> nonlinear_aabbs)
  {
    float3 e = surfel.radius.xxx;
    float3 minPos = surfel_center_in_cascade.pos - e*2; //fixme: has to be e*1
    float3 maxPos = surfel_center_in_cascade.pos + e*2;
    
    int3 minId = posInNonLinearCascadeToCellID(minPos, zFar);
    int3 maxId = posInNonLinearCascadeToCellID(maxPos, zFar);

    for (int x = minId.x; x <= maxId.x; ++x)
      for (int y = minId.y; y <= maxId.y; ++y)
        for (int z = minId.z; z <= maxId.z; ++z)
        {
          uint cellId = linearizeCellsID(uint3(x,y,z));
          AABB aabb = nonlinear_aabbs[cellId];

          bool isSurfelInsideCell = test_aabb_sphere_intersection(aabb.minp, aabb.maxp, surfel_center_in_cascade.pos, surfel.radius);
          if (isSurfelInsideCell)
            insertSurfelInCell(surfel_id, int3(x,y,z), surfel_center_in_cascade.cascade);
        }
  }

  void insertSurfel(SurfelData surfel, uint surfel_id, float3 camera_wpos, StructuredBuffer<AABB> nonlinear_aabbs)
  {
    float3 cameraToWorldPos = surfel.pos - camera_wpos;

    [BRANCH]
    if (isLinearTransform(cameraToWorldPos))
    {
      insertSurfelInLinearGrid(surfel, surfel_id, camera_wpos);

      //intersections with nonlinear grid
      float3 e = surfel.radius.xxx;
      float3 cameraToMin = cameraToWorldPos - e;
      float3 cameraToMax = cameraToWorldPos + e;

      SpatialInfo minP = calcLinearInfo(cameraToMin);
      SpatialInfo maxP = calcLinearInfo(cameraToMax);

      PointInCascade surfelCenterInCascade = (PointInCascade)0;
      if      (minP.id.x < 0 || maxP.id.x >= CELLS_DIM)
        surfelCenterInCascade = transformFromCameraWorldSpaceToXCascadeSpace(cameraToWorldPos);
      else if (minP.id.y < 0 || maxP.id.y >= CELLS_DIM)
        surfelCenterInCascade = transformFromCameraWorldSpaceToYCascadeSpace(cameraToWorldPos);
      else if (minP.id.z < 0 || maxP.id.z >= CELLS_DIM)
        surfelCenterInCascade = transformFromCameraWorldSpaceToZCascadeSpace(cameraToWorldPos);

      if (surfelCenterInCascade.cascade != CASCADE_ZERO)
        insertSurfelInNonLinearGrid(surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);
    }
    else
    {
      PointInCascade surfelCenterInCascade = transformFromCameraWorldSpaceToCascadeSpace(cameraToWorldPos);
      insertSurfelInNonLinearGrid(surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);
      
      // //intersect with linear cascade
      float3 e = surfel.radius.xxx;
      float3 minPos = surfelCenterInCascade.pos - e*2; //fixme: has to be e*1
      if (minPos.z <= NONLINEAR_NEAR_PLANE)
      {
        insertSurfelInLinearGrid(surfel, surfel_id, camera_wpos);
      }

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

  float4 drawSurfelsCoverage(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

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
      }
    }

    float t = N / (float)SURFEL_COUNT_PER_CELL;
    float3 color = heatmapColorRamp(t);

    return float4(color, N > 0 ? 1 : 0);
  }

  float4 drawSurfelsSDF(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
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

    float sR = calcSurfelRadius(spatialInfo.cellSize);
    float t = sdf / sR;

    float newSurfelRadius = calcSurfelRadius(spatialInfo.cellSize);
    float3 color = heatmapColorRamp(t);

    return float4(color,1);
  }

  float4 drawCellsCoverage(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idsCount = surfelsSpatialStorage[counter];
    
    float t = (float)idsCount / (float)SURFEL_COUNT_PER_CELL;
    float3 color = heatmapColorRamp(t);

    return float4(color,1);
  }

  float4 drawCells(float3 wpos, float3 camera_to_wpos)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    float3 colors[1+6] = {
      float3(0.8,0.8,0.8),
      float3(0.8, 0, 0),
      float3(0.4, 0.1, 0.1),
      float3(0, 0.8, 0),
      float3(0.1, 0.4, 0.1),
      float3(0, 0, 0.8),
      float3(0.1, 0.1, 0.4)
    };
    int3 dId = (spatialInfo.id.xyz % 2);
    float m = all(dId == int3(1,1,1)) ||  all(dId == int3(0,0,1)) ||  all(dId == int3(0,1,0)) ||  all(dId == int3(1,0,0)) ? 1.0 : 0.5;

    return float4(colors[spatialInfo.cascade] * m,1);
  }
};

#endif