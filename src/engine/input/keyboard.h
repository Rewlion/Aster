#pragma once

#include "actions.h"

#include <EASTL/fixed_vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/optional.h>

class DataBlock;

class Keyboard
{
  public:
    void init(const eastl::vector_map<string_hash, ActionSet>& registeredActions, const DataBlock& keyboardMappings);
    void process_input();
    void set_actionset(const string_hash actionSet);

  private:
    using ButtonMappings = eastl::fixed_vector<ButtonMapping, 256>;
    using HandledActionSet = eastl::vector_map<string_hash, ButtonMappings>;

  private:
    void dump_button_mappings();

  private:
    HandledActionSet m_ActionSets;
    eastl::optional<string_hash> m_ActiveSet;
};