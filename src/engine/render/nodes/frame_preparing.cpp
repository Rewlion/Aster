#include "nodes.h"

#include <engine/gapi/gapi.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/render/world_render.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

namespace Engine::Render
{
  fg::NodeHandle mk_backbuffer_acquiring_node()
  {
    return fg::register_node("backbuffer_acquiring", FG_FILE_DECL, [](fg::Registry& reg)
    {
      return [](gapi::CmdEncoder& encoder)
      {
        gapi::acquire_backbuffer();
      };
    });
  }

  fg::NodeHandle mk_frame_preparing_node()
  {
    return fg::register_node("frame_preparing", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.orderMeAfter("backbuffer_acquiring");
      auto cameraData = reg.createBlob<Engine::CameraData>("camera_data");
      auto wndSize = reg.createBlob<int2>("window_size");

      reg.importTextureProducer("backbuffer", []()
      {
        return fg::TextureImport{gapi::get_backbuffer(), gapi::TextureState::Present};
      });

      return [cameraData, wndSize](gapi::CmdEncoder& encoder)
      {
        cameraData.get() = get_camera();
        wndSize.get() = Window::get_window_size();

        world_render.m_DbgTextQueue.tick();

        tfx::set_extern("view_proj", cameraData->viewProj);
        tfx::set_extern("camera_pos", cameraData->pos);
        tfx::set_extern("sec_since_start", Time::get_sec_since_start());
        tfx::set_extern("model_sampler", world_render.m_ModelSampler);
        tfx::set_extern("viewport_size", float2(wndSize->x, wndSize->y));
        tfx::activate_scope("FrameScope", encoder);

        world_render.m_ImGuiRender->beforeRender(encoder);
      };
    });
  }

}