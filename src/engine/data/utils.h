#pragma once

#include "ed.h"
#include <engine/data/custom_type_registry.h>

#include <optional>

namespace ed
{
  auto load_from_file(const string_view path,
                      const std::shared_ptr<CustomTypeRegistry> reg = nullptr)
    -> std::optional<Scope>;
}