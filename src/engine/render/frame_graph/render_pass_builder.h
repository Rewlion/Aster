#pragma once

#include "handles.h"

#include <string_view>

namespace gapi
{
  struct TextureAllocationDescription;
  struct BufferAllocationDescription;
}

namespace fg
{
  class FrameGraph;
  class RenderPassPins;
  class Node;

  class RenderPassBuilder
  {
    public:
      RenderPassBuilder(FrameGraph& fg, RenderPassPins& pins, Node& node);

      [[nodiscard]] VirtualResourceHandle read(const VirtualResourceHandle resource);
      [[nodiscard]] VirtualResourceHandle write(const VirtualResourceHandle resource);

      [[nodiscard]] VirtualResourceHandle createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc);
      [[nodiscard]] VirtualResourceHandle createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc);

    private:
      FrameGraph& m_FrameGraph;
      RenderPassPins& m_Pins;
      Node& m_Node;
  };
}