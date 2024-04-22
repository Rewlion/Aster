#ifndef MATH_HLSL
#define MATH_HLSL

#define HAS_MATH

#define FLOAT_MAX 3.402823466e+38
#define MM2KM 1e3

#define PI (float)3.14159265359

float3x3 tsToWs(float3 N)
{
  float3 up = N.y < 0.99 ? float3(0.0, 1.0, 0.0) : float3(0.0, 0.0, 1.0);
  float3 tangent = normalize(cross(up, N));
  float3 bitangent = normalize(cross(N, tangent));
  
  return float3x3(tangent, N, bitangent);
}

float3 sphericalToDecartian(float theta, float phi)
{
  return float3(
    sin(theta)*sin(phi),
    cos(theta),
    sin(theta)*cos(phi)
  );
}

float3 altitudeAzimuthToDecartian(float altitude, float azimuth)
{
  return normalize(float3(cos(altitude)*sin(azimuth),
                   sin(altitude),
                   cos(altitude)*cos(azimuth)));
}

float2 decartianToAltitudeAzimuth(float3 dir)
{
  float altitude = asin(dir.y);
  float azimuth = fmod(atan2(dir.x, dir.z) + 2*PI, 2*PI);

  return float2(altitude, azimuth);
}

//theta: [0, PI], phi: [0, 2PI]
float2 decartianToSpherical(float3 dir)
{
  float phi = fmod(atan2(dir.x, dir.z)+2.0*PI, 2.0*PI);
  float theta = acos(dir.y);
  return float2(theta, phi);
}

float raySphereIntersection(float3 r0, float3 dir, float3 s, float radius, out float3 intersection)
{
  float3 dr = r0 - s;
  float k = dot(dr, dir);
  float c = pow(length(dr), 2.0) - pow(radius, 2.0);
  float D = k*k - c;
  if (D<0.0)
      return -1e6;

  bool isInsideSphere = k*k < D;
  float sqrtD = sqrt(D);
  float t = isInsideSphere ?
              -k + sqrtD :
              -k - sqrtD;

  intersection = r0 + dir * t;
  return t;
}

template<typename T>
void swap(inout T l, inout T r)
{
  T m = l;
  l = r;
  r = m;
}

#endif
