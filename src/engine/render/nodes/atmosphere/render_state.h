#pragma once

#include <engine/gapi/gapi.h>
#include <engine/gapi/resource_wrappers.h>

namespace ed
{
  class Scope;
}

enum class AtmosphereLutState : int
{
  Preparing = 0,
  Ready = 1
};

class AtmosphereRenderState
{
  public:
    AtmosphereRenderState() = default;
    AtmosphereRenderState(const ed::Scope* init) {}

    void init();

    auto getTrLut() -> gapi::TextureViewWithState* { return &m_TrLut; }
    auto getMsLut() -> gapi::TextureViewWithState* { return &m_MsLut; }
    auto getSkyLut() -> gapi::TextureViewWithState* { return &m_SkyLut; }
    auto getApLut() -> gapi::TextureViewWithState* { return &m_ApLut; }
    auto getEnviBRDFLut() -> gapi::TextureViewWithState* { return &m_EnviBRDFLut; }
    auto getEnviSpecular() -> gapi::TextureViewWithState* { return &m_EnviSpecular; }

  private:
    AtmosphereLutState m_LutState;
    gapi::UniqueTextureWithState m_TrLut;
    gapi::UniqueTextureWithState m_MsLut;
    gapi::UniqueTextureWithState m_SkyLut;
    gapi::UniqueTextureWithState m_ApLut;
    gapi::UniqueTextureWithState m_EnviBRDFLut;
    gapi::UniqueTextureWithState m_EnviSpecular;
};