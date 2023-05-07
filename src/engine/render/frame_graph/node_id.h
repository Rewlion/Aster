#pragma once

#include <engine/types.h>
#include <engine/utils/strong_typedef.h>

namespace fg
{
  STRONG_SCALAR_TYPEDEF(uint16_t, node_id_t);
  constexpr node_id_t INVALID_NODE_ID = node_id_t{(uint16_t)~0};

  class NodeHandle
  {
    public:
      explicit NodeHandle(node_id_t id)
        : m_Id(id)
      {
      }
      NodeHandle(NodeHandle&&) = default;

    private:
      node_id_t m_Id;
  };
}
