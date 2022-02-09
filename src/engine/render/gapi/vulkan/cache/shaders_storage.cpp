#pragma once

#include "shaders_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>
#include <engine/render/gapi/vulkan/device.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>

#include <boost/functional/hash.hpp>

#include <filesystem>

namespace gapi::vulkan
{
  constexpr size_t EMPTY_PIPELINE_LAYOUT_HASH = 0;

  void ShadersStorage::Init(Device* device)
  {
    m_Device = device;

    CreateEmptyPipelineLayout();
    CreateShaderModules();
  }

  void ShadersStorage::CreateEmptyPipelineLayout()
  {
    vk::PipelineLayoutCreateInfo ci;
    ci.pSetLayouts = nullptr;
    ci.pPushConstantRanges = nullptr;
    ci.setLayoutCount = 0;
    ci.pushConstantRangeCount = 0;
    m_PipelineLayouts.insert({
      EMPTY_PIPELINE_LAYOUT_HASH,
      m_Device->m_Device->createPipelineLayoutUnique(ci)
    });
  }

  void ShadersStorage::CreateShaderModules()
  {
    const char* shaderModulesDir = "bin/shaders/spirv";
    log("reading shader modules from {}", shaderModulesDir);

    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(shaderModulesDir))
    {
      if (!file.is_regular_file())
        continue;

      const string filePath = file.path().string().c_str();
      if (!Utils::CheckFileExt(filePath, "spv"))
        continue;

      const string fileName = Utils::GetFileName(filePath);

      eastl::vector<char> rawBlob = Utils::ReadFile(filePath.c_str());
      const char* shaderName = fileName.c_str();
      if (rawBlob.size() == 0)
      {
        logerror("failed to read shader {}", shaderName);
        continue;
      }

      uint32_t* code = reinterpret_cast<uint32_t*>(rawBlob.data());
      const size_t codeSize = rawBlob.size();
      const auto ci = vk::ShaderModuleCreateInfo()
        .setPCode(code)
        .setCodeSize(codeSize);

      auto shModule = m_Device->m_Device->createShaderModuleUnique(ci);
      const string_hash shaderNameHash = str_hash(shaderName);

      spirv::ParsedSpirv metadata{rawBlob};

      m_ShaderModules.insert({
        shaderNameHash,
        ShaderModule{
          std::move(metadata),
          std::move(shModule)
        }
      });

      log("added module `{}`", shaderName);
    }
  }

  const ShaderModule& ShadersStorage::GetShaderModule(const string_hash name)
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

  std::optional<vk::PipelineLayout> ShadersStorage::GetShadersProgramLayout(const ShaderStagesNames& stages)
  {
    const size_t hash = HashShadersProgram(stages);
    const auto it = m_PipelineLayouts.find(hash);
    if (it != m_PipelineLayouts.end())
      return it->second.get();

    return std::nullopt;
  }

  void ShadersStorage::GetShaderProgramInfo(const ShaderStagesNames& stages, ShaderProgramInfo& programInfo)
  {
    const ShaderModule* vertexShaderModule = nullptr;

    for (const auto& shaderName: stages)
    {
      const ShaderModule& sm = GetShaderModule(shaderName);
      vk::PipelineShaderStageCreateInfo stage;
      stage.stage = sm.metadata.m_Stage;
      stage.module = sm.module.get();
      stage.pName = sm.metadata.m_EntryPoint.c_str();
      programInfo.stages.Push(stage);

      if (sm.metadata.m_Stage == vk::ShaderStageFlagBits::eVertex)
        vertexShaderModule = &sm;
    }

    programInfo.inputBinding.binding = 0;
    programInfo.inputBinding.stride = vertexShaderModule->metadata.m_VertexStride;
    programInfo.inputBinding.inputRate = vk::VertexInputRate::eVertex;

    programInfo.vertexInput.vertexBindingDescriptionCount = vertexShaderModule->metadata.m_HasInput ? 1 : 0;
    programInfo.vertexInput.pVertexBindingDescriptions = &programInfo.inputBinding;
    programInfo.vertexInput.vertexAttributeDescriptionCount = vertexShaderModule->metadata.m_VertexAttributesCount;
    programInfo.vertexInput.pVertexAttributeDescriptions = vertexShaderModule->metadata.m_VertexAttributeDescriptions;

    const uint32_t pushConstantsSize = vertexShaderModule->metadata.m_PushConstantsSize;

    const bool isEmptyLayout = pushConstantsSize == 0;
    const size_t programHash = isEmptyLayout ? EMPTY_PIPELINE_LAYOUT_HASH : HashShadersProgram(stages);
    const auto it = m_PipelineLayouts.find(programHash);
    if (it != m_PipelineLayouts.end())
    {
      programInfo.layout = it->second.get();
    }
    else
    {
      vk::PipelineLayoutCreateInfo layoutCi;
      layoutCi.pushConstantRangeCount = pushConstantsSize == 0 ? 0 : 1;
      vk::PushConstantRange pushConstant;
      pushConstant.offset = 0;
      pushConstant.size = vertexShaderModule->metadata.m_PushConstantsSize;
      layoutCi.pPushConstantRanges = &pushConstant;

      vk::UniquePipelineLayout uniqueLayout = m_Device->m_Device->createPipelineLayoutUnique(layoutCi);
      programInfo.layout = uniqueLayout.get();

      m_PipelineLayouts.insert({
        programHash,
        std::move(uniqueLayout)
      });
    }
  }
}