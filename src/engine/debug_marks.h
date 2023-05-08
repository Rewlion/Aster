#pragma once

#include <engine/gapi/cmd_encoder.h>
#include <engine/utils/macros.h>

class ScopeDbgMarkerRegion
{
  public:
    ScopeDbgMarkerRegion(const char* name, gapi::CmdEncoder& encoder)
      : m_Encoder(encoder)
    {
      m_Encoder.beginMarkerRegion(name);
    }

    ~ScopeDbgMarkerRegion()
    {
      m_Encoder.endMarkerRegion();
    }

  private:
    gapi::CmdEncoder& m_Encoder;
};

#define GAPI_MARK(name, encoder)\
  ScopeDbgMarkerRegion NAME_WITH_LINE(dbgMarkerRegion_){name, encoder};