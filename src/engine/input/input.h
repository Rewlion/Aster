#pragma once

#include "actions.h"
#include "keyboard.h"

class DataBlock;

namespace Engine::Input
{
  class InputManager
  {
    public:
      void init();
      void processInput();
      void setActionSet(const string_hash actionSet);

    private:
      void loadRegisteredActions(const DataBlock& inputSettings);
      void loadControllerMappings(const DataBlock& controllerMappings);

    private:
      eastl::vector_map<string_hash, ActionSet> m_ActionSets;
      Keyboard m_Keyboard;
  };

  extern Engine::Input::InputManager manager;
}
