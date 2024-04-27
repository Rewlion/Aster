#ifndef SURFELS_HLSL
#define SURFELS_HLSL

#include "consts.hlsl"
#include "multiscale_mean_estimator.hlsl"

struct SurfelData
{
  float3 pos;
  float radius;
  float3 normal;
  float rayguidingPmfScale;
  float3 irradiance;
  uint rayguidingProcessedSamples;
  MultiscaleMeanEstimatorData estimatorData;
  uint rayBudget;
  uint3 _p;
  
#ifdef __preshader__
  void init(float3 _pos, float _radius, float3 _normal)
  {
    pos = _pos;
    radius = _radius;
    normal = _normal;
    rayguidingPmfScale = 1.0;
    irradiance = float3(0,0,0);
    rayguidingProcessedSamples = 0;
    estimatorData.init();
    rayBudget = 0;
  }
#endif
};

#ifdef __preshader__
float calcSurfelRadius(float cell_size)
{
  return SURFEL_RADIUS * cell_size;
}
#endif


#endif