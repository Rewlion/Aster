#include "level.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>
#include <engine/input/input.h>
#include <engine/scene/scene.h>
#include <engine/utils/string.hpp>

#include <ranges>

namespace
{
  class From
  {
    public:
      From(const DataBlock& entity)
        : m_Entity(entity)
      {
      }

      void operator()(const EntityId& eid, Engine::ECS::EntityInitializer& init)
      {
        for(const auto& attr: m_Entity.getAttributes())
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
            case DataBlock::ValueType::None:
            {
              ASSERT(!"no value inside datablock");
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
  static void add_entity(const DataBlock& entityBlk)
  {
    string tmpl = entityBlk.getAnnotation();
    if (tmpl != "")
    {
      Utils::remove_spaces(tmpl);
      eastl::vector<string> templates;

      for (const auto& strRange: tmpl | std::views::split(','))
        templates.push_back(string{strRange.begin(), strRange.end()});

      ECS::manager.createEntity(templates, From(entityBlk));
    }
    else
    {
      logerror("entity `{}` doesn't have template", entityBlk.getName());
      return;
    }
  }

  void load_level(const string& levelBlk)
  {
    DataBlock blk;
    const bool levelLoaded = load_blk_from_file(&blk, levelBlk.c_str());
    ASSERT(levelLoaded);

    for(const DataBlock& entityBlk: blk.getChildBlocks())
      if (entityBlk.getName() == "entity")
        add_entity(entityBlk);

    const string actionSet = blk.getText("input.initial_action_set");
    Input::manager.setActionSet(str_hash(actionSet.c_str()));

    const DataBlock* sceneBlk = blk.getChildBlock("scene");
    ::Engine::scene.loadScene(*sceneBlk);
  }

}