#include "input.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/debug_marks.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/time.h>
#include <engine/window.h>

#include <EASTL/vector_set.h>

#include <spdlog/fmt/fmt.h>

namespace Engine::Input
{
  Manager* Manager::m_This = nullptr;

  Manager::Manager()
  {
    Router::registerListener(this);
  }

  Manager::~Manager()
  {
    Router::unregisterListener(this);
  }

  auto Manager::onMouseButtonStateChanged(const IPointer& device,
                                               const int key, 
                                               const bool pressed) -> InputRouterProcessStatus
  {
    m_InputMsgQueue.addButtonInput(ButtonMsgInput{.key = key, .pressed = pressed, .device = InputDevice::Mouse});
    return InputRouterProcessStatus::ProcessFurther;
  }

  auto Manager::onMouseMove(const IPointer& device,
                                 const int2 new_pos,
                                 const int2 delta) -> InputRouterProcessStatus
  {
    m_InputMsgQueue.addMouseMoveInput(MouseMoveMsgInput{.newPos = new_pos, .posDelta = delta});
    return InputRouterProcessStatus::ProcessFurther;
  }

  auto Manager::onKeyStateChanged(const IKeyboard& device,
                                       const int key,
                                       const bool pressed) -> InputRouterProcessStatus
  {
    m_InputMsgQueue.addButtonInput(ButtonMsgInput{.key = key, .pressed = pressed, .device = InputDevice::Keyboard});
    return InputRouterProcessStatus::ProcessFurther;
  }

  void Manager::init()
  {
    ASSERT(Manager::m_This == nullptr);
    Manager::m_This = new Manager;

    string inputFile = Engine::get_app_settings()->getText("input_settings");
    ASSERT(inputFile != "");

    loginfo("loading input settings from {}", inputFile);

    DataBlock inputSettings;
    const bool inputLoaded = load_blk_from_file(&inputSettings, inputFile.c_str());
    ASSERT(inputLoaded);

    Manager::m_This->loadRegisteredActions(*inputSettings.getChildBlock("ActionSets"));

    string mappingsFile = Engine::get_app_settings()->getText("input_mappings");
    ASSERT(mappingsFile != "");

    DataBlock controllerMappings;
    const bool mappingsLoaded = load_blk_from_file(&controllerMappings, mappingsFile.c_str());
    ASSERT(mappingsLoaded);

    DataBlock* bindingsBlk = controllerMappings.getChildBlock("Bindings");
    Manager::m_This->loadControllerMappings(*bindingsBlk);

    Manager::m_This->removeActionsWithoutBinding();
  }

  void Manager::destroy()
  {
    ASSERT(Manager::m_This != nullptr);
    delete Manager::m_This;
    Manager::m_This = nullptr;
  }

  static
  auto read_action_type(const DataBlock& action) -> ActionType
  {
    const string& type = action.getAnnotation();
    if      (type == "button")    return ActionType::Button;
    else if (type == "joy_move")  return ActionType::JoyMove;
    else if (type == "joy_delta") return ActionType::JoyDelta;
    else {ASSERT(!"unsupported"); return ActionType{};};
  }

  static
  auto read_button_action(const DataBlock& action) -> ButtonAction::Mode
  {
    const string& mode = action.getText("mode");
    if      (mode == "on_press")        return ButtonAction::Mode::OnPress;
    else if (mode == "on_release")      return ButtonAction::Mode::OnRelease;
    else if (mode == "on_double_click") return ButtonAction::Mode::OnDblClick;
    else if (mode == "on_long_press")   return ButtonAction::Mode::OnLongPress;
    else if (mode == "long_pressed")    return ButtonAction::Mode::LongPressed;
    else if (mode == "pressed")         return ButtonAction::Mode::Pressed;
    else if (mode == "")                return ButtonAction::Mode::Pressed;
    else
    {
      logerror("input: action `{}` has unknown button mode `{}`", action.getName(), mode);
      return ButtonAction::Mode::Pressed;
    }
  }

  void Manager::loadRegisteredActions(const DataBlock& actions_blk)
  {
    for (const auto& set: actions_blk.getChildBlocks())
    {
      const auto& actSetName = set.getName();
      bool alreadyRegistered = m_ActionSets.find(actSetName) != m_ActionSets.end();
      if (alreadyRegistered) [[unlikely]]
      {
        logerror("input: action_set `{}` is already registered", actSetName);
        continue;
      }

      ActionSet newActSet;

      for (const auto& action: set.getChildBlocks())
      {
        const auto& actName = action.getName();
        alreadyRegistered = m_NameToActionInfo.find(actName) != m_NameToActionInfo.end();
        if (alreadyRegistered) [[unlikely]]
        {
          logerror("input: action with name `{}` is already declared", actName);
          continue;
        }

        const ActionType type = read_action_type(action);

        unsigned int actId = m_Actions.size();
        if (type == ActionType::Button)
        {
          ButtonAction::Mode mode = read_button_action(action);
          m_Actions.push_back() = ButtonAction{.name = actName, .mode = mode, .bindingId = UNKNOWN_BINDING};
        }
        else
          m_Actions.push_back() = AnalogAction{.name = actName, .type = type, .bindingId = UNKNOWN_BINDING};

        auto [actInfoIt, res] = m_NameToActionInfo.insert({
          actName,
          ActionInfo{
            .type = type,
            .id = actId,
          }
        });

        newActSet.actionIds.push_back(actId);
      }

      m_ActionSets.insert({actSetName, std::move(newActSet)});
    }
  }

  InputDevice read_input_device(const DataBlock& binding)
  {
    int device = binding.getInt("device", -1);
    device = (device >= 0 && device < (int)InputDevice::End) ? device : -1;

    return InputDevice{device};
  }

  void Manager::loadControllerMappings(const DataBlock& bindings_blk)
  {
    for (const auto binding: bindings_blk.getChildBlocks())
    {
      const auto& name = binding.getName();
      const auto actInfoIt =  m_NameToActionInfo.find(name);
      const bool hasCorrespondingAction = actInfoIt != m_NameToActionInfo.end();
      if (!hasCorrespondingAction)
      {
        logerror("input: binding for unknown action `{}`", name);
        continue;
      }

      const InputDevice device = read_input_device(binding);
      if (device == InputDevice::Unknown)
      {
        logerror("input: binding '{}' doesn't have a device", name);
        continue;
      }

      if (actInfoIt->second.type == ActionType::Button)
      {
        const int button = binding.getInt("button", -1);
        if (button == -1)
        {
          logerror("input: binding `{}` is a button binding but doesn't have a button specified", name);
          continue;
        }

        const int bindingId = m_ActionBindings.size();
        m_ActionBindings.push_back() = ButtonActionBinding{.device = device, .button = button};
        m_ActionDatas.push_back() = ButtonActionData{};

        auto& action = std::get<ButtonAction>(m_Actions[actInfoIt->second.id]);
        action.bindingId = bindingId;
      }
      else
      {
        const int xAxis = binding.getInt("x_axis", 0);
        const int yAxis = binding.getInt("y_axis", 0);
        const int bindingId = m_ActionBindings.size();

        m_ActionBindings.push_back() = AnalogActionBinding{.device = device, .axis = int2{xAxis, yAxis}};
        m_ActionDatas.push_back() = AnalogActionData{};

        auto& action = std::get<AnalogAction>(m_Actions[actInfoIt->second.id]);
        action.bindingId = bindingId;
      }
    }
  }

  void Manager::removeActionsWithoutBinding()
  {
    eastl::vector_set<int> emptyActionIds;
    for (const auto& [_, actSet]: m_ActionSets)
    {
      for (const int id : actSet.actionIds)
      {
        if (auto* act = std::get_if<ButtonAction>(&m_Actions[id]))
        {
          if (act->bindingId == UNKNOWN_BINDING)
            emptyActionIds.insert(id);
        }
        else if (auto* act = std::get_if<AnalogAction>(&m_Actions[id]))
        {
          if (act->bindingId == UNKNOWN_BINDING)
            emptyActionIds.insert(id);
        }
      }
    }

    for (auto& [_, actSet]: m_ActionSets)
    {
      eastl::remove_if(
        actSet.actionIds.begin(),
        actSet.actionIds.end(),
        [&emptyActionIds](const int id){
          return emptyActionIds.find(id) != emptyActionIds.end();
        }
      );
    }

    const auto dumpAction = [](const string& name) {
      logwarn("input: action `{}` is removed from processing because it's missing a binding", name);
    };

    for (int id : emptyActionIds)
      if (auto* act = std::get_if<ButtonAction>(&m_Actions[id]))
        dumpAction(act->name);
      else if (auto* act = std::get_if<AnalogAction>(&m_Actions[id]))
        dumpAction(act->name);
  }

  void Manager::processInput()
  {
    PROFILE_CPU();
    Manager::m_This->processInputMessages();
    Manager::m_This->processActions();
  }

  auto Manager::remapButtonId(const InputDevice device, const int id) -> int const
  {
    switch (device)
    {
      case InputDevice::Keyboard:      return id + KB_BUTTONS_START;
      case InputDevice::Mouse:         return id + MOUSE_BUTTONS_START;
      default: ASSERT(!"unsupported"); return -1;
    }
  }

  auto Manager::remapAxisId(const InputDevice device, const int2 id) -> int2 const
  {
    switch (device)
    {
      case InputDevice::Keyboard:      return int2{0,0};
      case InputDevice::Mouse:         return id*2 + int2{MOUSE_AXIS_START, MOUSE_AXIS_START};
      default: ASSERT(!"unsupported"); return int2{-1, -1};
    }
  }

  void Manager::processInputMessages()
  {
    for (int i = 0; i < TOTAL_BUTTONS; ++i)
      m_ButtonStates[i].prevState = m_ButtonStates[i].state;

    for (int i = 1; i < TOTAL_AXIS; i += 2)
      m_AxisStates[i].value = 0;

    m_InputMsgQueue.processMessages([this](const InputMsg& msg)
    {
      if (auto* btnMsg = std::get_if<ButtonMsgInput>(&msg))
      {
        const int btnId = remapButtonId(btnMsg->device, btnMsg->key);
        auto& btn = m_ButtonStates[btnId];
        btn.state = btnMsg->pressed;

        const uint64_t tMs = Time::get_time_ms_since_start();
        if (!btn.prevState && btn.state)
        {
          const uint64_t dtMs = tMs - btn.lastTimeMsChange;

          if (dtMs <= DBL_CLK_MSEC)
            btn.clicksCount++;
          else
            btn.clicksCount = 1;
        }

        btn.lastTimeMsChange = tMs;
      }
      else if (auto* mouseMoveMsg = std::get_if<MouseMoveMsgInput>(&msg))
      {
        m_AxisStates[MOUSE_AXIS_START + 0].value = mouseMoveMsg->newPos.x;
        m_AxisStates[MOUSE_AXIS_START + 1].value = mouseMoveMsg->posDelta.x;
        m_AxisStates[MOUSE_AXIS_START + 2].value = mouseMoveMsg->newPos.y;
        m_AxisStates[MOUSE_AXIS_START + 3].value = mouseMoveMsg->posDelta.y;
      }
      else
        ASSERT(!"unsupported msg");
    });
  }

  void Manager::processActions()
  {
    for (auto& [_, actSet]: m_ActionSets)
    {
      if (!actSet.isEnabled)
        continue;

      for (const auto& actId: actSet.actionIds)
      {
        if (auto* btnAction = std::get_if<ButtonAction>(&m_Actions[actId]))
        {
          const auto& binding = std::get<ButtonActionBinding>(m_ActionBindings[btnAction->bindingId]);
          const int btnId = remapButtonId(binding.device, binding.button);

          auto& btnState = m_ButtonStates[btnId];
          auto& actionData = std::get<ButtonActionData>(m_ActionDatas[btnAction->bindingId]);

          switch (btnAction->mode)
          {
            case ButtonAction::Mode::OnPress:
            {
              actionData.isActive = !btnState.prevState && btnState.state;
              break;
            }
            case ButtonAction::Mode::OnRelease:
            {
              actionData.isActive = btnState.prevState && !btnState.state;
              break;
            }
            case ButtonAction::Mode::OnDblClick:
            {
              actionData.isActive = !btnState.prevState && btnState.state && (btnState.clicksCount >= 2);
              if (actionData.isActive)
                btnState.clicksCount = 0;
              break;
            }
            case ButtonAction::Mode::OnLongPress:
            {
              const auto t = Time::get_time_ms_since_start();
              bool longPressActive = btnState.state && ((t - btnState.lastTimeMsChange) >= LONG_PRESS_MSEC);
              actionData.isActive = longPressActive && !actionData.longPress;
              actionData.longPress = longPressActive;
              break;
            }
            case ButtonAction::Mode::LongPressed:
            {
              const auto t = Time::get_time_ms_since_start();
              bool longPressActive = btnState.state && ((t - btnState.lastTimeMsChange) >= LONG_PRESS_MSEC);
              actionData.isActive = longPressActive;
              break;
            }
            case ButtonAction::Mode::Pressed:
            {
              actionData.isActive = btnState.state;
              break;
            }
          }
        }
        else if (auto* analogAction = std::get_if<AnalogAction>(&m_Actions[actId]))
        {
          const auto& binding = std::get<AnalogActionBinding>(m_ActionBindings[analogAction->bindingId]);
          const int2 axisBind = remapAxisId(binding.device, binding.axis);

          auto& actionData = std::get<AnalogActionData>(m_ActionDatas[analogAction->bindingId]);

          if (analogAction->type == ActionType::JoyMove)
          {
            const int2 currentPos{
              m_AxisStates[axisBind.x].value,
              m_AxisStates[axisBind.y].value
            };

            actionData.values = currentPos;
            actionData.isActive = currentPos != int2{0,0};
          }
          else //JoyDelta
          {
            int& axisX = m_AxisStates[axisBind.x + 1].value;
            int& axisY = m_AxisStates[axisBind.y + 1].value;

            actionData.values = int2{axisX, axisY};
            actionData.isActive = actionData.values != int2{0,0};

            axisX = 0;
            axisY = 0;
          }
        }
      }
    }
  }

  void Manager::enableActionSet(const string& name, const bool enable)
  {
    auto it = Manager::m_This->m_ActionSets.find(name);
    if (it != Manager::m_This->m_ActionSets.end())
    {
      it->second.isEnabled = enable;
      loginfo("input: activating actionSet `{}`", name);
    }
    else
      logerror("input: unknown actionSet `{}` for enabling", name);
  }

  auto Manager::getButtonActionId(const string& name) -> int
  {
    const auto it = Manager::m_This->m_NameToActionInfo.find(name);
    if (it != Manager::m_This->m_NameToActionInfo.end() && it->second.type == ActionType::Button)
      return it->second.id;
    
    return UNKNOWN_ACTION_ID;
  }

  auto Manager::getAnalogActionId(const string& name) -> int
  {
    const auto it = Manager::m_This->m_NameToActionInfo.find(name);
    if (it != Manager::m_This->m_NameToActionInfo.end() && ((int)it->second.type & (int)ActionType::JoyAction))
      return it->second.id;
    
    return UNKNOWN_ACTION_ID;
  }

  auto Manager::getButtonActionState(const int id) -> bool
  {
    return std::get<ButtonActionData>(
      Manager::m_This->m_ActionDatas[id]).isActive;
  }

  auto Manager::getAnalogActionState(const int id) -> bool
  {
    return std::get<AnalogActionData>(
      Manager::m_This->m_ActionDatas[id]).isActive;
  }

  auto Manager::getAnalogActionData(const int id) -> int2
  {
    return std::get<AnalogActionData>(
      Manager::m_This->m_ActionDatas[id]).values;
  }
}
