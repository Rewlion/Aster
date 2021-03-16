#pragma once

#include "common.h"

#include <map>
#include <vector>
#include <tuple>
#include <memory>
#include <optional>

namespace Vulkan
{
  class Core;

  struct SubpassKey
  {
    bool operator<(const SubpassKey& r) const;

    std::vector<vk::AttachmentReference> inputAttachmentReferences;
    std::vector<vk::AttachmentReference> outputColorAttachmentReferences;
    std::optional<vk::AttachmentReference> depthStencilAttachmentReference;
  };

  class RenderPassKey
  {
    friend class RenderPassStorage;
  public:
    RenderPassKey& SetSubpassesDescriptions(const std::vector<SubpassKey>& descs);

    RenderPassKey& SetDependencies(const std::vector<vk::SubpassDependency>& deps);

    RenderPassKey& SetImageAttachments(const std::vector<ImageAttachment>& a);

    RenderPassKey& SetBackbufferFormat(const vk::Format& f);

    bool operator<(const RenderPassKey& r) const;

  private:
    std::vector<SubpassKey> subpassesDescriptions;
    std::vector<vk::SubpassDependency> dependencies;
    std::vector<ImageAttachment> imageAttachments;
    vk::Format backbufferFormat;
  };

  class RenderPassStorage
  {
  public:
    RenderPassStorage(Core& core);

    vk::RenderPass GetRenderPass(const RenderPassKey& key);

  private:
    Core& core;
    std::map<RenderPassKey, vk::UniqueRenderPass> renderPasses;
  };
}