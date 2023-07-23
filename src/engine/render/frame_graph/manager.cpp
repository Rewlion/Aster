#include "manager.h"

#include <engine/assert.h>
#include <engine/debug_marks.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/utils.h>

#include <EASTL/vector_set.h>
#include <EASTL/bitvector.h>

namespace fg
{
  auto Manager::registerNode(const char* name, const char* file, BuildFunction build_cb) -> node_id_t
  {
    const node_id_t id = m_Registry.registerNode(name, file, build_cb);
    m_State = State::Compile;
    return id;
  }

  void Manager::execNewFrame()
  {
    ASSERT_FMT(m_State != State::Empty, "framegraph doesn't have nodes to process");

    switch (m_State)
    {
      case State::Compile:
      {
        compile();
        validateResources();
        orderNodes();
        placeBlobs();
        m_State = State::ExecReady;
      }
      case State::ExecReady:
      {
        flushResources();
        execNodes();
        break;
      }
      default:
        ASSERT(!"framegraph doesn't have nodes to process");

      ++m_iFrame;
    }
  }

  void Manager::compile()
  {
    m_Registry.reset();

    for (size_t i = 0; auto& node: m_Registry.m_Nodes)
    {
      m_Registry.m_CurrentExecNodeId = to_node_id(i++);
      node.exec = node.build(m_Registry);
    }
  }

  void Manager::validateResources()
  {
    eastl::vector_set<virt_res_id_t> processedReads, processedModifies, processedCreates;
    eastl::vector<virt_res_id_t> validReads, validModifies, validCreates;

    for (size_t i = 0; auto& node: m_Registry.m_Nodes)
    {
      const node_id_t nodeId = to_node_id(i++);

      const auto validateResourceCreated = [this, nodeId](const char* action, virt_res_id_t virt_res_id)
      {
        auto& vRes = m_Registry.m_VirtResources[virt_res_id];
        if (vRes.createdBy == INVALID_VIRT_RES_ID)
        {
          logerror("FG: node `{}` {} resource `{}` not produced by any node.",
              action,
              m_Registry.m_NodesNames.getString(to_name_id(nodeId)), 
              m_Registry.m_ResourcesNames.getString(to_name_id(virt_res_id)));
          return false;
        }

        return true;
      };

      for (virt_res_id_t vResId: node.reads)
      {
        if (validateResourceCreated("reads", vResId))
          validReads.push_back(vResId);
        processedReads.insert(vResId);
      }

      for (virt_res_id_t vResId: node.modifies)
      {
        if (validateResourceCreated("modifies", vResId))
          validModifies.push_back(vResId);
        
        processedModifies.insert(vResId);
      }

      for (virt_res_id_t vResId: node.creates)
      {
        auto& vRes = m_Registry.m_VirtResources[vResId];
        if (vRes.createdBy != INVALID_VIRT_RES_ID && (vRes.createdBy != nodeId))
        {
          logerror("FG: node `{}` tries to create resource `{}` that was already created by node `{}`.",
            m_Registry.m_NodesNames.getString(to_name_id(nodeId)), 
            m_Registry.m_ResourcesNames.getString(to_name_id(vResId)),
            m_Registry.m_NodesNames.getString(to_name_id(vRes.createdBy)));
        }
        else
          validCreates.push_back(vResId);

        processedCreates.insert(vResId);
      }

      node.reads = validReads;
      node.modifies = validModifies;
      node.creates = validCreates;

      processedReads.clear(); validReads.clear();
      processedModifies.clear(); validModifies.clear();
      processedCreates.clear(); validCreates.clear();
    }
  }

  void Manager::orderNodes()
  {
    m_NodesOrder.clear();
    const node_id_t closingNode = to_node_id(m_Registry.m_NodesNames.getStringId(m_ClosingNode));
    ASSERT_FMT(closingNode != INVALID_NODE_ID,
      "FG: there is no closing node registered. Expected node with name:{}", m_ClosingNode);
    
    const size_t nodesCount = m_Registry.m_Nodes.size();

    m_NodesOrder.reserve(nodesCount);

    const auto dfs = [nodesCount, this](const node_id_t node_id_ref)
    {
      eastl::bitvector visited, processed;
      visited.resize(nodesCount, 0);
      processed.resize(nodesCount, 0);

      node_id_t cycleNodeStart = INVALID_NODE_ID;
      eastl::vector<node_id_t> cycleNodesChain;

      const auto dfsImpl = [&cycleNodesChain, &cycleNodeStart, &visited, &processed, this](const node_id_t node_id, const auto dfs_impl) -> void
      {
        if (visited[node_id])
        {
          if (cycleNodeStart != INVALID_NODE_ID)
          {
            cycleNodesChain.push_back(node_id);
            if (node_id == cycleNodeStart)
            {
              string cycleSubGraph;
              for (const auto& cycleNode: cycleNodesChain)
                cycleSubGraph = fmt::format("{}->{}", cycleSubGraph, m_Registry.m_NodesNames.getString(to_name_id(cycleNode)));
              
              logerror("FG:cycle detected: {}", cycleSubGraph);
              cycleNodeStart = INVALID_NODE_ID;
              cycleNodesChain.clear();
              processed[node_id] = 1;
              return;
            }
          }
          else
          {
            cycleNodeStart = node_id;
            cycleNodesChain.push_back(node_id);
          }
        }
        
        visited[node_id] = 1;

        auto& node = m_Registry.m_Nodes[node_id];

        auto dfsCreation = [&](auto virt_res_id) {
          const auto& vRes = m_Registry.m_VirtResources[virt_res_id];
          const auto& createdBy = vRes.createdBy;
          if (createdBy != INVALID_NODE_ID && createdBy != node_id && processed[createdBy] == 0)
            dfs_impl(createdBy, dfs_impl);
        };

        for (const auto& vResId: node.modifies)
          dfsCreation(vResId);

        for (const auto& vResId: node.reads)
        {
          dfsCreation(vResId);

          const auto& vRes = m_Registry.m_VirtResources[vResId];
          if (vRes.clonnedVResId != INVALID_VIRT_RES_ID)
          {
            fg::virt_res_id_t clonnedFrom = vRes.clonnedVResId;
            while (clonnedFrom != INVALID_VIRT_RES_ID)
            {
              const auto& clonnedVRes = m_Registry.m_VirtResources[clonnedFrom];
              for (const auto prevNode: clonnedVRes.modificationChain)
              if (processed[prevNode] == 0)
                dfs_impl(prevNode, dfs_impl);
              
              clonnedFrom = clonnedVRes.clonnedVResId;
            }
          }

          for (const auto prevNode: vRes.modificationChain)
            if (processed[prevNode] == 0)
              dfs_impl(prevNode, dfs_impl);
        }

        for (const auto& nodeId: node.prevNodes)
        {
          if (processed[nodeId] == 0)
            dfs_impl(nodeId, dfs_impl);
        }

        processed[node_id] = 1;
        m_NodesOrder.push_back(node_id);
      };

      dfsImpl(node_id_ref, dfsImpl);      

      for (int i = 0; i < nodesCount; ++i)
        if (visited[i] == 0)
          logwarn("FG: node `{}` is pruned", m_Registry.m_NodesNames.getString(name_id_t{i}));
    };

    dfs(closingNode);
  }

  void Manager::placeBlobs()
  {
    size_t blobsSize = 0;
    for (node_id_t nodeId: m_NodesOrder)
    {
      for (virt_res_id_t vResId: m_Registry.m_Nodes[nodeId].creates)
      {
        const res_id_t resId = to_res_id(vResId);
        if (auto* blobRes = std::get_if<Registry::BlobResource>(&m_Registry.m_Resources[resId]))
        {
          blobRes->bufferStart = blobsSize;
          blobsSize += blobRes->size;
        }
      }
    }

    m_BlobsSize = blobsSize;
  }

  void Manager::flushResources()
  {
    m_iFrame = (m_iFrame+1) % Engine::Render::FRAMES_COUNT;
    m_ResourceStorages[m_iFrame].reset();
    m_ResourceStorages[m_iFrame].setBlobsStorageSize(m_BlobsSize);
  }

  void Manager::execNodes()
  {
    std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};
    const auto beginRp = [&encoder, &registry = m_Registry, &storage = m_ResourceStorages[m_iFrame]]
      (const Registry::NodeInfo& node)
    {
      const Registry::ExecState::RenderPass& rp = node.execState.renderPass;

      gapi::RenderTargets mrts;
      for (const auto& rt: rp.mrt)
      {
        const auto resId = registry.m_VirtResources[rt.vResId].resourceId;
        const auto [tex, currentState] = storage.getTextureAndCurrentState(resId);

        if (currentState != gapi::TextureState::RenderTarget)
        {
          logerror(
            "FG: Node `{}` requests render pass,\n"
            "but RT texture `{}` has declared state before: `{}`, required: RenderTarget",
            registry.m_NodesNames.getString(to_name_id(registry.m_CurrentExecNodeId)),
            registry.m_ResourcesNames.getString(to_name_id(rt.vResId)),
            gapi::to_string(currentState));
        }

        mrts.push(gapi::RenderPassAttachment{
          .texture = tex,
          .initialState = gapi::TextureState::RenderTarget,
          .finalState = gapi::TextureState::RenderTarget,
          .loadOp = rt.loadOp,
          .storeOp = rt.storeOp
        });
      }

      gapi::RenderPassDepthStencilAttachment depthStencil;
      if (rp.depth.vResId != INVALID_VIRT_RES_ID)
      {
        const auto resId = registry.m_VirtResources[rp.depth.vResId].resourceId;
        const auto [tex, currentState] = storage.getTextureAndCurrentState(resId);

        if (!gapi::is_depth_rp_state(currentState))
        {
          logerror(
            "FG: Node `{}` requests render pass,\n"
            "but depth texture `{}` has declared state before: `{}`,\n"
            "required: Depth[R|W] or Stencil[R|W] or Depth[R|W]Stencil[R|W]",
            registry.m_NodesNames.getString(to_name_id(registry.m_CurrentExecNodeId)),
            registry.m_ResourcesNames.getString(to_name_id(rp.depth.vResId)),
            gapi::to_string(currentState)
          );
        }

        depthStencil = {
          .texture = tex,
          .initialState = currentState,
          .finalState = currentState,
          .depthLoadOp = rp.depth.depthLoadOp,
          .depthStoreOp = rp.depth.depthStoreOp,
          .stencilLoadOp = rp.depth.stencilLoadOp,
          .stencilStoreOp = rp.depth.stencilStoreOp
        };
      }

      encoder->beginRenderpass(mrts, depthStencil);
    };

    {
      GAPI_MARK("framegraph", *encoder);

      for (const node_id_t nodeId: m_NodesOrder)
      {
        GAPI_MARK(m_Registry.m_NodesNames.getString(to_name_id(nodeId)), *encoder);

        auto& node = m_Registry.m_Nodes[nodeId];

        for (auto vResId: node.creates)
        {
          auto& vRes = m_Registry.m_VirtResources[vResId];
          if (vRes.clonnedVResId == INVALID_VIRT_RES_ID)
          {
            auto& res = m_Registry.m_Resources[vRes.resourceId];
            m_ResourceStorages[m_iFrame].create(vRes.resourceId, res);
          }
        }

        for (auto texState: node.execState.textureBeginStates)
        {
          auto& vRes = m_Registry.m_VirtResources[texState.virtResId];
          m_ResourceStorages[m_iFrame].transitTextureState(vRes.resourceId, texState.state, *encoder);
        }

        for (auto bufState: node.execState.bufferBeginStates)
        {
          auto& vRes = m_Registry.m_VirtResources[bufState.virtResId];
          m_ResourceStorages[m_iFrame].transitBufferState(vRes.resourceId, bufState.state, *encoder);
        }

        const bool declaredRp = (node.execState.renderPass.depth.vResId != INVALID_VIRT_RES_ID)
                               || node.execState.renderPass.mrt.empty();
        if (declaredRp)
          beginRp(node);

        node.exec(*encoder);

        if (declaredRp)
          encoder->endRenderpass();
      }
    }

    encoder->flush();
  }

  auto Manager::getTexture(const virt_res_id_t virt_res_id) -> gapi::TextureHandle
  {
    auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return m_ResourceStorages[m_iFrame].getTexture(vRes.resourceId);
    
    return gapi::TextureHandle::Invalid;
  }

  auto Manager::getTextureAllocDescription(const virt_res_id_t virt_res_id)
    -> const gapi::TextureAllocationDescription&
  {
    const auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return std::get<Registry::TextureResource>(
        m_Registry.m_Resources[vRes.resourceId]).allocDesc;
    
    return m_DefaultTexAllocDesc;
  }

  auto Manager::getSampler(const virt_res_id_t virt_res_id) -> gapi::SamplerHandler
  {
    auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return m_ResourceStorages[m_iFrame].getSampler(vRes.resourceId);
    
    return gapi::SamplerHandler::Invalid;
  }

  auto Manager::getSamplerAllocDescription(const virt_res_id_t virt_res_id)
    -> const gapi::SamplerAllocationDescription&
  {
    const auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return std::get<Registry::SamplerResource>(
        m_Registry.m_Resources[vRes.resourceId]).allocDesc;
    
    return m_DefaultSamplerAllocDesc;
  }

  auto Manager::getBuffer(const virt_res_id_t virt_res_id) -> gapi::BufferHandler
  {
    auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return m_ResourceStorages[m_iFrame].getBuffer(vRes.resourceId);
    
    return gapi::BufferHandler::Invalid;
  }

  auto Manager::getBufferAllocDescription(const virt_res_id_t virt_res_id) -> const gapi::BufferAllocationDescription&
  {
    const auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
      return std::get<Registry::BufferResource>(
        m_Registry.m_Resources[vRes.resourceId]).allocDesc;
    
    return m_DefaultBufferAllocDesc;
  }

  auto Manager::getBlob(const virt_res_id_t virt_res_id) -> std::byte*
  {
    auto& vRes = m_Registry.m_VirtResources[virt_res_id];
    if (vRes.resourceId != INVALID_RES_ID)
    {
      auto& blobRes = std::get<Registry::BlobResource>(m_Registry.m_Resources[vRes.resourceId]);
      return m_ResourceStorages[m_iFrame].getBlob(blobRes.bufferStart);
    }
    
    return nullptr;
  }
}
