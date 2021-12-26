#include "input.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/settings.h>
#include <engine/log.h>

#include <spdlog/fmt/fmt.h>

InputManager input_manager;

static void dump_action_set(const ActionSet& set)
{
  String buttons;
  for(const auto& btn: set.buttons)
    buttons += "    " + btn.name + "\n";

  log("added ActionSet `{}`\n  Buttons:\n{}", set.name, buttons);
}

void InputManager::init()
{
  String inputFile = get_app_settings()->get_text("input_settings");
  ASSERT(inputFile != "");

  log("loading input settings from {}", inputFile);

  DataBlock inputSettings;
  const bool inputLoaded = load_blk_from_file(&inputSettings, inputFile.c_str());
  ASSERT(inputLoaded);

  load_registered_actions(inputSettings);

  String mappingsFile = get_app_settings()->get_text("input_mappings");
  ASSERT(mappingsFile != "");

  DataBlock controllerMappings;
  const bool mappingsLoaded = load_blk_from_file(&controllerMappings, mappingsFile.c_str());
  ASSERT(mappingsLoaded);

  DataBlock* mappingsBlk = controllerMappings.get_child_block("ControllerMappings");
  load_controller_mappings(*mappingsBlk);
}

void InputManager::load_registered_actions(const DataBlock& inputSettings)
{
  const DataBlock* actionSetsBlk = inputSettings.get_child_block("Actions");
  for(const auto& actionSetBlk: actionSetsBlk->get_child_blocks())
  {
    ActionSet actionSet;
    actionSet.name = actionSetBlk.get_name();

    const DataBlock* buttons = actionSetBlk.get_child_block("Button");
    for(const auto& button: buttons->get_child_blocks())
    {
      const String actionName = button.get_name();
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

    dump_action_set(actionSet);

    m_ActionSets.insert({
      str_hash(actionSet.name.c_str()),
      actionSet
    });
  }
}

void InputManager::load_controller_mappings(const DataBlock& controllerMappings)
{
  const DataBlock* keyboardMappings = controllerMappings.get_child_block("Keyboard/Actions");
  m_Keyboard.init(m_ActionSets, *keyboardMappings);
}

void InputManager::process_input()
{
  m_Keyboard.process_input();
}

void InputManager::set_actionset(const string_hash actionSet)
{
  m_Keyboard.set_actionset(actionSet);
}