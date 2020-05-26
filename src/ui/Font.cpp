#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>

#include "Font.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

Font::Font(std::string font_location, int font_size) {
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
  std::string glyphs = base_glyphs+base_sign_glyphs+extended_sign_glyphs+extended_sign_glyphs+miscellaneous_glyphs;
  _glyph_cache = new Text::GlyphCache{Vector2i{static_cast<int>(glyphs.length()*4)}};
  _font->fillGlyphCache(*_glyph_cache, glyphs);

  _text_translation_matrix = Matrix3::translation(Vector2(0.006f,-0.1825f));
  _text_scaling_matrix = Matrix3::scaling(Vector2(0.023,0.03));
  _text_projection_matrix = Matrix3::projection(Vector2(1.0f))
          *_text_translation_matrix
          *_text_scaling_matrix;
  _glyph_texture = &_glyph_cache->texture();
  _glyph_texture->setMinificationFilter(SamplerFilter::Nearest)
                 .setMagnificationFilter(SamplerFilter::Nearest);
  std::tie(_mesh, std::ignore) = Text::Renderer2D::render(
          *_font, *_glyph_cache, 1.225f, // 1.225f pixel perfect
          "Cloud: Hello World!\n“Aeris?“", _vertex_buffer, _index_buffer,
          GL::BufferUsage::StaticDraw);
}

void Font::draw(FontRenderType type) {
  using namespace Math::Literals;
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::DestinationAlpha,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);

  switch(type) {
    case(FontRenderType::shadow):
      _text_shader.setTransformationProjectionMatrix(
              _text_projection_matrix*Matrix3::translation(Vector2(0.1,-0.1)))
          .setColor(0x000000ff_rgbaf);
      break;
    case(FontRenderType::normal):
    default:
      _text_shader.setTransformationProjectionMatrix(_text_projection_matrix)
          .setColor(0xffffffff_rgbaf);
  }

  _text_shader.setSmoothness(0.0f)
          .bindVectorTexture(*_glyph_texture)
          .draw(_mesh);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);
}

void Font::setRelativeBillboardRatio(Magnum::Vector2 relative_billboard_ratio) {
  _text_translation_matrix = Matrix3::translation(Vector2(0.006f,-0.1825f)*relative_billboard_ratio);
  _text_scaling_matrix = Matrix3::scaling(Vector2(0.023,0.03)*relative_billboard_ratio);
  _text_projection_matrix = Matrix3::projection(Vector2(1.0f))
          *_text_translation_matrix
          *_text_scaling_matrix;
}
