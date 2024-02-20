#ifndef LIFETIME_HLSL
#define LIFETIME_HLSL

#include "surfel_pool.hlsl"

#define SURFEL_STATE_UNUSED uint(-1)

bool isSurfelAlive(uint life_time)
{
  return life_time != SURFEL_STATE_UNUSED;
}

void freeSurfel(uint surfel_id, RWStructuredBuffer<uint> surfels_lifetime, SurfelsPool pool)
{
  surfels_lifetime[surfel_id] = SURFEL_STATE_UNUSED;
  pool.releaseSurfelId(surfel_id);
}

#endif