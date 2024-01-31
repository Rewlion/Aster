#include <engine/render/frame_graph/dsl.h>

static
void generate_base_mip(gapi::CmdEncoder& encoder,
                       const uint2& render_size,
                       const gapi::TextureHandle late_opaque_depth,
                       const gapi::TextureHandle hi_z_buffer)
{
  tfx::set_extern("hi_z_src", late_opaque_depth);
  tfx::set_extern("hi_z_dst", hi_z_buffer);
  tfx::activate_scope("HiZBaseScope", encoder);
  tfx::activate_technique("HiZBase", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("HiZBase", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, 1);
  encoder.transitTextureState(hi_z_buffer, gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite);
}                        

static
void generate_high_mips(gapi::CmdEncoder& encoder,
                        const uint2& render_size,
                        const fg::TextureRequest hi_z_buffer)
{
  const uint32_t mips = hi_z_buffer.describe().mipLevels;
  const gapi::TextureHandle tex = hi_z_buffer.get();
  
  uint2 size = render_size;
  for (uint32_t i = 1; i < mips; ++i)
  {
    size /= 2;

    tfx::set_extern("hi_z_src", tfx::Texture{tex, i-1});
    tfx::set_extern("hi_z_dst", tfx::Texture{tex, i});
    tfx::activate_scope("HiZScope", encoder);
    tfx::activate_technique("HiZHigh", encoder);
    encoder.updateResources();
    const uint3 gc = tfx::calc_group_count("HiZHigh", uint3{size, 1});
    encoder.dispatch(gc.x, gc.y, 1);
    
    encoder.transitTextureState(tex, gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite, i, 1);
  }
}

NODE_BEGIN(HiZ)
  READ_RENDER_SIZE_AS(render_size)
  READ_TEX_DEPTH(late_opaque_depth)
  CREATE_TEX_2D_MIP(hi_z_buffer, TEX_SIZE_RELATIVE(), R32_FLOAT, TEX_MIPS_RELATIVE(),
                    TEX_USAGE2(UAV, SRV), TEX_STATE(ShaderReadWrite))
  EXEC(hi_z)
NODE_END()


NODE_EXEC()
static
void hi_z(gapi::CmdEncoder& encoder,
          const uint2& render_size,
          const gapi::TextureHandle late_opaque_depth,
          const fg::TextureRequest hi_z_buffer)
{
  generate_base_mip(encoder, render_size, late_opaque_depth, hi_z_buffer.get());
  generate_high_mips(encoder, render_size, hi_z_buffer);
}
