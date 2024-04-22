#ifndef PBR_HLSL
#define PBR_HLSL

#include "math.hlsl"

//D, NDF
float trowbridgeReitzGGX(float ggx_alpha, float NoH)
{
  float a2 = ggx_alpha * ggx_alpha;
  float NoH2 = NoH * NoH;

  float div = NoH2 * (a2 - 1.0) + 1.0;
  div = PI * div * div;
  return a2 / div;
}

//G
float schlickSmithGGXSub(float NoB, float k)
{
  return NoB / (NoB * (1-k) + k);
}

float schlickSmithGGX(float NoL, float NoV, float roughness)
{
  float k = roughness + 1.0;
  k = (k * k) / 8.0;
  return schlickSmithGGXSub(NoV,k) * schlickSmithGGXSub(NoL,k);
}

float schlickSmithGGX_IBL(float NoL, float NoV, float ggx_alpha)
{
  float k = ggx_alpha / 2.0;
  return schlickSmithGGXSub(NoV,k) * schlickSmithGGXSub(NoL,k);
}

//F
float3 schlickFresnel(float3 f0, float VoH)
{
  float3 f90 = saturate (50.0 * dot(f0, 0.33) );
  return f0 + (f90 - f0) * pow(1.0 - VoH, 5.0);
}

float3 diffuseBRDF(float3 diffuse_color, float3 specular_reflection, float VoH)
{
  return diffuse_color * (1.0 - schlickFresnel(specular_reflection, VoH)) / PI;
}

float3 specularBRDF(float roughness, float ggx_alpha, float3 specular_reflection, float NoL, float NoV, float NoH, float VoH)
{
  float D = trowbridgeReitzGGX(ggx_alpha, NoH);
  float G = schlickSmithGGX(NoL, NoV, roughness);
  float3 F = schlickFresnel(specular_reflection, VoH);

  return (D*G*F) / (4.0 * NoL * NoV + 0.0001);
}

float roughnessToGGXAlpha(float roughness)
{
  return roughness * roughness;
}

#endif