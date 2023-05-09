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

ECS_QUERY()
static
void query_render_settings(eastl::function<void(const int2 render_window_size)>);

namespace Engine::Render
{
  fg::node_id_t mk_backbuffer_acquiring_node()
  {
    return fg::register_node("backbuffer_acquiring", FG_FILE_DECL, [](fg::Registry& reg)
    {
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
      reg.importTextureProducer("backbuffer", []()
      {
        return fg::TextureImport{gapi::get_backbuffer(), gapi::TextureState::Present};
      });

      return [modelSampler, cameraData, wndSize](gapi::CmdEncoder& encoder)
      {
        query_render_settings([wndSize](const int2 render_window_size){
          wndSize.get() = render_window_size;
        });

        const float aspect = (float)wndSize->x / (float)wndSize->y;
        cameraData.get() = get_camera(aspect);

        if (auto* dbgTextQueue = dbg::get_dbg_text_queue())
          dbgTextQueue->tick();

        tfx::set_extern("view_proj", cameraData->viewProj);
        tfx::set_extern("camera_pos", cameraData->pos);
        tfx::set_extern("sec_since_start", Time::get_sec_since_start());
        tfx::set_extern("model_sampler", modelSampler.get());
        tfx::set_extern("viewport_size", float2(wndSize->x, wndSize->y));
        tfx::activate_scope("FrameScope", encoder);

        if (auto* imguiRender = get_imgui_render())
          imguiRender->beforeRender(encoder);
      };
    });
  }

}