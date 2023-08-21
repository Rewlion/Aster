#pragma once

#include "vulkan.h"

#include <shaders_compiler/ast_types.h>

namespace gapi::vulkan
{
  inline auto shaders_resource_to_img_view_type(const ShadersSystem::ResourceType type) -> vk::ImageViewType 
  {
    switch (type)
    {
      case ShadersSystem::ResourceType::Texture2D: return vk::ImageViewType::e2D;
      case ShadersSystem::ResourceType::Texture3D: return vk::ImageViewType::e3D;
      case ShadersSystem::ResourceType::TextureCube: return vk::ImageViewType::eCube;
      case ShadersSystem::ResourceType::RWTexture2D: return vk::ImageViewType::e2D;
      case ShadersSystem::ResourceType::RWTexture3D: return vk::ImageViewType::e3D;
      default: ASSERT_FMT(false, "unsupported resource type {}", (int) type); return vk::ImageViewType::e2D;
    }
  }
}