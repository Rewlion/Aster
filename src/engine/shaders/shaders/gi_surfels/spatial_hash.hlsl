#include "consts.hlsl"

int4 calcLinearIndex(float3 raw_id)
{
  int3 id = int3(raw_id) + CELLS_HALF_DIM;
  return int4(id, CASCADE_ZERO);
}

//we have a perpsective transformation with 90* fov frustum for each principal axis
//it's basically a cube map sampling like
int4 calcNonLinearIndex(float3 raw_id, float3 camera_to_wpos, float far_plane)
{
  int4 idCascade;

  float3 dir = normalize(raw_id);
  float3 absDir = abs(dir);
  float dist = 0;
  float2 uv = float2(0,0);
  int face = 0;

  //X FACES
  if (absDir.x > absDir.y && absDir.x > absDir.z)
  {
    dist = abs(camera_to_wpos.x);
    uv = float2(absDir.z, absDir.y);
    face = dir.x > 0 ? CASCADE_X : CASCADE_MINUS_X;
  }
  //Y FACES
  else if (absDir.y > absDir.z)
  {
    dist = abs(camera_to_wpos.y);
    uv = float2(absDir.z, absDir.x);
    face = dir.y > 0 ? CASCADE_Y : CASCADE_MINUS_Y;
  }
  //Z FACES
  else
  {
    dist = abs(camera_to_wpos.z);
    uv = float2(absDir.x, absDir.y);
    face = dir.z > 0 ? CASCADE_Z : CASCADE_MINUS_Z;
  }

  float2 xy = uv * float2(CELLS_DIM_X - 1.0, CELLS_DIM_Y - 1.0);
  
  float nearPlane = CELLS_DIM_Z * CELL_SIZE;
  //z = znear * (zfar/znear)^(slice/nslices) from DOOM
  float zSlice = floor(log(dist/ nearPlane) / log(far_plane / nearPlane) * CELLS_DIM_Z);  

  return int4(xy, zSlice, face);
}

int4 calcSpatialIndex(float3 camera_to_wpos, float far_plane)
{
  float3 rawId = camera_to_wpos / CELL_SIZE;
  bool isLinearTransform = all(abs(rawId) < CELLS_HALF_DIM);
  
  [BRANCH]
  if (isLinearTransform)
    return calcLinearIndex(rawId);
  else
    return calcNonLinearIndex(rawId, camera_to_wpos, far_plane);
}