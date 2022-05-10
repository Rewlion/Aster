#include "graphics_state.h"

#include "compile_context.h"

#include <engine/assert.h>

namespace gapi::vulkan
{
  void ViewportTSF::apply(CompileContext& ctx, BackendState& state)
  {
    vk::Viewport vp;
    vp.x = 0;
    vp.y = 0;
    vp.width =  (float)dim.width;
    vp.height = (float)dim.height;
    vp.minDepth = 0;
    vp.maxDepth = 1;
    state.cmdBuffer.setViewport(0, 1, &vp);

    vk::Rect2D sc {{0,0}, dim};
    state.cmdBuffer.setScissor(0, 1, &sc);
  }

  void GraphicsPipelineTSF::apply(CompileContext& ctx, BackendState& state)
  {
    GraphicsPipelineDescription desc;
    desc.shaderNames = shaderNames;
    desc.topology = topology;
    desc.depthStencilState = depthStencilState;
    desc.blendState = blendState;

    pipeline = ctx.getPipeline(desc);

    bool hasLayout = ctx.getPipelineLayout(shaderNames, state.layout);
    ASSERT(hasLayout);

    state.cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    state.graphicsState.markDirty<ViewportTSF>();
  }

  void VertexBufferTSF::apply(CompileContext& ctx, BackendState& state)
  {
    if (inUse)
    {
      vk::DeviceSize offsets = {0};
      state.cmdBuffer.bindVertexBuffers(0, 1, &buffer, &offsets);
    }
  }

  void IndexBufferTSF::apply(CompileContext& ctx, BackendState& state)
  {
    if (inUse)
    {
      state.cmdBuffer.bindIndexBuffer(buffer, 0, vk::IndexType::eUint32);
    }
  }

  void PushConstantTSF::apply(CompileContext& ctx, BackendState& state)
  {
    if (size > 0 && data > 0)
    {
      const vk::PipelineLayout layout = state.layout->pipelineLayout.get();
      ASSERT(layout != vk::PipelineLayout{});

      state.cmdBuffer.pushConstants(layout, stage, 0 , size, data);
    }
  }

  void FlushDescriptorSetsTSF::apply(CompileContext& ctx, BackendState&)
  {
    ctx.updateDescriptorSets();
  }

  void RenderPassTSF::apply(CompileContext& ctx, BackendState& state)
  {
    state.renderPass = ctx.getRenderPass(renderTargets, depthStencil, clearing);
    state.framebuffer = ctx.getFramebuffer(renderArea.extent, renderTargets, depthStencil);

    std::array<uint32_t,4> clearColor{0,0,0,0};
    vk::ClearValue clearValue;
    clearValue.color = clearColor;

    vk::ClearValue clearValues[MAX_RENDER_TARGETS+1] {clearValue};
    clearValues[renderTargets.getSize()] = vk::ClearDepthStencilValue(1.0, 0.0);

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = state.renderPass;
    rpBeginInfo.framebuffer = state.framebuffer;
    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.clearValueCount = std::size(clearValues);
    rpBeginInfo.renderArea = renderArea;

    clearing = CLEAR_NONE;

    state.cmdBuffer.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    state.graphicsState.set<ViewportTSF, vk::Extent2D>(renderArea.extent);

    BlendState blendState;
    for (const auto& rt: renderTargets)
      blendState.attachments.push( {} );
    state.graphicsState.set<GraphicsPipelineTSF, BlendState>(blendState);
  }
}
