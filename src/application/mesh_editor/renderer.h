#pragma once

#include <engine/rendering/vulkan/core.h>

namespace Vulkan
{
  class Core;
}

namespace Editor
{
  class Scene;
  class Camera;
}

namespace Rendering
{
  class SceneRenderer
  {
  public:
    SceneRenderer(Vulkan::Core* vkCore);

    void Render(Editor::Scene& scene, const Editor::Camera& camera);

  private:
    void RenderEditorWires(Vulkan::FrameContext& ctx, const Editor::Camera& camera);
    void RenderSceneObjects(Vulkan::FrameContext& ctx, const Editor::Camera& camera, Editor::Scene& scene);

    void SyncObjects(Editor::Scene& scene);

  private:
    Vulkan::Core* m_VkCore;

    std::unique_ptr<Vulkan::ShaderProgram> m_SceneLinesProgram;
    std::unique_ptr<Vulkan::ShaderProgram> m_StaticMeshProgram;

    struct
    {
      Vulkan::Buffer vertices;
      uint32_t vertexCount;
    } m_SceneLines;
  };
}