#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>

#include "Text.h"
#include "../misc/TerminalOutput.h"

#include <sstream>
#include <iomanip>

namespace OpenFF {

using namespace Magnum;

Text::Text(std::string text_location, int text_size) :
        _shadow(OpenFF::ShadowTypes::full_shadow),
        _relative_billboard_ratio(Vector2i(1)),
        _viewport_size(Vector2(1)),
        _offset(Vector2(0)),
        _render_all_characters_instantly(false),
        _render_speed(text_render_state_step),
        _render_state(text_render_state_min) {
  _font = new Font(text_location, text_size, 0.08f); // 0.08f FFVII pixel perfect
  setText("");
}
Text::Text(Font* font) :
        _font(font),
        _shadow(OpenFF::ShadowTypes::full_shadow),
        _relative_billboard_ratio(Vector2i(1)),
        _viewport_size(Vector2(1)),
        _offset(Vector2(1)),
        _render_all_characters_instantly(false),
        _render_speed(text_render_state_step),
        _render_state(text_render_state_min) {
  setText("");
}

int getCharLength(const std::string& s)
{
    std::ostringstream ret;
    ret << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(s[0]);
    return ret.str().length()/2;
}

void Text::draw() {
  if( !_render_all_characters_instantly && _render_state <= _text.length() ) {
    // First, check the length of the unicode character and skip to its end
    if( _render_state == text_render_state_min
     || _render_state < floor(_render_state + _render_speed) ) {
      int char_length = getCharLength(_text.substr(int(_render_state),1));
      if( char_length > 1 ) _render_state = ceil(_render_state + char_length - 2);
    }
    // Substring to the current character
    tieText(_text.substr(0,int(_render_state)));
    // Advance the render state
    _render_state = _render_state + _render_speed;
    if( _render_state >= _text.length() ) _render_state = _text.length();
  }

  using namespace Math::Literals;

  if( _shadow == OpenFF::ShadowTypes::full_shadow ) {
    _text_shader.setTransformationProjectionMatrix(_shadow_mvp_x)
                .setColor(0x000000ff_rgbaf);
    _text_shader.draw(_mesh);
    _text_shader.setTransformationProjectionMatrix(_shadow_mvp_y)
                .setColor(0x000000ff_rgbaf);
  }
  if ( _shadow != OpenFF::ShadowTypes::no_shadow ) {
    _text_shader.draw(_mesh);
    _text_shader.setTransformationProjectionMatrix(_shadow_mvp_xy)
                .setColor(0x000000ff_rgbaf);
    _text_shader.draw(_mesh);
  }

  _text_shader.setTransformationProjectionMatrix(_mvp)
              .setColor(0xffffffff_rgbaf);
  _text_shader.draw(_mesh);
}
void Text::draw(std::string text) {
  setText(text);
  draw();
}

void Text::tieText(std::string text) {
  std::tie(_mesh, std::ignore) = Magnum::Text::Renderer2D::render(
          _font->getFont(),
          _font->getGlyphCache(),
          _font->getFontSizeFactor(),
          text, _vertex_buffer, _index_buffer,
          GL::BufferUsage::StaticDraw);
  _text_shader.setSmoothness(0.0f)
          .bindVectorTexture(_font->getGlyphTexture());
}

void Text::setText(std::string text) {
  _text = text;
  if( _render_all_characters_instantly ) {
    tieText(_text);
  }
}

void Text::setShadowType(ShadowTypes shadow) {
  _shadow = shadow;
}

void Text::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _relative_billboard_ratio = relative_billboard_ratio;
  recalculateMVP();
}
void Text::setViewportSize(Vector2i viewport_size) {
  _viewport_size = viewport_size;
  recalculateMVP();
}
void Text::setOffset(Vector2i offset) {
  _offset = offset;
  recalculateMVP();
}

void Text::enableInstantRendering() {
  _render_all_characters_instantly = true;
  setText(_text);
}
void Text::disableInstantRendering() {
  _render_all_characters_instantly = false;
  setText(_text);
}

void Text::hide() {
  _render_state = text_render_state_min;
}

void Text::recalculateMVP() {
  // Transform Text's pixel offset into cartesian offset:
  // ***************
  // *             *
  // *       A     *
  // *      *B     *
  // *       C     *
  // *       D     *
  // ***************
  // Text rendering behaves a little wonky: The mesh is rendered with [0,0] at
  // the first character's bottom-left.
  // So we calculate the vector from pixel origin [-1,1] to the pixel offset
  // devided by the viewport_size (base_offset), define the vector from origin
  // to pixel origin (top_left_corner) but subtract the font height from the
  // y-axis, so now the mesh's top-left corner aligns with the viewport's
  // top-left corner. Now we calculate the vector from origin to actual offset.
  // Add the border_offset and multiply the relative billboard ratio.
  // The cartesian_transform is universal, as the cartesian coordinates go from
  // [-1,1] to [1,-1], the normalised pixel coordinates go from [0,0] to [1,1].
  Vector2 cartesian_transform = Vector2(2.0f,-2.0f);
  Vector2 base_offset = Vector2(Vector2(_offset)/Vector2(_viewport_size));
          base_offset = base_offset*cartesian_transform;
  Vector2 top_left_corner = Vector2(-1.0f,1.0f-_font->getFontSizeFactor());
  Vector2 border_offset = Vector2(Vector2(8,7)/Vector2(_viewport_size));
          border_offset = border_offset*cartesian_transform;
  Vector2 shadow_offset_x = Vector2(Vector2(1,0)/Vector2(_viewport_size));
          shadow_offset_x = shadow_offset_x*cartesian_transform;
  Vector2 shadow_offset_y = Vector2(Vector2(0,1)/Vector2(_viewport_size));
          shadow_offset_y = shadow_offset_y*cartesian_transform;

  Vector2 offset = (top_left_corner+base_offset+border_offset)
                     *_relative_billboard_ratio;

  Vector2 size = Vector2(0.7f,1.0f)*_relative_billboard_ratio;

  _mvp = Matrix3(Vector3(size.x(),     0.0f, offset.x()),
                 Vector3(    0.0f, size.y(), offset.y()),
                 Vector3(    0.0f,     0.0f,      1.0f)).transposed();

  offset = (top_left_corner+base_offset+border_offset+shadow_offset_x)
             *_relative_billboard_ratio;
  _shadow_mvp_x = Matrix3(Vector3(size.x(),     0.0f, offset.x()),
                          Vector3(    0.0f, size.y(), offset.y()),
                          Vector3(    0.0f,     0.0f,      1.0f)).transposed();
  offset = (top_left_corner+base_offset+border_offset+shadow_offset_y)
             *_relative_billboard_ratio;
  _shadow_mvp_y = Matrix3(Vector3(size.x(),     0.0f, offset.x()),
                          Vector3(    0.0f, size.y(), offset.y()),
                          Vector3(    0.0f,     0.0f,      1.0f)).transposed();
  offset = (top_left_corner+base_offset+border_offset+shadow_offset_x+shadow_offset_y)
             *_relative_billboard_ratio;
  _shadow_mvp_xy = Matrix3(Vector3(size.x(),     0.0f, offset.x()),
                           Vector3(    0.0f, size.y(), offset.y()),
                           Vector3(    0.0f,     0.0f,      1.0f)).transposed();
}

}
