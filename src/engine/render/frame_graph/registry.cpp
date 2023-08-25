#include "registry.h"

#include <engine/assert.h>
#include <engine/log.h>

namespace fg
{
  blob_id_t Registry::BlobCompileTypeId::m_LastId{0};

  void Registry::orderMeBefore(const char* node)
  {
    const name_id_t nameId = m_NodesNames.getStringId(node);
    if (nameId == INVALID_NAME_ID) [[unlikely]]
    {
      logerror("FG: failed to order node `{}` before not exist node `{}`",
        m_NodesNames.getString(m_Nodes[m_CurrentExecNodeId].nameId), node);
      return;
    }
    
    const node_id_t nodeId = to_node_id(nameId);
    m_Nodes[nodeId].prevNodes.push_back(m_CurrentExecNodeId);
  }

  void Registry::orderMeAfter(const char* node)
  {
    const name_id_t nameId = m_NodesNames.getStringId(node);
    if (nameId == INVALID_NAME_ID) [[unlikely]]
    {
      logerror("FG: failed to order node `{}` after not exist node `{}`",
        m_NodesNames.getString(m_Nodes[m_CurrentExecNodeId].nameId), node);
      return;
    }
    
    m_Nodes[m_CurrentExecNodeId].prevNodes.push_back(to_node_id(nameId));
  }

  auto Registry::createBuffer(const char* name, const gapi::BufferAllocationDescription& desc, const gapi::BufferState init_state) -> BufferRequest
  {
    if (!desc.name)
      const_cast<gapi::BufferAllocationDescription&>(desc).name = name;

    return createResourceInternal(name, BufferResource{.allocDesc = desc, .initState = init_state},
      [this, init_state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      m_VirtResources[virt_res_id].modificationChain.insert(0, m_CurrentExecNodeId);
    });
  }

  auto Registry::modifyBuffer(const char* name, const gapi::BufferState state) -> BufferRequest
  {
    return modifyResourceInternal(name, [state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.bufferBeginStates.push_back({virt_res_id, state});
    });
  }

  auto Registry::readBuffer(const char* name, const gapi::BufferState state, const bool optional) -> BufferRequest
  {
    return readResourceInternal(name, optional, Timeline::Current, [state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.bufferBeginStates.push_back({virt_res_id, state});
    });
  }

  auto Registry::createTexture(const char* name, const gapi::TextureAllocationDescription& desc, const gapi::TextureState init_state) -> TextureRequest
  {
    if (!desc.name)
      const_cast<gapi::TextureAllocationDescription&>(desc).name = name;

    return createResourceInternal(name, TextureResource{.allocDesc = desc}, [this, init_state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      m_VirtResources[virt_res_id].modificationChain.insert(0, m_CurrentExecNodeId);
      node.execState.textureBeginStates.push_back({virt_res_id, init_state});
    });
  }

  auto Registry::createResourceInternal(const char* name, Resource&& res, CreateCb&& cb) -> virt_res_id_t
  {
    const name_id_t nameId = m_ResourcesNames.storeString(name);
    const virt_res_id_t vResId = to_virt_res_id(nameId);
    const res_id_t resId = to_res_id(vResId);

    auto& vRes = m_VirtResources[vResId];
    vRes.resourceId = resId;
    vRes.createdBy = m_CurrentExecNodeId;
    
    m_Resources[resId] = std::move(res);

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.creates.push_back(vResId);
    cb(vResId, node);

    return {vResId};
  }

  auto Registry::modifyResourceInternal(const char* name, ModifyCb&& cb) -> virt_res_id_t
  {
    const name_id_t nameId = m_ResourcesNames.storeString(name);
    const virt_res_id_t vResId = to_virt_res_id(nameId);

    m_VirtResources[vResId].modificationChain.push_back(m_CurrentExecNodeId); //todo: should be pushed at the begining because of the consumer at the end

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.modifies.push_back(vResId);

    cb(vResId, node);

    return vResId;
  }

  auto Registry::readResourceInternal(const char* name, const bool optional, const Timeline timeline, ReadCb&& cb) -> virt_res_id_t
  {
    const name_id_t nameId = m_ResourcesNames.storeString(name);
    const virt_res_id_t vResId = to_virt_res_id(nameId);

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.reads.push_back({vResId, optional, timeline});
    cb(vResId, node);

    return vResId;
  }

  auto Registry::importTextureProducer(const char* name, TextureProduceFunction producer) -> TextureRequest
  {
    const name_id_t nameId = m_ResourcesNames.storeString(name);
    const virt_res_id_t vResId = to_virt_res_id(nameId);
    const res_id_t resId = to_res_id(vResId);

    auto& vRes = m_VirtResources[vResId];
    vRes.resourceId = resId;
    vRes.createdBy = m_CurrentExecNodeId;
    vRes.modificationChain.insert(0, m_CurrentExecNodeId);
    
    auto& res = m_Resources[resId];
    res = TextureResource{
      .producer = producer
    };

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.creates.push_back(vResId);

    return {vResId};
  }

  auto Registry::modifyTexture(const char* name, const gapi::TextureState state) -> TextureRequest
  {
    return modifyResourceInternal(name, [state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.textureBeginStates.push_back({virt_res_id, state});
    });
  }

  auto Registry::readTexture(const char* name, const gapi::TextureState state, const Timeline timeline) -> TextureRequest
  {
    const bool optional = false;
    const virt_res_id_t vResId = readResourceInternal(name, optional, timeline, [state, timeline](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.textureBeginStates.push_back({virt_res_id, state, timeline});
    });

    return TextureRequest(vResId, timeline);
  }

  auto Registry::readTexture(const char* name, const gapi::TextureState state, const bool optional) -> TextureRequest
  {
    return readResourceInternal(name, optional, Timeline::Current, [state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.textureBeginStates.push_back({virt_res_id, state});
    });
  }

  auto Registry::renameTexture(const char* from, const char* to, const gapi::TextureState state) -> TextureRequest
  {
    const name_id_t nameIdFrom = m_ResourcesNames.storeString(from);
    const name_id_t nameIdTo = m_ResourcesNames.storeString(to);

    const virt_res_id_t vResIdFrom = to_virt_res_id(nameIdFrom);
    const virt_res_id_t vResIdTo = to_virt_res_id(nameIdTo);
    const res_id_t resId = to_res_id(vResIdFrom);

    auto& vResTo = m_VirtResources[vResIdTo];
    auto& vResFrom = m_VirtResources[vResIdFrom];

    if (vResFrom.consumedBy != INVALID_NODE_ID)
    {
      logerror("FG: node `{}` tries to consume resource `{}` that was already consumed by node `{}`.",
        m_NodesNames.getString(to_name_id(m_CurrentExecNodeId)), 
        m_ResourcesNames.getString(to_name_id(vResIdFrom)),
        m_NodesNames.getString(to_name_id(vResFrom.createdBy)));
      return {INVALID_VIRT_RES_ID};
    }
    vResFrom.consumedBy = m_CurrentExecNodeId;
    vResFrom.modificationChain.push_back(m_CurrentExecNodeId);
    
    vResTo.clonnedVResId = vResIdFrom;
    vResTo.createdBy = m_CurrentExecNodeId;
    vResTo.modificationChain.insert(0, m_CurrentExecNodeId);
    vResTo.resourceId = vResFrom.resourceId;

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.modifies.push_back(vResIdFrom);
    node.creates.push_back(vResIdTo);
    node.execState.textureBeginStates.push_back({vResIdFrom, state});

    return {vResIdTo};
  }

  auto Registry::createSampler(const char* name, const gapi::SamplerAllocationDescription& alloc_desc) -> SamplerRequest
  {
    return createResourceInternal(name, SamplerResource{alloc_desc}, [](auto,auto){});
  }

  auto Registry::requestRenderPass() -> RpBuilder
  {
    m_Nodes[m_CurrentExecNodeId].execState.renderPass = {};
    return {*this, m_Nodes[m_CurrentExecNodeId].execState};
  }

  auto Registry::RpBuilder::addTarget(const TextureRequest tex_req, const gapi::LoadOp load, const gapi::StoreOp store) && -> RpBuilder&&
  {
    m_State.renderPass.mrt.push({tex_req.m_Id, load, store});
    return std::move(*this);
  }

  auto Registry::RpBuilder::addTarget(const char* tex_name, const gapi::LoadOp load) && -> RpBuilder&&
  {
    auto tex = m_Registry.modifyTexture(tex_name, gapi::TextureState::RenderTarget);
    return std::move(*this).addTarget(tex, load, gapi::StoreOp::Store);
  }

  auto Registry::RpBuilder::addDepth(const TextureRequest tex_req, 
                                     const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                     const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store) && -> RpBuilder&&
  {
    m_State.renderPass.depth = {tex_req.m_Id, depth_load, depth_store, stencil_load, stencil_store};
    return std::move(*this);
  }

  auto Registry::RpBuilder::addRODepth(const char* tex_name, const gapi::LoadOp load) && -> RpBuilder&&
  {
    auto tex = m_Registry.readTexture(tex_name, gapi::TextureState::DepthReadStencilRead);
    return std::move(*this).addDepth(tex, load, gapi::StoreOp::Store,
                                          gapi::LoadOp::Load, gapi::StoreOp::Store);
  }

  auto Registry::registerNode(const char* name, const char* file, BuildFunction build_cb) -> node_id_t
  {
    const name_id_t nameId = m_NodesNames.storeString(name);
    const node_id_t nodeId = to_node_id(nameId);

    auto& node = m_Nodes[nodeId];
    node.nameId = nameId;
    node.fileSrc = file;
    node.build = build_cb;
    
    return nodeId;
  }

  void Registry::reset()
  {
    m_Resources.clear();
    m_ResourcesNames.clear();
    m_VirtResources.clear();

    for (auto& nodeInfo: m_Nodes)
      nodeInfo.resetResourcesAccess();
  }
}
