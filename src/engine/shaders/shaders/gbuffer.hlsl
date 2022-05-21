#define Pi 3.14159265359

//Trowbridge-Reitz GGX
float trowbridge_reitz_ggx(float3 N, float3 h, float roughness)
{
  float a = roughness * roughness;
  float a2 = a*a;
  float NoH = dot(N,h);
  float NoH2 = NoH * NoH;

  float div = NoH2 * (a2 - 1.0) + 1.0;
  div = Pi * div * div;
  return a2 / div;
}

// Schlick-GGX:
float schlick_ggx_sub(float3 N, float3 V, float k)
{
  float NoV = max(dot(N, V), 0);
  return NoV / (NoV * (1-k) + k);
}

float schlick_ggx(float3 N, float3 V, float3 L, float roughness)
{
  float k = roughness + 1.0;
  k = (k * k) / 8.0;
  return schlick_ggx_sub(N,V,k) * schlick_ggx_sub(N,L,k);
}

// Schlick-Fresnel
float4 schlick_fresnel(float4 albedo, float metal, float3 V, float3 h)
{
  float4 F0 = lerp(float4(0.04, 0.04, 0.04, 0.04), albedo, metal);
  float hoV = max(dot(h,V), 0.0);
  return F0 + (1.0 - F0) * pow(1.0 - hoV, 5.0);
}

struct Gbuffer
{
  float4 albedo;
  float metal;
  float roughness;
  float3 normal;
  float3 worldPos;
};

void init_albedo(inout Gbuffer g, float4 albedo)
{
  g.albedo = albedo;
}

void init_metal(inout Gbuffer g, float metal)
{
  g.metal = metal;
}

void init_roughness(inout Gbuffer g, float roughness)
{
  g.roughness = roughness;
}

void init_normal(inout Gbuffer g, float3 normal)
{
  g.normal = normal;
}

void init_worldPos(inout Gbuffer g, float3 worldPos)
{
  g.worldPos = worldPos;
}

float4 encode_gbuffer(Gbuffer g)
{
  float t = frame_uniforms.secSinceStart;
  float3 sunPos = float3( 5.0 * sin(t), -4.0, 5.0 * cos(t) );

  float3 N = g.normal;
  float3 L = normalize(sunPos - g.worldPos);
  float3 V = normalize(frame_uniforms.cameraPos - g.worldPos);
  float LoN = max(dot(L, N), 0.0);
  float VoN = max(dot(V,N), 0.0);

  float3 LV = L+V;
  float3 h = normalize(L+V);

  float NDF = trowbridge_reitz_ggx(N, h, g.roughness);
  float ggx = schlick_ggx(N, V, L, g.roughness);
  float4 F = schlick_fresnel(g.albedo, g.metal, V, h);

  float4 kD = (1.0 - F) * (1.0 - g.metal);
  float4 specular = (NDF*ggx*F) / (4.0 * LoN * VoN + 0.0001);
  float4 lambertDiffuse = kD * g.albedo / Pi;
  float4 ambient = g.albedo * 0.05;

  float4 Ri = float4(1.0, 1.0, 1.0, 1.0) * LoN;
  float4 Ro = (lambertDiffuse + specular) * Ri + ambient;

  Ro = pow(Ro, 2.2);

  return Ro;
}