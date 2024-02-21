#ifndef LIFETIME_HLSL
#define LIFETIME_HLSL

#define SURFEL_STATE_UNUSED          uint(0)
#define SURFEL_STATE_PENDING_RECYCLE uint(1)
#define SURFEL_STATE_ALLOCATED       uint(2)

#define SURFEL_LIFETIME_COUNT_MASK 0xFFFF
#define SURFEL_LIFETIME_COUNT_BITS 16

#ifdef __preshader__
#include "surfel_pool.hlsl"

struct Lifetime
{
  uint data;

  uint getState()
  {
    return data >> SURFEL_LIFETIME_COUNT_BITS;
  }

  void setState(uint state)
  {
    data = (state << SURFEL_LIFETIME_COUNT_BITS) | (data & SURFEL_LIFETIME_COUNT_MASK);
  }

  uint getCounter()
  {
    return data & SURFEL_LIFETIME_COUNT_MASK;
  }

  bool isAlive()
  {
    return getState() != SURFEL_STATE_UNUSED;
  }

  void markUnused()
  {
    data = 0;
  }

  void markPendingRecycle()
  {
    setState(SURFEL_STATE_PENDING_RECYCLE);
  }

  void markAllocated()
  {
    setState(SURFEL_STATE_ALLOCATED);
  }
};

void freeSurfel(uint surfel_id, RWStructuredBuffer<uint> surfels_lifetime, SurfelsPool pool)
{
  Lifetime lt = {0};
  lt.markUnused();

  surfels_lifetime[surfel_id] = lt.data;
  pool.releaseSurfelId(surfel_id);
}
#endif

#endif