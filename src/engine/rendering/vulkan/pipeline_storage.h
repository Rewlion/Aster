#pragma once

#include "Shader.h"
#include "pipeline.h"
#include "vertex_input_declaration.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <tuple>
#include <map>

namespace Vulkan
{
  class Core;
  struct FrameContext;

  class PipelineKey
  {
    friend class PipelineStorage;
  public:
    PipelineKey& SetShaderProgramId(const std::string& id);

    PipelineKey& SetVertexInputDeclaration(const VertexInputDeclaration& d);

    PipelineKey& SetTopology(const vk::PrimitiveTopology t);

    PipelineKey& SetDepthStencilSettings(const DepthStencilSettings& s);

    PipelineKey& SetRasterMode(const RasterizationMode& rasterMode);

    PipelineKey& SetViewportExtent(const vk::Extent2D e);

    PipelineKey& SetRenderPass(const vk::RenderPass r);

    PipelineKey& SetSubpassNumber(const uint32_t n);

    PipelineKey& SetColorAttachmentBlendStates(const std::vector<vk::PipelineColorBlendAttachmentState>& bs);

    bool operator<(const PipelineKey& r) const;

  private:
    std::string shaderProgramId;
    VertexInputDeclaration vertexInputDeclaration;
    vk::PrimitiveTopology topology;
    DepthStencilSettings depthStencilSettings;
    RasterizationMode rasterMode;
    vk::Extent2D viewportExtent;
    vk::RenderPass renderpass;
    uint32_t subpass = 0;
    std::vector<vk::PipelineColorBlendAttachmentState> colorAttachmentBlendStates;
  };

  class PipelineStorage
  {
  public:
    PipelineStorage(Core& core);

    Pipeline* GetPipeline(const ShaderProgram& program, 
                          const VertexInputDeclaration& vertexInputDeclaration, 
                          vk::PrimitiveTopology topology, 
                          const DepthStencilSettings& depthStencilSettings,
                          const RasterizationMode& rasterMode,
                          const vk::Extent2D& viewportExtent, 
                          vk::RenderPass renderPass, 
                          uint32_t subpassNumber, 
                          const std::vector<vk::PipelineColorBlendAttachmentState>& outputAttachmentBlendStates);

    Pipeline* GetPipeline(const ShaderProgram& program, 
                          const VertexInputDeclaration& vertexInputDeclaration, 
                          vk::PrimitiveTopology topology, 
                          const DepthStencilSettings& depthStencilSettings,
                          const RasterizationMode& rasterMode,
                          const FrameContext& frameContext);

  private:
    Core& core;

    std::map<PipelineKey, std::unique_ptr<Pipeline>> storage;
  };
}