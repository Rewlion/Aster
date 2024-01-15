#pragma once

#include "actions.h"
#include "input_device_type.h"
#include "input_msg_queue.h"
#include "input_router.h"

#include <engine/input/drivers/buttons.h>
#include <engine/utils/string_map.h>

#include <EASTL/array.h>
#include <EASTL/vector_map.h>

#include <limits>

#undef max

namespace ed
{
  class Scope;
}

namespace Engine::Input
{
  class StringHashCompare
  {
    public:
      bool operator()(const string& l, const string& r) const
      {
        return str_hash(l.c_str()) < str_hash(r.c_str());
      }
  };

  class Manager : public IInputRouterListener
  {
    public:
      Manager();
      ~Manager();
      
      auto getInputRouterPriority() -> int override { return PRIORITY_DONT_CARE; };

      auto onMouseButtonStateChanged(const IPointer& device,
                                     const int key, 
                                     const bool pressed) -> InputRouterProcessStatus override;

      auto onMouseMove(const IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> InputRouterProcessStatus override;

      auto onKeyStateChanged(const IKeyboard& device,
                             const int key,
                             const bool pressed) -> InputRouterProcessStatus override;

      static 
      void init();
      static 
      void destroy();

      static
      void processInput();

      static
      void enableActionSet(const string& name, const bool enable);

      static
      auto getButtonActionId(const string& name) -> int;
      static
      auto getAnalogActionId(const string& name) -> int;

      static
      auto getButtonActionState(const int id) -> bool;
      static
      auto getAnalogActionState(const int id) -> bool;
      static
      auto getAnalogActionData(const int id) -> int2;

    private:
      void loadRegisteredActions(const ed::Scope& inputSettings);
      void loadControllerMappings(const ed::Scope& controllerMappings);
      void removeActionsWithoutBinding();

      void processInputMessages();
      void processActions();

    private:
      static Manager* m_This;

      constexpr static uint64_t DBL_CLK_MSEC = 200;
      constexpr static uint64_t LONG_PRESS_MSEC = 250;

      InputMsgQueue m_InputMsgQueue;

      enum
      {
        KB_BUTTONS_START = 0,
        MOUSE_BUTTONS_START = BKEY_KEYBOARD_COUNT,
        TOTAL_BUTTONS = (int)BKEY_KEYBOARD_COUNT + (int)BKEY_MOUSE_COUNT,
      };

      enum
      {
        KB_AXIS_START = 0,
        MOUSE_AXIS_START = 0,
        TOTAL_AXIS = 4
      };

      auto remapButtonId(const InputDevice, const int id) -> int const;
      auto remapAxisId(const InputDevice, const int2 id) -> int2 const;

      struct ButtonState
      {
        bool prevState = false;
        bool state = false;
        uint64_t lastTimeMsChange = 0;
        unsigned clicksCount = 0;
      };
      struct AxisState
      {
        int value = 0;
      };
      eastl::array<ButtonState, TOTAL_BUTTONS> m_ButtonStates;
      eastl::array<AxisState, TOTAL_AXIS> m_AxisStates;

      eastl::vector_map<string, ActionSet, StringHashCompare> m_ActionSets;

      struct ActionInfo
      {
        ActionType type = ActionType::Button;
        unsigned int id = UNKNOWN_ACTION_ID;
      };
      eastl::vector_map<string, ActionInfo, StringHashCompare> m_NameToActionInfo;

      eastl::vector<Action> m_Actions;
      eastl::vector<ActionBinding> m_ActionBindings;
      eastl::vector<ActionData> m_ActionDatas;
  };
}
