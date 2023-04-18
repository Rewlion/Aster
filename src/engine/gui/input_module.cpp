#include <engine/qjs/inc.h>
#include <engine/qjs/module_registration.h>

namespace Engine::gui
{
  class InputModule: public qjs::ModuleDescription
  {
    public:
      InputModule()
      {
      }

      virtual const char* getName() const override
      {
        return "@input";
      }
  };

  REGISTER_MODULE(InputModule);
}
