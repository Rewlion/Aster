#ifndef SURFEL_NORM_HLSL
#define SURFEL_NORM_HLSL

#include "surfel.hlsl"

float calcSurfelEllipsoidNorm(float3 world_pos, SurfelData surfel)
{
  float3 dr = world_pos - surfel.pos;
  float F_scquash = 2;
  //https://www.cs.umd.edu/~zwicker/publications/MeshlessRepresentationForLightTransport-SIG08.pdf
  //2.2 weight functions
  return length(dr + F_scquash * abs(dot(dr, surfel.normal)) * surfel.normal);
}

#endif
