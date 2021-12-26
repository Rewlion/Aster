#pragma once

#include "actions.h"
#include "keyboard.h"

class DataBlock;

class InputManager
{
  public:
    void init();
    void process_input();
    void set_actionset(const string_hash actionSet);

  private:
    void load_registered_actions(const DataBlock& inputSettings);
    void load_controller_mappings(const DataBlock& controllerMappings);

  private:
    eastl::vector_map<string_hash, ActionSet> m_ActionSets;
    Keyboard m_Keyboard;
};

extern InputManager input_manager;
