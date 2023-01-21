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
      virtual BhvInputSupport getInputSupport() const override { return BHV_INPUT_MOUSE; }
      virtual BhvResult onMouseStateChange(Element& elem, const BhvStateChange stateChange, const float2 pos, const BhvResult previous_results) override
      {
        const bool cursorInside = elem.isInside(pos);
        if (( stateChange != BhvStateChange::OnMouseMove && !cursorInside)
          || elem.dynamicParams.observeBtnState.isUndefined() 
          || (previous_results & BHV_RES_PROCESSED))
          return BHV_RES_NONE;
        
        qjs::ObjectView btnState = elem.dynamicParams.observeBtnState.as<qjs::ObjectView>();
        qjs::Value v = btnState.getProperty("value");

        int state = 0;
        if (!v.isUndefined() && v.isInt())
          state = v.as<int>();
        
        BhvResult result = BHV_RES_NONE;

        switch (stateChange)
        {
          case BhvStateChange::OnMouseMove:
          {
            if (previous_results & BHV_RES_HOVERED)
            {
              if (state & BTN_ST_HOVERED)
              {
                Utils::clear_bit(state, BTN_ST_HOVERED | BTN_ST_CLICKED);
                result = BHV_RES_HOVERED;
                break;
              }
            }
            else
            {
              if (cursorInside)
              {
                Utils::set_bit(state, BTN_ST_HOVERED);
                result = BHV_RES_HOVERED;
                break;
              }
              else if (state & BTN_ST_HOVERED)
              {
                Utils::clear_bit(state, BTN_ST_HOVERED | BTN_ST_CLICKED);
                result = BHV_RES_HOVERED;
                break;
              }
            }
            break;
          }
          case BhvStateChange::OnMouseClickBegin:
          {
            Utils::set_bit(state, BTN_ST_CLICKED);
            if (elem.dynamicParams.onClick.isValid() && elem.dynamicParams.onClick.isFunction())
              elem.dynamicParams.onClick.as<qjs::FunctionView>()({}, 0, nullptr);
            result = BHV_RES_PROCESSED;
            break;
          }
          case BhvStateChange::OnMouseClickEnd:
          {
            Utils::clear_bit(state, BTN_ST_CLICKED);
            result = BHV_RES_PROCESSED;
            break;
          }
          default: ASSERT(!"unsupported");
        }

        JSContext* ctx = elem.dynamicParams.observeBtnState.getContext();
        JS_SetPropertyStr(ctx, elem.dynamicParams.observeBtnState.getJsValue(), "value",  JS_NewInt32(ctx, state));

        return result;
      }
  };

  REGISTER_BEHAVIOR(ButtonBehavior);
}
