#include "behavior.h"
#include "element.h"

#include <engine/assert.h>
#include <engine/log.h>
#include <engine/qjs/inc.h>
#include <engine/utils/bits.h>

namespace Engine::gui
{
  class ButtonBehavior: public IBehavior
  {
    public:
      virtual BehaviorType getType() const override { return BehaviorType::Button; }
      virtual BhvResult onStateChange(Element& elem, const BhvStateChange stateChange, const BhvResult previous_results) override
      {
        if (elem.params.observeBtnState.isUndefined() || (previous_results & BHV_RES_PROCESSED))
          return BHV_RES_PROCESSED;
        
        qjs::ObjectView btnState = elem.params.observeBtnState.as<qjs::ObjectView>();
        qjs::Value v = btnState.getProperty("value");

        int state = 0;
        if (!v.isUndefined() && v.isInt())
          state = v.as<int>();
        
        switch (stateChange)
        {
          case BhvStateChange::OnMouseHoverBegin:
          {
            Utils::set_bit(state, BTN_ST_HOVERED);
            break;
          }
          case BhvStateChange::OnMouseHoverEnd:
          {
            Utils::clear_bit(state, BTN_ST_HOVERED | BTN_ST_CLICKED);
            break;
          }
          case BhvStateChange::OnMouseClickBegin:
          {
            Utils::set_bit(state, BTN_ST_CLICKED);
            if (elem.params.onClick.isValid())
              elem.params.onClick.as<qjs::FunctionView>()({}, 0, nullptr);
            break;
          }
          case BhvStateChange::OnMouseClickEnd:
          {
            Utils::clear_bit(state, BTN_ST_CLICKED);
            break;
          }
          default: ASSERT(!"unsupported");
        }

        JSContext* ctx = elem.params.observeBtnState.getContext();
        JS_SetPropertyStr(ctx, elem.params.observeBtnState.getJsValue(), "value",  JS_NewInt32(ctx, state));

        return BHV_RES_PROCESSED;
      }
  };

  REGISTER_BEHAVIOR(ButtonBehavior);
}
