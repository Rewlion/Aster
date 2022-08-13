#include "topological_sorter.h"

#include "frame_graph.h"
#include "resources.h"

namespace fg
{
  TopologicalSorter::TopologicalSorter(const FrameGraph& fg)
    : m_Fg(fg)
    , m_NodesCount(m_Fg.m_RenderPassPins.size())
    , m_Visited(m_NodesCount, 0)
  {
  }

  eastl::vector<size_t> TopologicalSorter::getSortedOrder()
  {
    for (size_t i = 0; i < m_NodesCount; ++i)
    {
      if (m_Visited[i] == 0)
        dfs(i);
    }

    return std::move(m_Order);
  }

  void TopologicalSorter::dfs(const size_t i)
  {
    m_Visited[i] = 1;
    const auto& nodeParents = m_Fg.m_RenderPassPins[i].reads;
    for (const auto& parentVResId: nodeParents)
    {
      const VirtualResource& vrs = m_Fg.getVirtualResource(parentVResId);
      const size_t parentId = (size_t)vrs.producer->getId();
      if (m_Visited[parentId] == 0)
        dfs(parentId);
    }
    m_Order.emplace_back(i);
  }
}
