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

      const string filePath = file.path().string().c_str();
      const string fileName = Utils::GetFilename(filePath);

      if (!Utils::CheckFileExt(fileName, "spv"))
        continue;

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
}