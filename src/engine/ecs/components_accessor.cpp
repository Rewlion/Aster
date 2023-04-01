#include "components_accessor.h"

namespace ecs
{
  ComponentsAccessor::ComponentsAccessor(std::byte* entity_data,
                                         const component_offset_t* offsets,
                                         const eastl::vector_map<name_hash_t, component_id_t>& name_to_comp_map)
    : m_EntityData(entity_data)
    , m_Offsets(offsets)
    , m_NameToCompMap(name_to_comp_map)
  {
  }

  ComponentsAccessorById::ComponentsAccessorById(std::byte* entity_data,
                                                 const component_offset_t* offsets,
                                                 const component_type_id_t* types,
                                                 const components_count_t comps_count)
    : m_EntityData(entity_data)
    , m_Offsets(offsets)
    , m_Types(types)
    , m_CompsCount(comps_count)
  {
  }

  auto ComponentsAccessorById::operator[](const int i) -> std::byte*
  {
    return m_EntityData + m_Offsets[i];
  }

  auto ComponentsAccessorById::operator=(const ComponentsAccessorById& r) -> const ComponentsAccessorById&
  {
    std::memcpy(this, &r, sizeof(*this));
    return *this;
  }

  auto ComponentsAccessorById::begin() -> ComponentsAccessorById::Iterator
  {
    return {*this, 0};
  }

  auto ComponentsAccessorById::end() -> ComponentsAccessorById::Iterator
  {
    return {*this, m_CompsCount};
  }

  ComponentsAccessorById::Iterator::Iterator(ComponentsAccessorById& accessor, size_t pos)
            : m_Accessor(accessor)
            , m_Pos(pos)
  {
  }

  ComponentsAccessorById::Iterator::Access ComponentsAccessorById::Iterator::operator*() const
  {
    return {
      m_Accessor.m_EntityData + m_Accessor.m_Offsets[m_Pos],
      m_Accessor.m_Types[m_Pos]
    };
  }

  auto ComponentsAccessorById::Iterator::operator++() -> ComponentsAccessorById::Iterator
  {
    return Iterator{m_Accessor, ++m_Pos};
  }

  auto ComponentsAccessorById::Iterator::operator==(const ComponentsAccessorById::Iterator& it) const -> bool
  {
    return &m_Accessor == &it.m_Accessor && m_Pos == it.m_Pos;
  }

  auto ComponentsAccessorById::Iterator::operator!=(const ComponentsAccessorById::Iterator& it) const -> bool
  {
    return &m_Accessor != &it.m_Accessor || m_Pos != it.m_Pos;
  }
}
