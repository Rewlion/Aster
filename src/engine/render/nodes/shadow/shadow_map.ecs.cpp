#include <engine/assets/assets_manager.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/model_render.h>
#include <engine/scene/scene.h>
#include <engine/work_cycle/camera.h>
#include <engine/math.h>

#include <glm/gtx/transform.hpp>

NODE_BEGIN(shadow_map)
  EXEC(shadow_map_exec)

  CREATE_TEX_2D(shadow_map, TEX_SIZE(2048,2048, 1), D24_UNORM_S8_UINT,
                TEX_USAGE2(DEPTH_STENCIL, SRV), TEX_STATE(DepthWriteStencilWrite))

  RP_BEGIN()
    DEPTH_RW_CLEARED(shadow_map)
  RP_END()

  READ_BLOB(camera_data, Engine::CameraData)
  READ_BLOB(sun_azimuth_altitude, float2)
  CREATE_BLOB(shadow_map_viewProj, float4x4)

NODE_END()

NODE_EXEC()
static
void shadow_map_exec(gapi::CmdEncoder& encoder,
                    const Engine::CameraData& camera_data,
                    const float2& sun_azimuth_altitude,
                    float4x4& shadow_map_viewProj)
{
  float3 lightDir = math::altitudeAzimuthToDecartian(math::radians(sun_azimuth_altitude.y), math::radians(sun_azimuth_altitude.x));
  float size = 100.0;

  float4x4 proj = glm::orthoLH_ZO(-size,size,-size,size, camera_data.zNear, camera_data.zFar);
  const float4x4 view = math::look_at(camera_data.pos, camera_data.pos + lightDir*100.0f);

  const float4x4 viewProj = proj * view;
  render_models(encoder, viewProj, ModelPass::Depth);

  shadow_map_viewProj = viewProj;
}