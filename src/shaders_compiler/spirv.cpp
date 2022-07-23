#include "spirv.h"
#include "ast_processing_types.h"

#include <engine/log.h>
#include <engine/assert.h>

#include <vulkan/vulkan.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>

#include <EASTL/functional.h>

namespace spirv
{
  static vk::ShaderStageFlagBits get_shader_stage(const spirv_cross::EntryPoint& ep)
  {
    switch (ep.execution_model)
    {
    case spv::ExecutionModel::ExecutionModelVertex:
    {
      return vk::ShaderStageFlagBits::eVertex;
    }

    case spv::ExecutionModel::ExecutionModelFragment:
    {
      return vk::ShaderStageFlagBits::eFragment;
    }

    default:
      ASSERT(!"unknown shader stage");
      return vk::ShaderStageFlagBits::eVertex; //make compiler happy
    }
  }

  static bool is_vector_type(const spirv_cross::SPIRType& type)
  {
    return type.vecsize > 1;
  }

  static vk::Format get_one_elem_format(const spirv_cross::SPIRType& type)
  {
    using spirv_cross::SPIRType;

    switch(type.basetype)
    {
		  case SPIRType::BaseType::Int:
      {
        return vk::Format::eR32Sint;
      }
		  case SPIRType::BaseType::UInt:
      {
        return vk::Format::eR32Uint;
      }
		  case SPIRType::BaseType::Half:
      {
        return vk::Format::eR16Sfloat;
      }
		  case SPIRType::BaseType::Float:
      {
        return vk::Format::eR32Sfloat;
      }
		  case SPIRType::BaseType::Double:
      {
        return vk::Format::eR64Sfloat;
      }
      default:
      {
        ASSERT(!"unknown format");
        return vk::Format::eUndefined;
      }
    }
  }

  static vk::Format get_vector_type(const spirv_cross::SPIRType& type)
  {
    using spirv_cross::SPIRType;

    switch(type.vecsize)
    {
      case 2:
      {
        switch(type.basetype)
        {
		      case SPIRType::BaseType::Int:
          {
            return vk::Format::eR32G32Sint;
          }
		      case SPIRType::BaseType::UInt:
          {
            return vk::Format::eR32G32Uint;
          }
		      case SPIRType::BaseType::Half:
          {
            return vk::Format::eR16G16Sfloat;
          }
		      case SPIRType::BaseType::Float:
          {
            return vk::Format::eR32G32Sfloat;
          }
		      case SPIRType::BaseType::Double:
          {
            return vk::Format::eR64G64Sfloat;
          }
          default:
          {
            ASSERT(!"unknown format");
            return vk::Format::eUndefined;
          }
        }
      }

      case 3:
      {
        switch(type.basetype)
        {
		      case SPIRType::BaseType::Int:
          {
            return vk::Format::eR32G32B32Sint;
          }
		      case SPIRType::BaseType::UInt:
          {
            return vk::Format::eR32G32B32Uint;
          }
		      case SPIRType::BaseType::Half:
          {
            return vk::Format::eR16G16B16Sfloat;
          }
		      case SPIRType::BaseType::Float:
          {
            return vk::Format::eR32G32B32Sfloat;
          }
		      case SPIRType::BaseType::Double:
          {
            return vk::Format::eR64G64B64Sfloat;
          }
          default:
          {
            ASSERT(!"unknown format");
            return vk::Format::eUndefined;
          }
        }
      }

      case 4:
      {
        switch(type.basetype)
        {
		      case SPIRType::BaseType::Int:
          {
            return vk::Format::eR32G32B32A32Sint;
          }
		      case SPIRType::BaseType::UInt:
          {
            return vk::Format::eR32G32B32A32Uint;
          }
		      case SPIRType::BaseType::Half:
          {
            return vk::Format::eR16G16B16A16Sfloat;
          }
		      case SPIRType::BaseType::Float:
          {
            return vk::Format::eR32G32B32A32Sfloat;
          }
		      case SPIRType::BaseType::Double:
          {
            return vk::Format::eR64G64B64A64Sfloat;
          }
          default:
          {
            ASSERT(!"unknown format");
            return vk::Format::eUndefined;
          }
        }
      }

      default:
      {
        ASSERT(!"unknown format");
        return vk::Format::eUndefined;
      }
    }
  }

  static vk::Format get_format(const spirv_cross::SPIRType& type)
  {
    if (is_vector_type(type))
      return get_vector_type(type);

    return get_one_elem_format(type);
  }

  uint32_t get_attribute_size(const spirv_cross::SPIRType& type)
  {
    return (type.width / 8) * type.vecsize * type.columns;
  }

  vk::DescriptorType get_texture_type(const spirv_cross::SPIRType& type)
  {
    switch(type.image.dim)
    {
      case spv::Dim::Dim2D: return vk::DescriptorType::eSampledImage;
      default:
      {
        ASSERT(!"unsupported type");
        return vk::DescriptorType::eSampledImage;
      }
    }
  }

  Reflection reflect(const eastl::vector<char>& spirv)
  {
    Reflection ret;

    const uint32_t* code = reinterpret_cast<const uint32_t*>(spirv.data());
    const size_t wordsCount = spirv.size() / sizeof(uint32_t);

    spirv_cross::CompilerGLSL glsl{code, wordsCount};

    const spirv_cross::SmallVector<spirv_cross::EntryPoint> entryPoints = glsl.get_entry_points_and_stages();
    if (entryPoints.size() != 1)
      logwarn("shader has more than one entry point, using the first one.");

    const spirv_cross::EntryPoint entryPoint = entryPoints[0];
    std::snprintf(ret.entryName, SHADERS_STAGE_NAME_LEN, "%s", entryPoint.name.c_str());
    ret.stage = get_shader_stage(entryPoint);

    const spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    if (ret.stage == vk::ShaderStageFlagBits::eVertex)
    {
      size_t attributeOffset = 0;
      for(size_t i = 0; i < resources.stage_inputs.size(); ++i)
      {
        const spirv_cross::Resource& input = resources.stage_inputs[i];
        const spirv_cross::SPIRType& type = glsl.get_type(input.type_id);

        vk::VertexInputAttributeDescription attr;
        attr.location = glsl.get_decoration(input.id, spv::Decoration::DecorationLocation);
        attr.format = get_format(type);
        attr.offset = attributeOffset;
        attr.binding = 0;

        attributeOffset += get_attribute_size(type);

        ret.inputAssembly.attributes.push(attr);
      }
      ret.inputAssembly.stride = attributeOffset;
    }

    if (!resources.push_constant_buffers.empty())
    {
      const spirv_cross::Resource& constants = resources.push_constant_buffers[0];
      const spirv_cross::SPIRType& type = glsl.get_type(constants.type_id);

      ret.pushConstantsSize = glsl.get_declared_struct_size(type);
    }

    for (size_t i = 0; i < resources.separate_images.size(); ++i)
    {
      const spirv_cross::Resource& texture = resources.separate_images[i];
      const spirv_cross::SPIRType& type = glsl.get_type(texture.type_id);

      const unsigned int nSet = glsl.get_decoration(texture.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int nBinding = glsl.get_decoration(texture.id, spv::Decoration::DecorationBinding);

      const auto validate = [&](const char* rangeName, const size_t val, const size_t max)
                            {
                              if (val > max)
                              {
                                logerror("(set:{} binding:{}) texture {}: has unsupported {} number. Max: {}",
                                  nSet, nBinding, texture.name.c_str(), rangeName, max);
                                return false;
                              }
                              return true;
                            };

      if (!validate("set", nSet, MAX_SETS_COUNT) || !validate("binding", nBinding, MAX_BINDING_COUNT))
        continue;

      Binding& binding = ret.shaderArguments[nSet].bindings[nBinding];
      //binding.type = get_texture_type(type);
      binding.stages = ret.stage;
      std::snprintf(binding.name, BINDING_NAME_LEN, "%s", texture.name.c_str());
    }

    for (size_t i = 0; i < resources.separate_samplers.size(); ++i)
    {
      const spirv_cross::Resource& sampler = resources.separate_samplers[i];
      const spirv_cross::SPIRType& type = glsl.get_type(sampler.type_id);

      const unsigned int nSet = glsl.get_decoration(sampler.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int nBinding = glsl.get_decoration(sampler.id, spv::Decoration::DecorationBinding);

      const auto validate = [&](const char* rangeName, const size_t val, const size_t max)
                            {
                              if (val > max)
                              {
                                logerror("(set:{} binding:{}) sampler {}: has unsupported {} number. Max: {}",
                                  nSet, nBinding, sampler.name.c_str(), rangeName, max);
                                return false;
                              }
                              return true;
                            };

      if (!validate("set", nSet, MAX_SETS_COUNT) || !validate("binding", nBinding, MAX_BINDING_COUNT))
        continue;

      Binding& binding = ret.shaderArguments[nSet].bindings[nBinding];
      binding.type = BindingType::Sampler;
      binding.stages = ret.stage;
      std::snprintf(binding.name, BINDING_NAME_LEN, "%s", sampler.name.c_str());
    }

    for (size_t i = 0; i < resources.uniform_buffers.size(); ++i)
    {
      const spirv_cross::Resource& uniform = resources.uniform_buffers[i];
      const spirv_cross::SPIRType& type = glsl.get_type(uniform.type_id);

      const unsigned int nSet = glsl.get_decoration(uniform.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int nBinding = glsl.get_decoration(uniform.id, spv::Decoration::DecorationBinding);

      const auto validate = [&](const char* rangeName, const size_t val, const size_t max)
                            {
                              if (val > max)
                              {
                                logerror("(set:{} binding:{}) uniform {}: has unsupported {} number. Max: {}",
                                  nSet, nBinding, uniform.name.c_str(), rangeName, max);
                                return false;
                              }
                              return true;
                            };

      if (!validate("set", nSet, MAX_SETS_COUNT) || !validate("binding", nBinding, MAX_BINDING_COUNT))
        continue;

      Binding& binding = ret.shaderArguments[nSet].bindings[nBinding];
      binding.type = BindingType::UniformBufferDynamic;
      binding.stages = ret.stage;
      std::snprintf(binding.name, BINDING_NAME_LEN, "%s", uniform.name.c_str());
    }

    return ret;
  }

  namespace v2
  {
    static vk::ShaderStageFlagBits get_shader_stage(const gapi::ShaderStage stage)
    {
      switch (stage)
      {
      case gapi::ShaderStage::Vertex:
      {
        return vk::ShaderStageFlagBits::eVertex;
      }

      case gapi::ShaderStage::Fragment:
      {
        return vk::ShaderStageFlagBits::eFragment;
      }

      default:
        ASSERT(!"unknown shader stage");
        return vk::ShaderStageFlagBits::eVertex; //make compiler happy
      }
    }

    static vk::Format get_attribute_format(const gapi::AttributeType type)
    {
      switch (type)
      {
         case gapi::AttributeType::Float:  return vk::Format::eR32Sfloat;
         case gapi::AttributeType::Float2: return vk::Format::eR32G32Sfloat;
         case gapi::AttributeType::Float3: return vk::Format::eR32G32B32Sfloat;
         case gapi::AttributeType::Float4: return vk::Format::eR32G32B32A32Sfloat;
         case gapi::AttributeType::Int:    return vk::Format::eR32Sint;
         case gapi::AttributeType::Int2:   return vk::Format::eR32G32Sint;
         case gapi::AttributeType::Int3:   return vk::Format::eR32G32B32Sint;
         case gapi::AttributeType::Int4:   return vk::Format::eR32G32B32A32Sint;
         default:
         {
          ASSERT(!"unsupported");
          break;
         }
      }
    }

    InputAssembly shader_input_to_spirv_ia(const gapi::VertexInputDescription& input)
    {
      InputAssembly ia;
      ia.buffers.reserve(input.buffers.size());
      ia.attributes.reserve(input.attributes.size());

      for (const auto& buffer: input.buffers)
      {
        ia.buffers.push_back(vk::VertexInputBindingDescription{
          buffer.binding,
          buffer.stride,
          vk::VertexInputRate::eVertex
        });
      }

      for (const auto& attribute: input.attributes)
      {
        ia.attributes.push_back(vk::VertexInputAttributeDescription{
          attribute.location,
          attribute.binding,
          get_attribute_format(attribute.type),
          attribute.offset}
        );
      }

      return ia;
    }

    static vk::ShaderStageFlags get_vk_stage(const gapi::ShaderStage stage)
    {
      switch (stage)
      {
         case gapi::ShaderStage::Vertex: return vk::ShaderStageFlagBits::eVertex;
         case gapi::ShaderStage::TessellationControl: return vk::ShaderStageFlagBits::eTessellationControl;
         case gapi::ShaderStage::TessellationEvaluation: return vk::ShaderStageFlagBits::eTessellationEvaluation;
         case gapi::ShaderStage::Geometry: return vk::ShaderStageFlagBits::eGeometry;
         case gapi::ShaderStage::Fragment: return vk::ShaderStageFlagBits::eFragment;
         case gapi::ShaderStage::Compute: return vk::ShaderStageFlagBits::eCompute;
         case gapi::ShaderStage::AllGraphics: return vk::ShaderStageFlagBits::eAllGraphics;
         case gapi::ShaderStage::All: return vk::ShaderStageFlagBits::eAll;
         case gapi::ShaderStage::Raygen: return vk::ShaderStageFlagBits::eRaygenKHR;
         case gapi::ShaderStage::AnyHit: return vk::ShaderStageFlagBits::eAnyHitKHR;
         case gapi::ShaderStage::ClosestHit: return vk::ShaderStageFlagBits::eClosestHitKHR;
         case gapi::ShaderStage::Miss: return vk::ShaderStageFlagBits::eMissKHR;
         case gapi::ShaderStage::Intersection: return vk::ShaderStageFlagBits::eIntersectionKHR;
         case gapi::ShaderStage::Callable: return vk::ShaderStageFlagBits::eCallableKHR;
         case gapi::ShaderStage::Task: return vk::ShaderStageFlagBits::eTaskNV;
         case gapi::ShaderStage::Mesh: return vk::ShaderStageFlagBits::eMeshNV;

         default:
         {
          ASSERT(!"unsupported");
          return {};
         }
      }
    }

    eastl::vector<DescriptorSet> reflect(const eastl::vector<char>& spirv, const gapi::ShaderStage stage)
    {
      eastl::vector<DescriptorSet> ret;

      const uint32_t* code = reinterpret_cast<const uint32_t*>(spirv.data());
      const size_t wordsCount = spirv.size() / sizeof(uint32_t);

      spirv_cross::CompilerGLSL glsl{code, wordsCount};

      const spirv_cross::ShaderResources resources = glsl.get_shader_resources();

      const auto getBinding = [&](unsigned int nSet, unsigned int nBinding) -> vk::DescriptorSetLayoutBinding&
      {
        if (nSet >= ret.size())
          ret.resize(nSet+1);

        if (nBinding >= ret[nSet].size())
          ret[nSet].resize(nSet+1);

        return ret[nSet][nBinding];
      };

      const vk::ShaderStageFlags vkStage = get_vk_stage(stage);
      const auto setupBinding = [&](auto& resources, const auto getResType)
      {
        for (size_t i = 0; i < resources.size(); ++i)
        {
          const spirv_cross::Resource& res = resources[i];

          const unsigned int nSet = glsl.get_decoration(res.id, spv::Decoration::DecorationDescriptorSet);
          const unsigned int nBinding = glsl.get_decoration(res.id, spv::Decoration::DecorationBinding);

          vk::DescriptorSetLayoutBinding& binding = getBinding(nSet, nBinding);
          binding.binding = nBinding;
          binding.stageFlags = vkStage;
          binding.descriptorType = getResType(res);
          binding.descriptorCount = 1;
        }
      };

      const auto getTexType = [&](const spirv_cross::Resource& res)
      {
        const spirv_cross::SPIRType& type = glsl.get_type(res.type_id);
        return get_texture_type(type);
      };
      setupBinding(resources.separate_images, getTexType);
      setupBinding(resources.separate_samplers, [](const spirv_cross::Resource& res){return vk::DescriptorType::eSampler;});
      setupBinding(resources.uniform_buffers, [](const spirv_cross::Resource& res){return vk::DescriptorType::eUniformBufferDynamic;});

      return ret;
    }
  }
}
