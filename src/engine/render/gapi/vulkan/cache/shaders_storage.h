#pragma once

#include <engine/types.h>
#include <engine/render/gapi/vulkan/spirv/parser.h>
#include <engine/assert.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  struct ShaderModule
  {
    spirv::ParsedSpirv     metadata;
    vk::UniqueShaderModule module;
  };

  class ShadersStorage
  {

    public:
      void Init(Device* device);

      const ShaderModule& GetShaderModule(const string_hash name);

    private:
      void CreateShaderModules();

    private:
      Device* m_Device;

      eastl::hash_map<string_hash, ShaderModule> m_ShaderModules;
  };

};