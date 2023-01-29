#pragma once

#include <stdint.h>

namespace Engine::ECS
{
  class EntityId
  {
    friend class Registry;
    public:
      static EntityId generate()
      {
        static uint64_t id = 1;
        return EntityId{id++};
      }

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

      inline bool isValid() const { return id != 0; }

    private:
      uint64_t id:48, generation:16;
  };
}
