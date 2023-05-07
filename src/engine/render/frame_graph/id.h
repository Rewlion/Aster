#pragma once

#include <engine/utils/string_map.h>
#include <engine/utils/strong_typedef.h>

namespace fg
{
  STRONG_SCALAR_TYPEDEF(uint16_t, node_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, virt_res_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, res_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, blob_id_t);

  constexpr node_id_t INVALID_NODE_ID = node_id_t{(uint16_t)~0};
  constexpr res_id_t INVALID_RES_ID = res_id_t{(uint16_t)~0};
  constexpr virt_res_id_t INVALID_VIRT_RES_ID = virt_res_id_t{(uint16_t)~0};

  constexpr node_id_t to_node_id(size_t v) { return node_id_t{(uint16_t)v}; }
  constexpr node_id_t to_node_id(name_id_t name_id) { return node_id_t{(uint16_t)name_id}; }
  constexpr virt_res_id_t to_virt_res_id(name_id_t name_id) { return virt_res_id_t{(uint16_t)name_id}; }
  constexpr res_id_t to_res_id(virt_res_id_t virt_res_id) { return res_id_t{(uint16_t)virt_res_id}; }
  constexpr name_id_t to_name_id(node_id_t node_id) { return name_id_t{(uint16_t)node_id}; }
  constexpr name_id_t to_name_id(virt_res_id_t virt_res_id) { return name_id_t{(uint16_t)virt_res_id}; }

  class NodeHandle
  {
    public:
      explicit NodeHandle(node_id_t id)
        : m_Id(id)
      {
      }

    private:
      node_id_t m_Id;
  };
}
