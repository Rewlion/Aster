#pragma once

#include "components.h"
#include "types.h"

#include <engine/types.h>
#include <engine/assert.h>

#include <EASTL/vector.h>

namespace Engine::ECS
{
  class ComponentsInitializer
  {
    friend class EntityComponents;
    public:
      template<class T>
      void operator=(T&& v) &&
      {
        const component_type_id valType = ComponentTypeId::from<T>();
        
        ASSERT_FMT(valType == m_Meta.typeId,
          "ecs: can't initialize component: invalid type {}, expected: {}",
          get_meta_storage().getMeta(valType).typeName,
          m_Meta.typeName);

        if (m_Meta.isTrivial)
            std::memcpy(m_Data, &v, sizeof(v));
        else
          m_Meta.manager->copy(m_Data, &v);
      }

    private:
      ComponentsInitializer(std::byte* data, const ComponentMeta& meta)
        : m_Data(data)
        , m_Meta(meta)
      {
      }

    private:
      std::byte* m_Data;
      const ComponentMeta& m_Meta;
  };

  class EntityComponents
  {
    public:
      EntityComponents(const size_t block_size, 
                       const eastl::vector<ArchetypeComponent>& comps,
                       const CompToPlacementMap& placement_map);
      EntityComponents(EntityComponents&& rvl);
      ~EntityComponents();

      ComponentsInitializer operator[] (const component_name_id nameId);
      void moveData(std::byte* to);

    private:
      const CompToPlacementMap& m_CompToPlacementsMap;

    // [block_offsets | metas | components_data]
      std::byte* m_Buffer;
      size_t* m_BlockOffsets;
      const ComponentMeta** m_Metas;
      std::byte* m_Data;

      size_t m_N;
  };
}
