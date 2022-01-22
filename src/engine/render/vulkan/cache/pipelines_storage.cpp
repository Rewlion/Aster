#pragma once

#include "pipelines_storage.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/algorithm/hash.h>
#include <engine/render/vulkan/device.h>
#include <engine/utils/fs.h>
#include <engine/log.h>
#include <engine/settings.h>

#include <filesystem>

namespace gapi::vulkan
{
  void PipelineStorage::init(Device* device)
  {
    m_Device = device;

    createShaderModules();
  }

  void PipelineStorage::createShaderModules()
  {
    const char* shaderModulesDir = "bin/shaders/spirv";
    log("reading shader modules from {}", shaderModulesDir);

    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(shaderModulesDir))
    {
      if (!file.is_regular_file())
        continue;

      const String filePath = file.path().string().c_str();
      const String fileName = utils::getFilename(filePath);

      if (!utils::checkFileExt(fileName, "spv"))
        continue;

      eastl::vector<char> rawBlob = utils::readFile(filePath.c_str());
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

  void PipelineStorage::collectPipelineDescriptions()
  {
    DataBlock* settings = get_app_settings();
    DataBlock* graphics = settings->get_child_block("graphics");
    const String pipelinesBlk = graphics->get_text("pipelines_blk");
    DataBlock pipelines;
    const bool loaded = load_blk_from_file(&pipelines, pipelinesBlk.c_str());
    ASSERT(loaded);

    for(const auto p: pipelines.get_child_blocks())
      collectPipelineDescription(p);
  }

  void PipelineStorage::collectPipelineDescription(const DataBlock& ppBlk)
  {
    const string_hash pipelineName = str_hash(ppBlk.get_name().c_str());
    PipelineDescription desc;

    const auto setStageInfo = [&](const size_t stageId, const vk::ShaderStageFlagBits vkStage, const String& shaderModuleName)
    {
      const auto shmId = str_hash(shaderModuleName.c_str());
      const auto shmIt = m_ShaderModules.find(shmId);
      ASSERT(shmIt != m_ShaderModules.end());

      desc.stagesInfo[stageId].stage = vkStage;
      desc.stagesInfo[stageId].module = *shmIt->second.module;
    };

    setStageInfo(traits::vertex::ID, vk::ShaderStageFlagBits::eVertex, ppBlk.get_text("vs"));
    setStageInfo(traits::fragment::ID, vk::ShaderStageFlagBits::eFragment, ppBlk.get_text("ps"));

    m_PipelineDescriptions.insert({
      pipelineName,
      desc
    });
  }

  vk::Pipeline PipelineStorage::getPipeline(const string_hash pipelineName, const vk::RenderPass rp)
  {
    const auto pipeDescIt = m_PipelineDescriptions.find(pipelineName);
    ASSERT(pipeDescIt != m_PipelineDescriptions.end());
    const PipelineDescription& desc = pipeDescIt->second;

    auto rpToPipelineMapit = m_Pipelines.find(rp);
    if (rpToPipelineMapit != m_Pipelines.end())
    {

    }
    else
    {
      m_Pipelines.insert({
        rp,
        PipelinesMap{}
      });
    }
  }
}