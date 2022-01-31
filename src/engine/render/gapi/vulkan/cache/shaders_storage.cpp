#pragma once

#include "shaders_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>
#include <engine/render/gapi/vulkan/device.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>

#include <filesystem>

namespace gapi::vulkan
{
  void ShadersStorage::Init(Device* device)
  {
    m_Device = device;

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
  }
}