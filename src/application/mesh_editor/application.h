#pragma once

#include "renderer.h"
#include "scene.h"
#include "camera.h"

#include <memory>

namespace Vulkan
{
  class Core;
}

namespace Rendering
{
  class SceneRenderer;
}

struct GLFWwindow;

namespace Editor
{
  class Application
  {
    enum class Mode
    {
      Layout
    };

    enum class Action
    {
      None,
      RotateScene
    };

  public:
    Application();

    void Start();

    void ProcessKeyInput(int scancode, int action, int mods);
    void ProcessMouseInput(double xpos, double ypos);
    void ProcessMouseButtonInput(int button, int action, int mods);
    void ProcessScrollInput(double xoffset, double yoffset);

  private:
    void CreateDumbCubeObject();

  private:
    GLFWwindow* m_Wnd;
    std::unique_ptr<Vulkan::Core> m_VkCore;
    std::unique_ptr<Rendering::SceneRenderer> m_SceneRenderer;

    Scene m_Scene;
    Camera m_Camera;

    Mode m_CurrentMode;
    Action m_CurrentAction;

    struct {
      double xPos = 0.0f, yPos = 0.0f;
    } m_LastMouseInput;
  };
}