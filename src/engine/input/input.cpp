#include "input.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/settings.h>
#include <engine/log.h>

#include <spdlog/fmt/fmt.h>

InputManager input_manager;

static void DumpActionSet(const ActionSet& set)
{
  String buttons;
  for(const auto& btn: set.buttons)
    buttons += "    " + btn.name + "\n";

  log("added ActionSet `{}`\n  Buttons:\n{}", set.name, buttons);
}

void InputManager::Init()
{
  String inputFile = GetAppSettings()->GetText("input_settings");
  ASSERT(inputFile != "");

  log("loading input settings from {}", inputFile);

  DataBlock inputSettings;
  const bool inputLoaded = LoadBlkFromFile(&inputSettings, inputFile.c_str());
  ASSERT(inputLoaded);

  LoadRegisteredActions(inputSettings);

  String mappingsFile = GetAppSettings()->GetText("input_mappings");
  ASSERT(mappingsFile != "");

  DataBlock controllerMappings;
  const bool mappingsLoaded = LoadBlkFromFile(&controllerMappings, mappingsFile.c_str());
  ASSERT(mappingsLoaded);

  DataBlock* mappingsBlk = controllerMappings.GetChildBlock("ControllerMappings");
  LoadControllerMappings(*mappingsBlk);
}

void InputManager::LoadRegisteredActions(const DataBlock& inputSettings)
{
  const DataBlock* actionSetsBlk = inputSettings.GetChildBlock("Actions");
  for(const auto& actionSetBlk: actionSetsBlk->GetChildBlocks())
  {
    ActionSet actionSet;
    actionSet.name = actionSetBlk.GetName();

    const DataBlock* buttons = actionSetBlk.GetChildBlock("Button");
    for(const auto& button: buttons->GetChildBlocks())
    {
      const String actionName = button.GetName();
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

void InputManager::LoadControllerMappings(const DataBlock& controllerMappings)
{
  const DataBlock* keyboardMappings = controllerMappings.GetChildBlock("Keyboard/Actions");
  m_Keyboard.Init(m_ActionSets, *keyboardMappings);
}

void InputManager::ProcessInput()
{
  m_Keyboard.ProcessInput();
}

void InputManager::SetActionset(const string_hash actionSet)
{
  m_Keyboard.SetActionset(actionSet);
}