#pragma once

#include "Shader.h"
#include "common.h"

#include <vulkan/vulkan.hpp>

namespace Vulkan
{
  class VertexInputDeclaration;
  class DepthStencilSettings;

  class Pipeline
  {
  public:
    Pipeline(vk::Device logicalDevice,
      const ShaderProgram& program,
      const VertexInputDeclaration& vertexInputDeclaration,
      const std::vector<vk::DescriptorSetLayout> layouts,
      const vk::PrimitiveTopology topology,
      const DepthStencilSettings& depthStencilSettings,
      const RasterizationMode& rasterMode,
      const vk::Extent2D viewportExtent,
      const vk::RenderPass renderpass,
      const uint32_t subpass,
      const std::vector<vk::PipelineColorBlendAttachmentState>& colorAttachmentBlendStates);

    inline vk::Pipeline GetPipeline() const
    {
      return pipeline.get();
    }

    inline const PipelineUniforms& GetUniformsDescription() const
    {
      return uniformsDescriptions;
    }

    inline vk::PipelineLayout GetLayout() const
    {
      return layout.get();
    }

  private:
    vk::UniquePipeline pipeline;
    vk::UniquePipelineLayout layout;
    PipelineUniforms uniformsDescriptions;
  };
}