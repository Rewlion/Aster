#include "parser.h"

#include <engine/log.h>
#include <engine/assert.h>

#include <vulkan/vulkan.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>

namespace spirv
{
  static vk::ShaderStageFlagBits getShaderStage(const spirv_cross::EntryPoint& ep)
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

  vk::Format getIntegralFormat(const spirv_cross::SPIRType& type)
  {
    const bool isIntegral = spirv_cross::type_is_integral(type);
    ASSERT(isIntegral);

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

  uint32_t getAttributeSize(const spirv_cross::SPIRType& type)
  {
    return type.width * type.vecsize;
  }

  ParsedSpirv::ParsedSpirv(const eastl::vector<char>& binary)
  {
    const uint32_t* code = reinterpret_cast<const uint32_t*>(binary.data());
    const size_t wordsCount = binary.size() / sizeof(uint32_t);

    spirv_cross::CompilerGLSL glsl{code, wordsCount};

    const spirv_cross::SmallVector<spirv_cross::EntryPoint> entryPoints = glsl.get_entry_points_and_stages();
    if (entryPoints.size() != 1)
      logwarn("shader has more than one entry point, using the first one.");

    const spirv_cross::EntryPoint entryPoint = entryPoints[0];
    m_EntryPoint = entryPoint.name;
    m_Stage = getShaderStage(entryPoint);

    const spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    if (m_Stage == vk::ShaderStageFlagBits::eVertex)
    {
      size_t attributeOffset = 0;
      for(size_t i = 0; i < resources.stage_inputs.size(); ++i)
      {
        const spirv_cross::Resource& input = resources.stage_inputs[i];
        const spirv_cross::SPIRType type = glsl.get_type(input.type_id);

        vk::VertexInputAttributeDescription& attr = m_VertexAttributeDescriptions[i];
        attr.location = glsl.get_decoration(input.id, spv::Decoration::DecorationLocation);
        attr.format = getIntegralFormat(type);
        attr.offset = attributeOffset;
        attr.binding = 0;

        attributeOffset += getAttributeSize(type);
      }
      m_VertexStride = attributeOffset;
    }
  }
}