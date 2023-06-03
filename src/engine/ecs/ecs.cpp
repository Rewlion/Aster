#include "ecs.h"

#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>
#include <engine/vfs/vfs.h>

namespace ecs
{
  Registry registry;

  auto get_registry() -> Registry&
  {
    return registry;
  }

  void init_from_settings()
  {
    loginfo("ecs: initialization of ECS");
    DataBlock* settings = Engine::get_app_settings();

    loginfo("ecs: initializing components meta");
    init_meta_storage();

    loginfo("ecs: init templates");
    DataBlock* templates = settings->getChildBlock("entity_templates");
    for(const auto& attr: templates->getAttributes())
    {
      if (attr.type == DataBlock::Attribute::Type::Text)
      {
        const string blkWithTemplates = std::get<string>(attr.as);
        loginfo("ecs: reading templates from {}", blkWithTemplates);

        add_templates_from_blk(registry, blkWithTemplates);
      }
    }

    for (const char* p: {
      "@engine_res/templates/render.bk"})
    {
      loginfo("ecs: reading templates from {}", p);
      add_templates_from_blk(registry, p);
    }
  }
}
