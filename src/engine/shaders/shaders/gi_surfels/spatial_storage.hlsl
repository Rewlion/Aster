#ifndef SPATIAL_STORAGE_HLSL
#define SPATIAL_STORAGE_HLSL

#include "lifetime.hlsl"
#include "spatial_hash.hlsl"
#include "surfel.hlsl"

#include <color_ramp.hlsl>
#include <intersections.hlsl>
#include <intersection_structs.hlsl>

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

  bool insertSurfelInCell(uint surfel_id, int3 cell_id, int cascade)
  {
    uint baseAddr = getCellAddress(cell_id, cascade);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint vacantIdPos;
    InterlockedAdd(surfelsSpatialStorage[counter], 1, vacantIdPos);
    if (vacantIdPos < SURFEL_COUNT_PER_CELL)
    {
      surfelsSpatialStorage[idBegin+vacantIdPos] = surfel_id;
      return true;
    }
    else
    {
      uint prev;
      InterlockedMin(surfelsSpatialStorage[counter], SURFEL_COUNT_PER_CELL, prev);
      return false;
    }
  }

  bool insertSurfelCenterInLinearGrid(SurfelData surfel, uint surfel_id, float3 camera_wpos, out int3 center_cell_id)
  {
    float3 cameraToWpos = surfel.pos - camera_wpos;
    SpatialInfo spatialInfo = calcLinearInfo(cameraToWpos);
    center_cell_id = spatialInfo.id;

    return insertSurfelInCell(surfel_id, spatialInfo.id, CASCADE_ZERO);
  }

  void insertSurfelInLinearGridNeighbors(SurfelData surfel, uint surfel_id, float3 camera_wpos, int3 center_cell_id)
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
          if (all(cellId == center_cell_id))
            continue;

          GridAABB aabb = calcLinearAABB(cellId, camera_wpos);

          bool isSurfelInsideCell = test_aabb_sphere_intersection(aabb.minWS, aabb.maxWS, surfel.pos, surfel.radius);
          if (isSurfelInsideCell)
            insertSurfelInCell(surfel_id, cellId, CASCADE_ZERO);
        }
  }

  void insertSurfelInLinearGrid(SurfelData surfel, uint surfel_id, float3 camera_wpos)
  {
    int3 centerCellId;
    insertSurfelCenterInLinearGrid(surfel, surfel_id, camera_wpos, centerCellId);
    insertSurfelInLinearGridNeighbors(surfel, surfel_id, camera_wpos, centerCellId);
  }

  void calcMinMaxNonlinearCellIds(SurfelData surfel, PointInCascade surfel_center_in_cascade, out int3 min_id, out int3 max_id)
  {
    int2 minmaxX = calcNonlinearXYBounding(surfel.radius, surfel_center_in_cascade.pos.xz);
    int2 minmaxY = calcNonlinearXYBounding(surfel.radius, surfel_center_in_cascade.pos.yz);
    int2 minmaxZ = calcNonlinearZBounding(surfel.radius, surfel_center_in_cascade.pos.z, zFar);

    min_id = int3(minmaxX[0], minmaxY[0], minmaxZ[0]);
    max_id = int3(minmaxX[1], minmaxY[1], minmaxZ[1]);
  }

  void insertSurfelInNonLinearGridNeighbors(int3 center_cell_id, int3 min_cell_id, int3 max_cell_id, SurfelData surfel,
                                            uint surfel_id, PointInCascade surfel_center_in_cascade, StructuredBuffer<AABB> nonlinear_aabbs)
  {
    min_cell_id = clampSpatialId(min_cell_id);
    max_cell_id = clampSpatialId(max_cell_id);

    for (int x = min_cell_id.x; x <= max_cell_id.x; ++x)
      for (int y = min_cell_id.y; y <= max_cell_id.y; ++y)
        for (int z = min_cell_id.z; z <= max_cell_id.z; ++z)
        {
          uint cellId = linearizeCellsID(uint3(x,y,z));
          if (all(cellId == center_cell_id))
            continue;

          AABB aabb = nonlinear_aabbs[cellId];

          bool isSurfelInsideCell = test_aabb_sphere_intersection(aabb.minp, aabb.maxp, surfel_center_in_cascade.pos, surfel.radius);
          if (isSurfelInsideCell)
            insertSurfelInCell(surfel_id, int3(x,y,z), surfel_center_in_cascade.cascade);
        }
  }

  bool insertSurfelCenterInNonLinearGrid(PointInCascade surfel_center_in_cascade, uint surfel_id, out int3 center_cell_id)
  {
    center_cell_id = posInNonLinearCascadeToCellID(surfel_center_in_cascade.pos, zFar);
    return insertSurfelInCell(surfel_id, center_cell_id, surfel_center_in_cascade.cascade);
  }

  void insertSurfelInNonLinearGrid(SurfelData surfel, uint surfel_id, PointInCascade surfel_center_in_cascade, StructuredBuffer<AABB> nonlinear_aabbs)
  {
    int3 centerCellId;
    insertSurfelCenterInNonLinearGrid(surfel_center_in_cascade, surfel_id, centerCellId);

    int3 minId, maxId;
    calcMinMaxNonlinearCellIds(surfel, surfel_center_in_cascade, minId, maxId);
    insertSurfelInNonLinearGridNeighbors(centerCellId, minId, maxId, surfel, surfel_id, surfel_center_in_cascade, nonlinear_aabbs);
  }

  //fails if there is no free space in the surfel's cell
  bool insertSurfel(SurfelData surfel, uint surfel_id, float3 camera_wpos, StructuredBuffer<AABB> nonlinear_aabbs)
  {
    float3 cameraToWorldPos = surfel.pos - camera_wpos;

    [BRANCH]
    if (isLinearTransform(cameraToWorldPos))
    {
      int3 centerCellId;
      if(!insertSurfelCenterInLinearGrid(surfel, surfel_id, camera_wpos, centerCellId))
        return false;

      insertSurfelInLinearGridNeighbors(surfel, surfel_id, camera_wpos, centerCellId);

      //intersections with nonlinear grid
      float3 e = surfel.radius.xxx;
      float3 cameraToMin = cameraToWorldPos - e;
      float3 cameraToMax = cameraToWorldPos + e;

      SpatialInfo minP = calcLinearInfo(cameraToMin);
      SpatialInfo maxP = calcLinearInfo(cameraToMax);

      PointInCascade surfelCenterInCascade = (PointInCascade)0;

      if (minP.id.x < 0 || maxP.id.x >= CELLS_DIM)
      {
        surfelCenterInCascade = transformFromCameraWorldSpaceToXCascadeSpace(cameraToWorldPos);
        insertSurfelInNonLinearGrid(surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);
      }
      if (minP.id.y < 0 || maxP.id.y >= CELLS_DIM)
      {
        surfelCenterInCascade = transformFromCameraWorldSpaceToYCascadeSpace(cameraToWorldPos);
        insertSurfelInNonLinearGrid(surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);
      }
      if (minP.id.z < 0 || maxP.id.z >= CELLS_DIM)
      {
        surfelCenterInCascade = transformFromCameraWorldSpaceToZCascadeSpace(cameraToWorldPos);
        insertSurfelInNonLinearGrid(surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);
      }
    }
    else
    {
      PointInCascade surfelCenterInCascade = transformFromCameraWorldSpaceToCascadeSpace(cameraToWorldPos);

      int3 centerCellId;
      if (!insertSurfelCenterInNonLinearGrid(surfelCenterInCascade, surfel_id, centerCellId))
        return false;

      int3 minId, maxId;
      calcMinMaxNonlinearCellIds(surfel, surfelCenterInCascade, minId, maxId);
      insertSurfelInNonLinearGridNeighbors(centerCellId, minId, maxId, surfel, surfel_id, surfelCenterInCascade, nonlinear_aabbs);

      //intersect with neighbor grids
      if (minId.z < 0)
        insertSurfelInLinearGrid(surfel, surfel_id, camera_wpos);

      if (minId.x < 0 || maxId.x >= CELLS_DIM)
        insertSurfelInNonLinearGrid(surfel, surfel_id, getLeftRightCascadePoint(surfelCenterInCascade, cameraToWorldPos), nonlinear_aabbs);

      if (minId.y < 0 || maxId.y >= CELLS_DIM)
        insertSurfelInNonLinearGrid(surfel, surfel_id, getTopBotCascadePoint(surfelCenterInCascade, cameraToWorldPos), nonlinear_aabbs);
    }

    return true;
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

  bool checkFreeSpaceForNewSurfel(float3 wpos, float3 camera_to_wpos, StructuredBuffer<SurfelData> surfels_storage, RWStructuredBuffer<uint> surfels_lifetime)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

    if (idsCount >= SURFEL_COUNT_PER_CELL)
      return false;

    float prevOverlapScore = 0.0;
    uint overlapSurfelId;
    uint overlapsCount = 0;
    Lifetime overlappingSurfelLifetime;

    float sdf = MAX_FLOAT;
    for (uint i = idBegin; (i < idEnd) && (overlapsCount < 2); ++i)
    {
      uint surfelId = surfelsSpatialStorage[i];
      SurfelData surfel = surfels_storage[surfelId];

      float3 dr = wpos - surfel.pos;
      float dl = length(dr);
      float dist = dl - surfel.radius;
      sdf = min(sdf, dist);

      float overlapScore = 1.0 - dl / surfel.radius;
      if (dist < 0 && overlapScore > prevOverlapScore)
      {
        Lifetime lifetime = {surfels_lifetime[surfelId]};
        if (!lifetime.isPendingRecycle())
        {
          prevOverlapScore = overlapScore;
          overlappingSurfelLifetime = lifetime;
          overlapSurfelId = surfelId;
          ++overlapsCount;
        }
      }
    }

    float newSurfelRadius = calcSurfelRadius(spatialInfo.cellSize);

    if (overlapsCount > 1 && prevOverlapScore > 0.9)
    {
      overlappingSurfelLifetime.markPendingRecycle();
      surfels_lifetime[overlapSurfelId] = overlappingSurfelLifetime.data;
    }


    return newSurfelRadius*1.9 <= sdf;
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

    float t = N / 10.0;
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
      float3(0.2, 0.1, 0.1),
      float3(0, 0.8, 0),
      float3(0.1, 0.2, 0.1),
      float3(0, 0, 0.8),
      float3(0.1, 0.1, 0.2)
    };
    int3 dId = (spatialInfo.id.xyz % 2);
    float m = all(dId == int3(1,1,1)) ||  all(dId == int3(0,0,1)) ||  all(dId == int3(0,1,0)) ||  all(dId == int3(1,0,0)) ? 1.0 : 0.5;
    float m2 = (float((spatialInfo.id.x+1)*10) / CELLS_DIM) * (float(spatialInfo.id.y+1) / (CELLS_DIM*2));
    //float m = any(spatialInfo.id <= CELLS_DIM/2) ? 0.5 : 1.0;
    return float4(colors[spatialInfo.cascade] * m * m2,1);
    //return float4(spatialInfo.id, 1.0);
  }

  template<typename Cb>
  void forEachSurfelInCell(float3 wpos, float3 camera_to_wpos, inout Cb cb)
  {
    SpatialInfo spatialInfo = calcSpatialInfo(camera_to_wpos, zFar);
    uint baseAddr = getCellAddress(spatialInfo);
    uint counter = baseAddr;
    uint idBegin = baseAddr+1;
    uint idsCount = surfelsSpatialStorage[counter];
    uint idEnd = idBegin+idsCount;

    for (uint i = idBegin; (i < idEnd) && !cb.finish(); ++i)
    {
      uint surfelId = surfelsSpatialStorage[i];
      cb.onSurfel(surfelId);
    }
  }
};

#endif