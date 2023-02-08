#include "ecs.h"

#include <engine/ecs/components.h>
#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>

namespace Engine::ECS
{
  Registry registry;

  Registry& get_registry()
  {
    return registry;
  }

  void init_from_settings()
  {
    loginfo("ecs: initialization of ECS");
    DataBlock* settings = get_app_settings();

    loginfo("ecs: initializing components meta");
    init_meta_storage();

    loginfo("ecs: init templates");
    DataBlock* templates = settings->getChildBlock("entity_templates");
    for(const auto& attr: templates->getAttributes())
    {
      if (attr.type == DataBlock::ValueType::Text)
      {
        const string blkWithTemplates = std::get<string>(attr.as);
        loginfo("ecs: reading templates from {}", blkWithTemplates);

        add_templates_from_blk(registry, blkWithTemplates);
      }
    }
  }
}
