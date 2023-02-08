#include "level.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/ecs.h>
#include <engine/input/input.h>
#include <engine/scene/scene.h>

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

      void operator()(const Engine::ECS::EntityId& eid, Engine::ECS::EntityComponents& init)
      {
        #define DECL_COMP_INIT(blk_type, type)\
          case DataBlock::ValueType::blk_type:\
          {\
            init[str_hash(componentName)] = std::get<type>(attr.as);\
            break;\
          }


        for(const auto& attr: m_Entity.getAttributes())
        {
          const string_view componentName{attr.name};

          switch(attr.type)
          {
            DECL_COMP_INIT(Int,    int)
            DECL_COMP_INIT(Int2,   int2)
            DECL_COMP_INIT(Int3,   int3)
            DECL_COMP_INIT(Int4,   int4)
            DECL_COMP_INIT(Float,  float)
            DECL_COMP_INIT(Float2, float2)
            DECL_COMP_INIT(Float3, float3)
            DECL_COMP_INIT(Float4, float4)
            DECL_COMP_INIT(Text,   string)
            DECL_COMP_INIT(Bool,   bool)
            DECL_COMP_INIT(Mat3,   mat3)
            DECL_COMP_INIT(Mat4,   mat4)
           
            case DataBlock::ValueType::None:
            {
              ASSERT(!"no value inside datablock");
              break;
            }
          }
        }
      
      #undef DECL_COMP_INIT
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
      ECS::get_registry().createEntity(tmpl, From(entityBlk));
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