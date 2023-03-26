#pragma once

#include <stdint.h>

namespace ecs
{
  class EntityId
  {
    friend class Registry;
    public:
      static
      auto generate() -> EntityId;

      EntityId();
      EntityId(uint64_t id);

      auto operator=(const EntityId& r) -> const EntityId&;
      auto operator==(const EntityId& r) const -> bool;

      auto getId() const -> uint64_t;
      auto getGeneration() const -> uint64_t;

      auto isValid() const -> bool;

    private:
      uint64_t id:48, generation:16;
  };
}
