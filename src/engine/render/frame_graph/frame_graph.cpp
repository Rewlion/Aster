#include "frame_graph.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/gapi/gapi.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>

#include <EASTL/hash_map.h>

namespace fg
{
  ResourceAccessor::ResourceAccessor(eastl::vector_map<string_hash, TextureResource>&& allocated_textures,
                       eastl::vector_map<string_hash, TextureResource*>&& texture_aliases)
    : m_AllocatedTextures(std::move(allocated_textures))
    , m_TextureAliases(std::move(texture_aliases))
  {
  }

  gapi::TextureHandler ResourceAccessor::getTexture(const string_hash name_hash)
  {
    const TextureResource* res = getTextureResource(name_hash);
    return res != nullptr ? res->handler : gapi::TextureHandler::Invalid;
  }

  TextureResource* ResourceAccessor::getTextureResource(const string_hash name_hash)
  {
    const auto texIt = m_AllocatedTextures.find(name_hash);
    if (texIt != m_AllocatedTextures.end())
    {
      return &texIt->second;
    }
    else
    {
      const auto aliasIt = m_TextureAliases.find(name_hash);
      return (aliasIt != m_TextureAliases.end()) ?
              aliasIt->second :
              nullptr;
    }
  }

  Node::Node(const Node::Id id, const char* name)
        : m_Id(id)
        , m_Name(name)
  {
  }

  Node& Node::createTexture(const char* out_name, gapi::TextureState out_state, const gapi::TextureAllocationDescription& desc)
  {
    const string_hash nameHash = str_hash(out_name);
    m_OutputTextures.push_back(TexturePinDeclaration{
      .name = out_name,
      .nameHash = nameHash,
      .state = out_state,
    });

    m_CreatesTextures.push_back(TextureCreationDeclaration{
      .name = out_name,
      .nameHash = nameHash,
      .desc = desc
    });

    return *this;
  }

  Node& Node::readTexture(const char* in_name, gapi::TextureState in_state)
  {
    m_InputTextures.push_back(TexturePinDeclaration{
     .name = in_name,
     .nameHash = str_hash(in_name),
     .state = in_state
     });
    return *this;
  }

  Node& Node::writeTexture(const char* in_name, gapi::TextureState in_state, const char* out_name)
  {
    const string_hash inNameHash = str_hash(in_name);
    m_InputTextures.push_back(TexturePinDeclaration{
      .name = in_name,
      .nameHash = inNameHash,
      .state = in_state
    });

    const string_hash outNameHash = str_hash(out_name);
    m_OutputTextures.push_back(TexturePinDeclaration{
      .name = out_name,
      .nameHash = outNameHash,
      .state = gapi::TextureState::Undefined,
    });

    m_TextureAliases.push_back(ResourceAlias{
      .oldName = in_name,
      .oldNameHash = inNameHash,
      .newName = out_name,
      .newNameHash = outNameHash
    });

   return *this;
  }

  Node& Node::setCallback(Callback&& cb)
  {
    m_Cb = cb;
    return *this;
  }

  FrameGraph::FrameGraph(ResourceAccessor&& resAccessor, eastl::vector<Node>&& nodes, eastl::vector<Node::Id>&& order)
    : m_ResourceAccessor(std::move(resAccessor))
    , m_Nodes(std::move(nodes))
    , m_Order(std::move(order))
  {
  }

  void FrameGraph::execute(gapi::CmdEncoder* cmd_encoder)
  {
    m_CmdEncoder = cmd_encoder;
    for (const Node::Id id: m_Order)
    {
      const Node& node = m_Nodes[id];
      transitNodeResources(node);
      node.m_Cb(m_ResourceAccessor, *m_CmdEncoder);
    }
  }

  void FrameGraph::transitNodeResources(const Node& node)
  {
    const auto transitResources = [&](const auto& texturePins) {
      for (const auto& tex: texturePins)
      {
        TextureResource* res = m_ResourceAccessor.getTextureResource(tex.nameHash);
        if (tex.state != res->currentState && tex.state != gapi::TextureState::Undefined)
        {
          m_CmdEncoder->transitTextureState(res->handler, res->currentState, tex.state);
          res->currentState = tex.state;
        }
      }
    };

    transitResources(node.m_InputTextures);
    transitResources(node.m_OutputTextures);
  }

  void FrameGraphBuilder::linkNodes()
  {
    eastl::hash_map<string_hash, Node::Id> inputs;
    eastl::hash_map<string_hash, Node::Id> outputs;

    for (const auto& node: m_Nodes)
    {
      for (const auto& inputTex: node.m_InputTextures)
        inputs.insert({inputTex.nameHash, node.m_Id});

      for (const auto& outputTex: node.m_OutputTextures)
        outputs.insert({outputTex.nameHash, node.m_Id});
    }

    for (auto& node: m_Nodes)
    {
      for (const auto& inputTex: node.m_InputTextures)
      {
        const auto it = outputs.find(inputTex.nameHash);
        if (it != outputs.end())
        {
          node.m_Parents.push_back(it->second);
        }
        else
        {
          ASSERT_FMT(false, "node:{} requests input texture:{} but nobody produces it", node.m_Name, inputTex.name);
        }
      }
    }
  }

  TopologicalSorter::TopologicalSorter(eastl::vector<Node>& nodes)
    : m_Nodes(nodes)
  {
    m_VisitSet.resize(nodes.size(), 0);
    m_ReverseOrder.reserve(nodes.size());
  }

  void TopologicalSorter::dfs(const size_t id)
  {
    m_VisitSet.set(id, true);
    for (const auto& parent: m_Nodes[id].getParents())
    {
      if (m_VisitSet[parent] == false)
        dfs(parent);
    }
    m_ReverseOrder.push_back(id);
  }

  eastl::vector<Node::Id> TopologicalSorter::sort()
  {
    for (const auto& node: m_Nodes)
    {
      const Node::Id id = node.getId();
      if (m_VisitSet[id] == false)
        dfs(id);
    }

    eastl::reverse(m_ReverseOrder.begin(), m_ReverseOrder.end());
    return std::move(m_ReverseOrder);
  }

  void FrameGraphBuilder::topologicalSortNodes()
  {
    TopologicalSorter sorter(m_Nodes);
    m_SortedOrder = sorter.sort();
  };

  void FrameGraphBuilder::allocateResources()
  {
    for (const Node::Id id: m_SortedOrder)
    {
      Node& node = m_Nodes[id];
      for (const auto& createDecl: node.m_CreatesTextures)
      {
        const gapi::TextureHandler h = gapi::allocate_texture(createDecl.desc);
        m_AllocatedTextures.insert({
          createDecl.nameHash,
          TextureResource {
            .name = createDecl.name,
            .handler = h,
            .currentState = gapi::TextureState::Undefined
          }
        });
      }

      for (const auto& alias: node.m_TextureAliases)
      {
        const auto it = m_AllocatedTextures.find(alias.oldNameHash);
        ASSERT_FMT(it != m_AllocatedTextures.end(), "node:{} writes to texture:{} but it wasn't produced", node.m_Name, alias.oldName);
        m_TextureAliases.insert({
          alias.newNameHash,
          &it->second
        });
      }
    }
  }

  FrameGraph FrameGraphBuilder::constructFrameGraph()
  {
    ResourceAccessor resAccessor(std::move(m_AllocatedTextures), std::move(m_TextureAliases));
    return FrameGraph(std::move(resAccessor), std::move(m_Nodes), std::move(m_SortedOrder));
  }

  FrameGraph FrameGraphBuilder::build()
  {
    linkNodes();
    topologicalSortNodes();
    allocateResources();

    FrameGraph fg = constructFrameGraph();
    return fg;
  }
}
