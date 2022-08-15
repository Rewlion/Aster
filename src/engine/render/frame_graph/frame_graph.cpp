#include "frame_graph.h"
#include "topological_sorter.h"
#include "render_pass_resources.h"

#include <engine/assert.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>

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

      for (const auto& [vResHandle, beginState]: m_RenderPassPins[id].beginTextureStates)
        transitTextureState(vResHandle, beginState, encoder);

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
      res.handle = gapi::allocate_texture(res.allocDesc);
    }
    else
    {
      BufferResource& res = std::get<BufferResource>(r);
      res.handle = gapi::allocate_buffer(res.allocDesc.size, res.allocDesc.usage);
    }
  }

  void FrameGraph::transitTextureState(const VirtualResourceHandle h, const gapi::TextureState begin_state, gapi::CmdEncoder& encoder)
  {
    const VirtualResource& vr = m_VirtualResources[(size_t)h];
    Resource& r = m_Resources[(size_t)vr.resourceId];
    TextureResource& tex = std::get<TextureResource>(r);
    if (begin_state != tex.currentState)
    {
      encoder.transitTextureState(tex.handle, tex.currentState, begin_state);
      tex.currentState = begin_state;
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

  gapi::TextureHandler FrameGraph::getTexture(const VirtualResourceHandle h) const
  {
    const VirtualResource& vr = m_VirtualResources[(size_t)h];
    const Resource& r = m_Resources[(size_t)vr.resourceId];
    ASSERT_FMT(std::holds_alternative<TextureResource>(r), "FG: failed to get texture {}: it's not a texture handle", (size_t)h);
    return std::get<TextureResource>(r).handle;
  }

  gapi::BufferHandler FrameGraph::getBuffer(const VirtualResourceHandle h) const
  {
    const VirtualResource& vr = m_VirtualResources[(size_t)h];
    const Resource& r = m_Resources[(size_t)vr.resourceId];
    ASSERT_FMT(std::holds_alternative<BufferResource>(r), "FG: failed to get buffer {}: it's not a buffer handle", (size_t)h);
    return std::get<BufferResource>(r).handle;
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

  VirtualResourceHandle FrameGraph::importTexture(const std::string_view name, gapi::TextureHandler h, const gapi::TextureState current_state, Node* producer)
  {
    const ResourceHandle resId = (ResourceHandle)m_Resources.size();
    m_Resources.push_back(TextureResource{
      name,
      {},
      h,
      current_state,
      true
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

  VirtualResourceHandle FrameGraph::createTexture(const std::string_view name, const gapi::TextureAllocationDescription& desc, Node* producer)
  {
    const ResourceHandle resId = (ResourceHandle)m_Resources.size();
    m_Resources.push_back(TextureResource{
      name,
      desc,
      gapi::TextureHandler::Invalid,
      gapi::TextureState::Undefined,
      false
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
      name,
      desc,
      gapi::BufferHandler::Invalid,
      false
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
