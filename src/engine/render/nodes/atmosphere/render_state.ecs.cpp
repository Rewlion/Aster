#include "render_state.h"

#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>

#include <engine/shaders/shaders/atmosphere/consts.hlsl>

ECS_COMP_GETTER(AtmosphereRenderState, atmosphere_render_state)

ECS_EVENT_SYSTEM()
void on_sun_orientation_changed(const Engine::SunOrientationChanged& evt,
                                AtmosphereRenderState& atmosphere_render_state)
{
  atmosphere_render_state.markDirty();
}

auto AtmosphereRenderState::isDirty() -> bool
{
  if (auto* st = get_atmosphere_render_state())
    return st->m_LutState == AtmosphereLutState::Preparing;

  return false;
}

void AtmosphereRenderState::init()
{
  m_LutState = AtmosphereLutState::Preparing;

  m_TrLut = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(TR_LUT_SIZE, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
        .name =            "atm_tr_lut"
      }
    ),
    gapi::TextureState::Undefined
  };

  m_MsLut = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(MS_LUT_SIZE, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
        .name =             "atm_ms_lut"
      }
    ),
    gapi::TextureState::Undefined
  };

  m_SkyLut = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(SKY_LUT_SIZE, 1),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV),
        .name =            "atm_sky_lut"
      }
    ),
    gapi::TextureState::Undefined
  };

  m_ApLut = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(AP_LUT_SIZE),
        .mipLevels =       1,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_SRV | gapi::TextureUsage::TEX_USAGE_UAV)
      }
    ),
    gapi::TextureState::Undefined
  };

  const uint enviMips = std::min((uint)std::bit_width((uint)ENVI_SPECULAR_LUT_SIZE.x), 5u);
  m_EnviBRDFLut = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(ENVI_BRDF_SIZE, 1),
        .mipLevels =       enviMips,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      }
    ),
    gapi::TextureState::Undefined
  };

  m_EnviSpecular = {
    gapi::allocate_texture(
      gapi::TextureAllocationDescription{
        .format =          gapi::TextureFormat::R16G16B16A16_SFLOAT,
        .extent =          uint3(ENVI_SPECULAR_LUT_SIZE, 1),
        .mipLevels =       enviMips,
        .arrayLayers =     1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage =           (gapi::TextureUsage)(gapi::TextureUsage::TEX_USAGE_RT | gapi::TextureUsage::TEX_USAGE_SRV)
      }
    ),
    gapi::TextureState::Undefined
  };
}

DECLARE_INITABLE_ECS_OBJECT_COMPONENT(AtmosphereRenderState);