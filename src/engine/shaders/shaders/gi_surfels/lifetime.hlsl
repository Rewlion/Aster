#ifndef LIFETIME_HLSL
#define LIFETIME_HLSL

#define SURFEL_STATE_UNUSED uint(-1)

bool is_surfel_alive(uint life_time)
{
  return life_time != SURFEL_STATE_UNUSED;
}

#endif