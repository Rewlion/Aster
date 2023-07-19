#include "pipelines_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/log.h>
#include <engine/gapi/resources.h>
#include <engine/gapi/vulkan/device.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/gapi/vulkan/traits.h>

#include <boost/functional/hash.hpp>

namespace
{
  struct InputAssembly
  {
    eastl::vector<vk::VertexInputBindingDescription> buffers;
    eastl::vector<vk::VertexInputAttributeDescription> attributes;

    bool operator==(const InputAssembly& rvl) const
    {
      return std::tie(buffers, attributes) == std::tie(rvl.buffers, rvl.attributes);
    }
  };
}

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

  static InputAssembly shader_input_to_spirv_ia(const gapi::VertexInputDescription& input)
  {
    InputAssembly ia;
    ia.buffers.reserve(input.buffers.size());
    ia.attributes.reserve(input.attributes.size());

    for (const auto& buffer: input.buffers)
    {
      ia.buffers.push_back(vk::VertexInputBindingDescription{
        buffer.binding,
        buffer.stride,
        vk::VertexInputRate::eVertex
      });
    }

    for (const auto& attribute: input.attributes)
    {
      ia.attributes.push_back(vk::VertexInputAttributeDescription{
        attribute.location,
        attribute.binding,
        get_attribute_format(attribute.type),
        attribute.offset}
      );
    }

    return ia;
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

    InputAssembly ia = shader_input_to_spirv_ia(description.ia);
    vk::PipelineVertexInputStateCreateInfo vertexInputCi;
    vertexInputCi.pVertexBindingDescriptions = ia.buffers.data();
    vertexInputCi.vertexBindingDescriptionCount = ia.buffers.size();
    vertexInputCi.pVertexAttributeDescriptions = ia.attributes.data();
    vertexInputCi.vertexAttributeDescriptionCount = ia.attributes.size();

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
    rasterizationStateCi.cullMode = get_cull_mode(description.cullMode);
    rasterizationStateCi.frontFace = get_front_face(description.cullMode);
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

    const PipelineLayout& layout = m_ShadersStorage.getPipelineLayout(description.layout);
    const eastl::vector<vk::PipelineShaderStageCreateInfo>& cis = m_ShadersStorage.getShaderStagesCreateInfos(description.shaders);

    ci.stageCount = cis.size();
    ci.pStages = cis.data();
    ci.pVertexInputState = &vertexInputCi;
    ci.pInputAssemblyState = &inputAssemblyCi;
    ci.pTessellationState = nullptr;
    ci.pViewportState = &viewportCi;
    ci.pRasterizationState = &rasterizationStateCi;
    ci.pMultisampleState = &multisampleCi;
    ci.pDepthStencilState = &depthStencilCi;
    ci.pColorBlendState = &blendingCi;
    ci.pDynamicState = &dynamicStateCi;
    ci.layout = layout.pipelineLayout.get();
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

  vk::Pipeline PipelinesStorage::getComputePipeline(const ComputePipelineDescription& description)
  {
    size_t pipelineHash = description.hash();
    const auto it = m_ComputePipelines.find(pipelineHash);
    if (it != m_ComputePipelines.end())
      return it->second.get();

    const PipelineLayout& layout = m_ShadersStorage.getPipelineLayout(description.layout);
    vk::PipelineShaderStageCreateInfo cis = m_ShadersStorage.getShaderStageCreateInfo(description.shader);

    vk::ComputePipelineCreateInfo ci;
    ci.stage = cis;
    ci.layout = layout.pipelineLayout.get();

    auto result = m_Device->m_Device->createComputePipelineUnique({}, ci);
    if (result.result != vk::Result::eSuccess)
      logerror("failed to create compute pipeline");

    vk::Pipeline pipeline = result.value.get();

    m_ComputePipelines.insert({
      pipelineHash,
      std::move(result.value)
    });

    return pipeline;
  }

  ShaderModuleHandler PipelinesStorage::addModule(const ShadersSystem::ShaderBlob& blob)
  {
    return m_ShadersStorage.addModule(blob);
  }

  PipelineLayoutHandler PipelinesStorage::addPipelineLayout(const eastl::vector<spirv::v2::DescriptorSet>& dsets)
  {
    return m_ShadersStorage.addPipelineLayout(dsets);
  }

  const PipelineLayout& PipelinesStorage::getPipelineLayout(const PipelineLayoutHandler h)
  {
    return m_ShadersStorage.getPipelineLayout(h);
  }

  void PipelinesStorage::clear()
  {
    m_Pipelines.clear();
    m_ShadersStorage.clear();
  }
}
