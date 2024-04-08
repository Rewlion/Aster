#pragma once

#include <engine/gapi/resource_wrappers.h>

#include <EASTL/vector.h>

#include <array>
#include <string_view>

struct FT_LibraryRec_;
typedef FT_LibraryRec_* FT_Library;

struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;

namespace gapi
{
  class CmdEncoder;
}

namespace ed
{
  class Scope;
}

namespace Engine::Render
{
  class FontRender
  {
    public:
      FontRender() = default;
      FontRender(const FontRender& tmpl);
      FontRender(const ed::Scope*) {}

      const FontRender& operator=(const FontRender&);

      ~FontRender();
      void init();
      void init(std::string_view font_file);
      void render(std::string_view text, const float2& pos, 
        const float& size, const float4& color, gapi::CmdEncoder& encoder);

      float2 getBbox(const float size, std::string_view text) const;

    private:
      void destroy();
      void preLoadAllGlyphs();

    private:
      FT_Library m_Lib = nullptr;
      FT_Face m_Face = nullptr;

      static constexpr size_t INIT_GLYPH_HEIGHT = 48;
      static constexpr size_t MAX_GLYPHS = 128;
      struct GlyphInfo
      {
        gapi::TextureWrapper texture;
        float2 size = {0,0};
        float2 bearing = {0,0};
        float advance = 0; 
      };
      eastl::vector<GlyphInfo> m_Glyphs;
  };
}
