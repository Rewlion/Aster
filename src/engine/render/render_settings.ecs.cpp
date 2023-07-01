#include "render_settings.h"

#include <engine/ecs/macros.h>
#include <EASTL/functional.h>

ECS_QUERY()
static
void query_render_settings(eastl::function<void(const int2 render_window_size)>);

int2 get_render_size()
{
  int2 size{0,0};
  query_render_settings([&](const int2 render_window_size){ size = render_window_size; });
  return size;
}
