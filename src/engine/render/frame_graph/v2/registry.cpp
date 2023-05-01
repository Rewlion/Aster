#include "registry.h"

#include <engine/assert.h>
#include <engine/log.h>

namespace fg2
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

  auto Registry::createTexture(const char* name, const gapi::TextureAllocationDescription& desc, const gapi::TextureState init_state) -> TextureRequest
  {
    return createResourceInternal(name, TextureResource{.allocDesc = desc}, [init_state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
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

    m_VirtResources[vResId].modificationChain.push_back(m_CurrentExecNodeId);

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.modifies.push_back(vResId);

    cb(vResId, node);

    return vResId;
  }

  auto Registry::readResourceInternal(const char* name, ReadCb&& cb) -> virt_res_id_t
  {
    const name_id_t nameId = m_ResourcesNames.storeString(name);
    const virt_res_id_t vResId = to_virt_res_id(nameId);

    auto& node = m_Nodes[m_CurrentExecNodeId];
    node.reads.push_back(vResId);
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

  auto Registry::readTexture(const char* name, const gapi::TextureState state) -> TextureRequest
  {
    return readResourceInternal(name, [state](const virt_res_id_t virt_res_id, NodeInfo& node)
    {
      node.execState.textureBeginStates.push_back({virt_res_id, state});
    });
  }

  auto Registry::renameTexture(const char* from, const char* to) -> TextureRequest
  {
    return {INVALID_VIRT_RES_ID};
  }

  auto Registry::requestRenderPass() -> RpBuilder
  {
    m_Nodes[m_CurrentExecNodeId].execState.renderPass = {};
    return {m_Nodes[m_CurrentExecNodeId].execState};
  }

  auto Registry::RpBuilder::addTarget(const TextureRequest tex_req, const gapi::LoadOp load, const gapi::StoreOp store) && -> RpBuilder&&
  {
    m_State.renderPass.mrt.push({tex_req.m_Id, load, store});
    return std::move(*this);
  }

  auto Registry::RpBuilder::addDepth(const TextureRequest tex_req, 
                                     const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                     const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store) && -> RpBuilder&&
  {
    m_State.renderPass.depth = {tex_req.m_Id, depth_load, depth_store, stencil_load, stencil_store};
    return std::move(*this);
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
  }
}
