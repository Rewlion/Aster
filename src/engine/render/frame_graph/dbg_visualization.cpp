#include "manager.h"

#include <engine/assert.h>
#include <engine/console/cmd.h>
#include <engine/render/imgui/imgui.h>

#include <EASTL/bitvector.h>
#include <EASTL/vector_set.h>
#include <EASTL/vector_map.h>
#include <imgui-node-editor/imgui_node_editor.h>
namespace ed = ax::NodeEditor;

#include <ranges>

namespace fg
{

  static bool show_wnd = false;

  static
  void fg_show(eastl::span<string_view>)
  {
    show_wnd = !show_wnd;
  }
  CONSOLE_CMD("fg.show", 0, 0, fg_show);

  void Manager::imGuiDrawFramegraph()
  {
    size_t pinReserveId = 1;
    if (m_Registry.m_Nodes.empty())
      return;

    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

    constexpr bool OUTPUT = false;
    constexpr bool INPUT = true;
    constexpr bool PREV = false;
    constexpr bool NEXT = true;

    struct Edge
    {
      ed::PinId from, to;
    };
    eastl::vector<Edge> edges;

    struct NodeTreeData
    {
      const char* name;
      int column = 0;
      int row = 0;

      struct ResourcePins
      {
        ed::PinId input, output;

        ResourcePins() = default;
        ResourcePins(const bool is_input, ed::PinId reserve_id)
        {
          getId(is_input) = reserve_id;
        }
        
        ed::PinId getPin(const bool is_input, size_t* reserve_id)
        {
          ed::PinId& id = getId(is_input);
          if (!id)
          {
            id = (*reserve_id)++;
            return id;
          }
          return id;
        }

      private:
        ed::PinId& getId(const bool is_input)
        {
          return is_input ? input : output;
        }
      };

      ed::PinId getResourcePin(const virt_res_id_t virt_res_id, const bool is_input, size_t* reserve_id)
      {
        auto it = resources.find(virt_res_id);
        ResourcePins* resPins = nullptr;
        if (it != resources.end())
          return it->second.getPin(is_input, reserve_id);
        else
        {
          ed::PinId id = (*reserve_id)++;
          resources.insert({
            virt_res_id,
            ResourcePins{is_input, id}
          });
          return id;
        }
      }

      ed::PinId getNodePin(const bool is_input, size_t* reserve_id)
      {
        ed::PinId& id = is_input ? inputPin : outputPin;
        if (!id)
          id = (*reserve_id)++;
        return id;
      }

      ed::PinId inputPin, outputPin;
      eastl::vector_set<node_id_t> followingNodes;
      eastl::vector_map<virt_res_id_t, ResourcePins> resources;
    };

    const auto getModifier = [](const bool is_next, const node_id_t current_modifier, const auto& modification_chain)
    {
      if (
          (is_next && (current_modifier == modification_chain.back())) ||
          (!is_next && (current_modifier == modification_chain[0]))
        )
        return INVALID_NODE_ID;

      const size_t modifiersCount = modification_chain.size();
      for (size_t i = 0; i < modifiersCount; ++i)
      {
        if (modification_chain[i] == current_modifier)
          return modification_chain[is_next ? i+1 : i-1];
      }

      ASSERT_FMT(false, "node {} is not inside modification chain", (size_t)current_modifier);
      return INVALID_NODE_ID;
    };

    eastl::vector<NodeTreeData> nodesTree;
    nodesTree.resize(m_Registry.m_Nodes.size());

    const int nodesCount = nodesTree.size();
    for (int i = 0; i < nodesCount; ++i)
      nodesTree[i].name = m_Registry.m_NodesNames.getString(name_id_t{i});
    
    for (const node_id_t nodeId: m_NodesOrder)
    {
      const auto& node = m_Registry.m_Nodes[nodeId];
      auto& pins = nodesTree[(size_t)nodeId];

      for (const node_id_t prevNodeId: node.prevNodes)
      {
        nodesTree[(size_t)prevNodeId].followingNodes.insert(nodeId);
        const ed::PinId inputPin = nodesTree[(size_t)nodeId].getNodePin(INPUT, &pinReserveId);
        const ed::PinId outputPin = nodesTree[(size_t)prevNodeId].getNodePin(OUTPUT, &pinReserveId);
        edges.push_back({.from = outputPin, .to = inputPin});
      }

      for (const virt_res_id_t vResId: node.reads)
      {
        const node_id_t prevNodeId = m_Registry.m_VirtResources[vResId].modificationChain.back();
        nodesTree[(size_t)prevNodeId].followingNodes.insert(nodeId);

        const ed::PinId inputPin = nodesTree[(size_t)nodeId].getResourcePin(vResId, INPUT, &pinReserveId);
        const ed::PinId outputPin = nodesTree[(size_t)prevNodeId].getResourcePin(vResId, OUTPUT, &pinReserveId);
        edges.push_back({.from = outputPin, .to = inputPin});
      }

      for (const virt_res_id_t vResId: node.creates)
      {
        const auto& mChain = m_Registry.m_VirtResources[vResId].modificationChain;
        if (mChain.size() > 1)
          nodesTree[(size_t)nodeId].followingNodes.insert(mChain[1]);
      }

      for (const virt_res_id_t vResId: node.modifies)
      {
        const node_id_t nextNodeId = getModifier(NEXT, nodeId, m_Registry.m_VirtResources[vResId].modificationChain);
        if (nextNodeId != INVALID_NODE_ID)
          pins.followingNodes.insert(nextNodeId);
        
        node_id_t prevNodeId = getModifier(PREV, nodeId, m_Registry.m_VirtResources[vResId].modificationChain);

        const ed::PinId inputPin = nodesTree[(size_t)nodeId].getResourcePin(vResId, INPUT, &pinReserveId);
        const ed::PinId outputPin = nodesTree[(size_t)prevNodeId].getResourcePin(vResId, OUTPUT, &pinReserveId);
        edges.push_back({.from = outputPin, .to = inputPin});
      }

      for (const virt_res_id_t vResId: node.creates)
        nodesTree[(size_t)nodeId].getResourcePin(vResId, OUTPUT, &pinReserveId);
    }

    for (node_id_t nodeId: m_NodesOrder)
    {
      const auto createPins = [](const ed::PinId input, const ed::PinId output, const char* name){
        if (input)
        {
          ed::BeginPin(input, ed::PinKind::Input);
            ImGui::Text("%s", "o");
          ed::EndPin();
          ImGui::SameLine(); 
        }      
        ImGui::Dummy(ImVec2(20, 0)); ImGui::SameLine();
        ImGui::Text("%s", name); ImGui::SameLine(); ImGui::Dummy(ImVec2(20, 0));
        if (output)
        {
          ImGui::SameLine();
          ed::BeginPin(output, ed::PinKind::Output);
            ImGui::Text("o");
          ed::EndPin();
        }
      };

      const char* nodeName = m_Registry.m_NodesNames.getString(to_name_id(nodeId));
      ed::BeginNode(ed::NodeId{nodeId});
        const auto& node = nodesTree[(size_t)nodeId];
        createPins(node.inputPin, node.outputPin, nodeName);

        ImGui::Dummy(ImVec2(0, 20)); 

        for (const auto& [vResId, resPins]: node.resources)
          createPins(resPins.input, resPins.output, m_Registry.m_ResourcesNames.getString(to_name_id(vResId)));
      ed::EndNode();
    }

    eastl::vector<size_t> nodeIdToColumn;
    eastl::vector<size_t> columnToLastRow;
    nodeIdToColumn.reserve(m_Registry.m_Nodes.size());
    columnToLastRow.resize(m_NodesOrder.size(), 0);
    constexpr int2 nodeOffset = {300, 300};

    eastl::bitvector visitedNodes;
    visitedNodes.resize(m_Registry.m_Nodes.size(), 0);

    size_t currentExecColumn = 0;
    for (const node_id_t nodeId: m_NodesOrder)
    {
      auto& pins = nodesTree[(size_t)nodeId];
      if (!visitedNodes[(size_t)nodeId] && pins.column == 0)
        pins.column = currentExecColumn;

      pins.row = columnToLastRow[pins.column]++;

      const size_t nodesCount = m_NodesOrder.size();

      float xOffset = pins.column * nodeOffset.x;
      float yOffset = -pins.row * nodeOffset.y;
      ed::SetNodePosition(ed::NodeId{nodeId}, {xOffset, yOffset} );

      for (const node_id_t follower: pins.followingNodes)
        if (!visitedNodes[(size_t)follower])
          nodesTree[(size_t)follower].column = pins.column+1;

      visitedNodes[nodeId] = 1;
      if (pins.column > currentExecColumn)
        currentExecColumn = pins.column;
    }

    size_t linkId = 1;
    for (const auto& edge: edges)
      ed::Link(linkId++, edge.from, edge.to);

    ed::End();
  }

  void imgui_draw_framegraph()
  {
    if (!show_wnd)
      return;

    ImGui::Begin("framegraph", nullptr, 0);
    extern auto get_manager() -> Manager*;
    if (auto* manager = get_manager())
      manager->imGuiDrawFramegraph();
    ImGui::End();
  }

IMGUI_REG_WND(imgui_draw_framegraph);

}
