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

      void read(const std::string_view name, const gapi::TextureState begin_state);
      void write(const std::string_view name, const gapi::TextureState begin_state);

      void addRenderTarget(const std::string_view name, const gapi::LoadOp load, const gapi::StoreOp store);
      void setDepthStencil(const std::string_view name, const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                                        const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store);

      void createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc);
      void createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc);

    private:
      FrameGraph& m_FrameGraph;
      RenderPassPins& m_Pins;
      Node& m_Node;
  };
}
