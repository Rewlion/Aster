#pragma once

#include <engine/types.h>

inline
auto get_group_size(const uint render_dim, const uint tile_dim) -> uint
{
  const uint add = render_dim % tile_dim > 0 ? 1 : 0;
  return render_dim / tile_dim + add;
}
