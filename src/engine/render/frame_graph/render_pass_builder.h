#pragma once

#include "handles.h"

#include <engine/gapi/resources.h>

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

      VirtualResourceHandle read(const VirtualResourceHandle resource, const gapi::TextureState begin_state);
      VirtualResourceHandle write(const VirtualResourceHandle resource,  const gapi::TextureState begin_state);

      void addRenderTarget(const VirtualResourceHandle resource, const gapi::LoadOp load, const gapi::StoreOp store);
      void setDepthStencil(const VirtualResourceHandle resource, const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                                                 const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store);

      [[nodiscard]] VirtualResourceHandle createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc);
      [[nodiscard]] VirtualResourceHandle createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc);

    private:
      FrameGraph& m_FrameGraph;
      RenderPassPins& m_Pins;
      Node& m_Node;
  };
}
