#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

using namespace Magnum;

namespace OpenFF {

class Font{
  public:
    explicit Font(std::string location, int size, float font_size_factor);

    Text::AbstractFont& getFont() { return *_font; }
    Text::GlyphCache& getGlyphCache() const { return *_glyph_cache; }
    GL::Texture2D& getGlyphTexture() const { return *_glyph_texture; }
    float getFontSizeFactor() const { return _font_size_factor; }

  private:

    PluginManager::Manager<Text::AbstractFont>  _font_manager;
    Containers::Pointer<Text::AbstractFont>     _font;
    Text::GlyphCache*                           _glyph_cache;
    GL::Texture2D*                              _glyph_texture;
    float                                       _font_size_factor;
};

}
