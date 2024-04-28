#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_envi_specular)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)

  MODIFY_TEX_RT(atm_envi_specular)
  READ_BLOB(atm_envi_mips, int)

  EXEC(atm_envi_specular_exec)
NODE_END()

NODE_EXEC()
static
void atm_envi_specular_exec(gapi::CmdEncoder& encoder,
                            const gapi::TextureHandle atm_envi_specular,
                            const int atm_envi_mips)
{
  tfx::set_extern("enviMips", (float)atm_envi_mips);

  for (int i = 0; i < atm_envi_mips; ++i)
  {
    gapi::RenderTargets mrt {gapi::RenderPassAttachment{
      .texture = atm_envi_specular,
      .mipLevel = (uint32_t)i,
      .loadOp = gapi::LoadOp::DontCare
    }};
  
    encoder.beginRenderpass(mrt, {});

    float roughness = 1.0f / (float)atm_envi_mips * i;
    tfx::set_extern("enviRoughness", roughness);
    tfx::set_extern("renderSize", encoder.getRenderSize());

    tfx::activate_technique("EnviSpecular", encoder);
    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);

    encoder.endRenderpass();
  }
}
