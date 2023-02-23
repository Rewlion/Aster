#include "eid.h"

namespace ecs
{
  auto EntityId::generate() -> EntityId
  {
    static uint64_t id = 1;
    return EntityId{id++};
  }

  EntityId::EntityId()
    : id(0)
    , generation(0)
  {
  }

  EntityId::EntityId(uint64_t id)
    : id(id)
    , generation(0)
  {
  }

  auto EntityId::operator=(const EntityId& r) -> const EntityId&
  {
    id = r.id;
    generation = r.generation;
    return *this;
  }

  auto EntityId::getId() const -> uint64_t
  {
    return id;
  }

  auto EntityId::getGeneration() const -> uint64_t
  {
    return generation;
  }

  auto EntityId::isValid() const -> bool
  {
    return id != 0;
  }
}
