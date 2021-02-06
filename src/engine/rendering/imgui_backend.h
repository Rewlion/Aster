#pragma once

#include <engine/rendering/vulkan/shader.h>
#include <engine/rendering/vulkan/image.h>
#include <engine/rendering/vulkan/buffer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>

#include <functional>
#include <vector>

struct GLFWwindow;

namespace RHI::Vulkan
{
  class Core;

  class RenderGraph;

  class FrameContext;
}

namespace GUI
{
  class ImGuiBackend
  {
    struct FrameResources
    {
      RHI::Vulkan::Buffer vertexBuffer;
      RHI::Vulkan::Buffer indexBuffer;
    };

  public:
    ImGuiBackend(GLFWwindow* wnd, RHI::Vulkan::Core& vkCore);

    void DrawGUI(RHI::Vulkan::FrameContext& ctx, std::function<void()> renderCallback);

  private:
    std::tuple<RHI::Vulkan::Buffer, RHI::Vulkan::Buffer, size_t> GatherDrawBuffers(const ImDrawData* drawData);

  private:
    RHI::Vulkan::Core& vkCore;

    std::unique_ptr<RHI::Vulkan::ShaderProgram> imguiProgram;
    RHI::Vulkan::Image fontTexture;
    std::vector<FrameResources> frameResources;

    std::function<void()> savedRenderCallback;
    uint64_t iFrame;
  };
}
