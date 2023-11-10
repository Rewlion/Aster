#pragma once

#include <engine/gapi/cmd_encoder.h>
#include <engine/utils/macros.h>

#include <tracy/Tracy.hpp>

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


#ifdef TRACY_ENABLE
#define PROFILE_CPU() ZoneScoped;
#define PROFILE_CPU_NAMED(name) ZoneScopedN(name);
#define PROFILE_CPU_TNAMED(name) ZoneTransientN(NAME_WITH_LINE(cpuProfile), name, true);
#define PROFILE_GAPI_TNAMED(name, encoder)\
  GAPI_MARK(name, encoder)\
  PROFILE_CPU_TNAMED(name)\
  std::unique_ptr<gapi::GpuSectionProfileScope> NAME_WITH_LINE(gpuSectionProfile) {\
    (encoder).beginScopedProfileSection(__LINE__, __FILE__, std::strlen(__FILE__), __FUNCTION__, std::strlen(__FUNCTION__), name, std::strlen(name))\
  };\

#else
#define PROFILE_CPU() 
#define PROFILE_CPU_NAMED(name) 
#define PROFILE_CPU_TNAMED(name) 
#define PROFILE_GAPI(name, encoder)  GAPI_MARK(name, encoder)
#endif
