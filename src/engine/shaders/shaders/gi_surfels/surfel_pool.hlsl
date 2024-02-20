#ifndef SURFEL_POOL_HLSL
#define SURFEL_POOL_HLSL

#include "meta.hlsl"
#include "consts.hlsl"

struct SurfelsPool
{
  RWStructuredBuffer<SurfelsMeta> meta;
  RWStructuredBuffer<uint> pool;

  uint acquireFreeSurfelId()
  {
    uint id;
    InterlockedAdd(meta[0].allocatedSurfelsCount, 1, id);
    InterlockedMin(meta[0].allocatedSurfelsCount, SURFEL_COUNT_TOTAL);

    if (id != SURFEL_COUNT_TOTAL)
    {
      uint poolId = id;
      uint storageId = pool[poolId];

      return storageId;
    }
    else
      return NO_AVAILABLE_SURFEL;
  }

  void releaseSurfelId(uint surfel_id)
  {
    uint poolId;
    InterlockedAdd(meta[0].allocatedSurfelsCount, -1, poolId);

    pool[poolId] = surfel_id;
  }
};

#endif