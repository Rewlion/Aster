#include "level.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/ecs.h>
#include <engine/input/input.h>
#include <engine/scene/scene.h>

#include <ranges>

namespace
{
  ecs::EntityComponents entity_components(const DataBlock& entity)
  {
    ecs::EntityComponents init;

    #define DECL_COMP_INIT(blk_type, type)\
      case DataBlock::Attribute::Type::blk_type:\
      {\
        init[attr.name.c_str()] = std::get<type>(attr.as);\
        break;\
      }


    for(const auto& attr: entity.getAttributes())
    {
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
      }
    }
    #undef DECL_COMP_INIT
    return init;
  }
}

namespace Engine
{
  static void add_entity(const DataBlock& entityBlk)
  {
    string tmpl = entityBlk.getAnnotation();
    if (tmpl != "")
    {
      ecs::get_registry().createEntity(tmpl, entity_components(entityBlk));
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
    Input::Manager::enableActionSet(actionSet, true);

    const DataBlock* sceneBlk = blk.getChildBlock("scene");
    ::Engine::scene.loadScene(*sceneBlk);
  }

}