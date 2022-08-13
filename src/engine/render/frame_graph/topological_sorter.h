#pragma once

#include <EASTL/bitvector.h>
#include <EASTL/vector.h>

namespace fg
{
  class FrameGraph;

  class TopologicalSorter
  {
    public:
      TopologicalSorter(const FrameGraph& fg);

      eastl::vector<size_t> getSortedOrder();

    private:
      void dfs(const size_t i);

    private:
      const FrameGraph& m_Fg;
      const size_t m_NodesCount;
      eastl::bitvector<eastl::allocator> m_Visited;
      eastl::vector<size_t> m_Order;
  };
}
