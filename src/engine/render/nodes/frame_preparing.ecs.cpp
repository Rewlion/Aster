#include "nodes.h"

#include <engine/ecs/macros.h>
#include <engine/gapi/gapi.h>
#include <engine/render/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

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

      auto cameraData = reg.createBlob<Engine::CameraData>("camera_data");
      auto wndSize = reg.createBlob<int2>("window_size");
      auto modelSampler = reg.createSampler("model_sampler", {});
      auto linearClampSampler = reg.createSampler("linear_clamp_sampler", {
        .addressModeU = gapi::SamplerAddressMode::ClampToEdge,
        .addressModeV = gapi::SamplerAddressMode::ClampToEdge,
        .addressModeW = gapi::SamplerAddressMode::ClampToEdge,
      });

      return [linearClampSampler, modelSampler, cameraData, wndSize](gapi::CmdEncoder& encoder)
      {
        wndSize.get() = get_render_size();

        const float aspect = (float)wndSize->x / (float)wndSize->y;
        cameraData.get() = get_camera(aspect);

        auto viewProjTm = cameraData->proj * cameraData->view;

        auto viewRot = cameraData->view;
        viewRot[3] = float4(0,0,0,1);

        auto invViewRotProj = glm::inverse(cameraData->proj * viewRot);
        auto leftTopView =  float4(invViewRotProj * glm::vec4(-1,-1,0,1));
        auto rightTopView = float4(invViewRotProj * glm::vec4(1,-1,0,1));
        auto leftBotView =  float4(invViewRotProj * glm::vec4(-1,1,0,1));
        auto rightBotView = float4(invViewRotProj * glm::vec4(1,1,0,1));
        leftTopView  /= leftTopView.w;
        rightTopView /= rightTopView.w;
        leftBotView  /= leftBotView.w;
        rightBotView /= rightBotView.w;

        if (auto* dbgTextQueue = dbg::get_dbg_text_queue())
          dbgTextQueue->tick();

        tfx::set_extern("view_proj", viewProjTm);
        tfx::set_extern("camera_pos", cameraData->pos);
        tfx::set_extern("zNear_zFar", float2{cameraData->zNear, cameraData->zFar});
        tfx::set_extern("lt_view", float3(leftTopView));
        tfx::set_extern("rt_view", float3(rightTopView));
        tfx::set_extern("lb_view", float3(leftBotView));
        tfx::set_extern("rb_view", float3(rightBotView));

        tfx::set_extern("sec_since_start", Time::get_sec_since_start());

        tfx::set_extern("model_sampler", modelSampler.get());
        tfx::set_extern("linear_clamp_sampler", linearClampSampler.get());

        tfx::set_extern("viewport_size", float2(wndSize->x, wndSize->y));
        tfx::activate_scope("FrameScope", encoder);

        if (auto* imguiRender = get_imgui_render())
          imguiRender->beforeRender(encoder);
      };
    });
  }

}