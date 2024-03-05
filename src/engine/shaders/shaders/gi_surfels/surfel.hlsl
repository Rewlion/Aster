#ifndef SURFELS_HLSL
#define SURFELS_HLSL

#include "consts.hlsl"

float calcSurfelRadius(float cell_size)
{
  return SURFEL_RADIUS * cell_size;
}

struct SurfelData
{
  float3 pos;
  float radius;
  float3 normal;
  float _p;
  float3 irradiance;
  uint irradianceHistory;
  
  void init(float3 _pos, float _radius, float3 _normal)
  {
    pos = _pos;
    radius = _radius;
    normal = _normal;
    irradiance = float3(0,0,0);
    irradianceHistory = 0;
  }
};

#endif