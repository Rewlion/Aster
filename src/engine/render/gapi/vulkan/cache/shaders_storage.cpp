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
  void ShadersStorage::Init(Device* device)
  {
    m_Device = device;
    m_LayoutsStorage.Init(device);

    CreateShaderModules();
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
    return {};
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
    const auto it = m_CachedLayouts.find(hash);
    if (it != m_CachedLayouts.end())
      return it->second;

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

    const size_t programHash = HashShadersProgram(stages);
    const auto it = m_CachedLayouts.find(programHash);
    if (it != m_CachedLayouts.end())
    {
      programInfo.layout = it->second;
    }
    else
    {
      vk::PipelineLayoutCreateInfo layoutCi;
      layoutCi.pushConstantRangeCount = vertexShaderModule->metadata.m_PushConstantsSize == 0 ? 0 : 1;
      vk::PushConstantRange pushConstant;
      pushConstant.offset = 0;
      pushConstant.size = vertexShaderModule->metadata.m_PushConstantsSize;
      layoutCi.pPushConstantRanges = &pushConstant;

      programInfo.layout = m_LayoutsStorage.GetPipelineLayout(layoutCi);
      m_CachedLayouts.insert({
        programHash,
        programInfo.layout
      });
    }
  }
}