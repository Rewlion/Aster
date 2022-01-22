#pragma once

#include <engine/types.h>
#include <engine/algorithm/hash.h>

#include <EASTL/fixed_vector.h>
#include <EASTL/vector_map.h>

namespace Engine::Input
{
  struct ButtonAction
  {
    String name;
  };

  enum class ButtonStatus: char
  {
    Release = 0,
    Press = 1
  };

  struct ButtonMapping
  {
    string_hash action = 0;
    String actionName;
    char buttonId = 0;

    ButtonStatus status = ButtonStatus::Release;
  };

  struct ActionSet
  {
    String name;

    eastl::fixed_vector<ButtonAction, 256> buttons;
  };

}