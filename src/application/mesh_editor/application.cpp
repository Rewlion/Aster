#include "application.h"

#include <engine/rendering/vulkan/core.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace
{
  void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    auto* handler = reinterpret_cast<Editor::Application*>(glfwGetWindowUserPointer(window));
    handler->ProcessKeyInput(scancode, action, mods);
  }

  void GLFW_CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
  {
    auto* handler = reinterpret_cast<Editor::Application*>(glfwGetWindowUserPointer(window));
    handler->ProcessMouseInput(xpos, ypos);
  }

  void GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
  {
    auto* handler = reinterpret_cast<Editor::Application*>(glfwGetWindowUserPointer(window));
    handler->ProcessMouseButtonInput(button, action, mods);
  }

  void GLFW_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
  {
    auto* handler = reinterpret_cast<Editor::Application*>(glfwGetWindowUserPointer(window));
    handler->ProcessScrollInput(xoffset, yoffset);
  }
}

namespace Editor
{
  Application::Application()
  {
    const int wndHeight = 1080;
    const int wndWidth = 1024;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Wnd = glfwCreateWindow(wndWidth, wndHeight, "Vulkan window", nullptr, nullptr);

    glfwSetWindowUserPointer(m_Wnd, this);
    glfwSetKeyCallback(m_Wnd, GLFW_KeyCallback);
    glfwSetCursorPosCallback(m_Wnd, GLFW_CursorPositionCallback);
    glfwSetMouseButtonCallback(m_Wnd, GLFW_MouseButtonCallback);
    glfwSetScrollCallback(m_Wnd, GLFW_ScrollCallback);

    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    m_VkCore = std::make_unique<Vulkan::Core>( m_Wnd, extensions, count, vk::Extent2D{ wndWidth, wndHeight } );

    m_SceneRenderer = std::make_unique<Rendering::SceneRenderer>(m_VkCore.get());

    m_Camera = Camera{ 5.0f, -90.0f, 120.0f, {wndWidth, wndHeight} };

    m_CurrentMode = Mode::Layout;
    
    m_CurrentAction = Action::None;
  }

  void Application::Start()
  {
    CreateDumbCubeObject();

    while (!glfwWindowShouldClose(m_Wnd))
    {
      glfwPollEvents();
      m_SceneRenderer->Render(m_Scene, m_Camera);
    }
  }

  void Application::ProcessKeyInput(int scancode, int action, int mods)
  {
   
  }

  void Application::ProcessMouseInput(double xpos, double ypos)
  {
    static bool isInitialized = false;
    if (!isInitialized)
    {
      //to prevent `wtf` rotation after a first input, since lastX,lastY = 0.0f at the start.
      m_LastMouseInput.xPos = xpos;
      m_LastMouseInput.yPos = ypos;
      isInitialized = true;
      return;
    }

    const double dx = xpos - m_LastMouseInput.xPos;
    const double dy = ypos - m_LastMouseInput.yPos;

    m_LastMouseInput.xPos = xpos;
    m_LastMouseInput.yPos = ypos;

    if (m_CurrentAction == Action::RotateScene)
      m_Camera.Rotate(dx, dy);
  }

  void Application::ProcessMouseButtonInput(int button, int action, int mods)
  {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
      if (action == GLFW_PRESS && m_CurrentAction == Action::None)
        m_CurrentAction = Action::RotateScene;
      else if (action == GLFW_RELEASE && m_CurrentAction == Action::RotateScene)
        m_CurrentAction = Action::None;
    }
  }

  void Application::ProcessScrollInput(double xoffset, double yoffset)
  {
    m_Camera.AddRadius(-yoffset * 0.3f);
  }

  void Application::CreateDumbCubeObject()
  {
    Scene::Object& cubeObj = m_Scene.AddNewObject();

    const std::vector<glm::vec3> vertices{
      glm::vec3{-1,1,-1},
      glm::vec3{-1,-1,-1},
      glm::vec3{1,-1,-1},
      glm::vec3{1,1,-1},
      glm::vec3{1,1,1},
      glm::vec3{1,-1,1},
      glm::vec3{-1,-1,1},
      glm::vec3{-1,1,1}
    };

    const std::vector<std::vector<uint32_t>> polygons{
      {0,1,2},
      {2,3,0},
      {3,2,5},
      {5,4,3},
      {4,5,6},
      {6,7,4},
      {7,6,1},
      {1,0,7},
      {1,6,5},
      {5,2,1},
      {0,3,4},
      {4,7,0}
    };

    HalfedgeMesh& mesh = cubeObj.GetMesh();
    std::string err = mesh.ConstructFromPolygons(polygons, vertices);

    if (err != "")
      std::printf("Failed to construct an initial cube: %s\n", err.c_str());
  }
}