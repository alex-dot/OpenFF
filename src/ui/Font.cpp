#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>

#include "Font.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

Font::Font(std::string font_location, int font_size, float font_size_factor)
        : _font_size_factor(font_size_factor) {
  _font = _font_manager.loadAndInstantiate("StbTrueTypeFont");
  if(!_font || !_font->openFile(font_location, font_size))
      Fatal{} << "Can't open font with StbTrueTypeFont";
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
  std::string glyphs = base_glyphs+base_sign_glyphs+extended_sign_glyphs+extended_sign_glyphs+miscellaneous_glyphs+geometric_glyphs;
  _glyph_cache = new Text::GlyphCache{Vector2i{static_cast<int>(glyphs.length()*4)}};
  _font->fillGlyphCache(*_glyph_cache, glyphs);

  _glyph_texture = &_glyph_cache->texture();
  _glyph_texture->setMinificationFilter(SamplerFilter::Nearest)
                 .setMagnificationFilter(SamplerFilter::Nearest);
}
