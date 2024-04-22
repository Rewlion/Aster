#define __preshader__

#ifndef LIGHTS_APPLY_INCLUDE_GUARD
#define LIGHTS_APPLY_INCLUDE_GUARD

#include "utils.hlsl"
#include <pbr.hlsl>

float pointLightAttenuation(float light_radius, float distance_sq)
{
  //Moving Frostbite to Physically Based Rendering 3.0
  //4.4 Punctual lights

  float n = 4.0;
  float dSq = max(distance_sq, 0.01 * 0.01);
  float invSqAttenRadius = 1.0 / (light_radius * light_radius);
  float factor = invSqAttenRadius * distance_sq;
  return (1.0 / dSq) * pow(saturate( 1.0 - factor*factor ), 2.0);
}

struct DynamicLightsApplyParams
{
  uint2 tc;
  uint2 renderSize;
  float3 worldPos;
  float3 N;
  float NoV;
  float NoH;
  float VoH;
  float zView;
  float zFar;
  float zNear;

  float roughness;
  float ggxAlpha;
  float3 specularReflection;

  StructuredBuffer<ClusterInfo> clustersInfoBuffer;
  StructuredBuffer<ClusterIndirection> indirectionBuffer;
  StructuredBuffer<Light> lightBuffer;
};

void applyPointLight(in PointLight l, in DynamicLightsApplyParams p, out float3 diffuse_no_brdf, out float3 specular)
{
  float3 dl = p.worldPos - l.pos;
  float distanceSq = dot(dl, dl);
  float attenuation = pointLightAttenuation(l.attenuationRadius, distanceSq);
  float3 Li = attenuation * l.color;

  float3 LDir = normalize(dl);
  float NoL =  max(dot(LDir, p.N), 0.0);

  diffuse_no_brdf = Li;
  specular = specularBRDF(p.roughness, p.ggxAlpha, p.specularReflection, NoL, p.NoV, p.NoH, p.VoH) * Li;
}

void dynamicLights(DynamicLightsApplyParams p, out float3 diffuse, out float3 specular)
{
  float2 tileIDf = (float2)p.tc / (float2)p.renderSize * float2(CLUSTERS_DIM_X, CLUSTERS_DIM_Y);
  uint2 tileID = (uint2)tileIDf;

  //invz
  float zFar = p.zNear;
  float zNear = p.zFar;
  float z = p.zView;
  uint slice = floor(log(p.zView / zNear) / log(zFar / zNear) * CLUSTERS_DIM_Z);

  uint clusterId = tileID.x + tileID.y * CLUSTERS_DIM_X + slice * CLUSTERS_DIM_X * CLUSTERS_DIM_Y;
  ClusterInfo clusterInfo = p.clustersInfoBuffer[clusterId];

  uint indirectionOffset = clusterInfo.indirectionOffset;
  uint lightsCount = clusterInfo.lightsCount;

  float3 diffuseL = float3(0.0, 0.0, 0.0);
  float3 specularL = float3(0.0, 0.0, 0.0);

  for (uint i = 0; i < lightsCount; ++i)
  {
    ClusterIndirection lightIndirection = p.indirectionBuffer[indirectionOffset+i];
    uint lightIndex = lightIndirection.lightIndex;

    Light packedLight = p.lightBuffer[lightIndex];
    uint type = unpack_light_type(packedLight);
    if (type == POINT_LIGHT_TYPE)
    {
      PointLight l;
      unpack_point_light(packedLight, l);

      float3 diffuseLight;
      float3 specularLight;
      applyPointLight(l, p, diffuseLight, specularLight);

      diffuseL += diffuseLight;
      specularL += specularLight;
    }
  }

  diffuse = diffuseL;
  specular = specularL;
}

#endif