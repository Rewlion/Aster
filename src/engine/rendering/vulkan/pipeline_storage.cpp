#include "pipeline_storage.h"
#include "core.h"
#include "framecontext.h"

namespace vk
{
  bool operator<(const vk::Extent2D& l, const vk::Extent2D& r)
  {
    return std::tie(l.height, l.width) < std::tie(r.height, r.width);
  }

  bool operator<(const vk::PipelineColorBlendAttachmentState& l, const vk::PipelineColorBlendAttachmentState& r)
  {
    return std::tie(l.alphaBlendOp, l.blendEnable, l.colorBlendOp, l.colorWriteMask, l.dstAlphaBlendFactor, l.dstColorBlendFactor, l.srcAlphaBlendFactor, l.srcColorBlendFactor) <
           std::tie(r.alphaBlendOp, r.blendEnable, r.colorBlendOp, r.colorWriteMask, r.dstAlphaBlendFactor, r.dstColorBlendFactor, r.srcAlphaBlendFactor, r.srcColorBlendFactor);
  }
}

namespace Vulkan
{
  PipelineKey& PipelineKey::SetShaderProgramId(const std::string& id)
  {
    shaderProgramId = id;
    return *this;
  }

  PipelineKey& PipelineKey::SetVertexInputDeclaration(const VertexInputDeclaration& d)
  {
    vertexInputDeclaration = d;
    return *this;
  }

  PipelineKey& PipelineKey::SetTopology(const vk::PrimitiveTopology t)
  {
    topology = t;
    return *this;
  }

  PipelineKey& PipelineKey::SetDepthStencilSettings(const DepthStencilSettings& s)
  {
    depthStencilSettings = s;
    return *this;
  }

  PipelineKey& PipelineKey::SetRasterMode(const RasterizationMode& rm)
  {
    rasterMode = rm;
    return *this;
  }

  PipelineKey& PipelineKey::SetViewportExtent(const vk::Extent2D e)
  {
    viewportExtent = e;
    return *this;
  }

  PipelineKey& PipelineKey::SetRenderPass(const vk::RenderPass r)
  {
    renderpass = r;
    return *this;
  }

  PipelineKey& PipelineKey::SetSubpassNumber(const uint32_t n)
  {
    subpass = n;
    return *this;
  }

  PipelineKey& PipelineKey::SetColorAttachmentBlendStates(const std::vector<vk::PipelineColorBlendAttachmentState>& bs)
  {
    colorAttachmentBlendStates = bs;
    return *this;
  }

  bool PipelineKey::operator<(const PipelineKey& r) const
  {
    return std::tie(shaderProgramId, vertexInputDeclaration, topology, depthStencilSettings, rasterMode, viewportExtent, renderpass, subpass, colorAttachmentBlendStates) <
      std::tie(r.shaderProgramId, r.vertexInputDeclaration, r.topology, r.depthStencilSettings, r.rasterMode, r.viewportExtent, r.renderpass, r.subpass, r.colorAttachmentBlendStates);
  }

  PipelineStorage::PipelineStorage(Core& core)
    : core(core)
  {
  }

  Pipeline* PipelineStorage::GetPipeline(const ShaderProgram& program,
    const VertexInputDeclaration& vertexInputDeclaration,
    vk::PrimitiveTopology topology,
    const DepthStencilSettings& depthStencilSettings,
    const RasterizationMode& rasterMode,
    const vk::Extent2D& viewportExtent,
    vk::RenderPass renderPass,
    uint32_t subpassNumber,
    const std::vector<vk::PipelineColorBlendAttachmentState>& outputAttachmentBlendStates)
  {
    const auto key = PipelineKey()
      .SetShaderProgramId(program.GetID())
      .SetVertexInputDeclaration(vertexInputDeclaration)
      .SetTopology(topology)
      .SetDepthStencilSettings(depthStencilSettings)
      .SetRasterMode(rasterMode)
      .SetViewportExtent(viewportExtent)
      .SetRenderPass(renderPass)
      .SetSubpassNumber(subpassNumber)
      .SetColorAttachmentBlendStates(outputAttachmentBlendStates);

    if (storage.find(key) != storage.end())
      return storage[key].get();

    const std::vector<vk::DescriptorSetLayout>& layouts = program.GetLayouts();

    std::unique_ptr<Pipeline> pp = std::make_unique<Pipeline>(core.GetLogicalDevice(), program, vertexInputDeclaration, layouts, topology, depthStencilSettings, rasterMode, viewportExtent, renderPass, subpassNumber, outputAttachmentBlendStates);
    Pipeline* pipeline = pp.get();
    storage[key] = std::move(pp);

    return pipeline;
  }

  Pipeline* PipelineStorage::GetPipeline(const ShaderProgram& program, 
                                         const VertexInputDeclaration& vertexInputDeclaration,
                                         vk::PrimitiveTopology topology,
                                         const DepthStencilSettings& depthStencilSettings,
                                         const RasterizationMode& rasterMode,
                                         const FrameContext& frameContext)
  {
    return GetPipeline(program,
                       vertexInputDeclaration,
                       topology,
                       depthStencilSettings,
                       rasterMode,
                       frameContext.BackbufferSize,
                       frameContext.renderPass,
                       frameContext.subpassNumber,
                       frameContext.outputAttachmentBlendStates);
  }
}