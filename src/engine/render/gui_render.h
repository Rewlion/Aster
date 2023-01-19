#pragma once

namespace gapi
{
  class CmdEncoder;
}

namespace Engine::gui
{
  class Element;
}

namespace Engine::Render
{
  class FontRender;

  class GuiRender
  {
    public:
      GuiRender(FontRender&);
      void render(gapi::CmdEncoder& encoder);

    private:
      void renderElement(const gui::Element& elem, gapi::CmdEncoder& encoder);
      void renderBox(const gui::Element& elem, gapi::CmdEncoder& encoder);
      void renderText(const gui::Element& elem, gapi::CmdEncoder& encoder);

    private:
      FontRender& m_FontRender;
  };
}
