#pragma once

#include "concepts.hpp"
#include "render_pass.h"
#include "render_pass_builder.h"
#include "render_pass_pins.h"
#include "render_pass_resources.h"
#include "resources.h"

#include <EASTL/vector_map.h>

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
      template<class SetupCallback, class ExecCallback>
      requires NoDataSetupCbConcept<SetupCallback> &&
               NoDataExecCbConcept<ExecCallback>
      void addCallbackPass(const std::string_view name, SetupCallback&& setup, ExecCallback&& exec);

      void compile();
      void execute(gapi::CmdEncoder& encoder);

      void importTexture(const std::string_view name, gapi::TextureHandle h, const gapi::TextureState current_state);

    private:
      template<class Data, class SetupCallback, class ExecCallback>
      RenderPass<Data, ExecCallback>* addRenderPass(const std::string_view name, SetupCallback&& setup, ExecCallback&& exec);

      void beginRenderPass(const eastl::vector<RenderTarget>& targets, const DepthStencil& depth_stencil, gapi::CmdEncoder& encoder);
      void createResource(const VirtualResourceHandle h);
      void transitTextureState(const VirtualResourceHandle h, const gapi::TextureState begin_state, gapi::CmdEncoder& encoder);
      VirtualResource& getVirtualResource(const VirtualResourceHandle h);
      const VirtualResource& getVirtualResource(const VirtualResourceHandle h) const;
      bool isImportedResource(const VirtualResourceHandle h) const;
      TextureResourceView getTexture(const std::string_view name) const;
      gapi::BufferHandler getBuffer(const std::string_view name) const;

      VirtualResourceHandle cloneResource(const VirtualResourceHandle h, Node* producer);
      VirtualResourceHandle createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc, Node* producer);
      VirtualResourceHandle createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc, Node* producer);

      void storeResourceNameToHandleMap(const std::string_view name, const VirtualResourceHandle handle);
      VirtualResourceNameHash getVirtualResourceNameHash(const std::string_view name) const;
      VirtualResourceHandle getVirtualResourceHandleFromNameHash(const VirtualResourceNameHash hash) const;
      VirtualResourceHandle getVirtualResourceHandleFromName(const std::string_view name) const;

      void insureVirtualResourceExistance(const std::string_view name, const VirtualResourceHandle handle) const;

    private:
      eastl::vector<std::unique_ptr<Node>> m_RenderPasses;
      eastl::vector<RenderPassPins> m_RenderPassPins;
      eastl::vector<Resource> m_Resources;
      eastl::vector<VirtualResource> m_VirtualResources;
      eastl::vector_map<VirtualResourceNameHash, VirtualResourceHandle> m_ResourceNameToHandle;
      eastl::vector<size_t> m_Order;
  };
}

#include "frame_graph.inc"
