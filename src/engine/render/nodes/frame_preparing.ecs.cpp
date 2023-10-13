#include "nodes.h"

#include <engine/ecs/ecs.h>
#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/gapi/gapi.h>
#include <engine/render/debug/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

#include <engine/shaders/shaders/atmosphere/sph.hlsl>

namespace Engine::Render
{
  fg::node_id_t mk_backbuffer_acquiring_node()
  {
    return fg::register_node("backbuffer_acquiring", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.importTextureProducer("backbuffer", []()
      {
        return fg::TextureImport{gapi::get_backbuffer(), gapi::TextureState::Present};
      });
      
      return [](gapi::CmdEncoder& encoder)
      {
        gapi::acquire_backbuffer();
      };
    });
  }

  fg::node_id_t mk_frame_preparing_node()
  {
    return fg::register_node("frame_preparing", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.orderMeAfter("backbuffer_acquiring");

      auto paramsBuf = reg.createBuffer("sph_buf", gapi::BufferAllocationDescription{
        .size = ATM_PARAMS_COUNT * sizeof(float4),
        .usage = gapi::BufferUsage::BF_GpuVisible | gapi::BufferUsage::BF_BindUAV
      }, gapi::BufferState::BF_STATE_UAV_RW);

      auto cameraData = reg.createBlob<Engine::CameraData>("camera_data");
      auto wndSize = reg.createBlob<int2>("window_size");
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

      return [linearClampSampler, cubicClampSampler, pointClampSampler, modelSampler, cameraData, wndSize](gapi::CmdEncoder& encoder)
      {
        wndSize.get() = get_render_size();

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
        const float2 jitterOffs = { 1.0 / (float)(2 * wndSize->x), 1.0 / (float)(2 * wndSize->y)};
        const float2 cameraJitter = float2(haltonSequence[iFrame++ % haltonCount] * 2.0f - float2(1.0, 1.0)) * jitterOffs;
        const float2 prevCameraJitter = get_prev_jitter();
        set_prev_jitter(cameraJitter);

        const float aspect = (float)wndSize->x / (float)wndSize->y;
        cameraData.get() = get_camera(aspect, float3(cameraJitter, 0.0));

        auto prevViewProjTm = get_prev_view_proj();
        auto viewProjTm = cameraData->proj * cameraData->view;
        set_prev_view_proj(viewProjTm);

        auto viewRot = cameraData->view;
        viewRot[3] = float4(0,0,0,1);

        auto invViewRotProj = glm::inverse(cameraData->proj * viewRot);
        auto leftTopView =  float4(invViewRotProj * glm::vec4(-1,1,0,1));
        auto rightTopView = float4(invViewRotProj * glm::vec4(1,1,0,1));
        auto leftBotView =  float4(invViewRotProj * glm::vec4(-1,-1,0,1));
        auto rightBotView = float4(invViewRotProj * glm::vec4(1,-1,0,1));
        leftTopView  /= leftTopView.w;
        rightTopView /= rightTopView.w;
        leftBotView  /= leftBotView.w;
        rightBotView /= rightBotView.w;

        tfx::set_extern("cameraPrevJitter", prevCameraJitter);
        tfx::set_extern("cameraJitter", cameraJitter);
        tfx::set_extern("prev_view_proj", prevViewProjTm);
        tfx::set_extern("view_proj", viewProjTm);
        tfx::set_extern("camera_pos", cameraData->pos);
        tfx::set_extern("zNear_zFar", float2{cameraData->zNear, cameraData->zFar});
        tfx::set_extern("lt_view", float3(leftTopView));
        tfx::set_extern("rt_view", float3(rightTopView));
        tfx::set_extern("lb_view", float3(leftBotView));
        tfx::set_extern("rb_view", float3(rightBotView));

        tfx::set_extern("sec_since_start", Time::get_sec_since_start());

        tfx::set_extern("model_sampler", modelSampler.get());
        tfx::set_extern("point_clamp_sampler", pointClampSampler.get());
        tfx::set_extern("linear_clamp_sampler", linearClampSampler.get());
        tfx::set_extern("cubic_clamp_sampler", cubicClampSampler.get());

        tfx::set_extern("viewport_size", float2(wndSize->x, wndSize->y));
        tfx::activate_scope("FrameScope", encoder);

        auto evt = Engine::OnBeforeRender{};
        evt.encoder = &encoder;
        ecs::get_registry().broadcastEventSync(std::move(evt));
      };
    });
  }

}