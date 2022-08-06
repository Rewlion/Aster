#pragma once

#include <engine/gapi/resources.h>

#include <EASTL/bitvector.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

#include <functional>
#include <stdint.h>
#include <variant>

namespace gapi
{
  class CmdEncoder;
}

namespace fg
{
  struct TextureResource
  {
    const char* name = nullptr;
    gapi::TextureHandler handler = gapi::TextureHandler::Invalid;
    gapi::TextureState currentState = gapi::TextureState::Undefined;
  };

  class ResourceAccessor
  {
    friend class FrameGraph;
    public:
      ResourceAccessor(eastl::vector_map<string_hash, TextureResource>&& allocated_textures,
                       eastl::vector_map<string_hash, TextureResource*>&& texture_aliases);

      gapi::TextureHandler getTexture(const string_hash name_hash);

    private:
      TextureResource* getTextureResource(const string_hash name_hash);

    private:
      eastl::vector_map<string_hash, TextureResource> m_AllocatedTextures;
      eastl::vector_map<string_hash, TextureResource*> m_TextureAliases;
  };

  using Callback = std::function<void(ResourceAccessor&, gapi::CmdEncoder&)>;

  struct TexturePinDeclaration
  {
    const char* name;
    string_hash nameHash;
    gapi::TextureState state;
  };

  struct TextureCreationDeclaration
  {
    const char* name;
    string_hash nameHash;
    const gapi::TextureAllocationDescription& desc;
  };

  class Node
  {
    friend class FrameGraph;
    friend class FrameGraphBuilder;

    public:
      using Id = uint16_t;
    public:
      Node(const Id id, const char* name);

      Node& createTexture(const char* out_name, gapi::TextureState out_state, const gapi::TextureAllocationDescription& desc);
      Node& readTexture(const char* in_name, gapi::TextureState in_state);
      Node& writeTexture(const char* in_name, gapi::TextureState in_state, const char* out_name);
      Node& setCallback(Callback&& cb);

      inline const eastl::vector<Id>& getParents() const { return m_Parents; }
      inline Id getId() const { return m_Id; }

    private:
      const Id m_Id;
      const char* m_Name;
      Callback m_Cb;

      eastl::vector<TexturePinDeclaration> m_OutputTextures;
      eastl::vector<TexturePinDeclaration> m_InputTextures;
      eastl::vector<TextureCreationDeclaration> m_CreatesTextures;

      struct ResourceAlias
      {
        const char* oldName;
        string_hash oldNameHash;
        const char* newName;
        string_hash newNameHash;
      };
      eastl::vector<ResourceAlias> m_TextureAliases;

      eastl::vector<Id> m_Parents;
  };

  class FrameGraph
  {
    public:
      FrameGraph(ResourceAccessor&& resAccessor, eastl::vector<Node>&& nodes, eastl::vector<Node::Id>&& order);

      void execute(gapi::CmdEncoder* cmd_encoder);

    private:
      void transitNodeResources(const Node& node);

    private:
      ResourceAccessor m_ResourceAccessor;
      eastl::vector<Node> m_Nodes;
      eastl::vector<Node::Id> m_Order;

      gapi::CmdEncoder* m_CmdEncoder;
  };

  class TopologicalSorter
  {
    public:
      TopologicalSorter(eastl::vector<Node>&);
      eastl::vector<Node::Id> sort();
    private:
      void dfs(const size_t id);

    private:
      eastl::vector<Node>& m_Nodes;
      eastl::bitvector<eastl::allocator, uint64_t> m_VisitSet;
      eastl::vector<Node::Id> m_ReverseOrder;

  };

  class FrameGraphBuilder
  {
    public:
      Node& addNode(const char* name)
      {
        const Node::Id id = m_Nodes.size();
        m_Nodes.emplace_back(id, name);
        return m_Nodes.back();
      }

      FrameGraph build();

    private:
      void linkNodes();
      void topologicalSortNodes();
      void allocateResources();
      FrameGraph constructFrameGraph();

    private:
      eastl::vector<Node> m_Nodes;
      eastl::vector<Node::Id> m_SortedOrder;
      eastl::vector_map<string_hash, TextureResource> m_AllocatedTextures;
      eastl::vector_map<string_hash, TextureResource*> m_TextureAliases;
  };
}
