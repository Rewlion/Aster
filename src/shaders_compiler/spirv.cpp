#include "spirv.h"
#include "ast_processing_types.h"

#include <engine/assert.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

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

    Reflection reflect(const eastl::vector<char>& spirv, const gapi::ShaderStage stage)
    {
      Reflection ret;

      const uint32_t* code = reinterpret_cast<const uint32_t*>(spirv.data());
      const size_t wordsCount = spirv.size() / sizeof(uint32_t);

      spirv_cross::CompilerGLSL glsl{code, wordsCount};

      const spirv_cross::ShaderResources resources = glsl.get_shader_resources();

      const auto getBinding = [&](unsigned int nSet, unsigned int nBinding) -> vk::DescriptorSetLayoutBinding&
      {
        if (nSet >= ret.descriptorSets.size())
          ret.descriptorSets.resize(nSet+1);

        if (nBinding >= ret.descriptorSets[nSet].size())
          ret.descriptorSets[nSet].resize(nBinding+1);

        return ret.descriptorSets[nSet][nBinding];
      };

      const vk::ShaderStageFlags vkStage = gapi::vulkan::get_vk_stage(stage);
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
      setupBinding(resources.uniform_buffers, [](const spirv_cross::Resource& res){return vk::DescriptorType::eUniformBuffer;});

      return ret;
    }
  }
}
