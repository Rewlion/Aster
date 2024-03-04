#ifndef INTERSECTIONS_HEADER_GUARD
#define INTERSECTIONS_HEADER_GUARD

#include "intersection_structs.hlsl"
#include "math.hlsl"

#define TRACE_MISS FLOAT_MAX

float3 calc_aabb_sphere_closest_point(float3 aabb_min, float3 aabb_max, float3 sphere_center)
{
  return clamp(sphere_center, aabb_min, aabb_max);
}

bool test_aabb_sphere_intersection(float3 aabb_min, float3 aabb_max, float3 sphere_center, float sphere_radius)
{
  float3 dl = calc_aabb_sphere_closest_point(aabb_min, aabb_max, sphere_center) - sphere_center;
  return dot(dl, dl) <= (sphere_radius * sphere_radius);
}

float calc_aabb_ray_intersection_t(float3 aabb_min, float3 aabb_max, float3 ray_pos, float3 ray_dir)
{
  const float3 mint = (aabb_min - ray_pos) / ray_dir;
  const float3 maxt = (aabb_max - ray_pos) / ray_dir;

  const float tMin = max(max(min(mint.x, maxt.x), min(mint.y, maxt.y)), min(mint.z, maxt.z));
  const float tMax = min(min(max(mint.x, maxt.x), max(mint.y, maxt.y)), max(mint.z, maxt.z));

  if (tMax < 0.0f || tMin > tMax)
    return TRACE_MISS;

  return tMin < 0.0f ? tMax : tMin;
}

bool test_aabb_ray_intersection(float3 aabb_min, float3 aabb_max, float3 ray_pos, float3 ray_dir)
{
  return calc_aabb_ray_intersection_t(aabb_min, aabb_max, ray_pos, ray_dir) != TRACE_MISS;
}

struct TriangleIntersection
{
  float t;
  float3 uvw;
};

TriangleIntersection calc_triangle_ray_intersection(float3 a, float3 b, float3 c, float3 ray_pos, float3 ray_dir)
{
  const float3 ab = b - a;
  const float3 ac = c - a;
  const float3 bc = c - b;
  const float3 N = cross(ab,ac);

  float t = dot(a - ray_pos, N) / dot(ray_dir, N);

  TriangleIntersection res;
  if (t < 0)
  {
    res.t = TRACE_MISS;
    res.uvw = float3(0,0,0);
    return res;
  }

  const float3 p = ray_pos + t * ray_dir;
  const float3 bp = p - b;
  const float3 cp = p - c;

  const float S = length(N);
  const float S1 = length( cross(ab, bp) );
  const float S2 = length( cross(bc, cp) );
  const float S3 = length( cross(ac, cp) );

  const float k = (S1 + S2 + S3) / S;
  const float eps = 0.1e-5;

  res.t = abs(1.0f - k) <= eps ? t : TRACE_MISS;
  res.uvw = float3(S2,S3,S1) / S;
  
  return res;
}

#endif