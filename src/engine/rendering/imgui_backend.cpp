#include "imgui_backend.h"

#include <engine/rendering/vulkan/core.h>
#include <engine/rendering/vulkan/fileutils.h>
#include <engine/rendering/vulkan/framegraph.h>

#include <glm/glm.hpp>

namespace
{
  Vulkan::VertexInputDeclaration GetImGuiVID()
  {
    Vulkan::VertexInputDeclaration vid;

    vid.AddBindingDescription(0, sizeof(ImDrawVert));

    vid.AddAttributeDescription(vk::Format::eR32G32Sfloat, 0, 0, offsetof(ImDrawVert, pos));
    vid.AddAttributeDescription(vk::Format::eR32G32Sfloat, 0, 1, offsetof(ImDrawVert, uv));
    vid.AddAttributeDescription(vk::Format::eR8G8B8A8Unorm, 0, 2, offsetof(ImDrawVert, col));

    return vid;
  }

  struct ImGuiConstants
  {
    glm::vec2 scale;
    glm::vec2 translate;
  };

  ImGuiConstants GetImGuiScaleTranslate(const ImDrawData* drawData)
  {
    glm::vec2 scale{
           2.0f / drawData->DisplaySize.x,
           2.0f / drawData->DisplaySize.y
    };

    glm::vec2 translate{
      -1.0f - drawData->DisplayPos.x * scale.x,
      -1.0f - drawData->DisplayPos.y * scale.y
    };

    return ImGuiConstants{
      scale,
      translate
    };
  }
}

namespace GUI
{
  ImGuiBackend::ImGuiBackend(GLFWwindow* wnd, Vulkan::Core& vkCore)
    : vkCore(vkCore)
    , iFrame(0)
  {
    //imgui shaders
    auto vertexShader = vkCore.CreateShader(Vulkan::ReadFile("../data/shaders/spirv/imgui/shader.vert.spv"));
    auto fragmentShader = vkCore.CreateShader(Vulkan::ReadFile("../data/shaders/spirv/imgui/shader.frag.spv"));
    imguiProgram = std::make_unique< Vulkan::ShaderProgram>(vkCore, std::move(vertexShader), std::move(fragmentShader));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForVulkan(wnd, true);
    ImGui::StyleColorsDark();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    fontTexture = vkCore.Allocate2DImage((void*)pixels, width * height * 4, vk::Format::eR8G8B8A8Unorm, { (uint32_t)width, (uint32_t)height }, vk::ImageUsageFlagBits::eSampled);

    frameResources.resize(vkCore.GetSwapchainImagesCount());
  }

  std::tuple<Vulkan::Buffer, Vulkan::Buffer, size_t> ImGuiBackend::GatherDrawBuffers(const ImDrawData* drawData)
  {
    std::vector<ImDrawVert> vertices;
    vertices.reserve(drawData->TotalVtxCount);
    std::vector<uint32_t> indices;
    vertices.reserve(drawData->TotalIdxCount);

    for (int i = 0; i < drawData->CmdListsCount; ++i)
    {
      ImDrawList* drawCmd = drawData->CmdLists[i];

      for (auto& v : drawCmd->VtxBuffer)
        vertices.push_back(v);

      for (auto idx : drawCmd->IdxBuffer)
        indices.push_back(idx);
    }

    return {
      vkCore.AllocateDeviceBuffer(vertices.data(), vertices.size() * sizeof(ImDrawVert), vk::BufferUsageFlagBits::eVertexBuffer),
      vkCore.AllocateDeviceBuffer(indices.data(), indices.size() * sizeof(uint32_t), vk::BufferUsageFlagBits::eIndexBuffer),
      vertices.size()
    };
  }

  void ImGuiBackend::DrawGUI(Vulkan::FrameContext& ctx, std::function<void()> renderCallback)
  {
    savedRenderCallback = renderCallback;

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    savedRenderCallback();
    ImGui::Render();

    ImDrawData* drawData = ImGui::GetDrawData();
    if (drawData == nullptr || drawData->TotalVtxCount <= 0)
      return;

    FrameResources& fResources = frameResources[iFrame % frameResources.size()];

    auto [vertices, indices, vertexCount] = GatherDrawBuffers(drawData);

    fResources.vertexBuffer = std::move(vertices);
    fResources.indexBuffer = std::move(indices);

    Vulkan::Pipeline* pipeline = ctx.GetPipeline(*imguiProgram, GetImGuiVID(), vk::PrimitiveTopology::eTriangleList, Vulkan::DisableDepthTest, Vulkan::FillMode);
    ctx.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

    Vulkan::UniformsAccessor* uniforms = ctx.GetUniformsAccessor(*imguiProgram);
    uniforms->SetSampler2D("sTexture", fontTexture);

    ImGuiConstants constants = GetImGuiScaleTranslate(drawData);
    uniforms->SetUniformBuffer("Constants", &constants);

    std::vector<vk::DescriptorSet> descriptorSets = uniforms->GetUpdatedDescriptorSets();
    ctx.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->GetLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);

    vk::DeviceSize offset = 0;
    ctx.commandBuffer.bindVertexBuffers(0, 1, &fResources.vertexBuffer.GetBuffer(), &offset);
    ctx.commandBuffer.bindIndexBuffer(fResources.indexBuffer.GetBuffer(), 0, vk::IndexType::eUint32);

    uint32_t indexOffset = 0;
    uint32_t vertexOffset = 0;
    for (int nCmdList = 0; nCmdList < drawData->CmdListsCount; ++nCmdList)
    {
      ImDrawList* cmdList = drawData->CmdLists[nCmdList];
      for (int nCmd = 0; nCmd < cmdList->CmdBuffer.size(); ++nCmd)
      {
        const ImDrawCmd& cmd = cmdList->CmdBuffer[nCmd];
        ctx.commandBuffer.drawIndexed(cmd.ElemCount, 1, cmd.IdxOffset + indexOffset, cmd.VtxOffset + vertexOffset, 0);
      }

      indexOffset += cmdList->IdxBuffer.size();
      vertexOffset += cmdList->VtxBuffer.size();
    }

    ++iFrame;
  }
}