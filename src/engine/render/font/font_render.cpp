#include "font_render.h"

#include <engine/ecs/type_meta.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/tfx/tfx.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Engine::Render
{
  FontRender::FontRender(const FontRender& tmpl)
  {
    //std::memcpy(this, &tmpl, sizeof(*this));
  }

  const FontRender& FontRender::operator=(const FontRender& tmpl)
  {
    //std::memcpy(this, &tmpl, sizeof(*this));
    return *this;
  }

  FontRender::~FontRender()
  {
    destroy();
  }

  void FontRender::init()
  {
    const ed::Scope& settings = Engine::get_app_settings();
    const string_view fontFolder = settings.getVariable<string_view>("fonts_folder");
    if (fontFolder != "")
    {
      const string font = fmt::format("{}/arial.ttf", fontFolder);
      init(font);
    }
    else
      logerror("world_render: missing font folder inside settings");
  }

  void FontRender::init(std::string_view font_file)
  {
    loginfo("font_mgr: initializing library");

    m_Glyphs.resize(MAX_GLYPHS);

    if (m_Lib)
      destroy();

    if (FT_Init_FreeType(&m_Lib))
    {
      logerror("font_mgr: failed to init");
      return;
    }

    loginfo("font_mgr: loading new font `{}`", font_file);

    if (FT_New_Face(m_Lib, font_file.data(), 0, &m_Face))
    {
      logerror("font_mgr: failed to load the font `{}`", font_file);
      return;
    }

    FT_Set_Pixel_Sizes(m_Face, 0, INIT_GLYPH_HEIGHT);
    preLoadAllGlyphs();
  }

  void FontRender::destroy()
  {
    if (m_Lib)
    {
      FT_Done_FreeType(m_Lib);
      m_Lib = nullptr;
    }
    if (m_Face)
    {
      FT_Done_Face(m_Face);
      m_Face = nullptr;
    }
  }

  void FontRender::preLoadAllGlyphs()
  {
    loginfo("font_mgr: loading glyphs...");
    std::unique_ptr<gapi::CmdEncoder> cmdEncoder{gapi::allocate_cmd_encoder()};

    for (size_t i = 0 ; i < MAX_GLYPHS; ++i)
    {
      if (FT_Load_Char(m_Face, i, FT_LOAD_RENDER))
      {
        logwarn("font_mgr: failed to load `%d:glyph`", i);
        continue;
      }

      GlyphInfo& glyph = m_Glyphs[i];
      FT_Bitmap& bitmap = m_Face->glyph->bitmap;

      glyph.size = float2(bitmap.width, bitmap.rows);
      glyph.bearing = float2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top);
      glyph.advance = m_Face->glyph->advance.x;

      if (bitmap.width == 0 || bitmap.rows == 0)
        continue;

      gapi::TextureAllocationDescription allocDesc = {
        .format = gapi::TextureFormat::R8_UNORM,
        .extent = int3(glyph.size.x, glyph.size.y, 1),
        .mipLevels = 1,
        .arrayLayers = 1,
        .samplesPerPixel = gapi::TextureSamples::s1,
        .usage = gapi::TEX_USAGE_SRV
      };

      glyph.texture = gapi::allocate_texture(allocDesc);
      cmdEncoder->transitTextureState(glyph.texture, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
      cmdEncoder->copyBufferToTexture(glyph.texture, bitmap.buffer, glyph.size.x * glyph.size.y);
      cmdEncoder->transitTextureState(glyph.texture, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);
    }

    cmdEncoder->flush();
    loginfo("font_mgr: done.");
  }

  void FontRender::render(std::string_view text, const float2& pos, 
    const float& size, const float4& color, gapi::CmdEncoder& encoder)
  {
    if (m_Lib == nullptr || text == "")
      return;

    tfx::activate_technique("Font", encoder);
    tfx::set_extern("glyphColor", color);

    float advance = 0.0;

    for (char c: text)
    {
      if (c >= MAX_GLYPHS)
        c = '#';
      
      const GlyphInfo& glyph = m_Glyphs[c];
      const float scale = size / INIT_GLYPH_HEIGHT;
      const float scaledAdvance = (glyph.advance / 64.0 * scale);

      if (glyph.texture == gapi::TextureHandle::Invalid)
      {
        advance += scaledAdvance;
        continue;
      }

      const float2 glyphSize = glyph.size * scale;
      const float2 scaledBearing = glyph.bearing * scale;

      const float4 sizeBearing = float4(
        glyphSize,
        scaledBearing
      );

      const float2 topLeftOrigin = {pos.x, pos.y + scale * INIT_GLYPH_HEIGHT};

      const float3 advanceOriginPos = float3(
        advance,
        topLeftOrigin
      );

      tfx::set_extern("glyphSize_glyphBearing", sizeBearing);
      tfx::set_extern("glyphAdvance_glyphOriginPos", advanceOriginPos);
      tfx::set_extern("glyphTex", glyph.texture);
      tfx::activate_scope("FontTechniqueScope", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 1);

      advance += scaledAdvance;
    }
  }

  float2 FontRender::getBbox(const float size, std::string_view text) const
  {
    float2 bbox {0, size};

    for (char c: text)
    {
      if (c >= MAX_GLYPHS)
        c = '#';

      const GlyphInfo& glyph = m_Glyphs[c];
      const float scale = size / INIT_GLYPH_HEIGHT;
      const float scaledAdvance = (glyph.advance / 64.0 * scale);

      bbox.x += scaledAdvance;
    }

    return bbox;
  }
  
  DECLARE_ECS_COMPONENT(FontRender, "FontRender", (ecs::NonTrivialTypeManager<FontRender, ecs::INITABLE_TYPE>), false, true);
}
