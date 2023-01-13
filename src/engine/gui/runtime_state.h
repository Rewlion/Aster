#pragma once

#include "react_states_storage.h"
#include "timer.h"

#include <engine/qjs/user_state.h>

namespace Engine::gui
{
  struct RuntimeState
  {
    ReactStatesStorage reactStorage;
    TimersPool timers;
  };
}
