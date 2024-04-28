#include <engine/components/sun.h>
#include <engine/console/cmd.h>
#include <engine/ecs/macros.h>
#include <engine/render/imgui/imgui.h>

ECS_COMP_GETTER(SunComponent, sun);

static bool show_wnd = false;
void imgui_draw_sun_params()
{
  if (!show_wnd)
    return;

  float altitude = 0.0;
  float azimuth = 0.0;

  SunComponent* sun = get_sun();
  if (sun)
  {
    azimuth = sun->getAzimuth();
    altitude = sun->getAltitude();
  }

  ImGui::Begin("sun", nullptr, 0);
  ImGui::SliderFloat("altitude", &altitude, -90.0, 90.0);
  ImGui::SliderFloat("azimuth", &azimuth, 0.0, 360.0);
  ImGui::End();

  if (sun)
    sun->updateOrientation(azimuth, altitude);
}

static
void fg_show(eastl::span<string_view>)
{
  show_wnd = !show_wnd;
}

CONSOLE_CMD("sun.edit", 0, 0, fg_show);
IMGUI_REG_WND(imgui_draw_sun_params);
