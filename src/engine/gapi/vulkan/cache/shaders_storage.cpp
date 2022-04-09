#pragma once

#include "shaders_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>
#include <engine/gapi/vulkan/device.h>
#include <engine/settings.h>
#include <engine/shaders_compiler/constants.h>
#include <engine/utils/fs.h>

#include <boost/functional/hash.hpp>

#include <filesystem>
#include <functional>

namespace
{
  struct ShaderBlob
  {
    char shaderName[spirv::SHADERS_NAME_LEN];
    const char* blob = nullptr;
    size_t blobSize = 0;
    spirv::Reflection reflection;
  };

  class ShadersBinReader
  {
    public:
      bool Open(const char* file)
      {
        m_Bin = Utils::read_file(file);
        if (m_Bin.size() == 0)
        {
          logerror("failed to read shaders bin file: {}", file);
          return false;
        }

        m_Offset = 0;

        const uint64_t magic = Read64();
        if (magic != spirv::SHADERS_MAGIC)
        {
          logerror("failed to read shaders bin file: wrong magic");
          return false;
        }

        m_ShadersBlobsCount = Read64();
        if (m_ShadersBlobsCount == 0)
        {
          logerror("shaders bin has 0 shaders");
          return false;
        }

        m_BlobsStart = m_Offset;

        return true;
      }

      void ForEach(std::function<void(const ShaderBlob&)> cb)
      {
        m_Offset = m_BlobsStart;

        for(size_t i = 0; i < m_ShadersBlobsCount; ++i)
        {
          ShaderBlob blob;
          ReadBuf(blob.shaderName, spirv::SHADERS_NAME_LEN);
          blob.reflection = Read<spirv::Reflection>();
          blob.blobSize = Read64();
          blob.blob = m_Bin.data() + m_Offset;
          cb(blob);
          Move(blob.blobSize);
        }
      }

    private:
      void ReadBuf(void* dst, const size_t size)
      {
        std::memcpy(dst, m_Bin.data() + m_Offset, size);
        m_Offset += size;
      }

      uint64_t Read64()
      {
        return Read<uint64_t>();
      }

      template<class T>
      T Read()
      {
        T buf = *(T*)(m_Bin.data() + m_Offset);
        Move(sizeof(T));
        return buf;
      }

      void Move(const size_t v)
      {
        m_Offset += v;
      }

    private:
      eastl::vector<char> m_Bin;
      size_t m_BlobsStart = 0;
      size_t m_Offset = 0;
      size_t m_ShadersBlobsCount = 0;
  };
}

namespace gapi::vulkan
{
  constexpr size_t EMPTY_PIPELINE_LAYOUT_HASH = 0;

  void ShadersStorage::init(Device* device)
  {
    m_Device = device;

    createEmptyPipelineLayout();
    createShaderModules();
  }

  void ShadersStorage::createEmptyPipelineLayout()
  {
    vk::PipelineLayoutCreateInfo ci;
    ci.pSetLayouts = nullptr;
    ci.pPushConstantRanges = nullptr;
    ci.setLayoutCount = 0;
    ci.pushConstantRangeCount = 0;
    PipelineLayout layout;
    layout.pipelineLayout =  m_Device->m_Device->createPipelineLayoutUnique(ci);
    m_PipelineLayouts.insert({
      EMPTY_PIPELINE_LAYOUT_HASH,
      std::move(layout)
    });
  }

  void ShadersStorage::createShaderModules()
  {
    const char* shadersBin = "bin/shaders_spirv.bin";
    log("reading shader modules from {}", shadersBin);

    ShadersBinReader bin;
    ASSERT(bin.Open(shadersBin));

    bin.ForEach([&](const ShaderBlob& blob)
    {
      vk::ShaderModuleCreateInfo ci;
      ci.pCode = (uint32_t*)blob.blob;
      ci.codeSize = blob.blobSize;

      log("creating shader module `{}`", blob.shaderName);

      auto shModule = m_Device->m_Device->createShaderModuleUnique(ci);
      const string_hash shaderNameHash = str_hash(blob.shaderName);

      m_ShaderModules.insert({
        shaderNameHash,
        ShaderModule{
          std::move(blob.reflection),
          std::move(shModule)
        }
      });
    });
  }

  const ShaderModule& ShadersStorage::getShaderModule(const string_hash name)
  {
    const auto it = m_ShaderModules.find(name);
    if (it != m_ShaderModules.end())
      return it->second;

    ASSERT(!"shader is not founnd");
  }

  static size_t HashShadersProgram(const ShaderStagesNames& stages)
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(const auto& name: stages)
      hash_combine(hash, name);

    return hash;
  }

  const PipelineLayout& ShadersStorage::getPipelineLayout(const ShaderStagesNames& stages)
  {
    const size_t hash = HashShadersProgram(stages);
    const auto it = m_PipelineLayouts.find(hash);
    if (it != m_PipelineLayouts.end())
      return it->second;

    string stageDump;
    for(const auto& stage: stages)
      stageDump += stage + " ";

    logerror("vulkan: failed to get pipeline layout for {}", stageDump);
    return {};
  }

  void ShadersStorage::getShaderProgramInfo(const ShaderStagesNames& stages, ShaderProgramInfo& programInfo)
  {
    const ShaderModule* vertexShaderModule = nullptr;

    Utils::FixedStack<vk::PushConstantRange, 2> stagesPushConstants;

    for (const auto& shaderName: stages)
    {
      const ShaderModule& sm = getShaderModule(shaderName);
      vk::PipelineShaderStageCreateInfo stage;
      stage.stage = sm.metadata.stage;
      stage.module = sm.module.get();
      stage.pName = sm.metadata.entryName;
      programInfo.stages.push(stage);

      if (sm.metadata.stage == vk::ShaderStageFlagBits::eVertex)
        vertexShaderModule = &sm;

      if (sm.metadata.pushConstantsSize != 0)
      {
        auto pushConstant = vk::PushConstantRange{};
        pushConstant.offset = 0;
        pushConstant.size = sm.metadata.pushConstantsSize;
        pushConstant.stageFlags = sm.metadata.stage;
        stagesPushConstants.push(pushConstant);
      }
    }

    programInfo.inputBinding.binding = 0;
    programInfo.inputBinding.stride = vertexShaderModule->metadata.inputAssembly.stride;
    programInfo.inputBinding.inputRate = vk::VertexInputRate::eVertex;

    programInfo.vertexInput.vertexBindingDescriptionCount = vertexShaderModule->metadata.inputAssembly.attributes.getSize() != 0 ? 1 : 0;
    programInfo.vertexInput.pVertexBindingDescriptions = &programInfo.inputBinding;
    programInfo.vertexInput.vertexAttributeDescriptionCount = vertexShaderModule->metadata.inputAssembly.attributes.getSize();
    programInfo.vertexInput.pVertexAttributeDescriptions = vertexShaderModule->metadata.inputAssembly.attributes.getData();

    const bool isEmptyLayout = stagesPushConstants.getSize() == 0;
    const size_t programHash = isEmptyLayout ? EMPTY_PIPELINE_LAYOUT_HASH : HashShadersProgram(stages);
    const auto it = m_PipelineLayouts.find(programHash);
    if (it != m_PipelineLayouts.end())
    {
      programInfo.layout = it->second.pipelineLayout.get();
    }
    else
    {
      spirv::ShaderArgument arguments[spirv::MAX_SETS_COUNT];

      for (const auto& stage: stages)
      {
        const auto& shader = getShaderModule(stage).metadata;

        for (int nSet = 0; nSet < spirv::MAX_SETS_COUNT; ++nSet)
          for (int nBinding = 0; nBinding < spirv::MAX_BINDING_COUNT; ++nBinding)
          {
            const auto& binding = shader.shaderArguments[nSet].bindings[nBinding];
            auto& targetBinding = arguments[nSet].bindings[nBinding];

            if (binding.type != spirv::BindingType::None)
            {
              std::snprintf(targetBinding.name, spirv::BINDING_NAME_LEN, "%s", binding.name);
              targetBinding.type = binding.type;
              targetBinding.stages = targetBinding.stages | shader.stage;
            }
          }
      }

      PipelineLayout layout;
      vk::DescriptorSetLayout setLayouts[spirv::MAX_SETS_COUNT];
      for (size_t nSet = 0; nSet < std::size(arguments); ++nSet )
      {
        Utils::FixedStack<vk::DescriptorSetLayoutBinding, spirv::MAX_BINDING_COUNT> bindings;
        for (size_t nBinding = 0; nBinding < arguments[nSet].getBindingsCount(); ++nBinding)
        {
          const spirv::Binding& binding = arguments[nSet].bindings[nBinding];

          vk::DescriptorSetLayoutBinding bindingDesc;
          switch(binding.type)
          {
            case spirv::BindingType::Texture2D:
            {
              bindingDesc.descriptorType = vk::DescriptorType::eSampledImage;
              bindingDesc.descriptorCount = 1;
              break;
            }

            case spirv::BindingType::Sampler:
            {
              bindingDesc.descriptorType = vk::DescriptorType::eSampler;
              bindingDesc.descriptorCount = 1;
              break;
            }

            case spirv::BindingType::UniformBufferDynamic:
            {
              bindingDesc.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
              bindingDesc.descriptorCount = 1;
              break;
            }

            default:
            {
              ASSERT(!"Unsupported binding type");
            }
          }
          bindingDesc.stageFlags = binding.stages;
          bindingDesc.binding = nBinding;
          bindings.push(bindingDesc);
        }

        vk::DescriptorSetLayoutCreateInfo ci;
        ci.bindingCount = bindings.getSize();
        ci.pBindings = bindings.getData();

        layout.descriptorSetLayouts[nSet] = m_Device->m_Device->createDescriptorSetLayoutUnique(ci);
        setLayouts[nSet] = layout.descriptorSetLayouts[nSet].get();
      }
      std::memcpy(layout.sets, arguments, std::size(arguments) * sizeof(arguments[0]));

      vk::PipelineLayoutCreateInfo layoutCi;
      layoutCi.pushConstantRangeCount = stagesPushConstants.getSize();
      layoutCi.pPushConstantRanges = stagesPushConstants.getData();
      layoutCi.pSetLayouts = setLayouts;
      layoutCi.setLayoutCount = std::size(setLayouts);

      layout.pipelineLayout = m_Device->m_Device->createPipelineLayoutUnique(layoutCi);
      programInfo.layout = layout.pipelineLayout.get();

      m_PipelineLayouts.insert({
        programHash,
        std::move(layout)
      });
    }
  }
}