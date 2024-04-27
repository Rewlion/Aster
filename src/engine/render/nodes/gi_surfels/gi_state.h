#pragma once

#include <engine/gapi/resource_wrappers.h>

namespace ed
{
  class Scope;
}

class GIOnSurfels
{
  public:
    GIOnSurfels() = default;
    GIOnSurfels(const GIOnSurfels&) {}
    GIOnSurfels(const ed::Scope*) {}

    void init();
    
    void markInitialized() { m_Initialized = true; }
    void markDirty() { m_Initialized = false; }
    auto isInitialized() const -> bool { return m_Initialized; }

    auto getSurfelsLifetime() -> gapi::BufferViewWithState* { return &m_SurfelsLifetime; }
    auto getSurfelsStorage() -> gapi::BufferViewWithState* { return &m_SurfelsStorage; }
    auto getSurfelsPool() -> gapi::BufferViewWithState* { return &m_SurfelsPool; }
    auto getSurfelsMeta() -> gapi::BufferViewWithState* { return &m_SurfelsMeta; }
    auto getNonlinearAabbs() -> gapi::BufferViewWithState* { return &m_NonlinearAabbs; }
    auto getRayguidingMap() -> gapi::TextureViewWithState* { return &m_RayguidingMap; }

  private:
    bool m_Initialized;
    gapi::UniqueBufferWithState m_SurfelsLifetime;
    gapi::UniqueBufferWithState m_SurfelsStorage;
    gapi::UniqueBufferWithState m_SurfelsPool;
    gapi::UniqueBufferWithState m_SurfelsMeta;
    gapi::UniqueBufferWithState m_NonlinearAabbs;
    gapi::UniqueTextureWithState m_RayguidingMap;
};
