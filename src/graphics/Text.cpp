#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>

#include "Text.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

using namespace Magnum;

Text::Text(std::string text_location, int text_size) {
  _font = new Font(text_location, text_size, 1.225f); // 1.225f FFVII pixel perfect
}
Text::Text(Font* font) : _font(font) {}

void Text::draw(TextRenderType type) {
  std::tie(_mesh, std::ignore) = Magnum::Text::Renderer2D::render(
          _font->getFont(),
          _font->getGlyphCache(),
          _font->getFontSizeFactor(),
          "Cloud: Hello World!\n“Aeris?“",
          _vertex_buffer, _index_buffer,
          GL::BufferUsage::StaticDraw);
  _text_shader.setSmoothness(0.0f)
          .bindVectorTexture(_font->getGlyphTexture());

  using namespace Math::Literals;
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::DestinationAlpha,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);

  switch(type) {
    case(TextRenderType::shadow):
      _text_shader.setTransformationProjectionMatrix(
              _text_projection_matrix*Matrix3::translation(Vector2(0.1,-0.1)))
          .setColor(0x000000ff_rgbaf);
      break;
    case(TextRenderType::normal):
    default:
      _text_shader.setTransformationProjectionMatrix(_text_projection_matrix)
          .setColor(0xffffffff_rgbaf);
  }

  _text_shader.draw(_mesh);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);
}

void Text::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _text_translation_matrix = Matrix3::translation(Vector2(0.006f,-0.1825f)*relative_billboard_ratio);
  _text_scaling_matrix = Matrix3::scaling(Vector2(0.023,0.03)*relative_billboard_ratio);
  _text_projection_matrix = Matrix3::projection(Vector2(1.0f))
          *_text_translation_matrix
          *_text_scaling_matrix;
}

}
