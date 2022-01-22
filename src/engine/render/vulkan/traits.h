#pragma once

namespace gapi::vulkan::traits
{
  constexpr size_t MAX_DEPTH_STENCIL_STATES = 64;

  namespace vertex
  {
    constexpr size_t ID = 0;
  }

  namespace fragment
  {
    constexpr size_t ID = 1;
  }

  constexpr size_t LAST_SHADER = 2;
}