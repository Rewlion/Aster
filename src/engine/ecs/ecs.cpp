#include "ecs.h"

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
    loginfo("initialization of ECS");
    DataBlock* settings = get_app_settings();

    loginfo("init templates");
    DataBlock* templates = settings->getChildBlock("entity_templates");
    for(const auto& attr: templates->getAttributes())
    {
      if (attr.type == DataBlock::ValueType::Text)
      {
        const string blkWithTemplates = std::get<string>(attr.as);
        loginfo("reading templates from {}", blkWithTemplates);

        add_templates_from_blk(registry, blkWithTemplates);
      }
    }
  }
}
