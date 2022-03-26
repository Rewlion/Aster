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
  void PipelinesStorage::init(Device* device)
  {
    m_Device = device;
    m_ShadersStorage.init(device);
  }

  vk::Pipeline PipelinesStorage::getPipeline(const GraphicsPipelineDescription& description, const vk::RenderPass rp, const size_t subpass)
  {
    using boost::hash_combine;
    size_t pipelineHash = description.hash();
    hash_combine(pipelineHash, rp);
    hash_combine(pipelineHash, subpass);

    const auto it = m_Pipelines.find(pipelineHash);
    if (it != m_Pipelines.end())
      return it->second.get();

    vk::GraphicsPipelineCreateInfo ci;

    ShaderProgramInfo programInfo;
    m_ShadersStorage.getShaderProgramInfo(description.shaderNames, programInfo);

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCi;
    inputAssemblyCi.topology = get_primitive_topology(description.topology);

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

    vk::PipelineDepthStencilStateCreateInfo depthStencilCi = get_depth_stencil_state(description.depthStencilState);

    vk::PipelineColorBlendAttachmentState attachmentStates[MAX_RENDER_TARGETS];
    vk::PipelineColorBlendStateCreateInfo blendingCi = get_blend_state(description.blendState, attachmentStates);

    vk::PipelineDynamicStateCreateInfo dynamicStateCi;
    vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    dynamicStateCi.pDynamicStates = dynamicStates;
    dynamicStateCi.dynamicStateCount = std::size(dynamicStates);

    ci.stageCount = programInfo.stages.getSize();
    ci.pStages = programInfo.stages.getData();
    ci.pVertexInputState = &programInfo.vertexInput;
    ci.pInputAssemblyState = &inputAssemblyCi;
    ci.pTessellationState = nullptr;
    ci.pViewportState = &viewportCi;
    ci.pRasterizationState = &rasterizationStateCi;
    ci.pMultisampleState = &multisampleCi;
    ci.pDepthStencilState = &depthStencilCi;
    ci.pColorBlendState = &blendingCi;
    ci.pDynamicState = &dynamicStateCi;
    ci.layout = programInfo.layout;
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

  bool PipelinesStorage::getPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout)
  {
    layout = &m_ShadersStorage.getPipelineLayout(stageNames);
    return true;
  }
}
