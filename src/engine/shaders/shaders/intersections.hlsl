#ifndef INTERSECTIONS_HEADER_GUARD
#define INTERSECTIONS_HEADER_GUARD

float3 calc_aabb_sphere_closest_point(float3 aabb_min, float3 aabb_max, float3 sphere_center)
{
  return clamp(sphere_center, aabb_min, aabb_max);
}

bool test_aabb_sphere_intersection(float3 aabb_min, float3 aabb_max, float3 sphere_center, float sphere_radius)
{
  float3 dl = calc_aabb_sphere_closest_point(aabb_min, aabb_max, sphere_center) - sphere_center;
  return dot(dl, dl) <= (sphere_radius * sphere_radius);
}

#endif