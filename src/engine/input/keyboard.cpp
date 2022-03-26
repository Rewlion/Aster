#include "keyboard.h"
#include "events.h"

#include <engine/log.h>
#include <engine/datablock/datablock.h>
#include <engine/ecs/registry.h>

#include <spdlog/fmt/fmt.h>

namespace Engine::Input
{
  void Keyboard::init(const eastl::vector_map<string_hash, ActionSet>& registeredActions, const DataBlock& keyboardMappings)
  {
    log("Init keyboard mappings");

    for(const auto& actionSet: keyboardMappings.getChildBlocks())
    {
      const string& setName = actionSet.getName();
      if (registeredActions.end() == registeredActions.find(str_hash(setName.c_str())))
      {
        logerror("Keyboard mapping error: button action `{}` is not registered by the game", setName);
        continue;
      }

      ButtonMappings buttonMappings;
      const DataBlock* buttons = actionSet.getChildBlock("Button");
      for (const auto& action: buttons->getAttributes())
      {
        if (action.type != DataBlock::ValueType::Int)
          continue;

        const string& name = action.name;
        const string_hash actionHash = str_hash(action.name.c_str());
        const int buttonId = std::get<int>(action.as);

        buttonMappings.emplace_back(actionHash, action.name, buttonId, ButtonStatus::Release);
      }

      m_ActionSets.insert({
        str_hash(setName.c_str()),
        eastl::move(buttonMappings)
      });
    }

    dumpButtonMappings();
  }

  void Keyboard::dumpButtonMappings()
  {
    string dmp = "keyboard handled actions:\n";

    for(auto&[hash, mappings] : m_ActionSets)
    {
      string actionSetInfo = fmt::format("  ActionSet: {}\n", hash);
      for(const auto& mapping: mappings)
        actionSetInfo += fmt::format("    ButtonAction: name:{} devId:{}\n", mapping.actionName, (int)mapping.buttonId);

      dmp += actionSetInfo;
    }

    log(dmp);
  }

  void Keyboard::processInput()
  {
    if (!m_ActiveSet)
      return;

    const auto it = m_ActionSets.find(*m_ActiveSet);
    if (it != m_ActionSets.end())
    {
      ButtonMappings& mappings = it->second;

      const auto addInputEvent = [](string_hash action, ButtonStatus status)
                                 {
                                   auto inputEvent = ButtonActionInputEvent{};
                                   inputEvent.action = action;
                                   inputEvent.status = status;
                                   ECS::manager.broadcastEvent<ButtonActionInputEvent>(eastl::move(inputEvent));
                                 };

      for (auto& mapping: mappings)
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
    }
  }

  void Keyboard::setActionSet(const string_hash actionSet)
  {
    if (m_ActionSets.end() != m_ActionSets.find(actionSet))
    {
      log("Keyboard setting ActionSet:{}", actionSet);
      m_ActiveSet = actionSet;
    }
    else
      logerror("Keyaboard failed to set ActionSet `{}`: unknwon action set");
  }

}