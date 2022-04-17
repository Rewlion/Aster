#pragma once

#include <engine/types.h>
#include <engine/algorithm/hash.h>

#include <EASTL/fixed_vector.h>
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
  };

  struct ActionSetMappings
  {
    eastl::fixed_vector<ButtonMapping, 256> buttons;
    eastl::fixed_vector<AnalogMapping, 16> analogs;
  };

  struct ActionSet
  {
    string name;

    eastl::fixed_vector<ButtonAction, 256> buttons;
    eastl::fixed_vector<AnalogAction, 16> analogs;
  };

}