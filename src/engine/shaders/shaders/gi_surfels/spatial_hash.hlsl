#include "consts.hlsl"

#define NONLINEAR_NEAR_PLANE (float(CELLS_DIM * CELL_SIZE) * 0.5)

struct SpatialInfo
{
  int3 id;
  int cascade;
  float cellSize;
};

struct GridAABB
{
  float3 minWS;
  float3 maxWS;
};

float3 calcOffsetToMinLinearCorner()
{
  return -float3(CELLS_DIM.xxx)* 0.5 * CELL_SIZE;
}

SpatialInfo calcLinearInfo(float3 camera_to_wpos)
{
  float3 cameraToBegin = calcOffsetToMinLinearCorner();
  int3 id = int3(floor((camera_to_wpos - cameraToBegin) / CELL_SIZE));
  SpatialInfo si = {id, CASCADE_ZERO, CELL_SIZE};
  return si;
}

GridAABB calcLinearAABB(int3 cell_id, float3 camera_wpos)
{
  float3 cameraToBegin = calcOffsetToMinLinearCorner();
  float3 linearGridMinCorner = camera_wpos + cameraToBegin;
  float3 minAABB = linearGridMinCorner + cell_id * CELL_SIZE;
  float3 maxAABB = minAABB + CELL_SIZE.xxx;

  GridAABB aabb = {minAABB, maxAABB};
  return aabb;
}

int3 clampSpatialId(int3 id)
{
  return clamp(id, 0.xxx, (CELLS_DIM - 1).xxx);
}

float calcZSlice(float dist, float far_plane)
{
  //z = znear * (zfar/znear)^(slice/nslices) from DOOM
  return floor(log(dist/ NONLINEAR_NEAR_PLANE) / log(far_plane / NONLINEAR_NEAR_PLANE) * CELLS_DIM);  
}

float calcDepthFromSlice(int slice, float far_plane)
{
  return NONLINEAR_NEAR_PLANE * pow(far_plane / NONLINEAR_NEAR_PLANE, (float)slice/CELLS_DIM);
}

//we have a perpsective transformation with 90* fov frustum for each principal axis
//it's basically a cube map sampling like
//
//transformation will rotate the basis, thus Z will be aligned with vector from camera to far cascade plane
//and XY is a viewport of the cascade
//
//X,Y is expected to be inverted for some cases because we only care about hashing.

struct PointInCascade
{
  float3 pos;
  int cascade;
};

PointInCascade transformFromCameraWorldSpaceToXCascadeSpace(float3 camera_to_wpos)
{
  PointInCascade p = {
    float3(camera_to_wpos.z, camera_to_wpos.y, abs(camera_to_wpos.x)),
    camera_to_wpos.x > 0 ? CASCADE_X : CASCADE_MINUS_X
  };

  return p;
}

PointInCascade transformFromCameraWorldSpaceToYCascadeSpace(float3 camera_to_wpos)
{
  PointInCascade p = {
    float3(-camera_to_wpos.x, camera_to_wpos.z, abs(camera_to_wpos.y)),
    camera_to_wpos.y > 0 ? CASCADE_Y : CASCADE_MINUS_Y
  };

  return p;
}

PointInCascade transformFromCameraWorldSpaceToZCascadeSpace(float3 camera_to_wpos)
{
  PointInCascade p = {
    float3(camera_to_wpos.x, camera_to_wpos.y, abs(camera_to_wpos.z)),
    camera_to_wpos.z > 0 ? CASCADE_Z : CASCADE_MINUS_Z
  };

  return p;
}

PointInCascade transformFromCameraWorldSpaceToCascadeSpace(float3 camera_to_wpos)
{
  float3 absDir = abs(camera_to_wpos);
  float dist = 0;
  float2 uv = float2(0,0);
  int face = 0;

  //X FACES
  if (absDir.x > absDir.y && absDir.x > absDir.z)
    return transformFromCameraWorldSpaceToXCascadeSpace(camera_to_wpos);
  //Y FACES
  else if (absDir.y > absDir.z)
    return transformFromCameraWorldSpaceToYCascadeSpace(camera_to_wpos);
  //Z FACES
  else
    return transformFromCameraWorldSpaceToZCascadeSpace(camera_to_wpos);
}

int3 posInNonLinearCascadeToCellID(float3 pos_in_cascade_space, float far_plane)
{
  float2 uv = pos_in_cascade_space.xy;
  float dist = pos_in_cascade_space.z;

  uv = uv / dist; // project on the cube face
  uv = uv * 0.5 + 0.5;

  dist = clamp(dist, NONLINEAR_NEAR_PLANE, far_plane);
  uv = saturate(uv);

  float2 xy = uv * float2(CELLS_DIM, CELLS_DIM);
  float slice = calcZSlice(dist, far_plane);

  return int3(xy, slice);
}

SpatialInfo calcNonLinearInfo(float3 camera_to_wpos, float far_plane)
{
  PointInCascade pCascSpace = transformFromCameraWorldSpaceToCascadeSpace(camera_to_wpos);
  int3 cellId = posInNonLinearCascadeToCellID(pCascSpace.pos, far_plane);

  float halfPlaneSize = calcDepthFromSlice(cellId.z, far_plane);
  float cellSize = halfPlaneSize * 2.0 / CELLS_DIM;

  SpatialInfo si = {cellId, pCascSpace.cascade, cellSize};
  return si;
}

bool isLinearTransform(float3 camera_to_wpos)
{
  return all(abs(camera_to_wpos) < (CELLS_HALF_DIM + 0.5*CELL_SIZE));
}

SpatialInfo calcSpatialInfo(float3 camera_to_wpos, float far_plane)
{
  [BRANCH]
  if (isLinearTransform(camera_to_wpos))
    return calcLinearInfo(camera_to_wpos);
  else
    return calcNonLinearInfo(camera_to_wpos, far_plane);
}

uint calcSpatial1DIndex(int3 spatial_id)
{
  return spatial_id.x + spatial_id.y * CELLS_DIM + spatial_id.z * CELLS_DIM * CELLS_DIM;
}

uint calcSpatialHash(float3 pos, float cell_size)
{
  return uint(floor(pos.x / cell_size) * 73856093)
       ^ uint(floor(pos.y / cell_size) * 19349663)
       ^ uint(floor(pos.z / cell_size) * 83492791);
}

int2 calcNonlinearXYBounding(float surfel_radius, float2 surfel_center_in_cascade)
{
  float2 dirToCenter = normalize(surfel_center_in_cascade);
  float2 rDir = float2(-dirToCenter.y, dirToCenter.x);

  //overscales projection a bit since it's not a tangent but it's ok
  //we are cheating with nonlinear cell intersection already via aabb
  float2 a = surfel_center_in_cascade + rDir * surfel_radius;
  float2 b = surfel_center_in_cascade - rDir * surfel_radius;

  float2 projection = float2(a.x, b.x) / float2(a.y, b.y); // project on unit cube face
  int2 ids = floor((projection*0.5 + 0.5) * CELLS_DIM);

  int minId = min(ids.x, ids.y);
  int maxId = max(ids.x, ids.y);

  return int2(minId, maxId);
}

int2 calcNonlinearZBounding(float surfel_radius, float surfel_center_in_cascade, float far_plane)
{
  return int2( calcZSlice(surfel_center_in_cascade - surfel_radius, far_plane),
               calcZSlice(surfel_center_in_cascade + surfel_radius, far_plane));
}

PointInCascade getLeftRightCascadePoint(PointInCascade surfelCenterInCascade, float3 camera_to_wpos)
{
  if (surfelCenterInCascade.cascade == CASCADE_X || surfelCenterInCascade.cascade == CASCADE_MINUS_X)
    return transformFromCameraWorldSpaceToZCascadeSpace(camera_to_wpos);
  else
    return transformFromCameraWorldSpaceToXCascadeSpace(camera_to_wpos);

  // int leftCascadeFor[CASCADES_COUNT] = {
  //   CASCADE_ZERO,
  //   CASCADE_MINUS_Z, CASCADE_MINUS_Z,//X, -X
  //   CASCADE_MINUS_X, CASCADE_MINUS_X,//Y, -Y
  //   CASCADE_MINUS_X, CASCADE_MINUS_X // Z, -Z
  // };

  // int rightCascadeFor[CASCADES_COUNT] = {
  //   CASCADE_ZERO,
  //   CASCADE_Z, CASCADE_Z,//X, -X
  //   CASCADE_X, CASCADE_X,//Y, -Y
  //   CASCADE_X, CASCADE_X // Z, -Z
  // };
}

PointInCascade getTopBotCascadePoint(PointInCascade surfelCenterInCascade, float3 camera_to_wpos)
{
  if (surfelCenterInCascade.cascade == CASCADE_Y || surfelCenterInCascade.cascade == CASCADE_MINUS_Y)
    return transformFromCameraWorldSpaceToZCascadeSpace(camera_to_wpos);
  else
    return transformFromCameraWorldSpaceToYCascadeSpace(camera_to_wpos);

  // int topCascadeFor[CASCADES_COUNT] = {
  //   CASCADE_ZERO,
  //   CASCADE_Y, CASCADE_Y,//X, -X
  //   CASCADE_Z, CASCADE_Z,//Y, -Y
  //   CASCADE_Y, CASCADE_Y // Z, -Z
  // };

  // int botCascadeFor[CASCADES_COUNT] = {
  //   CASCADE_ZERO,
  //   CASCADE_MINUS_Y, CASCADE_MINUS_Y,//X 
  //   CASCADE_MINUS_Z, CASCADE_MINUS_Z,//Y
  //   CASCADE_MINUS_Y, CASCADE_MINUS_Y// Z 
  // };
}