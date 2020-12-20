#pragma once

#include "../ui/Font.h"
#include "../utilities/InputHandler.h"

namespace OpenFF {

enum ShadowTypes {
  no_shadow,
  simple_shadow,
  full_shadow,
  SHADOW_TYPES_MAX = full_shadow
};
int getCharLength(const std::string&);

const float text_render_state_min = 1.0;
const float text_render_state_step = 1.0;

using namespace Magnum;

class Text{
  public:
    explicit Text(std::string location, int size);
    explicit Text(Font* font);

    void draw();
    void draw(std::string);
    void hide();
    void setText(std::string text);
    void setShadowType(ShadowTypes);
    void setRelativeBillboardRatio(Vector2);
    void setViewportSize(Vector2i);
    void setOffset(Vector2i);
    void setBorderOffset(Vector2i);
    void enableInstantRendering();
    void disableInstantRendering();

    // always divided by 2 because the viewport goes from (-1,1)
    unsigned getFontMaximumGlyphWidth() { return _font->getMaximumGlyphWidth()/2; }
    unsigned getFontGlyphWidth(std::string str) { return _font->getGlyphWidth(str)/2; }
    unsigned getFontLineHeight() { return _font->getFontLineHeight(); }

    std::string getText();

  private:
    void recalculateMVP();
    void tieText(std::string);

    std::string                       _text;
    OpenFF::Font*                     _font;
    Shaders::DistanceFieldVector2D    _text_shader;
    GL::Buffer                        _vertex_buffer, _index_buffer;
    GL::Mesh                          _mesh;
    Matrix3                           _mvp;
    ShadowTypes                       _shadow;
    Matrix3                           _shadow_mvp_x;
    Matrix3                           _shadow_mvp_y;
    Matrix3                           _shadow_mvp_xy;
    Vector2                           _relative_billboard_ratio;
    Vector2i                          _viewport_size;
    Vector2i                          _offset;
    Vector2                           _border_offset;
    bool                              _render_all_characters_instantly;
    float                             _render_speed;
    float                             _render_state;
};

}
