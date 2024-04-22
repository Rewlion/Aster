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

//////////////////////////
// UP DIR: Z
//
// http://jcgt.org/published/0007/04/01/paper.pdf by Eric Heitz
// Input Ve: view direction
// Input alpha_x, alpha_y: roughness parameters
// Input U1, U2: uniform random numbers
// Output Ne: normal sampled with PDF D_Ve(Ne) = G1(Ve) * max(0, dot(Ve, Ne)) * D(Ne) / Ve.z
float3 sampleGGXvNDF(float3 Ve, float alpha_x, float alpha_y, float U1, float U2) {
    // Section 3.2: transforming the view direction to the hemisphere configuration
    float3 Vh = normalize(float3(alpha_x * Ve.x, alpha_y * Ve.y, Ve.z));
    // Section 4.1: orthonormal basis (with special case if cross product is zero)
    float lensq = Vh.x * Vh.x + Vh.y * Vh.y;
    float3 T1 = lensq > 0 ? float3(-Vh.y, Vh.x, 0) * rsqrt(lensq) : float3(1, 0, 0);
    float3 T2 = cross(Vh, T1);
    // Section 4.2: parameterization of the projected area
    float r = sqrt(U1);
    float phi = 2.0 * PI * U2;
    float t1 = r * cos(phi);
    float t2 = r * sin(phi);
    float s = 0.5 * (1.0 + Vh.z);
    t2 = (1.0 - s) * sqrt(1.0 - t1 * t1) + s * t2;
    // Section 4.3: reprojection onto hemisphere
    float3 Nh = t1 * T1 + t2 * T2 + sqrt(max(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;
    // Section 3.4: transforming the normal back to the ellipsoid configuration
    float3 Ne = normalize(float3(alpha_x * Nh.x, alpha_y * Nh.y, max(0.0, Nh.z)));
    return Ne;
}

float3 sampleGGXvNDFEllipsoid(float3 Ve, float alpha_x, float alpha_y, float U1, float U2) {
    return sampleGGXvNDF(Ve, alpha_x, alpha_y, U1, U2);
}

float3 sampleGGXvNDFHemisphere(float3 Ve, float alpha, float U1, float U2) {
    return sampleGGXvNDFEllipsoid(Ve, alpha, alpha, U1, U2);
}
//////////////////////////

#endif