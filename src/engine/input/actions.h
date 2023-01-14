#pragma once

#include <engine/types.h>
#include <engine/algorithm/hash.h>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace Engine::Input
{
  struct ButtonAction
  {
    string name;
  };

  struct AnalogAction
  {
    string name;
  };

  enum class ButtonStatus: char
  {
    Release = 0,
    Press = 1,
    Unknown = 2
  };

  struct ButtonMapping
  {
    string_hash action = 0;
    string actionName;
    char buttonId = 0;

    ButtonStatus status = ButtonStatus::Release;
  };

  struct AnalogMapping
  {
    string_hash action = 0;
    string actionName;
    char device = 0;

    uint8_t absoluteMouse : 1 = 0;
  };

  struct ActionSetMappings
  {
    eastl::vector<ButtonMapping> buttons;
    eastl::vector<AnalogMapping> analogs;
  };

  struct ActionSet
  {
    string name;

    eastl::vector<ButtonAction> buttons;
    eastl::vector<AnalogAction> analogs;
  };

}