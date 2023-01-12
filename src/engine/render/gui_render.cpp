#include "gui_render.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gui/constants.h>
#include <engine/gui/gui.h>
#include <engine/tfx/tfx.h>

namespace Engine::Render
{
  void GuiRender::renderElement(const gui::Element& elem, gapi::CmdEncoder& encoder) const
  {
    if (elem.params.render != gui::RenderType::None)
    {
      const float4 posSize = float4{elem.sceneParams.pos, elem.sceneParams.size};
      tfx::set_extern("guiPos_guiSize", posSize);
      tfx::set_extern("guiColor", elem.params.color.getRenderColor());

      tfx::activate_scope("GuiScope", encoder);
      tfx::activate_technique("Gui", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    }

    for (const auto& child: elem.childs)
      renderElement(child, encoder);
  }

  void GuiRender::render(gapi::CmdEncoder& encoder) const
  {
    const std::optional<gui::Element>& rootUI = gui::manager.getRootUI();
    if (!rootUI.has_value())
      return;

    renderElement(rootUI.value(), encoder);
  }
}
