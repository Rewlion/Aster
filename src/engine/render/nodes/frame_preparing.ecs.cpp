#include <engine/ecs/ecs.h>
#include <engine/gapi/gapi.h>
#include <engine/math.h>
#include <engine/render/debug/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

#include <engine/shaders/shaders/atmosphere/sph.hlsl>


static
auto import_backbuffer() -> fg::TextureImport
{
  return fg::TextureImport{gapi::get_backbuffer(), gapi::TextureState::Present};
}

NODE_BEGIN(backbuffer_acquiring)
  IMPORT_TEX(backbuffer, import_backbuffer)
  EXEC(backbuffer_acquiring_exec)
NODE_END()

NODE_EXEC()
static
void backbuffer_acquiring_exec(gapi::CmdEncoder& encoder)
{
  gapi::acquire_backbuffer();
}

//TODO: TEACH TFX TO CREATE SAMPLERS LIKE SCOPES
ECS_EVENT_SYSTEM()
static
void mk_frame_samplers_preparing_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("frame_samplers_preparing", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeBefore("frame_preparing");

    auto modelSampler = reg.createSampler("model_sampler", {});
    auto linearClampSampler = reg.createSampler("linear_clamp_sampler", {
      .addressModeU = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeV = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeW = gapi::SamplerAddressMode::ClampToEdge,
    });
    auto cubicClampSampler = reg.createSampler("cubic_clamp_sampler", {
      .magFilter = gapi::ImageFilter::Cubic,
      .minFilter = gapi::ImageFilter::Cubic,
      .addressModeU = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeV = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeW = gapi::SamplerAddressMode::ClampToEdge,
    });
    auto pointClampSampler = reg.createSampler("point_clamp_sampler", {
      .magFilter = gapi::ImageFilter::Nearest,
      .minFilter = gapi::ImageFilter::Nearest,
      .addressModeU = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeV = gapi::SamplerAddressMode::ClampToEdge,
      .addressModeW = gapi::SamplerAddressMode::ClampToEdge,
    });
    return [linearClampSampler, cubicClampSampler, pointClampSampler, modelSampler](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("model_sampler", modelSampler.get());
      tfx::set_extern("point_clamp_sampler", pointClampSampler.get());
      tfx::set_extern("linear_clamp_sampler", linearClampSampler.get());
      tfx::set_extern("cubic_clamp_sampler", cubicClampSampler.get());
    };
  });
}

NODE_BEGIN(frame_preparing)
  ORDER_ME_AFTER(backbuffer_acquiring)

  CREATE_GPU_BUF(sph_buf, BUF_USAGE(UAV), BUF_STATE(UAV_RW), BUF_SIZE(ATM_PARAMS_COUNT * sizeof(float4)))
  CREATE_BLOB(camera_data, Engine::CameraData)
  CREATE_BLOB(window_size, int2)

  EXEC(frame_preparing_exec)
NODE_END()

NODE_EXEC()
static
void frame_preparing_exec(gapi::CmdEncoder& encoder,
                          const gapi::BufferHandler sph_buf,
                          Engine::CameraData& camera_data,
                          int2& window_size)
{
  window_size = Engine::Render::get_render_size();

  static size_t iFrame = 0;
  const int haltonCount = 8;
  float2 haltonSequence[haltonCount] = {
    float2(1.0f / 2.0f,  1.0f / 3.0f),
    float2(1.0f / 4.0f,  2.0f / 3.0f),
    float2(3.0f / 4.0f,  1.0f / 9.0f),
    float2(1.0f / 8.0f,  4.0f / 9.0f),
    float2(5.0f / 8.0f,  7.0f / 9.0f),
    float2(3.0f / 8.0f,  2.0f / 9.0f),
    float2(7.0f / 8.0f,  5.0f / 9.0f),
    float2(1.0f / 16.0f, 8.0f / 9.0f)
  };
  const float2 jitterOffs = { 1.0 / (float)(2 * window_size.x), 1.0 / (float)(2 * window_size.y)};
  const float2 cameraJitter = float2(haltonSequence[iFrame++ % haltonCount] * 2.0f - float2(1.0, 1.0)) * jitterOffs;
  const float2 prevCameraJitter = Engine::Render::get_prev_jitter();
  Engine::Render::set_prev_jitter(cameraJitter);

  const float aspect = (float)window_size.x / (float)window_size.y;
  camera_data = Engine::get_camera(aspect, float3(cameraJitter, 0.0));

  auto prevViewProjTm = Engine::Render::get_prev_view_proj();
  auto viewProjTm = camera_data.proj * camera_data.view;
  Engine::Render::set_prev_view_proj(viewProjTm);
  
  float4x4 viewRot = camera_data.view;
  viewRot[3] = float4(0,0,0,1);

  const math::FarPlanePoints fpViewWS =
    math::get_far_plane_points_world_space(camera_data.proj, viewRot);
  const math::FarPlanePoints fpViewVS =
    math::get_far_plane_points_view_space(camera_data.proj);

  tfx::set_extern("cameraPrevJitter", prevCameraJitter);
  tfx::set_extern("cameraJitter", cameraJitter);
  tfx::set_extern("prev_view_proj", prevViewProjTm);
  tfx::set_extern("view_proj", viewProjTm);
  tfx::set_extern("view", camera_data.view);
  tfx::set_extern("proj", camera_data.proj);
  tfx::set_extern("camera_pos", camera_data.pos);
  tfx::set_extern("zNear_zFar", float2{camera_data.zNear, camera_data.zFar});
  tfx::set_extern("ltView_WS", float3(fpViewWS.leftTop));
  tfx::set_extern("rtView_WS", float3(fpViewWS.rightTop));
  tfx::set_extern("lbView_WS", float3(fpViewWS.leftBot));
  tfx::set_extern("rbView_WS", float3(fpViewWS.rightBot));
  tfx::set_extern("ltView_VS", float3(fpViewVS.leftTop));
  tfx::set_extern("rtView_VS", float3(fpViewVS.rightTop));
  tfx::set_extern("lbView_VS", float3(fpViewVS.leftBot));
  tfx::set_extern("rbView_VS", float3(fpViewVS.rightBot));

  tfx::set_extern("sec_since_start", Engine::Time::get_sec_since_start());

  tfx::set_extern("viewport_size", float2(window_size.x, window_size.y));
  tfx::activate_scope("FrameScope", encoder);

  auto evt = Engine::OnBeforeRender{};
  evt.encoder = &encoder;
  ecs::get_registry().broadcastEventSync(std::move(evt));
}
