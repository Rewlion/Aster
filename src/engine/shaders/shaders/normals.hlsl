#ifndef NORMALS_HLSL
#define NORMALS_HLSL

//http://www.thetenthplanet.de/archives/1180
float3 perturbNormal(float3 unpacked_normal_ts, float3 normal_ws, float3 view_dir, float2 uv)
{
  float3 p = -view_dir;
  float3 dp1 = ddx(p);
  float3 dp2 = ddy(p);
  float2 duv1 = ddx(uv);
  float2 duv2 = ddy(uv);

  // solve the linear system
  float3 dp2perp = cross(dp2, normal_ws);
  float3 dp1perp = cross(normal_ws, dp1);
  float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  // construct a scale-invariant frame 
  float invmax = rsqrt( max( dot(T,T), dot(B,B) ) );
  float3x3 TNB = float3x3( T * invmax, B * invmax, normal_ws);

  return normalize(mul(unpacked_normal_ts, TNB));
}

#endif