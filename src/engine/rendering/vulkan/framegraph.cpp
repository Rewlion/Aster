#include "framegraph.h"
#include "core.h"

namespace RHI::Vulkan
{
  SubpassInput::SubpassInput(const ResourceId& id, vk::ImageLayout layout)
    : id(id)
    , layout(layout)
  {
  }

  RenderSubpass::RenderSubpass(unsigned int id)
    : id(id)
  {
  }

  RenderSubpass& RenderSubpass::AddInputAttachment(const SubpassInput& desc)
  {
    inputAttachments.push_back(desc);

    return *this;
  }

  RenderSubpass& RenderSubpass::AddInputSampler(const ResourceId& id)
  {
    inputSamplers.push_back(id);

    return *this;
  }

  RenderSubpass& RenderSubpass::AddInputBuffer()
  {
    return *this;
  }

  RenderSubpass& RenderSubpass::AddNewOutputColorAttachment(const ResourceId& id, vk::Format format, const vk::PipelineColorBlendAttachmentState& blendState)
  {
    outputColorAttachments.push_back(id);

    outputAttachmentBlendStates.push_back(blendState);

    ImageAttachment createInfo;
    createInfo.id = id;
    createInfo.format = format;
    createInfo.type = ImageType::OutputColorAttachment;
    createInfo.initialLayout = vk::ImageLayout::eUndefined;
    createInfo.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
    createInfo.loadOp = vk::AttachmentLoadOp::eClear;
    createInfo.storeOp = vk::AttachmentStoreOp::eStore;
    createInfo.usageFlags = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment;

    imageAttachmentCreateInfos.push_back(createInfo);

    return *this;
  }

  RenderSubpass& RenderSubpass::AddExistOutputColorAttachment(const ResourceId& id, const vk::PipelineColorBlendAttachmentState& blendState)
  {
    outputColorAttachments.push_back(id);

    outputAttachmentBlendStates.push_back(blendState);

    return *this;
  }

  RenderSubpass& RenderSubpass::AddDepthOnlyAttachment(const ResourceId& id)
  {
    ImageAttachment createInfo;
    createInfo.id = id;
    createInfo.format = vk::Format::eD32Sfloat;
    createInfo.type = ImageType::DepthOnlyAttachment;
    createInfo.initialLayout = vk::ImageLayout::eUndefined;
    createInfo.finalLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    createInfo.loadOp = vk::AttachmentLoadOp::eClear;
    createInfo.storeOp = vk::AttachmentStoreOp::eStore;
    createInfo.usageFlags = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eInputAttachment;

    depthStencilAttachment = createInfo;

    return *this;
  }

  RenderSubpass& RenderSubpass::AddDepthStencilAttachment(const ResourceId& id)
  {
    ImageAttachment createInfo;
    createInfo.id = id;
    createInfo.format = vk::Format::eD32SfloatS8Uint;
    createInfo.type = ImageType::DepthStencilAttachment;
    createInfo.initialLayout = vk::ImageLayout::eUndefined;
    createInfo.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    createInfo.loadOp = vk::AttachmentLoadOp::eClear;
    createInfo.storeOp = vk::AttachmentStoreOp::eStore;
    createInfo.usageFlags = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eInputAttachment;

    depthStencilAttachment = createInfo;

    return *this;
  }

  RenderSubpass& RenderSubpass::AddOutputBuffer()
  {
    return *this;
  }

  RenderSubpass& RenderSubpass::AddOutputSampler()
  {
    return *this;
  }

  RenderSubpass& RenderSubpass::SetRenderCallback(RenderPassExecutionFunction callback)
  {
    renderCallback = callback;
    return *this;
  }

  RenderGraph::RenderGraph(Core& core)
    : core(core)
  {
  }

  RenderSubpass& RenderGraph::AddRenderSubpass()
  {
    const unsigned id = static_cast<unsigned int>(subpasses.size());
    subpasses.push_back(RenderSubpass{ id });

    return subpasses.back();
  }

  void RenderGraph::Compile()
  {
    AllocateSubpassesResources();

    this->renderPass = CreateRenderpass();
    this->framebuffer = CreateFramebuffer();
  }

  std::vector<vk::SubpassDependency> RenderGraph::GetAttachmentDependencies()
  {
    std::vector<vk::SubpassDependency> deps;

    std::map<ResourceId, SubpassId> ResourceIdToSubpassProducerMap;

    for (const RenderSubpass& subpass : subpasses)
    {
      for (const ResourceId& outputAttachmentId : subpass.outputColorAttachments)
      {
        ResourceIdToSubpassProducerMap[outputAttachmentId] = subpass.id;
      }
    }

    for (const RenderSubpass& subpass : subpasses)
    {
      std::set<SubpassId> parents;
      for (const SubpassInput& inputAttachment : subpass.inputAttachments)
      {
        parents.insert(ResourceIdToSubpassProducerMap[inputAttachment.id]);
      }

      for (SubpassId parentId : parents)
      {
        const SubpassId src = subpass.id < parentId ? subpass.id : parentId;
        const SubpassId dst = subpass.id > parentId ? subpass.id : parentId;

        const auto dep = vk::SubpassDependency()
          .setSrcSubpass(src)
          .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
          .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
          .setDstSubpass(dst)
          .setDstAccessMask(vk::AccessFlagBits::eInputAttachmentRead)
          .setDstStageMask(vk::PipelineStageFlagBits::eFragmentShader);

        deps.push_back(dep);
      }
    }

    return deps;
  }

  void RenderGraph::AddAttachmentResource(const ImageAttachment& attachment)
  {
    if (resourceIdToAttachmentIdMap.find(attachment.id) != resourceIdToAttachmentIdMap.end())
      throw std::runtime_error("AddAttachmentResource: resource with such id already exist");

    const AttachmentId attId = static_cast<AttachmentId>(imageAttachments.size());
    imageAttachments.push_back(attachment);
    resourceIdToAttachmentIdMap[attachment.id] = attId;
  }

  void RenderGraph::SetBackbufferDescription(const BackbufferDescription& bfDescription)
  {
    backbufferDescription = bfDescription;
  }

  void RenderGraph::SetCommandBuffer(vk::CommandBuffer cmdBuf)
  {
    cmdBuffer = cmdBuf;
  }

  void RenderGraph::SetUniformsAccessorStorage(UniformsAccessorStorage* s)
  {
    uaStorage = s;
  }

  void RenderGraph::Reset()
  {
    subpasses.clear();
    resourceIdToAttachmentIdMap.clear();
    imageAttachments.clear();
    ownedImages.clear();
  }

  void RenderGraph::Execute()
  {
    std::vector<vk::ClearValue> clearColors = GetClearColorsForImageAttachments();

    const auto rpBeginInfo = vk::RenderPassBeginInfo()
      .setRenderPass(renderPass)
      .setFramebuffer(framebuffer)
      .setRenderArea(vk::Rect2D{ {0,0}, backbufferDescription.size })
      .setClearValueCount(clearColors.size())
      .setPClearValues(clearColors.data());

    cmdBuffer.begin(vk::CommandBufferBeginInfo());
    cmdBuffer.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    FrameContext context;
    context.BackbufferSize = backbufferDescription.size;
    context.uniformsAccessorStorage = uaStorage;
    context.pipelineStorage = &core.GetPipelineStorage();
    context.framebuffer = framebuffer;
    context.renderPass = renderPass;
    context.commandBuffer = cmdBuffer;
    context.renderGraph = this;

    for (int i = 0; i < subpasses.size(); ++i)
    {
      const RenderSubpass& subpass = subpasses[i];

      context.subpassNumber = subpass.id;
      context.outputAttachmentBlendStates = subpass.outputAttachmentBlendStates;

      subpass.renderCallback(context);

      if (i != (subpasses.size() - 1))
        cmdBuffer.nextSubpass(vk::SubpassContents::eInline);
    }

    cmdBuffer.endRenderPass();
    cmdBuffer.end();
  }

  std::vector<vk::ClearValue> RenderGraph::GetClearColorsForImageAttachments() const
  {
    std::vector<vk::ClearValue> clearColors;
    for (const ImageAttachment& img : imageAttachments)
    {
      vk::ClearValue clearValue;

      if (img.type == ImageType::DepthOnlyAttachment || img.type == ImageType::DepthStencilAttachment || img.type == ImageType::StencilOnlyAttachment)
        clearValue = vk::ClearDepthStencilValue(1.0f, 0.0f);
      else
        clearValue = vk::ClearColorValue{ std::array<float,4>{ 0.5529f, 0.6f, 0.6823f, 0.0f} };

      clearColors.push_back(clearValue);
    }

    return clearColors;
  }

  vk::RenderPass RenderGraph::CreateRenderpass()
  {
    //describe subbpasses and it's attachment references
    std::vector<SubpassKey> subpassKeys;
    for (const RenderSubpass& subpass : subpasses)
    {
      SubpassKey subkey;

      for (const SubpassInput& inputDesc : subpass.inputAttachments)
      {
        AttachmentId attId = resourceIdToAttachmentIdMap[inputDesc.id];

        subkey.inputAttachmentReferences.push_back(
          vk::AttachmentReference()
          .setAttachment(attId)
          .setLayout(inputDesc.layout)
        );
      }

      for (const ResourceId& outputId : subpass.outputColorAttachments)
      {
        AttachmentId attId = resourceIdToAttachmentIdMap[outputId];

        subkey.outputColorAttachmentReferences.push_back(
          vk::AttachmentReference()
          .setAttachment(attId)
          .setLayout(vk::ImageLayout::eColorAttachmentOptimal)
        );
      }

      if (subpass.depthStencilAttachment.has_value())
      {
        const ResourceId& id = subpass.depthStencilAttachment.value().id;
        AttachmentId attId = resourceIdToAttachmentIdMap[id];

        subkey.depthStencilAttachmentReference = vk::AttachmentReference()
          .setAttachment(attId)
          .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
      }

      subpassKeys.push_back(std::move(subkey));
    }

    std::vector<vk::SubpassDependency> deps = GetAttachmentDependencies();

    const auto rpKey = RenderPassKey()
      .SetBackbufferFormat(backbufferDescription.format)
      .SetDependencies(deps)
      .SetSubpassesDescriptions(subpassKeys)
      .SetImageAttachments(imageAttachments);

    return core.GetRenderPassStorage().GetRenderPass(rpKey);
  }

  vk::Framebuffer RenderGraph::CreateFramebuffer()
  {
    std::vector<vk::ImageView> views;
    views.reserve(imageAttachments.size());

    for (const ImageAttachment& img : imageAttachments)
      views.push_back(img.view.Get());

    const auto fbKey = FramebufferKey()
      .SetAttachments(views)
      .SetHeight(backbufferDescription.size.height)
      .SetWidth(backbufferDescription.size.width)
      .SetLayers(1)
      .SetRenderPass(renderPass);

    return core.GetFramebufferStorage().GetFramebuffer(fbKey);
  }

  void RenderGraph::AllocateSubpassesResources()
  {
    for (const RenderSubpass& subpass : subpasses)
    {
      for (ImageAttachment createInfo : subpass.imageAttachmentCreateInfos)
      {
        if (resourceIdToAttachmentIdMap.find(createInfo.id) != resourceIdToAttachmentIdMap.end())
          throw std::runtime_error("AllocateSubpassesResources: can't add a new resource: index already in use.");

        if (createInfo.format == vk::Format::eUndefined)
          createInfo.format = backbufferDescription.format;

        Image img = core.Allocate2DImage(createInfo.format, backbufferDescription.size, createInfo.usageFlags);
        createInfo.view = img.GetView();
        AddAttachmentResource(createInfo);
        ownedImages.push_back(std::move(img));
      }

      if (subpass.depthStencilAttachment.has_value())
      {
        ImageAttachment createInfo = subpass.depthStencilAttachment.value();
        assert(createInfo.type == ImageType::DepthOnlyAttachment || createInfo.type == ImageType::DepthStencilAttachment);

        if (resourceIdToAttachmentIdMap.find(createInfo.id) != resourceIdToAttachmentIdMap.end())
          throw std::runtime_error("AllocateSubpassesResources: can't add a new depth|stencil resource: index already in use.");

        const vk::ImageAspectFlags aspectFlags = (createInfo.type == ImageType::DepthOnlyAttachment)
          ? vk::ImageAspectFlagBits::eDepth
          : vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;

        Image img = core.AllocateDepthStencilImage(createInfo.format, backbufferDescription.size, createInfo.usageFlags, aspectFlags);
        createInfo.view = img.GetView();
        AddAttachmentResource(createInfo);
        ownedImages.push_back(std::move(img));
      }
    }
  }

  const ImageView& RenderGraph::GetImageView(const ResourceId& id) const
  {
    const AttachmentId attId = resourceIdToAttachmentIdMap.at(id);
    const ImageAttachment& attachment = imageAttachments[attId];

    return attachment.view;
  }
}