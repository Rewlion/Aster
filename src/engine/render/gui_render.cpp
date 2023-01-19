#include "gui_render.h"
#include "font/font_render.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resource_wrappers.h>
#include <engine/gui/constants.h>
#include <engine/gui/gui.h>
#include <engine/tfx/tfx.h>

namespace Engine::Render
{
  GuiRender::GuiRender(FontRender& font_render)
    : m_FontRender(font_render)
  {
    
  }

  void GuiRender::renderElement(const gui::Element& elem, gapi::CmdEncoder& encoder)
  {
    switch (elem.params.render)
    {
      case gui::RenderType::Box:
      {
        renderBox(elem, encoder);
        break;
      }
      case gui::RenderType::Text:
      {
        renderText(elem, encoder);
        break;
      }
      default: {}
    }

    gapi::Scissor currentScissor;

    if (elem.params.clipChilds)
    {
      currentScissor = encoder.getScissor();

      gapi::Scissor sc;
      sc.offset = elem.sceneParams.pos;
      sc.size = elem.sceneParams.size;
      encoder.setScissor(sc);
    }

    for (const auto& child: elem.childs)
      renderElement(child, encoder);

    if (elem.params.clipChilds)
      encoder.setScissor(currentScissor);
  }

  void GuiRender::renderBox(const gui::Element& elem, gapi::CmdEncoder& encoder)
  {
    const float4 posSize = float4{elem.sceneParams.pos, elem.sceneParams.size};
    tfx::set_extern("guiPos_guiSize", posSize);
    tfx::set_extern("guiColor", elem.params.color.getRenderColor());

    tfx::activate_scope("GuiScope", encoder);
    tfx::activate_technique("Gui", encoder);

    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);
  }

  void GuiRender::renderText(const gui::Element& elem, gapi::CmdEncoder& encoder)
  {
    gapi::Scissor sc;
    sc.offset = elem.sceneParams.pos;
    sc.size = elem.sceneParams.size;
    gapi::ScopedScissor scopedSc{sc, encoder};

    m_FontRender.render(elem.params.text, elem.sceneParams.pos,
      elem.sceneParams.fontSize, elem.params.color.getRenderColor(), encoder);
  }

  void GuiRender::render(gapi::CmdEncoder& encoder)
  {
    const std::optional<gui::Element>& rootUI = gui::manager.getRootUI();
    if (!rootUI.has_value())
      return;

    renderElement(rootUI.value(), encoder);
  }
}
