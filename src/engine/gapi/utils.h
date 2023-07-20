#pragma once

#include "resources.h"

namespace gapi
{
  inline
  auto to_string(const PipelineType type) -> const char*
  {
    switch (type)
    {
      case PipelineType::Graphics: return "Graphics";
      case PipelineType::Compute: return "Compute";
      default: return "<unknown pipeline type>";
    }
  }

  inline
  auto to_string(const TextureState state) -> const char*
  {
    switch (state)
    {
      case TextureState::Undefined: return "Undefined";
      case TextureState::DepthRead: return "DepthRead";
      case TextureState::DepthWrite: return "DepthWrite";
      case TextureState::StencilRead: return "StencilRead";
      case TextureState::StencilWrite: return "StencilWrite";
      case TextureState::DepthStencilRead: return "DepthStencilRead";
      case TextureState::DepthStencilWrite: return "DepthStencilWrite";
      case TextureState::DepthReadStencilRead: return "DepthReadStencilRead";
      case TextureState::DepthWriteStencilRead: return "DepthWriteStencilRead";
      case TextureState::DepthReadStencilWrite: return "DepthReadStencilWrite";
      case TextureState::DepthWriteStencilWrite: return "DepthWriteStencilWrite";
      case TextureState::ShaderRead: return "ShaderRead";
      case TextureState::ShaderReadWrite: return "ShaderReadWrite";
      case TextureState::RenderTarget: return "RenderTarget";
      case TextureState::Present: return "Present";
      case TextureState::TransferSrc: return "TransferSrc";
      case TextureState::TransferDst: return "TransferDst";
      default: return "<unknown>";
    }
  }

  inline
  auto is_depth_rp_state(const TextureState state) -> bool
  {
    switch (state)
    {
      case TextureState::DepthRead:
      case TextureState::DepthWrite:
      case TextureState::StencilRead:
      case TextureState::StencilWrite:
      case TextureState::DepthStencilRead:
      case TextureState::DepthStencilWrite:
      case TextureState::DepthReadStencilRead:
      case TextureState::DepthWriteStencilRead:
      case TextureState::DepthReadStencilWrite:
      case TextureState::DepthWriteStencilWrite:
        return true;
      default:
        return false;
    }
  }
}
