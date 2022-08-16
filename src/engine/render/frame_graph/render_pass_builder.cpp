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

  VirtualResourceHandle RenderPassBuilder::read(const VirtualResourceHandle resource, const gapi::TextureState begin_state)
  {
    m_Pins.demandTextureState(resource, begin_state);
    return m_Pins.read(resource);
  }

  VirtualResourceHandle RenderPassBuilder::write(const VirtualResourceHandle resource, const gapi::TextureState begin_state)
  {
    if (m_FrameGraph.isImportedResource(resource))
      m_Pins.setSideEffect();

    m_Pins.demandTextureState(resource, begin_state);

    if (m_Pins.hasCreate(resource))
      return resource;

    m_Pins.read(resource);
    return m_Pins.write(m_FrameGraph.cloneResource(resource, &m_Node));
  }

  VirtualResourceHandle RenderPassBuilder::createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc)
  {
    const VirtualResourceHandle h = m_FrameGraph.createTexture(name, desc, &m_Node);
    return m_Pins.create(h);
  }

  VirtualResourceHandle RenderPassBuilder::createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc)
  {
    const VirtualResourceHandle h = m_FrameGraph.createBuffer(name, desc, &m_Node);
    return m_Pins.create(h);
  }
}
