#pragma once

#include "actions.h"

#include <EASTL/fixed_vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/optional.h>

class DataBlock;

namespace Engine::Input
{
  class Keyboard
  {
    public:
      void Init(const eastl::vector_map<string_hash, ActionSet>& registeredActions, const DataBlock& keyboardMappings);
      void ProcessInput();
      void SetActionset(const string_hash actionSet);

    private:
      using ButtonMappings = eastl::fixed_vector<ButtonMapping, 256>;
      using HandledActionSet = eastl::vector_map<string_hash, ButtonMappings>;

    private:
      void DumpButtonMappings();

    private:
      HandledActionSet m_ActionSets;
      eastl::optional<string_hash> m_ActiveSet;
  };
}