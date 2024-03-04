#include "shaders_storage.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
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

  ShaderModuleHandler ShadersStorage::addModule(const ShadersSystem::ShaderBlob& blob)
  {
    const string_hash hash = str_hash(blob.name.c_str());
    ShaderModuleHandler handler = (ShaderModuleHandler)hash;

    if (m_ShaderModules.find(handler) != m_ShaderModules.end())
      return handler;

    vk::ShaderModuleCreateInfo smCi;
    smCi.pCode = reinterpret_cast<const uint32_t*>(blob.data.data());
    smCi.codeSize = blob.data.size();

    auto mod = m_Device->m_Device->createShaderModuleUnique(smCi);
    VK_CHECK_RES(mod);

    ShaderModule m;
    m.module = std::move(mod.value);
    m.entry = blob.entry;
    m.stage = get_vk_stage(blob.stage);

    m_ShaderModules.insert({
      handler,
      std::move(m)
    });

    return handler;
  }

  PipelineLayoutHandler ShadersStorage::addPipelineLayout(const eastl::vector<spirv::v2::DescriptorSet>& dsets)
  {
    Utils::BitCapacity<gapi::MAX_DESCRIPTOR_SET> variableSizeDsets;

    eastl::vector<vk::UniqueDescriptorSetLayout> dsetLayoutsUnique;
    eastl::vector<vk::DescriptorSetLayout> dsetLayouts;
    dsetLayoutsUnique.reserve(dsets.size());
    dsetLayouts.reserve(dsets.size());

    eastl::vector<vk::DescriptorBindingFlags> bindingFlags;
    eastl::vector<vk::DescriptorSetLayoutBinding> bindings;
    const size_t maxDset = dsets.size();
    for (size_t iDset = 0; iDset < maxDset; ++iDset)
    {
      const auto& dset = dsets[iDset];
      bindings.clear();
      bindingFlags.clear();

      bindings.reserve(dset.size());
      bindingFlags.reserve(dset.size());
      for (const spirv::v2::DescriptorSetBinding& bindingInfo: dset)
      {
        auto& b = *reinterpret_cast<vk::DescriptorSetLayoutBinding*>(bindings.push_back_uninitialized());
        b = bindingInfo.vk;

        if (b.descriptorCount == 0)
        {
          variableSizeDsets.set(iDset);

          b.descriptorCount = gapi::MAX_VARIABLE_ARRAY_SIZE;
          bindingFlags.push_back(
            vk::DescriptorBindingFlagBits::ePartiallyBound |
            vk::DescriptorBindingFlagBits::eVariableDescriptorCount
          );
        }
        else
          bindingFlags.push_back(vk::DescriptorBindingFlagBits{});
      }

      vk::DescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsExtCi;
      bindingFlagsExtCi.bindingCount = bindingFlags.size();
      bindingFlagsExtCi.pBindingFlags = bindingFlags.data();

      vk::DescriptorSetLayoutCreateInfo ci;
      ci.bindingCount = dset.size();
      ci.pBindings = bindings.data();
      ci.pNext = &bindingFlagsExtCi;

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

    m_PipelineLayouts.push_back({
      PipelineLayout{
        .pipelineLayout = std::move(layout.value),
        .descriptorSetLayouts = std::move(dsetLayoutsUnique),
        .dsets = std::move(dsets),
        .variableSizeDsets = variableSizeDsets
      }
    });

    return PipelineLayoutHandler{m_PipelineLayouts.size()-1};
  }

  static size_t hash_shader_stages(const eastl::vector<ShaderModuleHandler>& modules)
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(const auto& m: modules)
      hash_combine(hash, m);

    return hash;
  }


  const PipelineLayout& ShadersStorage::getPipelineLayout(const PipelineLayoutHandler h)
  {
    const size_t id = (size_t)h;
    ASSERT_FMT(id < m_PipelineLayouts.size(), "invalid pipeline layout id (id:{} >= max:{})", id, m_PipelineLayouts.size());

    return m_PipelineLayouts[id];
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

  auto ShadersStorage::getShaderStageCreateInfo(const ShaderModuleHandler& shHandler) const -> vk::PipelineShaderStageCreateInfo
  {
    const ShaderModule* m = getModule(shHandler);
    return vk::PipelineShaderStageCreateInfo{}
            .setModule(m->module.get())
            .setPName(m->entry.c_str())
            .setStage(m->stage);
  }

  eastl::vector<vk::PipelineShaderStageCreateInfo> ShadersStorage::getShaderStagesCreateInfos(const eastl::vector<ShaderModuleHandler>& modules) const
  {
    eastl::vector<vk::PipelineShaderStageCreateInfo> cis;
    cis.reserve(modules.size());
    for (const auto& shHandler: modules)
      cis.push_back(getShaderStageCreateInfo(shHandler));

    return cis;
  }

  void ShadersStorage::clear()
  {
    m_PipelineLayouts.clear();
    m_ShaderModules.clear();
  }
}
