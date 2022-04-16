#include "input.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/settings.h>
#include <engine/log.h>

#include <spdlog/fmt/fmt.h>

namespace Engine::Input
{
  InputManager manager;

  static void DumpActionSet(const ActionSet& set)
  {
    string buttons;
    for(const auto& btn: set.buttons)
      buttons += "    " + btn.name + "\n";

    log("added ActionSet `{}`\n  Buttons:\n{}", set.name, buttons);
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

      DumpActionSet(actionSet);

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
  }

  void InputManager::processInput()
  {
    m_Keyboard.processInput();
  }

  void InputManager::setActionSet(const string_hash actionSet)
  {
    if (actionSet != str_hash(""))
      m_Keyboard.setActionSet(actionSet);
  }

  ButtonStatus InputManager::getKeyboardButtonStatus(const string_hash action) const
  {
    return m_Keyboard.getKeyboardButtonStatus(action);
  }
}
