#pragma once

#include <stdint.h>

namespace Engine::ECS
{
  class Registry;
}

class EntityId
{
  friend Engine::ECS::Registry;

  public:
    inline EntityId()
      : id(0)
      , generation(0)
    {
    }

    inline EntityId(uint64_t id)
      : id(id)
      , generation(0)
    {
    }

    inline const EntityId& operator=(const EntityId& r)
    {
      id = r.id;
      generation = r.generation;
      return *this;
    }

    inline uint64_t getId() const { return id; }
    inline uint64_t getGeneration() const { return generation; }

  private:
    uint64_t id:48, generation:16;
};
