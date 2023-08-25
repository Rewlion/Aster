#pragma once

#include <engine/types.h>

namespace Engine::Render
{
  struct VTile
  {
    VTile() { r = 0; }
    VTile(const uint16_t x, const uint16_t y, const uint8_t mip)
    {
      t.x = x; t.y = y; t.mip = mip;
    }
    VTile(const uint32_t v) {r = v;}
    struct Typed
    {
      uint32_t x  : 14;
      uint32_t y  : 14;
      uint32_t mip : 4;
    };
    union
    {
      Typed t;
      uint32_t r;
    };

    bool operator==(const VTile& rvl) const { return r == rvl.r;}
    bool operator<(const VTile& rvl) const { return r < rvl.r; }
  };
  static_assert(sizeof(VTile) == 4); // R32_UNORM

  inline
  auto get_tile_id(const uint2 pos, const size_t tiles_per_dim) -> size_t
  {
    return pos.x + pos.y * tiles_per_dim;
  }

  inline
  auto get_tile_id(const VTile tile, const size_t tiles_per_dim) -> size_t
  {
    return get_tile_id(uint2{tile.t.x, tile.t.y}, tiles_per_dim);
  }

  inline
  auto get_tile_pos_from_id(const size_t id, const size_t tiles_per_dim) -> uint2
  {
    return {
      id % tiles_per_dim,
      id / tiles_per_dim
    };
  }
}

namespace eastl
{
  template <class T> struct hash;

  template <> struct hash<Engine::Render::VTile>
		{ size_t operator()(Engine::Render::VTile val) const { return static_cast<size_t>(val.r); } };
}
