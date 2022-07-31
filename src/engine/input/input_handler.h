#pragma once

#include "actions.h"

#include <EASTL/vector_map.h>
#include <EASTL/optional.h>

class DataBlock;

namespace Engine::Input
{
  class InputHandler
  {
    public:
      void init(const eastl::vector_map<string_hash, ActionSet>& registeredActions, const DataBlock& keyboardMappings);
      void processInput();
      void setActionSet(const string_hash actionSet);

      ButtonStatus getButtonStatus(const string_hash action) const;
      virtual bool getAnalogStatus(const string_hash action, float2& delta) const { delta = {0.0, 0.0}; return false; };

    private:
      using Mappings = eastl::vector_map<string_hash, ActionSetMappings>;

      virtual void processAnalogInput() {}
      void dumpButtonMappings();

    protected:
      Mappings m_Mappings;
      eastl::optional<string_hash> m_ActiveSet;
  };
}