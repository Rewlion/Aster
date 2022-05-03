#pragma once

#include "state.hpp"

#include <engine/utils/fixed_stack.hpp>
#include <engine/gapi/resources.h>

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  struct BackendState;
  class CompileContext;
  struct PipelineLayout;

  struct ViewportTSF
  {
    TRACKED_STATE_FIELD_ATTR(vk::Extent2D, dim);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct VertexBufferTSF
  {
    TRACKED_STATE_FIELD_ATTR(vk::Buffer, buffer);
    TRACKED_STATE_FIELD_ATTR(bool, inUse);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct IndexBufferTSF
  {
    TRACKED_STATE_FIELD_ATTR(vk::Buffer, buffer);
    TRACKED_STATE_FIELD_ATTR(bool, inUse);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct GraphicsPipelineTSF
  {
    TRACKED_STATE_FIELD_ATTR(ShaderStagesNames, shaderNames);
    TRACKED_STATE_FIELD_ATTR(PrimitiveTopology, topology);
    TRACKED_STATE_FIELD_ATTR(DepthStencilStateDescription, depthStencilState);
    TRACKED_STATE_FIELD_ATTR(BlendState, blendState);
    TRACKED_STATE_FIELD_ATTR(vk::Pipeline, pipeline);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct RenderPassTSF
  {
    TRACKED_STATE_FIELD_ATTR(RenderTargets, renderTargets);
    TRACKED_STATE_FIELD_ATTR(RenderPassAttachment, depthStencil);
    TRACKED_STATE_FIELD_ATTR(ClearState, clearing);
    TRACKED_STATE_FIELD_ATTR(vk::Rect2D, renderArea);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct PushConstantTSF
  {
    TRACKED_STATE_FIELD_ATTR(vk::ShaderStageFlagBits, stage);
    TRACKED_STATE_FIELD_ATTR(size_t, size);
    TRACKED_STATE_FIELD_ATTR_MANAGED_PTR(void*, data);

    void apply(CompileContext& ctx, BackendState& state);
  };

  struct FlushDescriptorSetsTSF
  {
    void apply(CompileContext& ctx, BackendState& state);
  };

  class GraphicsState: public TrackedState<
    GraphicsPipelineTSF,
    ViewportTSF,
    VertexBufferTSF,
    IndexBufferTSF,
    PushConstantTSF,
    FlushDescriptorSetsTSF>
  {
  };

  class RenderPassState: public TrackedState<
    RenderPassTSF>
  {
  };

  struct BackendState
  {
    GraphicsState         graphicsState;
    RenderPassState       renderPassState;

    vk::CommandBuffer     cmdBuffer;
    vk::RenderPass        renderPass;
    vk::Framebuffer       framebuffer;
    const PipelineLayout* layout;
    size_t                renderTargetsCount;
  };
}
