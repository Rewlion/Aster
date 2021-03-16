#pragma once

#include <engine/rendering/vulkan/shader.h>
#include <engine/rendering/vulkan/image.h>
#include <engine/rendering/vulkan/buffer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>

#include <functional>
#include <vector>

struct GLFWwindow;

namespace Vulkan
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
      Vulkan::Buffer vertexBuffer;
      Vulkan::Buffer indexBuffer;
    };

  public:
    ImGuiBackend(GLFWwindow* wnd, Vulkan::Core& vkCore);

    void DrawGUI(Vulkan::FrameContext& ctx, std::function<void()> renderCallback);

  private:
    std::tuple<Vulkan::Buffer, Vulkan::Buffer, size_t> GatherDrawBuffers(const ImDrawData* drawData);

  private:
    Vulkan::Core& vkCore;

    std::unique_ptr<Vulkan::ShaderProgram> imguiProgram;
    Vulkan::Image fontTexture;
    std::vector<FrameResources> frameResources;

    std::function<void()> savedRenderCallback;
    uint64_t iFrame;
  };
}
