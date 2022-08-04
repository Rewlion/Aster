#include "shaders_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/gapi/vulkan/device.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>
#include <shaders_compiler/ast_processing_types.h>

#include <boost/functional/hash.hpp>

#include <filesystem>
#include <functional>

namespace gapi::vulkan
{
  void ShadersStorage::init(Device* device)
  {
    m_Device = device;
  }

  ShaderModuleHandler ShadersStorage::addModule(const ShadersSystem::ShaderBlob& blob, const spirv::v2::Reflection& reflection)
  {
    const string_hash hash = str_hash(blob.name.c_str());
    ShaderModuleHandler handler = (ShaderModuleHandler)hash;

    if (m_ShaderModules.find(handler) != m_ShaderModules.end())
      return handler;

    ShaderModule m;
    m.entry = blob.entry;
    m.stage = get_vk_stage(blob.stage);
    m.descriptorSets = reflection.descriptorSets;
    vk::ShaderModuleCreateInfo smCi;
    smCi.pCode = reinterpret_cast<const uint32_t*>(blob.data.data());
    smCi.codeSize = blob.data.size();

    auto mod = m_Device->m_Device->createShaderModuleUnique(smCi);
    VK_CHECK_RES(mod);
    m.module = std::move(mod.value);

    m_ShaderModules.insert({
      handler,
      std::move(m)
    });

    return handler;
  }


  static size_t hash_shader_stages(const eastl::vector<ShaderModuleHandler>& modules)
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(const auto& m: modules)
      hash_combine(hash, m);

    return hash;
  }


  const PipelineLayout* ShadersStorage::getPipelineLayout(const eastl::vector<ShaderModuleHandler>& modules)
  {
    const string_hash hash = hash_shader_stages(modules);

    auto it = m_PipelineLayouts.find(hash);
    if (it != m_PipelineLayouts.end())
      return &it->second;

    eastl::vector<vk::PipelineShaderStageCreateInfo> stagesCi;
    stagesCi.reserve(modules.size());
    for (const auto& moduleHandler: modules)
    {
      const ShaderModule* sh = getModule(moduleHandler);
      vk::PipelineShaderStageCreateInfo stageCi;
      stageCi.module = sh->module.get();
      stageCi.stage = sh->stage;
      stageCi.pName = sh->entry.c_str();
      stagesCi.push_back(stageCi);
    }

    eastl::vector<spirv::v2::DescriptorSet> dsets = getModulesDescriptorSets(modules);
    eastl::vector<vk::UniqueDescriptorSetLayout> dsetLayoutsUnique;
    eastl::vector<vk::DescriptorSetLayout> dsetLayouts;
    dsetLayoutsUnique.reserve(dsets.size());
    dsetLayouts.reserve(dsets.size());

    for (const auto& dset: dsets)
    {
      vk::DescriptorSetLayoutCreateInfo ci;
      ci.bindingCount = dset.size();
      ci.pBindings = dset.data();
      auto l = m_Device->m_Device->createDescriptorSetLayoutUnique(ci);
      VK_CHECK_RES(l);
      dsetLayouts.push_back(l.value.get());
      dsetLayoutsUnique.push_back(std::move(l.value));
    }

    vk::PipelineLayoutCreateInfo layoutCi;
    layoutCi.pushConstantRangeCount = 0;
    layoutCi.pPushConstantRanges = nullptr;
    layoutCi.pSetLayouts = dsetLayouts.data();
    layoutCi.setLayoutCount = dsetLayouts.size();

    auto layout = m_Device->m_Device->createPipelineLayoutUnique(layoutCi);
    VK_CHECK_RES(layout);

    const auto [v,b] = m_PipelineLayouts.insert({
      hash,
      PipelineLayout{
        .pipelineLayout = std::move(layout.value),
        .descriptorSetLayouts = std::move(dsetLayoutsUnique),
        .dsets = std::move(dsets),
        .stagesCi = std::move(stagesCi)
      }
    });

    ASSERT(b);
    return &v->second;
  }

  eastl::vector<spirv::v2::DescriptorSet> ShadersStorage::getModulesDescriptorSets(const eastl::vector<ShaderModuleHandler>& modules) const
  {
    eastl::vector<spirv::v2::DescriptorSet> dsets;

    const auto mergeBindings = [](eastl::vector<vk::DescriptorSetLayoutBinding>& to, const eastl::vector<vk::DescriptorSetLayoutBinding>& from) {
      if (to.size() < from.size())
      {
        size_t i = to.size();
        to.resize(from.size());
        for (; i < from.size(); ++i)
          to[i].binding = i;
      }

      for (size_t i = 0; i < from.size(); ++i)
      {
        if (from[i].stageFlags != vk::ShaderStageFlags{})
        {
          if (to[i].stageFlags != vk::ShaderStageFlags{})
          {
            ASSERT(from[i].descriptorType == to[i].descriptorType);
            ASSERT(from[i].descriptorCount == to[i].descriptorCount);
            to[i].stageFlags |= from[i].stageFlags;
          }
          else
            to[i] = from[i];
        }
      }
    };

    for (const auto mHandler: modules)
    {
      auto shIt = m_ShaderModules.find(mHandler);
      if (shIt == m_ShaderModules.end())
      {
        ASSERT(!"no shader module");
        return {};
      }

      const auto& shDsets = shIt->second.descriptorSets;

      if (dsets.size() < shDsets.size())
        dsets.resize(shDsets.size());

      for (size_t i = 0; i < shDsets.size(); ++i)
        mergeBindings(dsets[i], shDsets[i]);
    }

    return dsets;
  }

  const ShaderModule* ShadersStorage::getModule(const ShaderModuleHandler h) const
  {
    const auto it = m_ShaderModules.find(h);
    if (it != m_ShaderModules.end())
    {
      return &it->second;
    }
    {
      ASSERT(!"module doesn't exist");
      return nullptr;
    }
  }
}