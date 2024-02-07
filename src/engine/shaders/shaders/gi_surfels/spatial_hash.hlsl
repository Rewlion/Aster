#include "consts.hlsl"

int4 calcLinearIndex(float3 camera_to_wpos)
{
  float3 cameraToBegin = -float3(CELLS_DIM.xxx)*0.5;
  int3 id = int3((camera_to_wpos - cameraToBegin) / CELL_SIZE);
  return int4(id, CASCADE_ZERO);
}

//we have a perpsective transformation with 90* fov frustum for each principal axis
//it's basically a cube map sampling like
int4 calcNonLinearIndex(float3 camera_to_wpos, float far_plane)
{
  float3 absDir = abs(camera_to_wpos);
  float dist = 0;
  float2 uv = float2(0,0);
  int face = 0;

  //X FACES
  if (absDir.x > absDir.y && absDir.x > absDir.z)
  {
    dist = abs(camera_to_wpos.x);
    uv = camera_to_wpos.zy;
    face = camera_to_wpos.x > 0 ? CASCADE_X : CASCADE_MINUS_X;
  }
  //Y FACES
  else if (absDir.y > absDir.z)
  {
    dist = abs(camera_to_wpos.y);
    uv = camera_to_wpos.zx;
    face = camera_to_wpos.y > 0 ? CASCADE_Y : CASCADE_MINUS_Y;
  }
  //Z FACES
  else
  {
    dist = abs(camera_to_wpos.z);
    uv = camera_to_wpos.xy;
    face = camera_to_wpos.z > 0 ? CASCADE_Z : CASCADE_MINUS_Z;
  }
  uv = uv / dist; // project on the cube face
  uv = uv * 0.5 + 0.5;
  float2 xy = uv * float2(CELLS_DIM, CELLS_DIM);
  
  float nearPlane = CELLS_DIM * CELL_SIZE; //+0.5 cell size?
  //z = znear * (zfar/znear)^(slice/nslices) from DOOM
  float zSlice = floor(log(dist/ nearPlane) / log(far_plane / nearPlane) * CELLS_DIM);  

  return int4(xy, zSlice, face);
}

int4 calcSpatialIndex(float3 camera_to_wpos, float far_plane)
{
  bool isLinearTransform = all(abs(camera_to_wpos) < (CELLS_HALF_DIM + 0.5*CELL_SIZE));
  [BRANCH]
  if (isLinearTransform)
    return calcLinearIndex(camera_to_wpos);
  else
    return calcNonLinearIndex(camera_to_wpos, far_plane);
}