#pragma once

#include "concepts.hpp"
#include "render_pass.h"
#include "render_pass_pins.h"
#include "render_pass_builder.h"
#include "resources.h"

#include <memory>

namespace gapi
{
  class CmdEncoder;
}

namespace fg
{
  class FrameGraph
  {
    friend class RenderPassBuilder;
    friend class TopologicalSorter;
    friend class RenderPassResources;

    public:
      template<class Data, class SetupCallback, class ExecCallback>
      requires SetupCbConcept<SetupCallback, Data> &&
               ExecCbConcept<ExecCallback, Data>
      Data& addCallbackPass(const std::string_view name, SetupCallback&& setup, ExecCallback&& exec);

      void compile();
      void execute(gapi::CmdEncoder& encoder);

    private:
      void createResource(const VirtualResourceHandle h);
      VirtualResource& getVirtualResource(const VirtualResourceHandle h);
      const VirtualResource& getVirtualResource(const VirtualResourceHandle h) const;
      bool isImportedResource(const VirtualResourceHandle h) const;

      VirtualResourceHandle cloneResource(const VirtualResourceHandle h, Node* producer);
      VirtualResourceHandle createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc, Node* producer);
      VirtualResourceHandle createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc, Node* producer);

    private:
      eastl::vector<std::unique_ptr<Node>> m_RenderPasses;
      eastl::vector<RenderPassPins> m_RenderPassPins;
      eastl::vector<Resource> m_Resources;
      eastl::vector<VirtualResource> m_VirtualResources;
      eastl::vector<size_t> m_Order;
  };
}

#include "frame_graph.inc"
