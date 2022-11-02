#include "render_pass_builder.h"
#include "render_pass_pins.h"
#include "frame_graph.h"

namespace fg
{
  RenderPassBuilder::RenderPassBuilder(FrameGraph& fg, RenderPassPins& pins, Node& node)
    : m_FrameGraph(fg)
    , m_Pins(pins)
    , m_Node(node)
  {
  }

  void RenderPassBuilder::read(const std::string_view name, const gapi::TextureState begin_state)
  {
    const VirtualResourceHandle resource = m_FrameGraph.getVirtualResourceHandleFromName(name);
    m_FrameGraph.insureVirtualResourceExistance(name, resource);

    m_Pins.demandTextureState(resource, begin_state);
    m_Pins.read(resource);
  }

  void RenderPassBuilder::write(const std::string_view name, const gapi::TextureState begin_state)
  {
    const VirtualResourceNameHash nameHash = m_FrameGraph.getVirtualResourceNameHash(name);
    const VirtualResourceHandle resource = m_FrameGraph.getVirtualResourceHandleFromNameHash(nameHash);
    m_FrameGraph.insureVirtualResourceExistance(name, resource);

    if (m_FrameGraph.isImportedResource(resource))
      m_Pins.setSideEffect();

    m_Pins.demandTextureState(resource, begin_state);

    if (!m_Pins.hasCreate(resource))
    {
      m_Pins.read(resource);

      const VirtualResourceHandle newHandle = m_FrameGraph.cloneResource(resource, &m_Node);
      m_FrameGraph.m_ResourceNameToHandle[nameHash] = newHandle;

      m_Pins.write(newHandle);
    }
  }

  void RenderPassBuilder::addRenderTarget(const std::string_view name, const gapi::LoadOp load, const gapi::StoreOp store)
  {
    const VirtualResourceHandle resource = m_FrameGraph.getVirtualResourceHandleFromName(name);
    m_FrameGraph.insureVirtualResourceExistance(name, resource);
    m_Pins.addRenderTarget(resource, load, store);
  }
  void RenderPassBuilder::setDepthStencil(const std::string_view name, const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                          const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store)
  {
    const VirtualResourceHandle resource = m_FrameGraph.getVirtualResourceHandleFromName(name);
    m_FrameGraph.insureVirtualResourceExistance(name, resource);
    m_Pins.setDepthStencil(resource, depth_load, depth_store, stencil_load, stencil_store);
  }

  void RenderPassBuilder::createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc)
  {
    const VirtualResourceHandle h = m_FrameGraph.createTexture(name, desc, &m_Node);
    m_Pins.create(h);
  }

  void RenderPassBuilder::createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc)
  {
    const VirtualResourceHandle h = m_FrameGraph.createBuffer(name, desc, &m_Node);
    m_Pins.create(h);
  }
}
