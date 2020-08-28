#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include "../ui/Font.h"
#include "../utilities/InputHandler.h"

namespace OpenFF {

enum ShadowTypes {
  no_shadow,
  simple_shadow,
  full_shadow,
  SHADOW_TYPES_MAX = full_shadow
};

using namespace Magnum;

class Text{
  public:
    explicit Text(std::string location, int size);
    explicit Text(Font* font);

    void setText(std::string text);
    void draw();
    void draw(std::string);
    void setShadowType(ShadowTypes);
    void setRelativeBillboardRatio(Vector2);
    void setViewportSize(Vector2i);
    void setOffset(Vector2i);

  private:
    void recalculateMVP();

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
};

}
