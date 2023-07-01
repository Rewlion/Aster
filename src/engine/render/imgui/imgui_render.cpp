#include "imgui_render.h"
#include "imgui.h"

#include <engine/ecs/type_meta.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>

#include <memory>

namespace Engine::Render
{
  void ImGuiRender::init()
  {
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int fontWidth, fontHeight;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &fontWidth, &fontHeight);

    gapi::TextureHandle texHandle = gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format = gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent = int3{fontWidth, fontHeight, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage = gapi::TextureUsage::TEX_USAGE_UNIFORM
      }
    );

    std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};

    encoder->transitTextureState(texHandle, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder->copyBufferToTexture(texHandle, pixels, 4 * fontWidth * fontHeight);
    encoder->transitTextureState(texHandle, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);

    std::unique_ptr<gapi::Fence> fence{gapi::allocate_fence()};
    encoder->flush(fence.get());
    fence->wait();

    m_FontTexture = texHandle;
  }

  ImGuiRender::~ImGuiRender()
  {
  }

  void ImGuiRender::beforeRender(gapi::CmdEncoder& encoder)
  {
    m_FrameGC.nextFrame();

    if (imgui::Manager::getState() == imgui::Manager::State::Disabled)
    {
      m_DrawData.imDrawData = nullptr;
      return;
    }

    ImDrawData* drawData = ImGui::GetDrawData();
    if (drawData == nullptr || drawData->TotalVtxCount <= 0)
    {
      m_DrawData.imDrawData = nullptr;
      return;
    }

    const float2 scale{
      2.0f / drawData->DisplaySize.x,
      2.0f / drawData->DisplaySize.y
    };

    const float2 translate{
      -1.0f - drawData->DisplayPos.x * scale.x,
      -1.0f - drawData->DisplayPos.y * scale.y
    };

    m_DrawData = GatherDrawData(encoder, drawData);

    const float4 scale_translate = {scale, translate};
    tfx::set_extern("imgui_scale_translate", scale_translate);
  }

  ImGuiRender::DrawData ImGuiRender::GatherDrawData(gapi::CmdEncoder& encoder, const ImDrawData* drawData)
  {
    eastl::vector<ImDrawVert> vertices;
    vertices.reserve(drawData->TotalVtxCount);
    eastl::vector<gapi::index_type> indices;
    vertices.reserve(drawData->TotalIdxCount);

    for (int i = 0; i < drawData->CmdListsCount; ++i)
    {
      ImDrawList* drawCmd = drawData->CmdLists[i];

      for (auto& v : drawCmd->VtxBuffer)
        vertices.push_back(v);

      for (auto idx : drawCmd->IdxBuffer)
        indices.push_back(idx);
    }

    const size_t verticesSize = vertices.size() * sizeof(vertices[0]);
    const size_t indicesSize = indices.size() * sizeof(indices[0]);

    DrawData res;
    res.imDrawData = drawData;
    res.vb = m_FrameGC.allocate([verticesSize](){
      return gapi::allocate_buffer(verticesSize, gapi::BF_GpuVisible | gapi::BF_BindVertex);
    });
    res.ib = m_FrameGC.allocate([indicesSize](){
      return gapi::allocate_buffer(indicesSize, gapi::BF_GpuVisible | gapi::BF_BindIndex);
    });
    res.count = indices.size();

    encoder.writeBuffer(res.vb, vertices.data(), 0, verticesSize);
    encoder.writeBuffer(res.ib, indices.data(), 0, indicesSize);

    encoder.insertGlobalBufferBarrier(gapi::BF_STATE_TRANSFER_DST,
                                      (gapi::BufferState)(gapi::BF_STATE_INDEX | gapi::BF_STATE_VERTEX));
    return res;
  }

  void ImGuiRender::render(gapi::CmdEncoder& encoder)
  { 
    if (m_DrawData.imDrawData == nullptr)
      return;

    tfx::set_extern("imgui_fontTex", m_FontTexture);
    tfx::activate_scope("ImGuiScope", encoder);
    tfx::activate_technique("ImGui", encoder);
    encoder.updateResources();

    encoder.bindIndexBuffer(m_DrawData.ib);
    encoder.bindVertexBuffer(m_DrawData.vb);

    uint32_t indexOffset = 0;
    uint32_t vertexOffset = 0;
    for (int nCmdList = 0; nCmdList < m_DrawData.imDrawData->CmdListsCount; ++nCmdList)
    {
      ImDrawList* cmdList = m_DrawData.imDrawData->CmdLists[nCmdList];
      for (int nCmd = 0; nCmd < cmdList->CmdBuffer.size(); ++nCmd)
      {
        const ImDrawCmd& cmd = cmdList->CmdBuffer[nCmd];
        encoder.drawIndexed(cmd.ElemCount, 1, indexOffset + cmd.IdxOffset, vertexOffset + cmd.VtxOffset, 0);
      }

      indexOffset += cmdList->IdxBuffer.size();
      vertexOffset += cmdList->VtxBuffer.size();
    }
  }

  DECLARE_ECS_COMPONENT(ImGuiRender, "ImGuiRender", (ecs::NonTrivialTypeManager<ImGuiRender, ecs::INITABLE_TYPE>), false, true);
}
