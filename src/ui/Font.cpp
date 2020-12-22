#include "Font.h"
#include "../misc/TerminalOutput.h"

#include <Corrade/Containers/Optional.h>
#include <Magnum/ImageView.h>

#include <uchar.h>
#include <cstring>

using namespace OpenFF;

Font::Font(std::string font_location, int font_size, float font_size_factor)
        : _font_size_factor(font_size_factor) {
  _font = _font_manager.loadAndInstantiate("StbTrueTypeFont");
  if(!_font || !_font->openFile(font_location, font_size))
      Fatal{} << "Can't open font with StbTrueTypeFont";

  makeGlyphCache(nullptr);
  calculateMetadata();
}

Font::Font(
        std::string font_location,
        int font_size,
        Containers::Optional<Trade::ImageData2D>& glyph_texture,
        float font_size_factor)
                : _font_size_factor(font_size_factor) {
  _font = _font_manager.loadAndInstantiate("StbTrueTypeFont");
  if(!_font || !_font->openFile(font_location, font_size))
      Fatal{} << "Can't open font by cached image";

  makeGlyphCache(&glyph_texture);
  calculateMetadata();
}

void Font::makeGlyphCache(Containers::Optional<Trade::ImageData2D>* glyph_texture) {
  std::string base_glyphs =
          "abcdefghijklmnopqrstuvwxyz"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "0123456789 ";
  std::string base_sign_glyphs = "!?#$&%()*+-/,.^~:;<>'\"";
  std::string extended_sign_glyphs =
          "¡¢£¤¥€¦§¨©ª«¬®¯°±´µ¶·¸º»¿@[]"
          "‐–—―\\_`{}|…†‡‰÷×‸※‽‾⁋⁖℅™⌘“„";
  std::string super_subscript_glyphs =
          "⁰¹²³⁴⁵⁶⁷⁸⁹⁺⁻⁼⁽⁾ⁿ"
          "₀₁₂₃₄₅₆₇₈₉₊₋₌₍₎ₐ"
          "ₑₒₓₔₕₖₗₘₙₚₛₜ";
  std::string roman_numeral_glyphs = "ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ";
  std::string ligature_glyphs = "ﬀﬁﬂﬃﬄ";
  std::string arrow_glyphs = "←↑→↓↔↕↼↽↾↿⇀⇁⇂⇃⇐⇑⇒⇓⇔⇕⟵⟶";
  std::string math_glyphs =
          "%()*+-/÷×·°^~"
          "∂∆∇∑−∓∕∖∙√∝∞∫∼∽≈≠≡≢≤≥";
  std::string geometric_glyphs =
          "▀▁▂▃▄▅▆▇█▉▊▋▌▍▎▏▐▔▕"
          "▖▗▘▙▚▛▜▝▞▟■□▢▣▤▥▦▧▨▩▪▫▬▭▮▯▰▱"
          "▲△▴▵▶▷▸▹►▻▼▽▾▿◀◁◂◃◄◅"
          "◆◇◈◉◊○◌◍◎●◐◑◒◓◔◕◖◗◯"
          "◜◝◞◟◠◡◢◣◤◥◦◧◨◩◪◫◸◹◺◻◼◽◾◿"
          "⬒⬓⬔⬕⬖⬗⬘⬙⬚";
  std::string miscellaneous_glyphs = "★☆☉☥♀♂♡♥";
  std::string cjk_symbol_glyphs = "、。々「」『』〜";
  // As defined at https://decodeunicode.org/en/languages/de#de
  std::string german_auxiliary_glyphs =
          "ÄäÖöÜüßÁáÀàÂâåÃãĀāÆæÇç"  // ĂăÅ
          "ÉéÈèÊêËëĒēÍíÌìÎîÏïİ"     // ĔĕĞğĬĭ
          "ĪīıÑñÓóÒòÔôØøŌōŒœŞşÚúÙù" // ŎŏŬŭ
          "ÛûŪūŸÿ";
  std::string music_menu_glyphs = "«■▶Ⅱ▯»";
  _glyphs = base_glyphs+base_sign_glyphs+music_menu_glyphs;
  _glyph_cache = new Text::GlyphCache{Vector2i{static_cast<int>(_glyphs.length()*4)}};
  if( glyph_texture ) {
    ImageView2D glyph_texture_data = **glyph_texture;
    _glyph_cache->setImage(Vector2i(0), glyph_texture_data);
  }
  _font->fillGlyphCache(*_glyph_cache, _glyphs);

  _glyph_texture = &_glyph_cache->texture();
  _glyph_texture->setMinificationFilter(SamplerFilter::Nearest)
                 .setMagnificationFilter(SamplerFilter::Nearest);
}

void Font::calculateMetadata() {
  _glyph_max_width = 0;
  for( unsigned i = 0; i < _glyphs.length(); ++i ) {
    int char_length = getCharLength(_glyphs.substr(i,1));
    std::string str = _glyphs.substr(i,char_length);
    unsigned int width = getGlyphWidth(str);
    if( char_length > 1 ) i = i + char_length - 1;
    if( width > _glyph_max_width )
      _glyph_max_width = width;
  }
  _font_line_height = _font->lineHeight()/2;
}

unsigned Font::getGlyphWidth(std::string str) {
  char32_t c32str;
  mbstate_t mbs;
  memset (&mbs, 0, sizeof (mbs));
  mbrtoc32 (&c32str, str.c_str(), 6, &mbs);
  return _font->glyphAdvance(_font->glyphId(c32str))[0];
}

int OpenFF::getCharLength(const std::string& s)
{
  if((s[0] & 0xf8) == 0xf0) return 4;
  else if((s[0] & 0xf0) == 0xe0) return 3;
  else if((s[0] & 0xe0) == 0xc0) return 2;
  return 1;
}
