#include "input_handler.h"
#include "events.h"

#include <engine/log.h>
#include <engine/datablock/datablock.h>
#include <engine/ecs/ecs.h>

#include <spdlog/fmt/fmt.h>

namespace Engine::Input
{
  void InputHandler::init(const eastl::vector_map<string_hash, ActionSet>& registeredActions, const DataBlock& keyboardMappings)
  {
    for(const auto& actionSet: keyboardMappings.getChildBlocks())
    {
      const string& setName = actionSet.getName();
      if (registeredActions.end() == registeredActions.find(str_hash(setName.c_str())))
      {
        logerror("mapping error: actionset `{}` is not registered by the game", setName);
        continue;
      }

      ActionSetMappings mappings;
      const DataBlock* buttons = actionSet.getChildBlock("Button");
      for (const auto& action: buttons->getAttributes())
      {
        if (action.type != DataBlock::ValueType::Int)
          continue;

        const string& name = action.name;
        const string_hash actionHash = str_hash(action.name.c_str());
        const int buttonId = std::get<int>(action.as);

        mappings.buttons.push_back(ButtonMapping{
          .action = actionHash,
          .actionName = action.name,
          .buttonId = (char)buttonId,
          .status = ButtonStatus::Release
        });
      }

      const DataBlock* analogs = actionSet.getChildBlock("Analog");
      for (const auto& action: analogs->getChildBlocks())
      {
        const string& name = action.getName();
        const string_hash actionHash = str_hash(name.c_str());
        const int deviceId = action.getInt("device");
        const bool isAbsoluteMouse = action.getBool("absoluteMouse");

        mappings.analogs.push_back(AnalogMapping{
          .action = actionHash,
          .actionName = name,
          .device = (char)deviceId,
          .absoluteMouse = isAbsoluteMouse
        });
      }

      m_Mappings.insert({
        str_hash(setName.c_str()),
        eastl::move(mappings)
      });
    }

    dumpButtonMappings();
  }

  void InputHandler::dumpButtonMappings()
  {
    string dmp = "handled actions:\n";

    for(auto&[hash, mappings] : m_Mappings)
    {
      string actionSetInfo = fmt::format("  ActionSet: {}\n", hash);
      for(const auto& mapping: mappings.buttons)
        actionSetInfo += fmt::format("    ButtonAction: name:{} devId:{}\n", mapping.actionName, (int)mapping.buttonId);

      dmp += actionSetInfo;
    }

    loginfo(dmp);
  }

  void InputHandler::processInput()
  {
    if (!m_ActiveSet)
      return;

    const auto it = m_Mappings.find(*m_ActiveSet);
    if (it == m_Mappings.end())
      return;

    const auto addInputEvent = [](string_hash action, ButtonStatus status)
                               {
                                 auto inputEvent = ButtonActionInputEvent{};
                                 inputEvent.action = action;
                                 inputEvent.status = status;
                                 ECS::get_registry().broadcastEvent<ButtonActionInputEvent>(eastl::move(inputEvent));
                               };

    for (auto& mapping: it->second.buttons)
    {
      const SHORT state = GetAsyncKeyState(mapping.buttonId);
      const auto isDown = state & 0x8000;

      if (isDown && mapping.status == ButtonStatus::Release)
      {
        mapping.status = ButtonStatus::Press;
        addInputEvent(mapping.action, mapping.status);
      }
      else if (!isDown && mapping.status == ButtonStatus::Press)
      {
        mapping.status = ButtonStatus::Release;
        addInputEvent(mapping.action, mapping.status);
      }
    }

    processAnalogInput();
  }

  void InputHandler::setActionSet(const string_hash actionSet)
  {
    if (m_Mappings.end() != m_Mappings.find(actionSet))
    {
      loginfo("setting ActionSet:{}", actionSet);
      m_ActiveSet = actionSet;
    }
    else
      logerror("failed to set ActionSet `{}`: unknown action set", actionSet);
  }

  ButtonStatus InputHandler::getButtonStatus(const string_hash action) const
  {
    if (m_ActiveSet)
    {
      const auto it = m_Mappings.find(*m_ActiveSet);
      if (it != m_Mappings.end())
      {
        for (const auto& buttonMapping: it->second.buttons)
        {
          if (buttonMapping.action == action)
            return buttonMapping.status;
        }
      }
    }

    return ButtonStatus::Unknown;
  }

}