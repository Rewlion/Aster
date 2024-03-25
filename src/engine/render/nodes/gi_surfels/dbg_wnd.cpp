#include <engine/console/cmd.h>
#include <engine/render/imgui/imgui.h>

bool gibs_enable_surfels_transform = true;
bool gibs_force_reset = false;
bool gibs_freeze_allocation = false;
bool gibs_show_cells = false;
bool gibs_show_cells_coverage = false;
bool gibs_show_surfels = false;
bool gibs_show_surfels_coverage = false;
bool gibs_show_surfels_irradiance = false;
bool gibs_show_surfels_sdf = false;
bool gibs_slow_allocation = false;

static bool show_dbg_wnd = false;

static
void show_dbg_wnd_cb(eastl::span<string_view> args)
{
  show_dbg_wnd = !show_dbg_wnd;
}

static
void imgui_dbg_wnd()
{
  if (!show_dbg_wnd)
    return;

  const auto showtex = [](string_view tex_name, bool& toggle)
  {
    extern void show_tex(eastl::span<string_view> args);
    toggle = !toggle;
    if (toggle)
      show_tex(eastl::span<string_view>(&tex_name, 1));
    else
      show_tex({});
  };

  ImGui::Begin("gibs_dbg", nullptr, 0);
  ImGui::Checkbox("freeze surfels allocation", &gibs_freeze_allocation);
  ImGui::Checkbox("transform surfels", &gibs_enable_surfels_transform);
  ImGui::Checkbox("slow surfels allocation", &gibs_slow_allocation);

  if (ImGui::Button("show surfels", ImVec2(200, 50)))
    showtex("gibs_dbg_surfels", gibs_show_surfels);

  {
    const char* showSurfelsOptions[] = {"base", "sdf", "surfels_coverage", "cells_coverage", "cells", "irradiance"};
    static const char* curOpt = showSurfelsOptions[0];
    bool blob = false;
    bool* values[] = {&blob, &gibs_show_surfels_sdf, &gibs_show_surfels_coverage, &gibs_show_cells_coverage, &gibs_show_cells, &gibs_show_surfels_irradiance};
    if (ImGui::BeginCombo("format", curOpt))
    {
      for (int i = 0; i < IM_ARRAYSIZE(showSurfelsOptions); ++i)
      {
        bool isSelected = curOpt == showSurfelsOptions[i];
        if (ImGui::Selectable(showSurfelsOptions[i], isSelected))
        {
          curOpt = showSurfelsOptions[i];
          isSelected = true;
        }

        *values[i] = isSelected;
      }
      ImGui::EndCombo();
    }
  }

  if (ImGui::Button("reset surfels", ImVec2(100, 25)))
    gibs_force_reset = true;

  ImGui::End();
}

CONSOLE_CMD("render.gibs", 0, 0, show_dbg_wnd_cb);
IMGUI_REG_WND(imgui_dbg_wnd);
