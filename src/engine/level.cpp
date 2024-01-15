#include "level.h"

#include <engine/data/utils.h>
#include <engine/ecs/ecs.h>
#include <engine/input/input.h>
#include <engine/scene/scene.h>

#include <ranges>

namespace
{
  ecs::EntityComponents entity_components(const ed::Scope& entity)
  {
    ecs::EntityComponents init;

    #define DECL_COMP_INIT(ed_type, type)\
      case ed::ValueType::ed_type:\
      {\
        init[var.name.c_str()] = std::get<type>(var.value);\
        break;\
      }


    for(const ed::Variable& var: entity.getVariables())
    {
      switch(var.getValueType())
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
        default: {}
      }
    }
    #undef DECL_COMP_INIT
    return init;
  }
}

namespace Engine
{
  static void add_entity(const ed::Scope& entity)
  {
    const string_view tmpl = entity.getAnnotation();
    if (tmpl != "")
    {
      ecs::get_registry().createEntity(tmpl, entity_components(entity));
    }
    else
    {
      logerror("entity `{}` doesn't have a template", entity.getName());
      return;
    }
  }

  void load_level(const string_view file)
  {
    std::optional<ed::Scope> level = ed::load_from_file(file);
    ASSERT_FMT_RETURN(level.has_value(), , "failed to load the level `{}`", file);

    for(const ed::Scope& entity: level->getScopes())
      if (entity.getName() == "entity")
        add_entity(entity);

    const string actionSet = level->getVariable<string>("input.initial_action_set");
    Input::Manager::enableActionSet(actionSet, true);

    const ed::Scope& scene = level->getScope("scene");
    ::Engine::scene.loadScene(scene);
  }

}