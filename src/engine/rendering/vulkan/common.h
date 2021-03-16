#pragma once

#include "enums.h"
#include "image.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <tuple>

namespace Vulkan
{
  typedef std::string ResourceId;

  struct ImageAttachment
  {
    ResourceId id;
    vk::Format format;
    ImageView view;
    ImageType type;
    vk::ImageLayout initialLayout;
    vk::ImageLayout finalLayout;
    vk::AttachmentLoadOp loadOp;
    vk::AttachmentStoreOp storeOp;
    vk::ImageUsageFlags usageFlags;

    inline bool operator<(const ImageAttachment& r) const
    {
      return std::tie(id, format, type, initialLayout, finalLayout, loadOp, storeOp, usageFlags) <
        std::tie(r.id, r.format, r.type, r.initialLayout, r.finalLayout, r.loadOp, r.storeOp, r.usageFlags);
    }
  };

  struct DepthStencilSettings
  {
    bool depthTestEnabled = false;
    bool depthWriteEnabled = false;

    inline bool operator<(const DepthStencilSettings& r) const
    {
      return std::tie(depthTestEnabled, depthWriteEnabled) <
             std::tie(r.depthTestEnabled, r.depthWriteEnabled);
    }
  };

  constexpr DepthStencilSettings EnableDepthTest{ true, true };
  constexpr DepthStencilSettings DisableDepthTest{ false, false };

  struct RasterizationMode
  {
    bool depthClampEnable = false;
    bool rasterizerDiscardEnable = false;
    vk::PolygonMode polygonMode = vk::PolygonMode::eFill;
    vk::CullModeFlagBits cullMode = vk::CullModeFlagBits::eBack;
    vk::FrontFace frontFace = vk::FrontFace::eClockwise;
    bool depthBiasEnable = false;
    float lineWidth = 1.0f;

    inline bool operator<(const RasterizationMode& r) const
    {
      return std::tie(depthClampEnable, rasterizerDiscardEnable, polygonMode, cullMode, frontFace, depthBiasEnable, lineWidth) <
             std::tie(r.depthClampEnable, r.rasterizerDiscardEnable, r.polygonMode, r.cullMode, r.frontFace, r.depthBiasEnable, r.lineWidth);
    }
  };

  constexpr RasterizationMode FillMode{ false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, false, 1.0f };
  constexpr RasterizationMode WireframeNoCullMode{ false, false, vk::PolygonMode::eLine, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise, false, 1.0f };
  constexpr RasterizationMode WireframeMode{ false, false, vk::PolygonMode::eLine, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, false, 1.0f };
}
