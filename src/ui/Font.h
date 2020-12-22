#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/Trade/ImageData.h>

using namespace Magnum;

namespace OpenFF {

int getCharLength(const std::string&);

class Font{
  public:
    explicit Font(std::string location, int size, float font_size_factor);
    explicit Font(
            std::string location,
            int size,
            Containers::Optional<Trade::ImageData2D>& glyph_texture,
            float font_size_factor);

    Text::AbstractFont& getFont() { return *_font; }
    Text::GlyphCache& getGlyphCache() const { return *_glyph_cache; }
    GL::Texture2D& getGlyphTexture() const { return *_glyph_texture; }
    float getFontSizeFactor() const { return _font_size_factor; }
    unsigned getMaximumGlyphWidth() const { return _glyph_max_width; }
    unsigned getGlyphWidth(std::string);
    unsigned getFontLineHeight() const { return _font_line_height; }

  private:
    void makeGlyphCache(Containers::Optional<Trade::ImageData2D>*);
    void calculateMetadata();

    PluginManager::Manager<Text::AbstractFont>  _font_manager;
    Containers::Pointer<Text::AbstractFont>     _font;
    Text::GlyphCache*                           _glyph_cache;
    GL::Texture2D*                              _glyph_texture;
    std::string                                 _glyphs;
    unsigned int                                _glyph_max_width;
    float                                       _font_size_factor;
    unsigned int                                _font_border_offset;
    unsigned int                                _font_line_height;
};

}
