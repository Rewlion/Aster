#pragma once

#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>

namespace Engine::gui
{
  class ReactStateClass;

  using DirtyStates = eastl::vector_set<const ReactStateClass*>;

  class ReactStatesStorage
  {
    public:
      void add(ReactStateClass*);
      void remove(ReactStateClass*);

      void incUiStateRC(const ReactStateClass*);
      void decUiStateRC(const ReactStateClass*);

      void markDirtyState(const ReactStateClass*);
      const DirtyStates& getDirtyStates() const;
      inline bool hasDirtyStates() const { return !m_DirtyStates.empty(); }
      inline void clearDirtyStates() { m_DirtyStates.clear(); }

    private:
      eastl::vector_map<uint64_t, ReactStateClass*> m_RegisteredStates;
      eastl::vector_map<const ReactStateClass*, size_t> m_UiStatesRC;
      DirtyStates m_DirtyStates;
  };
}
