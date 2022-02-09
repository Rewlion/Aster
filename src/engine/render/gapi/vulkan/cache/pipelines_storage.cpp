#include "pipelines_storage.h"

#include <engine/log.h>
#include <engine/render/gapi/resources.h>
#include <engine/render/gapi/vulkan/device.h>
#include <engine/render/gapi/vulkan/gapi_to_vk.h>
#include <engine/render/gapi/vulkan/traits.h>

#include <boost/functional/hash.hpp>

namespace boost
{
  template<>
  inline void hash_combine<vk::RenderPass>(std::size_t& seed, vk::RenderPass const& v)
  {
    VkRenderPass rp = static_cast<VkRenderPass>(v);
    uint64_t val = reinterpret_cast<uint64_t>(rp);
    hash_combine(seed, val);
  }
}

namespace gapi::vulkan
{
  void PipelinesStorage::Init(Device* device)
  {
    m_Device = device;
    m_ShadersStorage.Init(device);
    m_LayoutsStorage.Init(device);
  }

  vk::Pipeline PipelinesStorage::GetPipeline(const GraphicsPipelineDescription& description, const vk::RenderPass rp, const size_t subpass)
  {
    Utils::FixedStack<vk::PipelineShaderStageCreateInfo, traits::graphics::LAST_SHADER_ID>
      stages;
    const ShaderModule* vertexShaderModule = nullptr;

    for (const string_hash& shaderName: description.shaderNames)
    {
      const ShaderModule& sm = m_ShadersStorage.GetShaderModule(shaderName);
      vk::PipelineShaderStageCreateInfo stage;
      stage.stage = sm.metadata.m_Stage;
      stage.module = sm.module.get();
      stage.pName = sm.metadata.m_EntryPoint.c_str();
      stages.Push(stage);

      if (sm.metadata.m_Stage == vk::ShaderStageFlagBits::eVertex)
        vertexShaderModule = &sm;
    }
    vk::PipelineLayoutCreateInfo layoutCi;
    layoutCi.pushConstantRangeCount = vertexShaderModule->metadata.m_PushConstantsSize == 0 ? 0 : 1;
    vk::PushConstantRange pushConstant;
    pushConstant.offset = 0;
    pushConstant.size = vertexShaderModule->metadata.m_PushConstantsSize;
    layoutCi.pPushConstantRanges = &pushConstant;

    using boost::hash_combine;
    size_t pipelineHash = description.hash();
    hash_combine(pipelineHash, rp);
    hash_combine(pipelineHash, subpass);

    const auto it = m_Pipelines.find(pipelineHash);
    if (it != m_Pipelines.end())
      return it->second.get();

    vk::GraphicsPipelineCreateInfo ci;

    vk::VertexInputBindingDescription bindingDesc;
    bindingDesc.binding = 0;
    bindingDesc.stride = vertexShaderModule->metadata.m_VertexStride;
    bindingDesc.inputRate = vk::VertexInputRate::eVertex;

    vk::PipelineVertexInputStateCreateInfo inputStateCi;
    inputStateCi.vertexBindingDescriptionCount = vertexShaderModule->metadata.m_HasInput ? 1 : 0;
    inputStateCi.pVertexBindingDescriptions = &bindingDesc;
    inputStateCi.vertexAttributeDescriptionCount = vertexShaderModule->metadata.m_VertexAttributesCount;
    inputStateCi.pVertexAttributeDescriptions = vertexShaderModule->metadata.m_VertexAttributeDescriptions;

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCi;
    inputAssemblyCi.topology = GetPrimitiveTopology(description.topology);

    vk::PipelineViewportStateCreateInfo viewportCi;
    viewportCi.scissorCount = 1;
    viewportCi.viewportCount = 1;
    viewportCi.pViewports = nullptr; //dynamic
    viewportCi.pScissors = nullptr; //dynamic

    vk::PipelineRasterizationStateCreateInfo rasterizationStateCi;
    rasterizationStateCi.depthClampEnable = false;
    rasterizationStateCi.polygonMode = vk::PolygonMode::eFill;
    rasterizationStateCi.cullMode = vk::CullModeFlagBits::eFront;
    rasterizationStateCi.frontFace = vk::FrontFace::eClockwise;
    rasterizationStateCi.depthBiasEnable = false;
    rasterizationStateCi.lineWidth = 1.0f;

    vk::PipelineMultisampleStateCreateInfo multisampleCi;
    multisampleCi.sampleShadingEnable = false;
    multisampleCi.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineDepthStencilStateCreateInfo depthStencilCi = GetDepthStencilState(description.depthStencilState);

    vk::PipelineColorBlendAttachmentState attachmentStates[MAX_RENDER_TARGETS];
    vk::PipelineColorBlendStateCreateInfo blendingCi = GetBlendState(description.blendState, attachmentStates);

    vk::PipelineDynamicStateCreateInfo dynamicStateCi;
    vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    dynamicStateCi.pDynamicStates = dynamicStates;
    dynamicStateCi.dynamicStateCount = std::size(dynamicStates);

    ci.stageCount = stages.GetSize();
    ci.pStages = stages.GetData();
    ci.pVertexInputState = &inputStateCi;
    ci.pInputAssemblyState = &inputAssemblyCi;
    ci.pTessellationState = nullptr;
    ci.pViewportState = &viewportCi;
    ci.pRasterizationState = &rasterizationStateCi;
    ci.pMultisampleState = &multisampleCi;
    ci.pDepthStencilState = &depthStencilCi;
    ci.pColorBlendState = &blendingCi;
    ci.pDynamicState = &dynamicStateCi;
    ci.layout = m_LayoutsStorage.GetPipelineLayout(layoutCi);
    ci.renderPass = rp;
    ci.subpass = subpass;

    auto result = m_Device->m_Device->createGraphicsPipelineUnique({}, ci);
    if (result.result != vk::Result::eSuccess)
      logerror("failed to create graphics pipeline");

    vk::Pipeline pipeline = result.value.get();

    m_Pipelines.insert({
      pipelineHash,
      std::move(result.value)
    });

    return pipeline;
  }
}
