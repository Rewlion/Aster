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

  void ReactStatesStorage::incUiStateRC(const ReactStateClass* state)
  {
    const auto it = m_UiStatesRC.find(state);
    if (it != m_UiStatesRC.end())
      ++it->second;
    else
      m_UiStatesRC.emplace(eastl::make_pair(state, 1));
  }

  void ReactStatesStorage::decUiStateRC(const ReactStateClass* state)
  {
    const auto it = m_UiStatesRC.find(state);
    ASSERT(it != m_UiStatesRC.end());
    ASSERT(it->second != 0);

    --it->second;
    if (it->second == 0)
      m_UiStatesRC.erase(it);
  }

  void ReactStatesStorage::markDirtyState(const ReactStateClass* state)
  {
    m_DirtyStates.insert(state);
  }

  const eastl::vector_set<const ReactStateClass*>& ReactStatesStorage::getDirtyStates() const
  {
    return m_DirtyStates;
  }
}
