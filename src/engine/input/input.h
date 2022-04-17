#pragma once

#include "actions.h"
#include "keyboard.h"
#include "mouse.h"

class DataBlock;

namespace Engine::Input
{
  class InputManager
  {
    public:
      void init();
      void processInput();
      void setActionSet(const string_hash actionSet);

      ButtonStatus getButtonStatus(const string_hash action) const;
      float2 getAnalogStatus(const string_hash action) const;

    private:
      void loadRegisteredActions(const DataBlock& inputSettings);
      void loadControllerMappings(const DataBlock& controllerMappings);

    private:
      eastl::vector_map<string_hash, ActionSet> m_ActionSets;
      Keyboard m_Keyboard;
      Mouse m_Mouse;
  };

  extern Engine::Input::InputManager manager;
}
