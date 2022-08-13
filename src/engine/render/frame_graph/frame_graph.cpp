#include "frame_graph.h"
#include "topological_sorter.h"
#include "render_pass_resources.h"

namespace fg
{
  void FrameGraph::compile()
  {
    TopologicalSorter sorter(*this);
    m_Order = sorter.getSortedOrder();
  }

  void FrameGraph::execute(gapi::CmdEncoder& encoder)
  {
    RenderPassResources rpResources(*this);
    for (const size_t id: m_Order)
    {
      for (const auto& vResHandle: m_RenderPassPins[id].creates)
        createResource(vResHandle);

      Node& rp = *m_RenderPasses[id];
      rp.exec(rpResources, encoder);
    }
  }

  void FrameGraph::createResource(const VirtualResourceHandle h)
  {
    const VirtualResource& vr = m_VirtualResources[(size_t)h];
    Resource& r = m_Resources[(size_t)vr.resourceId];
    if (std::holds_alternative<TextureResource>(r))
    {
      TextureResource& res = std::get<TextureResource>(r);
      res.handle = gapi::TextureWrapper(gapi::allocate_texture(res.allocDesc));
    }
    else
    {
      BufferResource& res = std::get<BufferResource>(r);
      res.handle = gapi::BufferWrapper(gapi::allocate_buffer(res.allocDesc.size, res.allocDesc.usage));
    }
  }

  VirtualResource& FrameGraph::getVirtualResource(const VirtualResourceHandle h)
  {
    return m_VirtualResources[(size_t)h];
  }

  const VirtualResource& FrameGraph::getVirtualResource(const VirtualResourceHandle h) const
  {
    return const_cast<FrameGraph&>(*this).getVirtualResource(h);
  }

  bool FrameGraph::isImportedResource(const VirtualResourceHandle h) const
  {
    const VirtualResource& vr = m_VirtualResources[(size_t)h];
    const Resource& r = m_Resources[(size_t)vr.resourceId];
    if (std::holds_alternative<TextureResource>(r))
      return std::get<TextureResource>(r).isImported;
    else
      return std::get<BufferResource>(r).isImported;
  }

  VirtualResourceHandle FrameGraph::cloneResource(const VirtualResourceHandle h, Node* producer)
  {
    VirtualResource& res = m_VirtualResources[(size_t)h];
    VirtualResourceHandle nh = (VirtualResourceHandle)m_VirtualResources.size();

    m_VirtualResources.push_back(res);
    VirtualResource& newRes = m_VirtualResources.back();
    newRes.lastAccessor = nullptr;
    newRes.producer = producer;
    newRes.version += 1;

    return nh;
  }

  VirtualResourceHandle FrameGraph::createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc, Node* producer)
  {
    const ResourceHandle resId = (ResourceHandle)m_Resources.size();
    m_Resources.push_back(TextureResource{
      .name = name,
      .allocDesc = desc,
      .handle = gapi::TextureHandler::Invalid,
      .isImported = false
    });

    const VirtualResourceHandle vResId = (VirtualResourceHandle)m_VirtualResources.size();
    m_VirtualResources.push_back(VirtualResource{
      .resourceId = resId,
      .version = 0,
      .producer = producer,
      .lastAccessor = nullptr
    });

    return vResId;
  }

  VirtualResourceHandle FrameGraph::createBuffer(const std::string_view name, const gapi::BufferAllocationDescription& desc, Node* producer)
  {
    const ResourceHandle resId = (ResourceHandle)m_Resources.size();
    m_Resources.push_back(BufferResource{
      .name = name,
      .allocDesc = desc,
      .handle = gapi::BufferHandler::Invalid,
      .isImported = false
    });

    const VirtualResourceHandle vResId = (VirtualResourceHandle)m_VirtualResources.size();
    m_VirtualResources.push_back(VirtualResource{
      .resourceId = resId,
      .version = 0,
      .producer = producer,
      .lastAccessor = nullptr
    });

    return vResId;
  }
}
