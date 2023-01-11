#include "react_states_storage.h"
#include "react_state.h"

#include <engine/assert.h>

namespace Engine::gui
{
  void ReactStatesStorage::add(ReactStateClass* state)
  {
    m_RegisteredStates.emplace(eastl::make_pair(state->getId(), state));
  }

  void ReactStatesStorage::remove(ReactStateClass* state)
  {
    const auto it = m_RegisteredStates.find(state->getId());
    ASSERT(it != m_RegisteredStates.end());
    m_RegisteredStates.erase(it);
  }
}
