#include "render_state.h"

#include <engine/components/atmosphere.h>
#include <engine/components/sun.h>
#include <engine/math.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/work_cycle/camera.h>

ECS_COMP_GETTER(AtmosphereRenderState, atmosphere_render_state);
ECS_COMP_GETTER(AtmosphereComponent, atmosphere);
ECS_COMP_GETTER(SunComponent, sun);

#define GET_ATM(res) [](){ return get_atmosphere_render_state()->get##res(); }

NODE_BEGIN(atm_resources)
  ORDER_ME_AFTER(frame_preparing)
  READ_BLOB(camera_data, Engine::CameraData)
  
  IMPORT_TEX(atm_tr_lut,        GET_ATM(TrLut))
  IMPORT_TEX(atm_ms_lut,        GET_ATM(MsLut))
  IMPORT_TEX(atm_sky_lut,       GET_ATM(SkyLut))
  IMPORT_TEX(atm_ap_lut,        GET_ATM(ApLut))
  IMPORT_TEX(atm_envi_specular, GET_ATM(EnviSpecular))
  IMPORT_TEX(atm_envi_brdf,     GET_ATM(EnviBRDFLut))

  CREATE_BLOB(atm_envi_mips, int)
  CREATE_BLOB(sun_azimuth_altitude, float2)

  EXEC(atm_resources_exec)
NODE_END()

NODE_EXEC()
static
void atm_resources_exec(gapi::CmdEncoder& encoder,
                         int& atm_envi_mips,
                         const Engine::CameraData& camera_data,
                         float2& sun_azimuth_altitude,
                         const fg::TextureRequest& atm_envi_specular)
{
  atm_envi_mips = atm_envi_specular.describe().mipLevels;

  const AtmosphereComponent* atmosphere = get_atmosphere();
  const SunComponent* sun = get_sun();
  if (!atmosphere || !sun)
    return;

  const float azimuth = sun->getAzimuth();
  const float altitude = sun->getAltitude();

  sun_azimuth_altitude = float2(azimuth, altitude);

  float4 sunAzimuth_sunAltitude_rTopMM_rBotMM{
    math::radians(azimuth),
    math::radians(altitude),
    atmosphere->getTopRadiusKm() / 1000.0,
    atmosphere->getBotRadiusKm() / 1000.0
  };

  float cameraH = sunAzimuth_sunAltitude_rTopMM_rBotMM.w + camera_data.pos.y;
  cameraH = (cameraH < 10.0 ? 10.0 : cameraH) / 1e6;

  tfx::set_extern("atmPosMM", float3(0.0f, sunAzimuth_sunAltitude_rTopMM_rBotMM.w + cameraH, 0.0f));
  tfx::set_extern("maxAerialDist_mm", 0.032f);
  tfx::set_channel("sunAzimuth_sunAltitude_rTopMM_rBotMM", sunAzimuth_sunAltitude_rTopMM_rBotMM);
  tfx::activate_scope("AtmosphereScope", encoder);
}
