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
      void render(gapi::CmdEncoder& encoder) const;

    private:
      void renderElement(const gui::Element& elem, gapi::CmdEncoder& encoder) const;
  };
}
