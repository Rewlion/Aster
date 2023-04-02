#pragma once

#include "eid.h"

#include <engine/types.h>

namespace ecs
{
  auto add_sub_template(const EntityId, string_view tmpl) -> EntityId;
  auto remove_sub_template(const EntityId, string_view tmpl_to_remove) -> EntityId;
}
