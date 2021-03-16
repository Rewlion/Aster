#pragma once

namespace Vulkan
{
  enum class ImageType
  {
    Present,
    DepthOnlyAttachment,
    StencilOnlyAttachment,
    DepthStencilAttachment,
    SubpassInput,
    OutputColorAttachment
  };
}
