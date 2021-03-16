#include "uniforms_accessor_storage.h"
#include "core.h"
#include "shader.h"

namespace Vulkan
{
  UniformsAccessorStorage::UniformsAccessorStorage(Core& core, vk::DescriptorPool descriptorPool)
    : core(core)
    , descriptorPool(descriptorPool)
  {
  }

  UniformsAccessor* UniformsAccessorStorage::GetUniformsAccessor(const ShaderProgram& program)
  {
    const PipelineUniforms& uniforms = program.GetCombinedUniformsInformation();
    const std::vector<vk::DescriptorSetLayout>& layouts = program.GetLayouts();

    const auto it = contexts.find(uniforms);
    if (it != contexts.end())
    {
      return it->second.get();
    }

    contexts[uniforms] = std::make_unique<UniformsAccessor>(core, descriptorPool, layouts, uniforms);

    return contexts.at(uniforms).get();
  }

  void UniformsAccessorStorage::Reset()
  {
    contexts.clear();
  }
}