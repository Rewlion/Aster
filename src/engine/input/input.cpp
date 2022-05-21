#include "input.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/settings.h>
#include <engine/log.h>
#include <engine/window.h>

#include <spdlog/fmt/fmt.h>

namespace Engine::Input
{
  InputManager manager;

  static void dump_action_set(const ActionSet& set)
  {
    string buttons;
    for(const auto& btn: set.buttons)
      buttons += "    " + btn.name + "\n";

    string analogs;
    for(const auto& analog: set.analogs)
      analogs += "    " + analog.name + "\n";

    log("added ActionSet `{}`\n  Buttons:\n{}  Analogs:\n{}", set.name, buttons, analogs);
  }

  void InputManager::init()
  {
    string inputFile = Engine::get_app_settings()->getText("input_settings");
    ASSERT(inputFile != "");

    log("loading input settings from {}", inputFile);

    DataBlock inputSettings;
    const bool inputLoaded = load_blk_from_file(&inputSettings, inputFile.c_str());
    ASSERT(inputLoaded);

    loadRegisteredActions(inputSettings);

    string mappingsFile = Engine::get_app_settings()->getText("input_mappings");
    ASSERT(mappingsFile != "");

    DataBlock controllerMappings;
    const bool mappingsLoaded = load_blk_from_file(&controllerMappings, mappingsFile.c_str());
    ASSERT(mappingsLoaded);

    DataBlock* mappingsBlk = controllerMappings.getChildBlock("ControllerMappings");
    loadControllerMappings(*mappingsBlk);
  }

  void InputManager::loadRegisteredActions(const DataBlock& inputSettings)
  {
    const DataBlock* actionSetsBlk = inputSettings.getChildBlock("Actions");
    for(const auto& actionSetBlk: actionSetsBlk->getChildBlocks())
    {
      ActionSet actionSet;
      actionSet.name = actionSetBlk.getName();

      const DataBlock* buttons = actionSetBlk.getChildBlock("Button");
      for(const auto& button: buttons->getChildBlocks())
      {
        const string actionName = button.getName();
        if (actionName != "")
        {
          actionSet.buttons.emplace_back(actionName);
        }
        else
        {
          logerror("button action without name encountered");
          continue;
        }
      }

      const DataBlock* analogs = actionSetBlk.getChildBlock("Analog");
      for(const auto& analog: analogs->getChildBlocks())
      {
        const string actionName = analog.getName();
        if (actionName != "")
        {
          actionSet.analogs.emplace_back(actionName);
        }
        else
        {
          logerror("analog action without name encountered");
          continue;
        }
      }

      dump_action_set(actionSet);

      m_ActionSets.insert({
        str_hash(actionSet.name.c_str()),
        actionSet
      });
    }
  }

  void InputManager::loadControllerMappings(const DataBlock& controllerMappings)
  {
    const DataBlock* keyboardMappings = controllerMappings.getChildBlock("Keyboard/Actions");
    m_Keyboard.init(m_ActionSets, *keyboardMappings);

    const DataBlock* mouseMappings = controllerMappings.getChildBlock("Mouse/Actions");
    m_Mouse.init(m_ActionSets, *mouseMappings);
  }

  void InputManager::processInput()
  {
    if (Window::is_active_window())
    {
      m_Keyboard.processInput();
      m_Mouse.processInput();
    }
  }

  void InputManager::setActionSet(const string_hash actionSet)
  {
    if (actionSet != str_hash(""))
    {
      m_Keyboard.setActionSet(actionSet);
      m_Mouse.setActionSet(actionSet);
    }
  }

  ButtonStatus InputManager::getButtonStatus(const string_hash action) const
  {
    for (const auto& handler: { (InputHandler&)m_Keyboard, (InputHandler&)m_Mouse })
    {
      ButtonStatus status = handler.getButtonStatus(action);
      if (status != ButtonStatus::Unknown)
        return status;
    }

    return ButtonStatus::Unknown;
  }

  float2 InputManager::getAnalogStatus(const string_hash action) const
  {
    float2 delta = {0.0f, 0.0f};
    if (m_Keyboard.getAnalogStatus(action, delta))
      return delta;

    if (m_Mouse.getAnalogStatus(action, delta))
      return delta;

    return {0.0f, 0.0f};
  }
}
