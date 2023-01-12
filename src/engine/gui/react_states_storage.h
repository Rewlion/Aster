#pragma once

#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>

namespace Engine::gui
{
  class ReactStateClass;

  class ReactStatesStorage
  {
    public:
      void add(ReactStateClass*);
      void remove(ReactStateClass*);

      void useUiState(const ReactStateClass*);
      void removeUiState(const ReactStateClass*);

      void markDirtyState(const ReactStateClass*);
      const eastl::vector_set<const ReactStateClass*>& getDirtyStates() const;

    private:
      eastl::vector_map<uint64_t, ReactStateClass*> m_RegisteredStates;
      eastl::vector_map<const ReactStateClass*, size_t> m_UiStatesRC;
      eastl::vector_set<const ReactStateClass*> m_DirtyStates;
  };
}
