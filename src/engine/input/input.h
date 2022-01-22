#pragma once

#include "actions.h"
#include "keyboard.h"

class DataBlock;

namespace Engine::Input
{
  class InputManager
  {
    public:
      void Init();
      void ProcessInput();
      void SetActionset(const string_hash actionSet);

    private:
      void LoadRegisteredActions(const DataBlock& inputSettings);
      void LoadControllerMappings(const DataBlock& controllerMappings);

    private:
      eastl::vector_map<string_hash, ActionSet> m_ActionSets;
      Keyboard m_Keyboard;
  };

  extern Engine::Input::InputManager manager;
}
