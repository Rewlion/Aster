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

    for (const gui::Element::Ptr& child: elem.childs)
      renderElement(*child, encoder);

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

    const float2 textBbox = m_FontRender.getBbox(elem.sceneParams.fontSize, elem.params.text);

    const auto alignX = [](gui::HorAlignType align, float parent_width, float width){
      switch (align)
      {
        case gui::HorAlignType::Left: return 0.0f;
        case gui::HorAlignType::Right: return parent_width - width;
        case gui::HorAlignType::Center: return (parent_width - width) / 2.0f;
        default: return 0.0f;
      }
    };

    const auto alignY = [](gui::VerAlignType align, float parent_height, float height){
      switch (align)
      {
        case gui::VerAlignType::Top: return 0.0f;
        case gui::VerAlignType::Bottom: return parent_height - height;
        case gui::VerAlignType::Center: return (parent_height - height) / 2.0f;
        default: return 0.0f;
      }
    };

    const float2 textOffset = {
      alignX(elem.params.textHalign, elem.sceneParams.size.x, textBbox.x),
      alignY(elem.params.textValign, elem.sceneParams.size.y, textBbox.y)
    };

    const float2 textOrigin = elem.sceneParams.pos + textOffset;

    m_FontRender.render(elem.params.text, textOrigin,
      elem.sceneParams.fontSize, elem.params.color.getRenderColor(), encoder);
  }

  void GuiRender::render(gapi::CmdEncoder& encoder)
  {
    for (auto* elem: gui::Gui::getElemsToRender())
      renderElement(*elem, encoder);
  }
}
