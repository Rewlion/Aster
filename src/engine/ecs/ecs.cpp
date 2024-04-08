#include "ecs.h"

#include <engine/assert.h>
#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>
#include <engine/fs/vfs.h>

namespace ecs
{
  std::unique_ptr<Registry> registry;

  auto get_registry() -> Registry&
  {
    return *registry.get();
  }

  void init_from_settings()
  {
    loginfo("ecs: initialization of ECS");
    ASSERT_FMT_RETURN(registry.get() == nullptr, , "ecs: registry has been initialized already");

    registry.reset(new Registry);

    const ed::Scope& settings = Engine::get_app_settings();

    loginfo("ecs: initializing components meta");
    init_meta_storage();
    loginfo("ecs: initializing engine data custom type registry");
    init_custom_type_registry();

    loginfo("ecs: init templates");
    for (const char* p: {
      "@engine_res/templates/render.ed"})
    {
      loginfo("ecs: reading templates from {}", p);
      add_templates_from_ed(*registry, p);
    }

    const ed::Scope& templates = settings.getScope("entity_templates");
    for(const ed::Variable& var: templates.getVariables())
    {
      if (var.getValueType() == ed::ValueType::Text)
      {
        const string_view edWithTemplates = std::get<string>(var.value);
        loginfo("ecs: reading templates from {}", edWithTemplates);

        add_templates_from_ed(*registry, edWithTemplates);
      }
    }

    registry->initialRegistration();
  }

  void shutdown()
  {
    registry.reset();
  }
}
