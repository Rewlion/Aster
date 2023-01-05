#include "constants.h"

#include <engine/qjs/module_registration.h>
#include <engine/qjs/inc.h>
#include <engine/log.h>

namespace Engine::gui
{
  class UiModule: public qjs::ModuleDescription
  {
    public:
      UiModule()
      {
        addParam("RENDER_BOX", (int)RenderType::Box);

        addParam("HALIGN_CENTER", (int)HorAlignType::Center);
        addParam("HALIGN_LEFT", (int)HorAlignType::Left);
        addParam("HALIGN_RIGHT", (int)HorAlignType::Right);

        addParam("VALIGN_CENTER", (int)VerAlignType::Center);
        addParam("VALIGN_TOP", (int)VerAlignType::Top);
        addParam("VALIGN_BOTTOM", (int)VerAlignType::Bottom);

        addParam("FLOW_ROW", (int)Flow::Row);
        addParam("FLOW_COLUMN", (int)Flow::Column);
        addParam("FLOW_FLEX_ROW", (int)Flow::FlexRow);
        addParam("FLOW_FLEX_COLUMN", (int)Flow::FlexColumn);
      }

      virtual const char* getName() const override
      {
        return "@ui";
      }
  };

  REGISTER_MODULE(UiModule);
}
