#pragma once

#include "common.h"
#include "image.h"
#include "enums.h"
#include "framecontext.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <vector>
#include <queue>
#include <map>
#include <set>
#include <optional>

namespace Vulkan
{
  class Core;
  class PipelineStorage;
  class FramebufferStorage;
  class RenderPassStorage;
  class UniformsAccessorStorage;

  typedef unsigned int SubpassId;
  typedef unsigned int AttachmentId;
  
  constexpr vk::ColorComponentFlags ColorWriteAll = vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB;

  #define BACKBUFFER_RESOURCE_ID Vulkan::ResourceId("__backbuffer_resource")

  struct SubpassInput
  {
    SubpassInput(const ResourceId& id, vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal);

    ResourceId id;
    vk::ImageLayout layout;
  };

  struct DepthStencilAttachmentDescription
  {
    ResourceId id;
  };

  struct OutputColorAttachmentDescription
  {
    ResourceId id;
  };

  class ITask
  {
  public:
    virtual void Execute() = 0;
  };

  typedef std::function<void(FrameContext&)> RenderPassExecutionFunction;

  enum class SubpassDependencyType
  {
    //write to the same output resource 
    Write,
  };

  enum class SubpassResourceType
  {
    OutputColorAttachment
  };

  // Rendering task
  // describes a unique subpass of Render Pass
  class RenderSubpass
  {
    friend class RenderGraph;
  public:
    RenderSubpass(unsigned int id);

    RenderSubpass& AddInputAttachment(const SubpassInput& desc);

    RenderSubpass& AddInputSampler(const ResourceId& id);

    RenderSubpass& AddInputBuffer();

    RenderSubpass& AddNewOutputColorAttachment(const ResourceId& id, vk::Format format = vk::Format::eUndefined, 
                                               const vk::PipelineColorBlendAttachmentState& blendState = vk::PipelineColorBlendAttachmentState{}
                                                  .setColorWriteMask(ColorWriteAll)
                                              );

    RenderSubpass& AddExistOutputColorAttachment(const ResourceId& id,
                                                 const vk::PipelineColorBlendAttachmentState& blendState = vk::PipelineColorBlendAttachmentState{}
                                                   .setColorWriteMask(ColorWriteAll)
                                                );

    RenderSubpass& AddDepthOnlyAttachment(const ResourceId& id);

    RenderSubpass& AddDepthStencilAttachment(const ResourceId& id);

    RenderSubpass& AddOutputBuffer();

    RenderSubpass& AddOutputSampler();

    RenderSubpass& SetRenderCallback(RenderPassExecutionFunction callback);

    inline unsigned int GetId() const
    {
      return id;
    }

    std::optional<SubpassResourceType> GetResourceTypeFromId(const ResourceId& resourceId) const;

  private:
    const unsigned int id;
    RenderPassExecutionFunction renderCallback;
    
    std::vector<ResourceId> inputSamplers;
    std::vector<SubpassInput> inputAttachments;
    std::vector<ResourceId> outputColorAttachments;
    std::vector<vk::PipelineColorBlendAttachmentState> outputAttachmentBlendStates;

    std::vector<ImageAttachment> imageAttachmentCreateInfos;
    std::optional<ImageAttachment> depthStencilAttachment;
  };

  struct BackbufferDescription
  {
    vk::Format format;
    vk::Extent2D size;

    BackbufferDescription& SetFormat(vk::Format f)
    {
      format = f;
      return *this;
    }

    BackbufferDescription& SetSize(const vk::Extent2D& s)
    {
      size = s;
      return *this;
    }
  };

  class RenderGraph
  {
  public:
    RenderGraph(Core& core);

    RenderSubpass& AddRenderSubpass();

    void Compile();

    void AddAttachmentResource(const ImageAttachment& attachment);

    void SetBackbufferDescription(const BackbufferDescription& bfDescription);

    void SetCommandBuffer(vk::CommandBuffer cmdBuf);

    void SetUniformsAccessorStorage(UniformsAccessorStorage* s);

    void Reset();

    void Execute();

    std::vector<vk::ClearValue> GetClearColorsForImageAttachments() const;

    const ImageView& GetImageView(const ResourceId& id) const;

    void AddDependencyFromOutputResource(const unsigned int srcId, const unsigned int dstId, const ResourceId& resourceId, SubpassDependencyType dependencyType);

  private:
    std::vector<vk::SubpassDependency> GetSubpassDependencies() const;
    std::vector<vk::SubpassDependency> GetAttachmentDependencies() const;

    vk::RenderPass CreateRenderpass();

    vk::Framebuffer CreateFramebuffer();

    void AllocateSubpassesResources();

  private:
    Core& core;

    UniformsAccessorStorage* uaStorage;
    vk::CommandBuffer cmdBuffer;
    BackbufferDescription backbufferDescription;
    std::vector<RenderSubpass> subpasses;

    std::map<ResourceId, AttachmentId> resourceIdToAttachmentIdMap;
    std::vector<ImageAttachment> imageAttachments;

    std::vector<Image> ownedImages;
    std::vector<vk::SubpassDependency> m_ManualSetDependency;

    vk::RenderPass renderPass;
    vk::Framebuffer framebuffer;
  };


}