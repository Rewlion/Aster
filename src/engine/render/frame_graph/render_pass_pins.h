#pragma once

#include "handles.h"

#include <engine/gapi/resources.h>

#include <EASTL/vector.h>

namespace fg
{
  struct TextureState
  {
    VirtualResourceHandle vResId;
    gapi::TextureState beginState;
  };

  struct RenderTarget
  {
    VirtualResourceHandle vResId;
    gapi::LoadOp  loadOp = gapi::LoadOp::DontCare;
    gapi::StoreOp storeOp = gapi::StoreOp::DontCare;;
  };

  struct DepthStencil
  {
    VirtualResourceHandle vResId = VirtualResourceHandle::Invalid;
    gapi::LoadOp  depthLoadOp = gapi::LoadOp::DontCare;
    gapi::StoreOp depthStoreOp = gapi::StoreOp::DontCare;
    gapi::LoadOp  stencilLoadOp = gapi::LoadOp::DontCare;
    gapi::StoreOp stencilStoreOp = gapi::StoreOp::DontCare;;
  };

  struct RenderPassPins
  {
    inline VirtualResourceHandle write(const VirtualResourceHandle h)
    {
      return hasWrite(h) ? h : writes.emplace_back(h);
    }

    inline VirtualResourceHandle read(const VirtualResourceHandle h)
    {
      return hasRead(h) ? h : reads.emplace_back(h);
    }

    inline VirtualResourceHandle create(const VirtualResourceHandle h)
    {
      return hasCreate(h) ? h : creates.emplace_back(h);
    }

    inline bool hasWrite(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)writes.size();
    }

    inline bool hasRead(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)reads.size();
    }

    inline bool hasCreate(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)creates.size();
    }

    inline void demandTextureState(const VirtualResourceHandle h, const gapi::TextureState begin_state)
    {
      beginTextureStates.push_back(TextureState{
        .vResId = h,
        .beginState = begin_state
      });
    }

    inline void addRenderTarget(const VirtualResourceHandle resource, const gapi::LoadOp load, const gapi::StoreOp store)
    {
      renderTargets.push_back(RenderTarget{
        .vResId = resource,
        .loadOp = load,
        .storeOp = store
      });
    }

    inline void setDepthStencil(const VirtualResourceHandle resource, const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                                                                      const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store)
    {
      depthStencil = DepthStencil{
        .vResId = resource,
        .depthLoadOp = depth_load,
        .depthStoreOp = depth_store,
        .stencilLoadOp = stencil_load,
        .stencilStoreOp = stencil_store
      };
    }

    inline void setSideEffect()
    {
      sideEffect = true;
    }

    inline bool hasSideEffect()
    {
      return sideEffect;
    }

    bool sideEffect = false;
    eastl::vector<VirtualResourceHandle> writes;
    eastl::vector<VirtualResourceHandle> reads;
    eastl::vector<VirtualResourceHandle> creates;

    eastl::vector<TextureState> beginTextureStates;
    eastl::vector<RenderTarget> renderTargets;
    DepthStencil depthStencil;
  };
}