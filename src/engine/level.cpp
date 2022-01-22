#include "level.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>

namespace
{
  class From
  {
    public:
      From(const DataBlock& entity)
        : m_Entity(entity)
      {
      }

      void operator()(const EntityId& eid, EntityInitializer& init)
      {
        for(const auto& attr: m_Entity.GetAttributes())
        {
          const string_hash componentName = str_hash(attr.name.c_str());

          switch(attr.type)
          {
            case DataBlock::ValueType::Int:
            {
              auto value = std::get<int>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Int2:
            {
              auto value = std::get<int2>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Int3:
            {
              auto value = std::get<int3>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Int4:
            {
              auto value = std::get<int4>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Float:
            {
              auto value = std::get<float>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Float2:
            {
              auto value = std::get<float2>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Float3:
            {
              auto value = std::get<float3>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Float4:
            {
              auto value = std::get<float4>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Text:
            {
              auto value = std::get<string>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Mat3:
            {
              auto value = std::get<mat3>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Mat4:
            {
              auto value = std::get<mat4>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
            case DataBlock::ValueType::Bool:
            {
              auto value = std::get<bool>(attr.as);
              init.init_component(componentName, std::move(value));
              break;
            }
          }
        }
      }

    private:
      const DataBlock& m_Entity;
  };
}

namespace Engine
{
  static void AddEntity(const DataBlock& entityBlk)
  {
    const string tmpl = entityBlk.GetAnnotation();
    if (tmpl != "")
    {
      const auto tmplHash = str_hash(tmpl.c_str());
      ecs.create_entity(tmplHash, From(entityBlk));
    }
    else
    {
      logerror("entity `{}` doesn't have template", entityBlk.GetName());
      return;
    }
  }

  void LoadLevel(const string& levelBlk)
  {
    DataBlock blk;
    const bool levelLoaded = LoadBlkFromFile(&blk, levelBlk.c_str());
    ASSERT(levelLoaded);

    for(const DataBlock& entityBlk: blk.GetChildBlocks())
      if (entityBlk.GetName() == "entity")
        AddEntity(entityBlk);
  }

}