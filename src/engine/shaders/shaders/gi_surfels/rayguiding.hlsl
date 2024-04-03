#ifndef RAYGUIDING_HLSL
#define RAYGUIDING_HLSL

#ifdef __preshader__

#include "consts.hlsl"
#include "surfel.hlsl"

#include <encoding.hlsl>
#include <math.hlsl>

uint2 calcRayguidingMapTexelBegin(uint surfel_id)
{
  uint xStorage = surfel_id % RAYGUIDE_TEX_SURFEL_DIM;
  uint yStorage = surfel_id / RAYGUIDE_TEX_SURFEL_DIM;

  return uint2(xStorage, yStorage) * RAYGUIDE_CELL_DIM;
}

void initializeRayguidingMap(inout RWTexture2D<float2> rayguiding_map, uint surfel_id)
{
  uint2 storageTexelBegin = calcRayguidingMapTexelBegin(surfel_id);
  float initPmf = 1.0 / RAYGUIDE_CELL_COUNT;
  float initLuminance = 0.0f;
  float2 initValue;
  initValue[RAYGUIDE_MAP_PMF] = initPmf;
  initValue[RAYGUIDE_MAP_LUMINANCE] = initLuminance;

  for (uint y = 0; y < RAYGUIDE_CELL_DIM; ++y)
    for (uint x = 0; x < RAYGUIDE_CELL_DIM; ++x)
    {
      uint2 texel = storageTexelBegin + uint2(x,y);
      rayguiding_map[texel] = initValue;
    }
}

float renormalizeRayguidingMap(RWTexture2D<float2> rayguiding_map, uint surfel_id)
{
  uint2 beginTexel = calcRayguidingMapTexelBegin(surfel_id);
  
  float luminanceCache[RAYGUIDE_CELL_COUNT];
  float luminanceSum = 0.0;

  for (uint y = 0; y < RAYGUIDE_CELL_DIM; ++y)
  {
    for (uint x = 0; x < RAYGUIDE_CELL_DIM; ++x)
    {
      float luminance = rayguiding_map[beginTexel + uint2(x,y)][RAYGUIDE_MAP_LUMINANCE];

      luminanceSum += luminance;
      luminanceCache[x + y * RAYGUIDE_CELL_DIM] = luminance;
    }
  }

  if (luminanceSum == 0.0)
    luminanceSum = 1.0;
  
  float minLuminance = luminanceSum / RAYGUIDE_CELL_COUNT;
  
  float pmfSum = 0.0;
  for (uint y = 0; y < RAYGUIDE_CELL_DIM; ++y)
  {
    for (uint x = 0; x < RAYGUIDE_CELL_DIM; ++x)
    {
      float luminance = luminanceCache[x + y * RAYGUIDE_CELL_DIM];

      float newPmf = max(luminance / luminanceSum, minLuminance);
      float newLuminance = 0.0;

      float2 guideVal;
      guideVal[RAYGUIDE_MAP_PMF] = newPmf;
      guideVal[RAYGUIDE_MAP_LUMINANCE] = newLuminance;

      rayguiding_map[beginTexel + uint2(x,y)] = guideVal;
      pmfSum += newPmf;
    }
  }

  return 1.0 / pmfSum;
}

struct RayGuiding
{
  uint surfelID;
  uint2 storageTexelBegin;
  RWTexture2D<float2> rayguidingMap;
  float pmfScale;

  void init(RWTexture2D<float2> rayguiding_map, float pmf_scale, uint surfel_id)
  {
    surfelID = surfel_id;
    storageTexelBegin = calcRayguidingMapTexelBegin(surfelID);
    rayguidingMap = rayguiding_map;
    pmfScale = pmf_scale;
  }

  float getPmf(uint2 texel)
  {
    return rayguidingMap[storageTexelBegin + texel][RAYGUIDE_MAP_PMF] * pmfScale;
  }

  float3 guide(float3 random, out uint2 feedback_texel, out float probability)
  {
    float sumF = random.x;

    uint2 texel = uint2(0,0);
    float cdf = 0.0;
    bool found = false;
    float targetPmf = 1.0 / RAYGUIDE_CELL_COUNT;
    uint2 targetTexel = uint2(0,0);

    for (texel.y = 0; texel.y < RAYGUIDE_CELL_DIM && !found; ++texel.y)
      for (texel.x = 0; texel.x < RAYGUIDE_CELL_DIM && !found; ++texel.x)
      {
        float pmf = getPmf(texel);
        cdf += pmf;

        if (cdf >= sumF)
        {
          found = true;
          targetPmf = pmf;
          targetTexel = texel;
        }
      }

    float2 relativeTexel = targetTexel;

    float texelSize = 1.0 / RAYGUIDE_CELL_DIM;
    float2 texelCenter = texelSize.xx * 0.5;

    float2 texelUV = texelCenter + texelSize * relativeTexel;
    float2 perturbRadius = texelCenter;
    float2 preturbedTexelUV = texelUV + lerp(-perturbRadius, perturbRadius, random.yz);
    
    float2 encodedRotation = preturbedTexelUV * 2.0 - 1.0;

    feedback_texel = storageTexelBegin + relativeTexel;
    
    float P_texel = targetPmf;
    float texelAreaOnHemisphere = 2.0 * PI / RAYGUIDE_CELL_COUNT;
    float P_perturbedDir = 1.0 / texelAreaOnHemisphere;
    probability = P_texel * P_perturbedDir;

    return hemioct_to_float32x3(encodedRotation).xzy;
  }

  void feedbackLuminance(uint2 selected_texel, float new_luminance)
  {
    rayguidingMap[selected_texel][RAYGUIDE_MAP_LUMINANCE] += new_luminance;
  }
};

#endif

#endif
