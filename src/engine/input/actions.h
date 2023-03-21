#pragma once

#include "input_device_type.h"

#include <engine/types.h>
#include <engine/algorithm/hash.h>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

#include <variant>

/*
--------
input processing:

array of button status (pressed|unpressed + clicks, last click time)
|  KEYBOARD  | MOUSE  |

array of axis

--------
actions type:
1. Digital  : ON | OFF

2. Analog: 1+ axes

1. Button
  : type = "button"
  : signal = on_press, on_release, on_dbl_clk, on_long_press, pressed
binding
  : device, button

2. Analog
  : type = joy_move, joy_delta 
binding
  : device
  x_axis, y_axis 

actions are unique!
actions declared inside actionsets

MyActionSet {
  MyButtonAction {
    text type = "button"
    text signal = "on_press"
  }

  MyAnalogAction {
    text type = "joy_move"
  }
}

//bindings
MyButtonAction {
  int device = 1
  int button = 3
}

MyAnalogAction {
  device = 3
  x_axis = 0
  x_axis = 1
}

---------------------

buttonActionData{
  bool prevState;
  bool currentState;
}

--------------------


1. loads actionssets
 each action is checked that wasn't declared before already
 because it should be globally unique (by name to action_id map)

each action is added to global actions array
and to the map (action_name to action_id)

each action_id is added to the actions array inside actionset

2. loads action bindings
 
 
where to store action to binding map ?
inside the action itself
by default it's UNKNOWN_BINDING == -1

 actions says actions events (long pressed and etc)
 bindings says where to take it from
 so we iterate over bindings, during actions processing
but logically it's better to iterate over actions!!
so we need action => binding map!!

processing cycle:
for each actionset
 if it's active
  for each action
   take binding
   if it's ok
    process action binding (take data from the state and so on)
*/


namespace Engine::Input
{
  constexpr int UNKNOWN_BINDING = -1;
  constexpr int UNKNOWN_ACTION_ID = -1;

  enum class ActionType
  {
    Button   = 1,
    JoyMove  = 1 << 2,
    JoyDelta = 1 << 3,
    JoyAction = JoyMove | JoyDelta
  };

  struct ButtonAction
  {
    enum class Mode
    {
      OnPress,
      OnRelease,
      OnDblClick,
      OnLongPress,
      LongPressed,
      Pressed
    };

    string name;
    Mode mode;
    int bindingId;
  };
  struct AnalogAction
  {
    string name;
    ActionType type;
    int bindingId;
  };
  using Action = std::variant<ButtonAction, AnalogAction>;

  struct ButtonActionBinding
  {
    InputDevice device;
    int button;
  };
  struct AnalogActionBinding
  {
    InputDevice device;
    int2 axis;
  };
  using ActionBinding = std::variant<ButtonActionBinding, AnalogActionBinding>;

  struct ButtonActionData
  {
    int isActive  :1 = 0;
    int longPress :1 = 0;
  };
  struct AnalogActionData
  {
    bool isActive = false;
    int2 values = {0,0};
  };
  using ActionData = std::variant<ButtonActionData, AnalogActionData>;

  struct ActionSet
  {
    bool isEnabled = false;
    eastl::vector<int> actionIds;
  };
}
