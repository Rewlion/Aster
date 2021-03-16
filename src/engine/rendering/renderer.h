#pragma once

#include <engine/rendering/vulkan/core.h>

#include <ecs/BaseSystems.h>

#include <memory>

namespace Vulkan
{
  class Core;
}

class Context;
class Group;
struct GLFWwindow;
struct CameraComponent;

class RenderSystem : public LogicSystem
{
public:
  RenderSystem(Context* ctx, Vulkan::Core& vkCore);

  virtual void Update(const double dt) override;

private:
  void RenderStaticMeshes(CameraComponent* camera, Vulkan::Pipeline* pipeline, Vulkan::UniformsAccessor* uniforms, vk::CommandBuffer& commandBuffer);

  void RenderGBuffer(CameraComponent* camera, Vulkan::RenderGraph* rg);
  void RenderLight(CameraComponent* camera, Vulkan::RenderGraph* rg);

private:
  Vulkan::Core& vkCore;

  Group* cameraGroup;
  Group* staticMeshGroup;
  Group* skyboxGroup;

  std::unique_ptr<Vulkan::ShaderProgram> staticMeshShaderGbufferProgram;
  std::unique_ptr<Vulkan::ShaderProgram> skyBoxShaderProgram;
  std::unique_ptr<Vulkan::ShaderProgram> deferredLightProgram;
};