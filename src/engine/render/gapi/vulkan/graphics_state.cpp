#include "graphics_state.h"

#include "compile_context.h"

#include <engine/assert.h>

namespace gapi::vulkan
{
  void ViewportTSF::Apply(CompileContext& ctx, BackendState& state)
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

  void GraphicsPipelineTSF::Apply(CompileContext& ctx, BackendState& state)
  {
    GraphicsPipelineDescription desc;
    desc.shaderNames = shaderNames;
    desc.topology = topology;
    desc.depthStencilState = depthStencilState;
    desc.blendState = blendState;

    pipeline = ctx.GetPipeline(desc);

    bool hasLayout = ctx.GetPipelineLayout(shaderNames, state.layout);
    ASSERT(hasLayout);

    state.cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    state.graphicsState.MarkDirty<ViewportTSF>();
  }

  void VertexBufferTSF::Apply(CompileContext& ctx, BackendState& state)
  {
    if (inUse)
    {
      vk::DeviceSize offsets = {0};
      state.cmdBuffer.bindVertexBuffers(0, 1, &buffer, &offsets);
    }
  }

  void IndexBufferTSF::Apply(CompileContext& ctx, BackendState& state)
  {
    if (inUse)
    {
      state.cmdBuffer.bindIndexBuffer(buffer, 0, vk::IndexType::eUint32);
    }
  }

  void PushConstantTSF::Apply(CompileContext& ctx, BackendState& state)
  {
    const vk::PipelineLayout layout = state.layout->pipelineLayout.get();
    ASSERT(layout != vk::PipelineLayout{});

    state.cmdBuffer.pushConstants(layout, stage, 0 , size, data);
  }

  void FlushDescriptorSetsTSF::Apply(CompileContext& ctx, BackendState&)
  {
    ctx.UpdateDescriptorSets();
  }

  void RenderPassTSF::Apply(CompileContext& ctx, BackendState& state)
  {
    state.renderPass = ctx.GetRenderPass(renderTargets, depthStencil, clearing);
    state.framebuffer = ctx.GetFramebuffer(renderArea.extent, renderTargets, depthStencil);

    std::array<uint32_t,4> clearColor{0,0,0,0};
    vk::ClearValue clearValue;
    clearValue.color = clearColor;
    clearValue.depthStencil = vk::ClearDepthStencilValue(0,0);

    vk::ClearValue clearValues[MAX_RENDER_TARGETS+1] {clearValue};

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = state.renderPass;
    rpBeginInfo.framebuffer = state.framebuffer;
    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.clearValueCount = std::size(clearValues);
    rpBeginInfo.renderArea = renderArea;

    clearing = CLEAR_NONE;

    state.cmdBuffer.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    state.graphicsState.Set<ViewportTSF, vk::Extent2D>(renderArea.extent);
  }
}
