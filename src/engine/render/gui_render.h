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
  class GuiRender
  {
    public:
      void render(gapi::CmdEncoder& encoder);

    private:
      void renderElement(const gui::Element& elem, gapi::CmdEncoder& encoder);
      void renderBox(const gui::Element& elem, gapi::CmdEncoder& encoder);
      void renderText(const gui::Element& elem, gapi::CmdEncoder& encoder);

    private:
      char __pod = 0; //ecs doesn't support 0size component|entity yet
  };
}
